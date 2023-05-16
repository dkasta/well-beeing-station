/*
 * statusLED.h
 *
 *  Created on: Feb 6, 2021
 *      Author: giova
 */

#ifndef INC_STATUSLED_H_
#define INC_STATUSLED_H_

#include "main.h"

typedef struct {
	GPIO_TypeDef *gpio;
	uint16_t pin_low;
	uint16_t pin_medium;
	uint16_t pin_high;
} StatusLED_device_t;


typedef enum {
	StatusLED_OFF,
	StatusLED_LOW,
	StatusLED_MEDIUM,
	StatusLED_HIGH
} StatusLED_level_t;

void StatusLED_Init(StatusLED_device_t *device);
void StatusLED_setLevel(StatusLED_device_t *device, StatusLED_level_t level);

#endif /* INC_STATUSLED_H_ */
