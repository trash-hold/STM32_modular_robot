#include "trig.h"
#include "stdint.h"
#include "math.h"

#define LUT_SIZE 90

const float LUT_sine[] = {  0.00000,  0.01745,  0.03490,  0.05234,  0.06976,  0.08716,  0.10453,  0.12187,  0.13917,  0.15643,  0.17365,  0.19081,  0.20791,  0.22495,  0.24192,  0.25882,  0.27564,  0.29237,  0.30902,  0.32557,  0.34202,  0.35837,  0.37461,  0.39073,  0.40674,  0.42262,  0.43837,  0.45399,  0.46947,  0.48481,  0.50000,  0.51504,  0.52992,  0.54464,  0.55919,  0.57358,  0.58779,  0.60182,  0.61566,  0.62932,  0.64279,  0.65606,  0.66913,  0.68200,  0.69466,  0.70711,  0.71934,  0.73135,  0.74314,  0.75471,  0.76604,  0.77715,  0.78801,  0.79864,  0.80902,  0.81915,  0.82904,  0.83867,  0.84805,  0.85717,  0.86603,  0.87462,  0.88295,  0.89101,  0.89879,  0.90631,  0.91355,  0.92050,  0.92718,  0.93358,  0.93969,  0.94552,  0.95106,  0.95630,  0.96126,  0.96593,  0.97030,  0.97437,  0.97815,  0.98163,  0.98481,  0.98769,  0.99027,  0.99255,  0.99452,  0.99619,  0.99756,  0.99863,  0.99939,  0.99985,  1.00000 };

const float LUT_tan[] = {  0.0000,  0.0175,  0.0349,  0.0524,  0.0699,  0.0875,  0.1051,  0.1228,  0.1405,  0.1584,  0.1763,  0.1944,  0.2126,  0.2309,  0.2493,  0.2679,  0.2867,  0.3057,  0.3249,  0.3443,  0.3640,  0.3839,  0.4040,  0.4245,  0.4452,  0.4663,  0.4877,  0.5095,  0.5317,  0.5543,  0.5774,  0.6009,  0.6249,  0.6494,  0.6745,  0.7002,  0.7265,  0.7536,  0.7813,  0.8098,  0.8391,  0.8693,  0.9004,  0.9325,  0.9657,  1.0000,  1.0355,  1.0724,  1.1106,  1.1504,  1.1918,  1.2349,  1.2799,  1.3270,  1.3764,  1.4281,  1.4826,  1.5399,  1.6003,  1.6643,  1.7321,  1.8040,  1.8807,  1.9626,  2.0503,  2.1445,  2.2460,  2.3559,  2.4751,  2.6051,  2.7475,  2.9042,  3.0777,  3.2709,  3.4874,  3.7321,  4.0108,  4.3315,  4.7046,  5.1446,  5.6713,  6.3138,  7.1154,  8.1443,  9.5144,  11.4301,  14.3007,  19.0811,  28.6363,  57.2900,  10000};


// Find x that satisfies f(x) = value
static float interpolate_x(float high_val, float low_val, float value, int higher_index)
{
	float a = high_val - low_val;
	float b = high_val - higher_index * a;

	if( a == 0)
		return high_val;

	return ((value - b) / a);
}

// Find y that satisfies f(x)
static float interpolate_y(float high_val, float low_val, float x, int higher_index)
{
	float a = high_val - low_val;
	float b = high_val - higher_index * a;

	return (a*x + b);
}


static float LUT_Interpolate_X(float val, float* LUT_table)
{
	// Find sine and interpolate
	for(uint16_t i = 0; i < LUT_SIZE; i++)
	{
		if(val == *(LUT_table + i))
			return (float) i;

		else if( val < *(LUT_table + i))
		{
			// Interpolate between two points
			float big_angle = *(LUT_table + i);
			float small_angle = *(LUT_table + i - 1) ;  // Dont have to check for index because of first two conditions

			return (interpolate_x(big_angle, small_angle, val, i));
		}
	}
}

