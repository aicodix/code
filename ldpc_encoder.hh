/*
LDPC SISO encoder

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef LDPC_ENCODER_HH
#define LDPC_ENCODER_HH

namespace CODE {

template <typename TABLE>
class LDPCEncoder
{
	static const int M = TABLE::M;
	static const int N = TABLE::N;
	static const int K = TABLE::K;
	static const int R = N-K;
	static const int q = R/M;
	static const int CNC = TABLE::LINKS_MAX_CN - 2;

	uint16_t pos[R * CNC];
	uint8_t cnc[R];
public:
	LDPCEncoder()
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
	}
	void operator()(int8_t *data, int8_t *parity)
	{
		int8_t tmp = 1;
		for (int i = 0; i < R; ++i) {
			for (int j = 0; j < cnc[i]; ++j)
				tmp *= data[pos[CNC*i+j]];
			parity[i] = tmp;
		}
	}
};

}

#endif
