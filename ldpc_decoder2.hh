/*
LDPC SISO layered decoder v2

This version stores and uses all word positions

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
	static const int CNC = TABLE::LINKS_MAX_CN - 2;
	static const int BNL = (TABLE::LINKS_TOTAL + D-1) / D;
	static const int POS = (TABLE::LINKS_TOTAL - (2*R-1) + D-1) / D;

	typedef SIMD<int8_t, SIMD_SIZE> TYPE;
	Rotate<TYPE, D> rotate;

	TYPE bnl[BNL];
	TYPE msg[MSG];
	TYPE pty[PTY];
	uint16_t off[POS];
	uint8_t shi[POS];
	uint8_t cnc[q];

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
	static void cnp(TYPE *out, const TYPE *inp, int cnt)
	{
		TYPE mags[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = vqabs(inp[i]);

		if (BETA) {
			auto beta = vunsigned(vdup<TYPE>(BETA));
			for (int i = 0; i < cnt; ++i)
				mags[i] = vsigned(vqsub(vunsigned(mags[i]), beta));
		}

		TYPE mins[2];
		mins[0] = vmin(mags[0], mags[1]);
		mins[1] = vmax(mags[0], mags[1]);
		for (int i = 2; i < cnt; ++i) {
			mins[1] = vmin(mins[1], vmax(mins[0], mags[i]));
			mins[0] = vmin(mins[0], mags[i]);
		}

		TYPE signs = inp[0];
		for (int i = 1; i < cnt; ++i)
			signs = eor(signs, inp[i]);

		for (int i = 0; i < cnt; ++i)
			out[i] = vsign(other(mags[i], mins[0], mins[1]), mine(signs, inp[i]));
	}
	static TYPE selfcorr(TYPE a, TYPE b)
	{
		return vreinterpret<TYPE>(vand(vmask(b), vorr(vceqz(a), veor(vcgtz(a), vcltz(b)))));
	}

	bool bad()
	{
		uint16_t *of = off;
		uint8_t *sh = shi;
		for (int i = 0; i < q; ++i) {
			int cnt = cnc[i];
			auto res = vmask(vzero<TYPE>());
			for (int j = 0; j < W; ++j) {
				TYPE par[2];
				if (i) {
					par[0] = pty[W*(i-1)+j];
				} else if (j) {
					par[0] = pty[W*(q-1)+j-1];
				} else {
					par[0] = rotate(pty[PTY-1], 1);
					par[0].v[0] = 127;
				}
				par[1] = pty[W*i+j];
				TYPE mes[cnt];
				for (int c = 0; c < cnt; ++c)
					mes[c] = rotate(msg[of[c]], -sh[c]);
				TYPE cnv = vdup<TYPE>(1);
				for (int c = 0; c < 2; ++c)
					cnv = vsign(cnv, par[c]);
				for (int c = 0; c < cnt; ++c)
					cnv = vsign(cnv, mes[c]);
				res = vorr(res, vclez(cnv));
				of += cnt;
				sh += cnt;
			}
			for (int n = 0; n < D; ++n)
				if (res.v[n])
					return true;
		}
		return false;
	}
	void update()
	{
		TYPE *bl = bnl;
		uint16_t *of = off;
		uint8_t *sh = shi;
		for (int i = 0; i < q; ++i) {
			int cnt = cnc[i];
			int deg = cnt + 2;
			for (int j = 0; j < W; ++j) {
				TYPE par[2];
				if (i) {
					par[0] = pty[W*(i-1)+j];
				} else if (j) {
					par[0] = pty[W*(q-1)+j-1];
				} else {
					par[0] = rotate(pty[PTY-1], 1);
					par[0].v[0] = 127;
				}
				par[1] = pty[W*i+j];
				TYPE mes[cnt];
				for (int c = 0; c < cnt; ++c)
					mes[c] = rotate(msg[of[c]], -sh[c]);
				TYPE inp[deg], out[deg];
				for (int c = 0; c < cnt; ++c)
					inp[c] = vqsub(mes[c], bl[c]);
				inp[cnt] = vqsub(par[0], bl[cnt]);
				inp[cnt+1] = vqsub(par[1], bl[cnt+1]);
				cnp(out, inp, deg);
				for (int d = 0; d < deg; ++d)
					out[d] = vclamp(out[d], -32, 31);
				for (int d = 0; d < deg; ++d)
					out[d] = selfcorr(bl[d], out[d]);
				for (int d = 0; d < deg; ++d)
					bl[d] = out[d];
				for (int c = 0; c < cnt; ++c)
					mes[c] = vqadd(inp[c], out[c]);
				par[0] = vqadd(inp[cnt], out[cnt]);
				par[1] = vqadd(inp[cnt+1], out[cnt+1]);
				if (i) {
					pty[W*(i-1)+j] = par[0];
				} else if (j) {
					pty[W*(q-1)+j-1] = par[0];
				} else {
					par[0].v[0] = pty[PTY-1].v[D-1];
					pty[PTY-1] = rotate(par[0], -1);
				}
				pty[W*i+j] = par[1];
				for (int c = 0; c < cnt; ++c)
					msg[of[c]] = rotate(mes[c], sh[c]);
				of += cnt;
				sh += cnt;
				bl += deg;
			}
		}
		//assert(bl <= bnl + BNL);
		//std::cerr << BNL - (bl - bnl) << std::endl;
	}
public:
	LDPCDecoder()
	{
		uint16_t pos[q * CNC];
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
		uint16_t *of = off;
		uint8_t *sh = shi;
		for (int i = 0; i < q; ++i) {
			int cnt = cnc[i];
			int offset[cnt], shift[cnt];
			for (int c = 0; c < cnt; ++c) {
				shift[c] = pos[CNC*i+c] % M;
				offset[c] = pos[CNC*i+c] - shift[c];
			}
			for (int j = 0; j < W; ++j) {
				for (int c = 0; c < cnt; ++c) {
					of[c] = offset[c] / D + shift[c] % W;
					sh[c] = shift[c] / W;
					shift[c] = (shift[c] + 1) % M;
				}
				of += cnt;
				sh += cnt;
			}
		}
		//assert(of <= off + POS);
		//std::cerr << POS - (sh - shi) << std::endl;
		//assert(sh <= shi + POS);
		//std::cerr << POS - (of - off) << std::endl;
	}
	int operator()(int8_t *message, int8_t *parity, int trials = 25)
	{
		for (int i = 0; i < BNL; ++i)
			bnl[i] = vzero<TYPE>();
		for (int i = 0; i < K/M; ++i)
			for (int j = 0; j < W; ++j)
				for (int n = 0; n < D; ++n)
					msg[W*i+j].v[n] = message[M*i+W*n+j];
		for (int i = 0; i < q; ++i)
			for (int j = 0; j < W; ++j)
				for (int n = 0; n < D; ++n)
					pty[W*i+j].v[n] = parity[q*(W*n+j)+i];
		while (bad() && --trials >= 0)
			update();
		for (int i = 0; i < K/M; ++i)
			for (int j = 0; j < W; ++j)
				for (int n = 0; n < D; ++n)
					message[M*i+W*n+j] = msg[W*i+j].v[n];
		for (int i = 0; i < q; ++i)
			for (int j = 0; j < W; ++j)
				for (int n = 0; n < D; ++n)
					parity[q*(W*n+j)+i] = pty[W*i+j].v[n];
		return trials;
	}
};

}

#endif
