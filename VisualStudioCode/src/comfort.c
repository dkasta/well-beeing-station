#include "comfort.h"
#include <stdlib.h>

typedef struct {
	float x;
	float y;
} point_2d_t;

#define N_COMF_VERT 4
const point_2d_t comf_vert[N_COMF_VERT] = { { 17.8, 75 }, { 22, 68 },
		{ 24, 37 }, { 19.1, 38 } };

#define N_STILL_COMF_VERT 8
const point_2d_t still_comf_vert[N_STILL_COMF_VERT] = { { 16.8, 87 },
		{ 20.2, 81 }, { 24.9, 60 }, { 26.8, 30 }, { 25.9, 19 }, { 19.8, 20 }, {
				17.2, 37 }, { 15.9, 74 } };
const float comf_temp_low = 19.1;
const float comf_temp_high = 22;
const float comf_hum_low = 38;
const float comf_hum_high = 68;
const float still_comf_temp_middle = 20;
const float still_comf_temp_low = 17.2;
const float still_comf_temp_high = 24.9;
const float still_comf_hum_left_high = 74;
const float still_comf_hum_left_low = 37;
const float still_comf_hum_right_high = 60;
const float still_comf_hum_right_low = 30;

/**
 * @brief Returns 1 if point is contained by vertices, 0 otherwise
 *
 * @note This is an implementation of the pnpoly algorithm described at:
 *	https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
 *
 * @param number of vertices
 * @param array containing vertices
 * @param point to be tested
 * @retval containment (1: true; 0: false)
 */
int vert_contain_point(const unsigned int n_vert, const point_2d_t vert[],
		point_2d_t test) {
	int i;
	int j;
	int c;

	for (i = 0, j = n_vert - 1, c = 0; i < n_vert; j = i++) {
		if (((vert[i].y > test.y) != (vert[j].y > test.y)) &&
				(test.x < (vert[j].x - vert[i].x) * (test.y - vert[i].y) /
				(vert[j].y - vert[i].y) + vert[i].x))
			c = !c;
	}

	return c;
}

/**
  * @brief  Returns the comfort zone index, based on the
  *  		passed temperature and humidity values.
  *
  * @param  The float value of the temperature.
  * @param  The float value of the humidity.
  * @retval	comfort level
  */
comfort_level_t Comfort_GetIndex(float temperature, float humidity) {
	point_2d_t curr_point;

	curr_point.x = temperature;
	curr_point.y = humidity;

	if (vert_contain_point(N_COMF_VERT, comf_vert, curr_point))
		return COMFORTABLE;
	else if (vert_contain_point(N_STILL_COMF_VERT, still_comf_vert, curr_point))
		return STILL_COMFORTABLE;
	else
		return UNCOMFORTABLE;
}

/**
 * @brief Returns relationship between temperature and Comfortable zone
 *
 * @param float value of temperature in Celsius degrees
 * @retval 1 => hot; 0 => ok; -1 => cold
 */
int comfort_temp_is_comf(float temperature)
{
	if (temperature > comf_temp_high)
		return 1;

	if (temperature < comf_temp_low)
		return -1;

	return 0;
}

/**
 * @brief Returns relationship between humidity and Comfortable zone
 *
 * @param float value of relative humidity (percentage)
 * @retval 1 => wet; 0 => ok; -1 => dry
 */
int comfort_hum_is_comf(float humidity)
{
	if (humidity > comf_hum_high)
		return 1;

	if (humidity < comf_hum_low)
		return -1;

	return 0;
}

/**
 * @brief Returns relationship between temperature and Still Comfortable zone
 *
 * @param float value of temperature in Celsius degrees
 * @retval 1 => hot; 0 => ok; -1 => cold
 */
int comfort_temp_is_still_comf(float temperature)
{
	if (temperature > still_comf_temp_high)
		return 1;

	if (temperature < still_comf_temp_low)
		return -1;

	return 0;
}

/**
 * @brief Returns relationship between humidity and Still Comfortable zone
 *
 * @param float value of temperature in Celsius degrees
 * @param float value of relative humidity (percentage)
 * @retval 1 => wet; 0 => ok; -1 => dry
 */
int comfort_hum_is_still_comf(float temperature, float humidity)
{
	float thr_high;
	float thr_low;
	
	thr_high = (temperature > still_comf_temp_middle) ?
		still_comf_hum_right_high : still_comf_hum_left_high;
	thr_low = (temperature > still_comf_temp_middle) ?
		still_comf_hum_right_low : still_comf_hum_left_low;

	if (humidity > thr_high)
		return 1;

	if (humidity < thr_low)
		return -1;

	return 0;
}

