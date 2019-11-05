/*
LDPC SISO layered decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef LDPC_DECODER_HH
#define LDPC_DECODER_HH

#include <algorithm>
#include "simd.hh"
#include "rotate.hh"

namespace CODE {

template <typename TABLE, int BETA>
class LDPCDecoder
{
#ifdef __AVX2__
	static const int SIMD_SIZE = 32;
	// M = 360 = 30 * 12
	static const int WORD_SIZE = 30;
#else
	static const int SIMD_SIZE = 16;
	// M = 360 = 15 * 24
	static const int WORD_SIZE = 15;
#endif
	static_assert(TABLE::M % WORD_SIZE == 0, "M must be multiple of word size");
	static_assert(WORD_SIZE <= SIMD_SIZE, "SIMD size must be bigger or equal word size");
	static const int M = TABLE::M;
	static const int N = TABLE::N;
	static const int K = TABLE::K;
	static const int R = N-K;
	static const int q = R/M;
	static const int D = WORD_SIZE;
	static const int W = M/D;
	static const int PTY = R/D;
	static const int MSG = K/D;
	static const int VAR = N/D;
	static const int CNC = TABLE::LINKS_MAX_CN - 2;
	static const int BNL = (TABLE::LINKS_TOTAL + D-1) / D;
	static const int LOC = (TABLE::LINKS_TOTAL + D-1) / D;

	typedef SIMD<int8_t, SIMD_SIZE> TYPE;
	typedef struct { uint16_t off; uint16_t shi; } Loc;
	typedef uint32_t wdm_t;
	static_assert(sizeof(wdm_t) * 8 >= TABLE::LINKS_MAX_CN, "write disable mask needs at least as many bits as max check node links");
	Rotate<TYPE, D> rotate;

	TYPE bnl[BNL];
	TYPE var[VAR];
	Loc loc[LOC];
	wdm_t wdm[PTY];
	int16_t csh[VAR];
	uint8_t cnt[PTY];

	static TYPE eor(TYPE a, TYPE b)
	{
		return vreinterpret<TYPE>(veor(vmask(a), vmask(b)));
	}
	static TYPE orr(TYPE a, TYPE b)
	{
		return vreinterpret<TYPE>(vorr(vmask(a), vmask(b)));
	}
	static TYPE other(TYPE a, TYPE b, TYPE c)
	{
		return vreinterpret<TYPE>(vbsl(vceq(a, b), vmask(c), vmask(b)));
	}
	static TYPE mine(TYPE a, TYPE b)
	{
		return orr(eor(a, b), vdup<TYPE>(127));
	}
	static TYPE selfcorr(TYPE a, TYPE b)
	{
		return vreinterpret<TYPE>(vand(vmask(b), vorr(vceqz(a), veor(vcgtz(a), vcltz(b)))));
	}

	bool update()
	{
		TYPE *bl = bnl;
		Loc *lo = loc;
		wdm_t *wd = wdm;
		auto bad = vmask(vzero<TYPE>());
		for (int i = 0; i < PTY; ++i) {
			int deg = cnt[i];
			TYPE mags[deg], inps[deg];
			TYPE min0 = vdup<TYPE>(127);
			TYPE min1 = vdup<TYPE>(127);
			TYPE signs = vdup<TYPE>(127);
			TYPE cnv = vdup<TYPE>(127);
			bool write_conflict = false;
			int last_offset = -1;
			int8_t prev_val = 0;

			for (int k = 0; k < deg; ++k) {
				int offset = lo[k].off;
				int shift = lo[k].shi;
				shift -= csh[offset];
				shift %= D;
				TYPE tmp = rotate(var[offset], shift);
				if (i == 0 && offset == VAR-1) {
					prev_val = tmp.v[0];
					tmp.v[0] = 127;
				}

				if (last_offset == offset)
					write_conflict = true;
				last_offset = offset;

				TYPE inp = vqsub(tmp, bl[k]);

				TYPE mag = vqabs(inp);

				if (BETA) {
					auto beta = vunsigned(vdup<TYPE>(BETA));
					mag = vsigned(vqsub(vunsigned(mag), beta));
				}

				min1 = vmin(min1, vmax(min0, mag));
				min0 = vmin(min0, mag);

				signs = eor(signs, inp);

				inps[k] = inp;
				mags[k] = mag;
			}
			for (int k = 0; k < deg; ++k) {
				TYPE mag = mags[k];
				TYPE inp = inps[k];

				TYPE out = vsign(other(mag, min0, min1), mine(signs, inp));

				out = vclamp(out, -32, 31);

				out = selfcorr(bl[k], out);

				TYPE tmp = vqadd(inp, out);

				cnv = vsign(cnv, tmp);

				int offset = lo[k].off;
				int shift = lo[k].shi;

				if (i == 0 && offset == VAR-1)
					tmp.v[0] = prev_val;

				if (!write_conflict || !((*wd>>k)&1)) {
					bl[k] = out;
					var[offset] = tmp;
					csh[offset] = shift;
				}
			}
			bad = vorr(bad, vclez(cnv));

			if (write_conflict) {
				for (int first = 0, k = 1; k < deg; ++k) {
					if (lo[first].off != lo[k].off || k == deg-1) {
						int last = k - 1;
						if (k == deg-1)
							++last;
						if (last != first) {
							int count = last - first + 1;
							wdm_t mask = ((1 << count) - 1) << first;
							wdm_t cur = *wd;
							wdm_t tmp = cur & mask;
							wdm_t ror = (tmp >> 1) | (tmp << (count-1));
							*wd = (cur & ~mask) | (ror & mask);
						}
						first = k;
					}
				}
				++wd;
			}
			lo += deg;
			bl += deg;
		}
		//assert(bl <= bnl + BNL);
		//std::cerr << BNL - (bl - bnl) << std::endl;
		for (int n = 0; n < D; ++n)
			if (bad.v[n])
				return true;
		return false;
	}
public:
	LDPCDecoder()
	{
		uint16_t pos[q * CNC];
		uint8_t cnc[q];
		for (int i = 0; i < q; ++i)
			cnc[i] = 0;
		int bit_pos = 0;
		const int *row_ptr = TABLE::POS;
		for (int g = 0; TABLE::LEN[g]; ++g) {
			int bit_deg = TABLE::DEG[g];
			for (int r = 0; r < TABLE::LEN[g]; ++r) {
				for (int d = 0; d < bit_deg; ++d) {
					int n = row_ptr[d] % q;
					int m = row_ptr[d] / q;
					pos[CNC*n+cnc[n]++] = bit_pos + (M - m) % M;
				}
				row_ptr += bit_deg;
				bit_pos += M;
			}
		}
		for (int i = 0; i < q; ++i)
			for (int j = 0; j < W; ++j)
				cnt[W*i+j] = cnc[i] + 2;
		Loc *lo = loc;
		wdm_t *wd = wdm;
		for (int i = 0; i < q; ++i) {
			int cnt = cnc[i];
			int deg = cnt + 2;
			int offset[cnt], shift[cnt];
			for (int c = 0; c < cnt; ++c) {
				shift[c] = pos[CNC*i+c] % M;
				offset[c] = pos[CNC*i+c] - shift[c];
			}
			for (int j = 0; j < W; ++j) {
				for (int c = 0; c < cnt; ++c) {
					lo[c].off = offset[c] / D + shift[c] % W;
					lo[c].shi = (D - shift[c] / W) % D;
					shift[c] = (shift[c] + 1) % M;
				}
				if (i) {
					lo[cnt].off = MSG+W*(i-1)+j;
					lo[cnt].shi = 0;
				} else if (j) {
					lo[cnt].off = MSG+W*(q-1)+j-1;
					lo[cnt].shi = 0;
				} else {
					lo[cnt].off = VAR-1;
					lo[cnt].shi = 1;
				}
				lo[cnt+1].off = MSG+W*i+j;
				lo[cnt+1].shi = 0;

				std::sort(lo, lo + deg, [](const Loc &a, const Loc &b){ return a.off < b.off; });
				wdm_t tmp = 0;
				for (int d = 0; d < deg-1; ++d)
					if (lo[d].off == lo[d+1].off)
						tmp |= 1 << d;
				if (tmp)
					*wd++ = tmp;
				lo += deg;
			}
		}
		//assert(lo <= loc + LOC);
		//std::cerr << LOC - (lo - loc) << std::endl;
	}
	int operator()(int8_t *message, int8_t *parity, int trials = 25)
	{
		for (int i = 0; i < BNL; ++i)
			bnl[i] = vzero<TYPE>();
		for (int i = 0; i < VAR; ++i)
			csh[i] = 0;
		for (int i = 0; i < K/M; ++i)
			for (int j = 0; j < W; ++j)
				for (int n = 0; n < D; ++n)
					var[W*i+j].v[n] = message[M*i+W*n+j];
		for (int i = 0; i < q; ++i)
			for (int j = 0; j < W; ++j)
				for (int n = 0; n < D; ++n)
					var[MSG+W*i+j].v[n] = parity[q*(W*n+j)+i];

		while (--trials >= 0 && update());

		for (int i = 0; i < VAR; ++i)
			var[i] = rotate(var[i], -csh[i]);

		for (int i = 0; i < K/M; ++i)
			for (int j = 0; j < W; ++j)
				for (int n = 0; n < D; ++n)
					message[M*i+W*n+j] = var[W*i+j].v[n];
		for (int i = 0; i < q; ++i)
			for (int j = 0; j < W; ++j)
				for (int n = 0; n < D; ++n)
					parity[q*(W*n+j)+i] = var[MSG+W*i+j].v[n];
		return trials;
	}
};

}

#endif
