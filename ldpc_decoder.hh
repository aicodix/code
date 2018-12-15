/*
LDPC SISO layered decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef LDPC_DECODER_HH
#define LDPC_DECODER_HH

#include <algorithm>
#include "simd.hh"

namespace CODE {

template <typename TABLE, int BETA>
class LDPCDecoder
{
#ifdef __AVX2__
	static const int SIMD_SIZE = 32;
#else
	static const int SIMD_SIZE = 16;
#endif
	static const int M = TABLE::M;
	static const int N = TABLE::N;
	static const int K = TABLE::K;
	static const int R = N-K;
	static const int q = R/M;
	static const int CNC = TABLE::LINKS_MAX_CN - 2;
	static const int BNL = TABLE::LINKS_TOTAL / SIMD_SIZE + TABLE::LINKS_MAX_CN * q;

	typedef SIMD<int8_t, SIMD_SIZE> TYPE;

	TYPE bnl[BNL];
	int8_t pty[R];
	uint16_t pos[R * CNC];
	uint8_t cnc[R];

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
	static void finalp(TYPE *links, int cnt)
	{
		auto beta = vunsigned(vdup<TYPE>(BETA));
		TYPE mags[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = vsigned(vqsub(vunsigned(vqabs(links[i])), beta));

		TYPE mins[2];
		mins[0] = vmin(mags[0], mags[1]);
		mins[1] = vmax(mags[0], mags[1]);
		for (int i = 2; i < cnt; ++i) {
			mins[1] = vmin(mins[1], vmax(mins[0], mags[i]));
			mins[0] = vmin(mins[0], mags[i]);
		}

		TYPE signs = links[0];
		for (int i = 1; i < cnt; ++i)
			signs = eor(signs, links[i]);

		for (int i = 0; i < cnt; ++i)
			links[i] = vsign(other(mags[i], mins[0], mins[1]), mine(signs, links[i]));
	}

	bool bad(int8_t *data, int8_t *parity)
	{
		for (int i = 0; i < q; ++i) {
			int cnt = cnc[i];
			auto res = vmask(vzero<TYPE>());
			for (int j = 0; j < M; j += SIMD_SIZE) {
				int num = std::min(M - j, SIMD_SIZE);
				TYPE par[2];
				if (i) {
					for (int n = 0; n < num; ++n)
						par[0].v[n] = parity[M*(i-1)+j+n];
				} else if (j) {
					for (int n = 0; n < num; ++n)
						par[0].v[n] = parity[M*(q-1)-1+j+n];
				} else {
					par[0].v[0] = 127;
					for (int n = 1; n < num; ++n)
						par[0].v[n] = parity[M*(q-1)-1+j+n];
				}
				for (int n = 0; n < num; ++n)
					par[1].v[n] = parity[M*i+j+n];
				TYPE dat[cnt];
				for (int c = 0; c < cnt; ++c)
					for (int n = 0; n < num; ++n)
						dat[c].v[n] = data[pos[CNC*(M*i+j+n)+c]];
				TYPE cnv = vdup<TYPE>(1);
				for (int c = 0; c < 2; ++c)
					cnv = vsign(cnv, par[c]);
				for (int c = 0; c < cnt; ++c)
					cnv = vsign(cnv, dat[c]);
				for (int n = num; n < SIMD_SIZE; ++n)
					cnv.v[n] = 1;
				res = vorr(res, vclez(cnv));
			}
			for (int n = 0; n < SIMD_SIZE; ++n)
				if (res.v[n])
					return true;
		}
		return false;
	}
	void update(int8_t *data, int8_t *parity)
	{
		TYPE *bl = bnl;
		for (int i = 0; i < q; ++i) {
			int cnt = cnc[i];
			int deg = cnt + 2;
			for (int j = 0; j < M; j += SIMD_SIZE) {
				int num = std::min(M - j, SIMD_SIZE);
				TYPE par[2];
				if (i) {
					for (int n = 0; n < num; ++n)
						par[0].v[n] = parity[M*(i-1)+j+n];
				} else if (j) {
					for (int n = 0; n < num; ++n)
						par[0].v[n] = parity[M*(q-1)-1+j+n];
				} else {
					par[0].v[0] = 127;
					for (int n = 1; n < num; ++n)
						par[0].v[n] = parity[M*(q-1)-1+j+n];
				}
				for (int n = 0; n < num; ++n)
					par[1].v[n] = parity[M*i+j+n];
				TYPE dat[cnt];
				for (int c = 0; c < cnt; ++c)
					for (int n = 0; n < num; ++n)
						dat[c].v[n] = data[pos[CNC*(M*i+j+n)+c]];
				TYPE inp[deg], out[deg];
				for (int c = 0; c < cnt; ++c)
					inp[c] = out[c] = vqsub(dat[c], bl[c]);
				inp[cnt] = out[cnt] = vqsub(par[0], bl[cnt]);
				inp[cnt+1] = out[cnt+1] = vqsub(par[1], bl[cnt+1]);
				finalp(out, deg);
				for (int c = 0; c < cnt; ++c)
					dat[c] = vqadd(inp[c], out[c]);
				par[0] = vqadd(inp[cnt], out[cnt]);
				par[1] = vqadd(inp[cnt+1], out[cnt+1]);
				for (int d = 0; d < deg; ++d)
					*bl++ = vclamp(out[d], -32, 31);
				if (i) {
					for (int n = 0; n < num; ++n)
						parity[M*(i-1)+j+n] = par[0].v[n];
				} else if (j) {
					for (int n = 0; n < num; ++n)
						parity[M*(q-1)-1+j+n] = par[0].v[n];
				} else {
					for (int n = 1; n < num; ++n)
						parity[M*(q-1)-1+j+n] = par[0].v[n];
				}
				for (int n = 0; n < num; ++n)
					parity[M*i+j+n] = par[1].v[n];
				for (int c = 0; c < cnt; ++c)
					for (int n = 0; n < num; ++n)
						data[pos[CNC*(M*i+j+n)+c]] = dat[c].v[n];
			}
		}
		//assert(bl <= bnl + BNL);
		//std::cerr << BNL - (bl - bnl) << std::endl;
	}
public:
	LDPCDecoder()
	{
		for (int i = 0; i < R; ++i)
			cnc[i] = 0;
		int bit_pos = 0;
		const int *row_ptr = TABLE::POS;
		for (int g = 0; TABLE::LEN[g]; ++g) {
			int bit_deg = TABLE::DEG[g];
			for (int r = 0; r < TABLE::LEN[g]; ++r) {
				int acc_pos[bit_deg];
				for (int d = 0; d < bit_deg; ++d)
					acc_pos[d] = row_ptr[d];
				row_ptr += bit_deg;
				for (int j = 0; j < M; ++j) {
					for (int d = 0; d < bit_deg; ++d) {
						int n = acc_pos[d];
						pos[CNC*n+cnc[n]++] = bit_pos;
					}
					++bit_pos;
					for (int d = 0; d < bit_deg; ++d)
						acc_pos[d] = (acc_pos[d] + q) % R;
				}
			}
		}
		uint16_t tmp[R * CNC];
		for (int i = 0; i < q; ++i)
			for (int j = 0; j < M; ++j)
				for (int c = 0; c < CNC; ++c)
					tmp[CNC*(M*i+j)+c] = pos[CNC*(q*j+i)+c];
		for (int i = 0; i < R * CNC; ++i)
			pos[i] = tmp[i];
	}
	int operator()(int8_t *data, int8_t *parity, int trials = 25)
	{
		for (int i = 0; i < BNL; ++i)
			bnl[i] = vzero<TYPE>();
		for (int i = 0; i < q; ++i)
			for (int j = 0; j < M; ++j)
				pty[M*i+j] = parity[q*j+i];
		while (bad(data, pty) && --trials >= 0)
			update(data, pty);
		for (int i = 0; i < q; ++i)
			for (int j = 0; j < M; ++j)
				parity[q*j+i] = pty[M*i+j];
		return trials;
	}
};

}

#endif
