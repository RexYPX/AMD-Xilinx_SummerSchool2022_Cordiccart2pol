#include "cordiccart2pol.h"

const data_t cordic_phase[] = {
	45.0, 26.565051177077986, 14.036243467926479, 7.125016348901799, 3.5763343749973515,
	1.7899106082460694, 0.8951737102110744, 0.4476141708605531, 0.22381050036853808,
	0.1119056770662069, 0.05595289189380367, 0.02797645261700368, 0.013988227142265015,
	0.006994113675352919, 0.003497056850704011, 0.0017485284269804495, 0.0008742642136937803,
	0.00043713210687233457, 0.00021856605343934782, 0.00010928302672007149, 5.464151336008544e-05,
	2.732075668004893e-05, 1.3660378340025243e-05, 6.830189170012718e-06, 3.4150945850063712e-06,
	1.7075472925031871e-06, 8.537736462515938e-07, 4.2688682312579694e-07, 2.1344341156289847e-07,
	1.0672170578144923e-07, 5.336085289072462e-08, 2.668042644536231e-08, 1.3340213222681154e-08,
	6.670106611340577e-09, 3.3350533056702886e-09, 1.6675266528351443e-09, 8.337633264175721e-10,
	4.1688166320878607e-10, 2.0844083160439303e-10, 1.0422041580219652e-10, 5.211020790109826e-11,
	2.605510395054913e-11, 1.3027551975274565e-11, 6.513775987637282e-12, 3.256887993818641e-12,
	1.6284439969093206e-12, 8.142219984546603e-13, 4.0711099922733015e-13, 2.0355549961366507e-13,
	1.0177774980683254e-13, 5.088887490341627e-14, 2.5444437451708134e-14, 1.2722218725854067e-14,
	6.3611093629270335e-15, 3.1805546814635168e-15, 1.5902773407317584e-15, 7.951386703658792e-16,
	3.975693351829396e-16, 1.987846675914698e-16, 9.93923337957349e-17, 4.969616689786745e-17,
	2.4848083448933725e-17, 1.2424041724466862e-17, 6.212020862233431e-18, 3.1060104311167156e-18,
	1.5530052155583578e-18, 7.765026077791789e-19, 3.8825130388958945e-19, 1.9412565194479472e-19,
	9.706282597239736e-20, 4.853141298619868e-20, 2.426570649309934e-20, 1.213285324654967e-20,
	6.066426623274835e-21, 3.0332133116374176e-21, 1.5166066558187088e-21, 7.583033279093544e-22,
	3.791516639546772e-22, 1.895758319773386e-22, 9.47879159886693e-23, 4.739395799433465e-23,
	2.3696978997167325e-23, 1.1848489498583662e-23, 5.924244749291831e-24, 2.9621223746459156e-24,
	1.4810611873229578e-24, 7.405305936614789e-25, 3.7026529683073945e-25, 1.8513264841536972e-25,
	9.256632420768486e-26, 4.628316210384243e-26, 2.3141581051921216e-26, 1.1570790525960608e-26,
	5.785395262980304e-27, 2.892697631490152e-27, 1.446348815745076e-27, 7.23174407872538e-28,
	3.61587203936269e-28, 1.807936019681345e-28, 9.039680098406725e-29};

const data_t cordic_tan[50] = {
	1, 5.0000000000e-01, 2.5000000000e-01, 1.2500000000e-01, 6.2500000000e-02, 3.1250000000e-02, 1.5625000000e-02,
	7.8125000000e-03, 3.9062500000e-03, 1.9531250000e-03, 9.7656250000e-04, 4.8828125000e-04, 2.4414062500e-04,
	1.2207031250e-04, 6.1035156250e-05, 3.0517578125e-05, 1.5258789062e-05, 7.6293945312e-06, 3.8146972656e-06,
	1.9073486328e-06, 9.5367431641e-07, 4.7683715820e-07, 2.3841857910e-07, 1.1920928955e-07, 5.9604644775e-08,
	2.9802322388e-08, 1.4901161194e-08, 7.4505805969e-09, 3.7252902985e-09, 1.8626451492e-09, 9.3132257462e-10,
	4.6566128731e-10, 2.3283064365e-10, 1.1641532183e-10, 5.8207660913e-11, 2.9103830457e-11, 1.4551915228e-11,
	7.2759576142e-12, 3.6379788071e-12, 1.8189894035e-12, 9.0949470177e-13, 4.5474735089e-13, 2.2737367544e-13,
	1.1368683772e-13, 5.6843418861e-14, 2.8421709430e-14, 1.4210854715e-14, 7.1054273576e-15, 3.5527136788e-15,
	1.7763568394e-15};

void cordiccart2pol(data_t x, data_t y, data_t *r, data_t *theta)
{
#pragma HLS INTERFACE mode = s_axilite port = x bundle = BUS_A
#pragma HLS INTERFACE mode = s_axilite port = y bundle = BUS_A
#pragma HLS INTERFACE mode = s_axilite port = r bundle = BUS_A
#pragma HLS INTERFACE mode = s_axilite port = theta bundle = BUS_A
#pragma HLS INTERFACE mode = s_axilite port = return bundle = CTRL

	const data_t cordic_K = 0.6072529350088814;
	const data_t pi_180 = 0.017453292519944;

	data_t current_theta = 0;
	data_t xx, yy;

	xx = y > 0 ? y : -y;
	yy = y > 0 ? -x : x;
	current_theta = y > 0 ? current_theta + 90 : current_theta - 90;

#pragma HLS PIPELINE II = 1
	for (int j = 0; j < NO_ITER; j++)
	{
		data_t temp_x = xx;
		data_t factor = cordic_tan[j];

		data_t diff_xx = yy * factor;
		data_t diff_yy = temp_x * factor;

		xx = (yy > 0) ? (xx + diff_xx) : (xx - diff_xx);
		current_theta = (yy > 0) ? (current_theta + cordic_phase[j]) : (current_theta - cordic_phase[j]);
		yy = (yy > 0) ? (yy - diff_yy) : (yy + diff_yy);
	}

	*theta = current_theta * pi_180;
	*r = xx * cordic_K;
}
