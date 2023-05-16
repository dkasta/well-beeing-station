#ifndef INC_COMFORT_H_
#define INC_COMFORT_H_

#include "main.h"

typedef enum {
	COMFORTABLE,
	STILL_COMFORTABLE,
	UNCOMFORTABLE
} comfort_level_t;

comfort_level_t Comfort_GetIndex(float temperature, float humidity);
int comfort_temp_is_comf(float temperature);
int comfort_hum_is_comf(float humidity);
int comfort_temp_is_still_comf(float temperature);
int comfort_hum_is_still_comf(float temperature, float humidity);

#endif /* INC_COMFORT_H_ */