static float LUT_Interpolate_Y(float angle, float* LUT_table)
{
	// Casting causes to round down
	uint32_t angle_int = (uint32_t) angle;

	if(angle_int > 90)
		return *(LUT_table + LUT_SIZE - 1);

	// Linearize and find interpolated value
	float value = interpolate_y(*(LUT_table + angle_int + 1), *(LUT_table + angle_int), angle, angle_int + 1);
	return value;
}

// Return x for which sin(x) = val
float LUT_ASine(float val)
{
	if(val > 90)
			return 100;

	float sign = 1.0;
	// Return for val that's out of bounds
	if(val < 0)
	{
		val = -val;
		sign = -1.0;
	}

	// Find sine and interpolate
	for(uint16_t i = 0; i < LUT_SIZE; i++)
	{
		if(val == LUT_sine[i])
			return (float) i;

		else if( val < LUT_sine[i])
		{
			// Interpolate between two points
			float big_angle = LUT_sine[i];
			float small_angle = LUT_sine[i - 1];  // Dont have to check for index because of first two conditions

			float res = interpolate_x(big_angle, small_angle, val, i);
			res = res * sign;
			return res;
		}
	}
}

float LUT_Sine(float angle)
{
	float sign = 1.0;

	if (angle < 0)
	{
		angle = -angle;
		sign = -1.0;
	}

	// Casting causes to round down
	uint32_t angle_int = (uint32_t) angle;

	if(angle_int > 90)
		return LUT_sine[LUT_SIZE - 1];

	// Linearize and find interpolated value
	float value = interpolate_y(LUT_sine[angle_int + 1], LUT_sine[angle_int], angle, angle_int + 1);
	value = value * sign;	// include sign of the function

	return value;
}

float LUT_Tan(float angle)
{
	float sign = 1.0;

	if (angle < 0)
	{
		angle = -angle;
		sign = -1.0;
	}

	// Casting causes to round down
	uint32_t angle_int = (uint32_t) angle;

	if(angle_int > 90)
		return LUT_tan[LUT_SIZE - 1];

	// Linearize and find interpolated value
	float value = interpolate_y(LUT_tan[angle_int + 1], LUT_tan[angle_int], angle, angle_int + 1);
	value = value * sign;	// include sign of the function

	return value;
}

// Find x such that tan(x) = val
float LUT_ATan(float val)
{
	if(val > LUT_tan[89])
			return 100;

	float sign = 1.0;

	// Return for val that's out of bounds
	if(val < 0)
	{
		val = -val;
		sign = -1.0;
	}

	// Find sine and interpolate
	for(uint16_t i = 0; i < LUT_SIZE; i++)
	{
		if(val == LUT_tan[i])
			return (float) i;

		else if( val < LUT_tan[i])
		{
			// Interpolate between two points
			float big_angle = LUT_tan[i];
			float small_angle = LUT_tan[i - 1];  // Dont have to check for index because of first two conditions

			float res = interpolate_x(big_angle, small_angle, val, i);
			res = res * sign;

			return res;
		}
	}
}

void GetTiltAngles(float *abc_buffer, int16_t *xyz_acc)
{
	float a_yz, a_xy, a_xz;
	float x_2, y_2, z_2;

	// Helper variables
	x_2 = (*(xyz_acc)) * (*(xyz_acc));
	y_2 = (*(xyz_acc + 1)) * (*(xyz_acc + 1));
	z_2 = (*(xyz_acc + 2)) * (*(xyz_acc + 2));

	a_yz = sqrtf(y_2 + z_2);
	a_xy = sqrtf(x_2 + y_2);
	a_xz = sqrtf(x_2 + z_2);

	*(abc_buffer) = LUT_ATan( ((*(xyz_acc)) / a_yz ) );
	*(abc_buffer + 1) = LUT_ATan( ((*(xyz_acc + 1)) / a_xz ) );
	*(abc_buffer + 2) = LUT_ATan( (a_xy / (*(xyz_acc + 2))) );
}




