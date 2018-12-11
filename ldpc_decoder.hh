/*
LDPC SISO layered decoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef LDPC_DECODER_HH
#define LDPC_DECODER_HH

#include <algorithm>
#include "exclusive_reduce.hh"

namespace CODE {

template <typename TABLE, int BETA>
class LDPCDecoder
{
	static const int M = TABLE::M;
	static const int N = TABLE::N;
	static const int K = TABLE::K;
	static const int R = N-K;
	static const int q = R/M;
	static const int CNC = TABLE::LINKS_MAX_CN - 2;
	static const int LT = TABLE::LINKS_TOTAL;

	int8_t bnl[LT];
	int8_t pty[R];
	uint16_t pos[R * CNC];
	uint8_t cnc[R];

	static int8_t sqadd(int8_t a, int8_t b)
	{
		int16_t x = int16_t(a) + int16_t(b);
		x = std::min<int16_t>(std::max<int16_t>(x, -128), 127);
		return x;
	}
	static int8_t sqsub(int8_t a, int8_t b)
	{
		int16_t x = int16_t(a) - int16_t(b);
		x = std::min<int16_t>(std::max<int16_t>(x, -128), 127);
		return x;
	}
	static uint8_t sqsubu(uint8_t a, uint8_t b)
	{
		int16_t x = int16_t(a) - int16_t(b);
		x = std::max<int16_t>(x, 0);
		return x;
	}
	static int8_t smin(int8_t a, int8_t b)
	{
		return std::min(a, b);
	}
	static int8_t sclamp(int8_t x, int8_t a, int8_t b)
	{
		return std::min(std::max(x, a), b);
	}
	static int8_t seor(int8_t a, int8_t b)
	{
		return a ^ b;
	}
	static int8_t sqabs(int8_t a)
	{
		return std::abs(std::max<int8_t>(a, -127));
	}
	static int8_t ssign(int8_t a, int8_t b)
	{
		return ((b > 0) - (b < 0)) * a;
	}

	bool bad(int8_t *data, int8_t *parity)
	{
		{
			int cnt = cnc[0];
			{
				int8_t cnv = ssign(1, parity[0]);
				for (int c = 0; c < cnt; ++c)
					cnv = ssign(cnv, data[pos[c]]);
				if (cnv <= 0)
					return true;
			}
			for (int j = 1; j < M; ++j) {
				int8_t cnv = ssign(ssign(1, parity[j+(q-1)*M-1]), parity[j]);
				for (int c = 0; c < cnt; ++c)
					cnv = ssign(cnv, data[pos[CNC*j+c]]);
				if (cnv <= 0)
					return true;
			}
		}
		for (int i = 1; i < q; ++i) {
			int cnt = cnc[i];
			for (int j = 0; j < M; ++j) {
				int8_t cnv = ssign(ssign(1, parity[M*(i-1)+j]), parity[M*i+j]);
				for (int c = 0; c < cnt; ++c)
					cnv = ssign(cnv, data[pos[CNC*(M*i+j)+c]]);
				if (cnv <= 0)
					return true;
			}
		}
		return false;
	}
	void finalp(int8_t *links, int cnt)
	{
		int8_t mags[cnt], mins[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = sqsubu(sqabs(links[i]), BETA);
		exclusive_reduce(mags, mins, cnt, smin);

		int8_t signs[cnt];
		exclusive_reduce(links, signs, cnt, seor);
		for (int i = 0; i < cnt; ++i)
			signs[i] |= 127;

		for (int i = 0; i < cnt; ++i)
			links[i] = ssign(mins[i], signs[i]);
	}
	void update(int8_t *data, int8_t *parity)
	{
		int8_t *bl = bnl;
		{
			int cnt = cnc[0];
			{
				int deg = cnt + 1;
				int8_t inp[deg], out[deg];
				for (int c = 0; c < cnt; ++c)
					inp[c] = out[c] = sqsub(data[pos[c]], bl[c]);
				inp[cnt] = out[cnt] = sqsub(parity[0], bl[cnt]);
				finalp(out, deg);
				for (int c = 0; c < cnt; ++c)
					data[pos[c]] = sqadd(inp[c], out[c]);
				parity[0] = sqadd(inp[cnt], out[cnt]);
				for (int d = 0; d < deg; ++d)
					*bl++ = sclamp(out[d], -32, 31);
			}
			int deg = cnt + 2;
			for (int j = 1; j < M; ++j) {
				int8_t inp[deg], out[deg];
				for (int c = 0; c < cnt; ++c)
					inp[c] = out[c] = sqsub(data[pos[CNC*j+c]], bl[c]);
				inp[cnt] = out[cnt] = sqsub(parity[j+(q-1)*M-1], bl[cnt]);
				inp[cnt+1] = out[cnt+1] = sqsub(parity[j], bl[cnt+1]);
				finalp(out, deg);
				for (int c = 0; c < cnt; ++c)
					data[pos[CNC*j+c]] = sqadd(inp[c], out[c]);
				parity[j+(q-1)*M-1] = sqadd(inp[cnt], out[cnt]);
				parity[j] = sqadd(inp[cnt+1], out[cnt+1]);
				for (int d = 0; d < deg; ++d)
					*bl++ = sclamp(out[d], -32, 31);
			}
		}
		for (int i = 1; i < q; ++i) {
			int cnt = cnc[i];
			int deg = cnt + 2;
			for (int j = 0; j < M; ++j) {
				int8_t inp[deg], out[deg];
				for (int c = 0; c < cnt; ++c)
					inp[c] = out[c] = sqsub(data[pos[CNC*(M*i+j)+c]], bl[c]);
				inp[cnt] = out[cnt] = sqsub(parity[M*(i-1)+j], bl[cnt]);
				inp[cnt+1] = out[cnt+1] = sqsub(parity[M*i+j], bl[cnt+1]);
				finalp(out, deg);
				for (int c = 0; c < cnt; ++c)
					data[pos[CNC*(M*i+j)+c]] = sqadd(inp[c], out[c]);
				parity[M*(i-1)+j] = sqadd(inp[cnt], out[cnt]);
				parity[M*i+j] = sqadd(inp[cnt+1], out[cnt+1]);
				for (int d = 0; d < deg; ++d)
					*bl++ = sclamp(out[d], -32, 31);
			}
		}
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
		for (int i = 0; i < LT; ++i)
			bnl[i] = 0;
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
