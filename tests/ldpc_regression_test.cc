/*
Regression Test for the Low-density parity-check Encoder and Decoder

Table entries below copied from:
https://www.etsi.org/deliver/etsi_en/302700_302799/302755/01.04.01_60/en_302755v010401p.pdf

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <random>
#include <cmath>
#include <cassert>
#include <chrono>
#include <algorithm>
#include <functional>
#include "ldpc_encoder.hh"
#if 0
#include "ldpc_decoder.hh"
#else
#include "ldpc_decoder2.hh"
#endif

struct DVB_T2_TABLE_A1
{
	static const int M = 360;
	static const int N = 64800;
	static const int K = 32400;
	static const int LINKS_MIN_CN = 6;
	static const int LINKS_MAX_CN = 7;
	static const int LINKS_TOTAL = 226799;
	static const int DEG_MAX = 8;
	static constexpr int DEG[] = {
		8, 3, 0
	};
	static constexpr int LEN[] = {
		36, 54, 0
	};
	static constexpr int POS[] = {
		54,	9318,	14392,	27561,	26909,	10219,	2534,	8597,
		55,	7263,	4635,	2530,	28130,	3033,	23830,	3651,
		56,	24731,	23583,	26036,	17299,	5750,	792,	9169,
		57,	5811,	26154,	18653,	11551,	15447,	13685,	16264,
		58,	12610,	11347,	28768,	2792,	3174,	29371,	12997,
		59,	16789,	16018,	21449,	6165,	21202,	15850,	3186,
		60,	31016,	21449,	17618,	6213,	12166,	8334,	18212,
		61,	22836,	14213,	11327,	5896,	718,	11727,	9308,
		62,	2091,	24941,	29966,	23634,	9013,	15587,	5444,
		63,	22207,	3983,	16904,	28534,	21415,	27524,	25912,
		64,	25687,	4501,	22193,	14665,	14798,	16158,	5491,
		65,	4520,	17094,	23397,	4264,	22370,	16941,	21526,
		66,	10490,	6182,	32370,	9597,	30841,	25954,	2762,
		67,	22120,	22865,	29870,	15147,	13668,	14955,	19235,
		68,	6689,	18408,	18346,	9918,	25746,	5443,	20645,
		69,	29982,	12529,	13858,	4746,	30370,	10023,	24828,
		70,	1262,	28032,	29888,	13063,	24033,	21951,	7863,
		71,	6594,	29642,	31451,	14831,	9509,	9335,	31552,
		72,	1358,	6454,	16633,	20354,	24598,	624,	5265,
		73,	19529,	295,	18011,	3080,	13364,	8032,	15323,
		74,	11981,	1510,	7960,	21462,	9129,	11370,	25741,
		75,	9276,	29656,	4543,	30699,	20646,	21921,	28050,
		76,	15975,	25634,	5520,	31119,	13715,	21949,	19605,
		77,	18688,	4608,	31755,	30165,	13103,	10706,	29224,
		78,	21514,	23117,	12245,	26035,	31656,	25631,	30699,
		79,	9674,	24966,	31285,	29908,	17042,	24588,	31857,
		80,	21856,	27777,	29919,	27000,	14897,	11409,	7122,
		81,	29773,	23310,	263,	4877,	28622,	20545,	22092,
		82,	15605,	5651,	21864,	3967,	14419,	22757,	15896,
		83,	30145,	1759,	10139,	29223,	26086,	10556,	5098,
		84,	18815,	16575,	2936,	24457,	26738,	6030,	505,
		85,	30326,	22298,	27562,	20131,	26390,	6247,	24791,
		86,	928,	29246,	21246,	12400,	15311,	32309,	18608,
		87,	20314,	6025,	26689,	16302,	2296,	3244,	19613,
		88,	6237,	11943,	22851,	15642,	23857,	15112,	20947,
		89,	26403,	25168,	19038,	18384,	8882,	12719,	7093,
		0,	14567,	24965,
		1,	3908,	100,
		2,	10279,	240,
		3,	24102,	764,
		4,	12383,	4173,
		5,	13861,	15918,
		6,	21327,	1046,
		7,	5288,	14579,
		8,	28158,	8069,
		9,	16583,	11098,
		10,	16681,	28363,
		11,	13980,	24725,
		12,	32169,	17989,
		13,	10907,	2767,
		14,	21557,	3818,
		15,	26676,	12422,
		16,	7676,	8754,
		17,	14905,	20232,
		18,	15719,	24646,
		19,	31942,	8589,
		20,	19978,	27197,
		21,	27060,	15071,
		22,	6071,	26649,
		23,	10393,	11176,
		24,	9597,	13370,
		25,	7081,	17677,
		26,	1433,	19513,
		27,	26925,	9014,
		28,	19202,	8900,
		29,	18152,	30647,
		30,	20803,	1737,
		31,	11804,	25221,
		32,	31683,	17783,
		33,	29694,	9345,
		34,	12280,	26611,
		35,	6526,	26122,
		36,	26165,	11241,
		37,	7666,	26962,
		38,	16290,	8480,
		39,	11774,	10120,
		40,	30051,	30426,
		41,	1335,	15424,
		42,	6865,	17742,
		43,	31779,	12489,
		44,	32120,	21001,
		45,	14508,	6996,
		46,	979,	25024,
		47,	4554,	21896,
		48,	7989,	21777,
		49,	4972,	20661,
		50,	6612,	2730,
		51,	12742,	4418,
		52,	29194,	595,
		53,	19267,	20113,
	};
};

struct DVB_T2_TABLE_A6
{
	static const int M = 360;
	static const int N = 64800;
	static const int K = 54000;
	static const int LINKS_MIN_CN = 21;
	static const int LINKS_MAX_CN = 22;
	static const int LINKS_TOTAL = 237599;
	static const int DEG_MAX = 13;
	static constexpr int DEG[] = {
		13, 3, 0
	};
	static constexpr int LEN[] = {
		15, 135, 0
	};
	static constexpr int POS[] = {
		0,	4362,	416,	8909,	4156,	3216,	3112,	2560,	2912,	6405,	8593,	4969,	6723,
		1,	2479,	1786,	8978,	3011,	4339,	9313,	6397,	2957,	7288,	5484,	6031,	10217,
		2,	10175,	9009,	9889,	3091,	4985,	7267,	4092,	8874,	5671,	2777,	2189,	8716,
		3,	9052,	4795,	3924,	3370,	10058,	1128,	9996,	10165,	9360,	4297,	434,	5138,
		4,	2379,	7834,	4835,	2327,	9843,	804,	329,	8353,	7167,	3070,	1528,	7311,
		5,	3435,	7871,	348,	3693,	1876,	6585,	10340,	7144,	5870,	2084,	4052,	2780,
		6,	3917,	3111,	3476,	1304,	10331,	5939,	5199,	1611,	1991,	699,	8316,	9960,
		7,	6883,	3237,	1717,	10752,	7891,	9764,	4745,	3888,	10009,	4176,	4614,	1567,
		8,	10587,	2195,	1689,	2968,	5420,	2580,	2883,	6496,	111,	6023,	1024,	4449,
		9,	3786,	8593,	2074,	3321,	5057,	1450,	3840,	5444,	6572,	3094,	9892,	1512,
		10,	8548,	1848,	10372,	4585,	7313,	6536,	6379,	1766,	9462,	2456,	5606,	9975,
		11,	8204,	10593,	7935,	3636,	3882,	394,	5968,	8561,	2395,	7289,	9267,	9978,
		12,	7795,	74,	1633,	9542,	6867,	7352,	6417,	7568,	10623,	725,	2531,	9115,
		13,	7151,	2482,	4260,	5003,	10105,	7419,	9203,	6691,	8798,	2092,	8263,	3755,
		14,	3600,	570,	4527,	200,	9718,	6771,	1995,	8902,	5446,	768,	1103,	6520,
		15,	6304,	7621,
		16,	6498,	9209,
		17,	7293,	6786,
		18,	5950,	1708,
		19,	8521,	1793,
		20,	6174,	7854,
		21,	9773,	1190,
		22,	9517,	10268,
		23,	2181,	9349,
		24,	1949,	5560,
		25,	1556,	555,
		26,	8600,	3827,
		27,	5072,	1057,
		28,	7928,	3542,
		29,	3226,	3762,
		0,	7045,	2420,
		1,	9645,	2641,
		2,	2774,	2452,
		3,	5331,	2031,
		4,	9400,	7503,
		5,	1850,	2338,
		6,	10456,	9774,
		7,	1692,	9276,
		8,	10037,	4038,
		9,	3964,	338,
		10,	2640,	5087,
		11,	858,	3473,
		12,	5582,	5683,
		13,	9523,	916,
		14,	4107,	1559,
		15,	4506,	3491,
		16,	8191,	4182,
		17,	10192,	6157,
		18,	5668,	3305,
		19,	3449,	1540,
		20,	4766,	2697,
		21,	4069,	6675,
		22,	1117,	1016,
		23,	5619,	3085,
		24,	8483,	8400,
		25,	8255,	394,
		26,	6338,	5042,
		27,	6174,	5119,
		28,	7203,	1989,
		29,	1781,	5174,
		0,	1464,	3559,
		1,	3376,	4214,
		2,	7238,	67,
		3,	10595,	8831,
		4,	1221,	6513,
		5,	5300,	4652,
		6,	1429,	9749,
		7,	7878,	5131,
		8,	4435,	10284,
		9,	6331,	5507,
		10,	6662,	4941,
		11,	9614,	10238,
		12,	8400,	8025,
		13,	9156,	5630,
		14,	7067,	8878,
		15,	9027,	3415,
		16,	1690,	3866,
		17,	2854,	8469,
		18,	6206,	630,
		19,	363,	5453,
		20,	4125,	7008,
		21,	1612,	6702,
		22,	9069,	9226,
		23,	5767,	4060,
		24,	3743,	9237,
		25,	7018,	5572,
		26,	8892,	4536,
		27,	853,	6064,
		28,	8069,	5893,
		29,	2051,	2885,
		0,	10691,	3153,
		1,	3602,	4055,
		2,	328,	1717,
		3,	2219,	9299,
		4,	1939,	7898,
		5,	617,	206,
		6,	8544,	1374,
		7,	10676,	3240,
		8,	6672,	9489,
		9,	3170,	7457,
		10,	7868,	5731,
		11,	6121,	10732,
		12,	4843,	9132,
		13,	580,	9591,
		14,	6267,	9290,
		15,	3009,	2268,
		16,	195,	2419,
		17,	8016,	1557,
		18,	1516,	9195,
		19,	8062,	9064,
		20,	2095,	8968,
		21,	753,	7326,
		22,	6291,	3833,
		23,	2614,	7844,
		24,	2303,	646,
		25,	2075,	611,
		26,	4687,	362,
		27,	8684,	9940,
		28,	4830,	2065,
		29,	7038,	1363,
		0,	1769,	7837,
		1,	3801,	1689,
		2,	10070,	2359,
		3,	3667,	9918,
		4,	1914,	6920,
		5,	4244,	5669,
		6,	10245,	7821,
		7,	7648,	3944,
		8,	3310,	5488,
		9,	6346,	9666,
		10,	7088,	6122,
		11,	1291,	7827,
		12,	10592,	8945,
		13,	3609,	7120,
		14,	9168,	9112,
		15,	6203,	8052,
		16,	3330,	2895,
		17,	4264,	10563,
		18,	10556,	6496,
		19,	8807,	7645,
		20,	1999,	4530,
		21,	9202,	6818,
		22,	3403,	1734,
		23,	2106,	9023,
		24,	6881,	3883,
		25,	3895,	2171,
		26,	4062,	6424,
		27,	3755,	9536,
		28,	4683,	2131,
		29,	7347,	8027,
	};
};

struct DVB_T2_TABLE_B7
{
	static const int M = 360;
	static const int N = 16200;
	static const int K = 13320;
	static const int LINKS_MIN_CN = 15;
	static const int LINKS_MAX_CN = 19;
	static const int LINKS_TOTAL = 49319;
	static const int DEG_MAX = 13;
	static constexpr int DEG[] = {
		13, 3, 0
	};
	static constexpr int LEN[] = {
		1, 36, 0
	};
	static constexpr int POS[] = {
		3,	2409,	499,	1481,	908,	559,	716,	1270,	333,	2508,	2264,	1702,	2805,
		4,	2447,	1926,
		5,	414,	1224,
		6,	2114,	842,
		7,	212,	573,
		0,	2383,	2112,
		1,	2286,	2348,
		2,	545,	819,
		3,	1264,	143,
		4,	1701,	2258,
		5,	964,	166,
		6,	114,	2413,
		7,	2243,	81,
		0,	1245,	1581,
		1,	775,	169,
		2,	1696,	1104,
		3,	1914,	2831,
		4,	532,	1450,
		5,	91,	974,
		6,	497,	2228,
		7,	2326,	1579,
		0,	2482,	256,
		1,	1117,	1261,
		2,	1257,	1658,
		3,	1478,	1225,
		4,	2511,	980,
		5,	2320,	2675,
		6,	435,	1278,
		7,	228,	503,
		0,	1885,	2369,
		1,	57,	483,
		2,	838,	1050,
		3,	1231,	1990,
		4,	1738,	68,
		5,	2392,	951,
		6,	163,	645,
		7,	2644,	1704,
	};
};

constexpr int DVB_T2_TABLE_A1::DEG[];
constexpr int DVB_T2_TABLE_A1::LEN[];
constexpr int DVB_T2_TABLE_A1::POS[];

constexpr int DVB_T2_TABLE_A6::DEG[];
constexpr int DVB_T2_TABLE_A6::LEN[];
constexpr int DVB_T2_TABLE_A6::POS[];

constexpr int DVB_T2_TABLE_B7::DEG[];
constexpr int DVB_T2_TABLE_B7::LEN[];
constexpr int DVB_T2_TABLE_B7::POS[];

#if 0
typedef DVB_T2_TABLE_A1 TABLE;
static const float QEF_SNR = -1.8;
static const int BCH_T = 12;
#endif
#if 0
typedef DVB_T2_TABLE_A6 TABLE;
static const float QEF_SNR = 2.2;
static const int BCH_T = 10;
#endif
#if 1
typedef DVB_T2_TABLE_B7 TABLE;
static const float QEF_SNR = 2.5;
static const int BCH_T = 12;
#endif

int main()
{
	const int TRIALS = 25;
	const int FACTOR = 2;
	const int BETA = 1;

	const int CODE_LEN = TABLE::N;
	const int DATA_LEN = TABLE::K;

	CODE::LDPCEncoder<TABLE> encode;
	CODE::LDPCDecoder<TABLE, BETA> decode;

	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<int> uniform;
	typedef std::normal_distribution<float> normal;

	int8_t *code = new int8_t[CODE_LEN];
	int8_t *orig = new int8_t[CODE_LEN];
	int8_t *noisy = new int8_t[CODE_LEN];
	float *symb = new float[CODE_LEN];

	float min_SNR = 20, min_mbs = 1000, max_mbs = 0;

	for (float SNR = -5; SNR <= 15; SNR += 0.1) {
		//float mean_signal = 0;
		float sigma_signal = 1;
		float mean_noise = 0;
		float sigma_noise = std::sqrt(sigma_signal * sigma_signal / (2 * std::pow(10, SNR / 10)));

		auto data = std::bind(uniform(0, 1), generator);
		auto awgn = std::bind(normal(mean_noise, sigma_noise), generator);

		for (int i = 0; i < DATA_LEN; ++i)
			code[i] = 1 - 2 * data();

		encode(code, code + DATA_LEN);

		for (int i = 0; i < CODE_LEN; ++i)
			orig[i] = code[i];

		for (int i = 0; i < CODE_LEN; ++i)
			symb[i] = code[i];

		for (int i = 0; i < CODE_LEN; ++i)
			symb[i] += awgn();

		// $LLR=log(\frac{p(x=+1|y)}{p(x=-1|y)})$
		// $p(x|\mu,\sigma)=\frac{1}{\sqrt{2\pi}\sigma}}e^{-\frac{(x-\mu)^2}{2\sigma^2}}$
		float DIST = 2; // BPSK
		float fact = DIST * FACTOR / (sigma_noise * sigma_noise);
		for (int i = 0; i < CODE_LEN; ++i)
			code[i] = std::min<float>(std::max<float>(std::nearbyint(fact * symb[i]), -128), 127);

		for (int i = 0; i < CODE_LEN; ++i)
			noisy[i] = code[i];

		auto start = std::chrono::system_clock::now();
		int count = decode(code, code + DATA_LEN, TRIALS);
		auto end = std::chrono::system_clock::now();
		auto usec = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		float mbs = (float)DATA_LEN / usec.count();
		int iters = count < 0 ? TRIALS : TRIALS - count;

		int awgn_errors = 0;
		for (int i = 0; i < CODE_LEN; ++i)
			awgn_errors += noisy[i] * orig[i] < 0;
		int quantization_erasures = 0;
		for (int i = 0; i < CODE_LEN; ++i)
			quantization_erasures += !noisy[i];
		int uncorrected_errors = 0;
		for (int i = 0; i < CODE_LEN; ++i)
			uncorrected_errors += code[i] * orig[i] <= 0;
		int decoder_errors = 0;
		for (int i = 0; i < CODE_LEN; ++i)
			decoder_errors += code[i] * orig[i] <= 0 && orig[i] * noisy[i] > 0;
		float bit_error_rate = (float)uncorrected_errors / (float)(CODE_LEN);

		if (uncorrected_errors <= BCH_T)
			min_SNR = std::min(min_SNR, SNR);
		min_mbs = std::min(min_mbs, mbs);
		max_mbs = std::max(max_mbs, mbs);

		if (0) {
			std::cerr << SNR << " Es/N0 => AWGN with standard deviation of " << sigma_noise << " and mean " << mean_noise << std::endl;
			std::cerr << awgn_errors << " errors caused by AWGN." << std::endl;
			std::cerr << quantization_erasures << " erasures caused by quantization." << std::endl;
			std::cerr << decoder_errors << " errors caused by decoder." << std::endl;
			std::cerr << uncorrected_errors << " errors uncorrected." << std::endl;
			std::cerr << bit_error_rate << " bit error rate." << std::endl;
			if (count < 0) {
				std::cerr << "decoder failed at converging to a code word!" << std::endl;
			} else {
				std::cerr << iters << " iterations were needed." << std::endl;
			}
			std::cerr << usec.count() << " microseconds to decode." << std::endl;
			std::cerr << mbs << " megabit per second." << std::endl;
		} else {
			std::cout << SNR << " " << bit_error_rate << " " << iters << " " << mbs << std::endl;
		}
	}

	delete[] code;
	delete[] orig;
	delete[] noisy;
	delete[] symb;

	std::cerr << "QEF at: " << min_SNR << " SNR, speed min: " << min_mbs << " Mb/s and speed max: " << max_mbs << " Mb/s." << std::endl;
	assert(min_SNR < QEF_SNR);
	std::cerr << "Low-density parity-check code regression test passed!" << std::endl;
	return 0;
}

