/*
 * statusLED.c
 *
 *  Created on: Feb 6, 2021
 *      Author: giova
 */

#include "statusLED.h"

void StatusLED_Init(StatusLED_device_t *device) {

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin = device->pin_low;
	HAL_GPIO_Init(device->gpio, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = device->pin_medium;
	HAL_GPIO_Init(device->gpio, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = device->pin_high;
	HAL_GPIO_Init(device->gpio, &GPIO_InitStruct);

	/* Switch on medium at init */
	HAL_GPIO_WritePin(device->gpio, device->pin_low, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(device->gpio, device->pin_medium, GPIO_PIN_SET);
	HAL_GPIO_WritePin(device->gpio, device->pin_high, GPIO_PIN_RESET);
}

void StatusLED_setLevel(StatusLED_device_t *device, StatusLED_level_t level) {
	HAL_GPIO_WritePin(device->gpio, device->pin_low,
			level == StatusLED_LOW ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(device->gpio, device->pin_medium,
			level == StatusLED_MEDIUM ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(device->gpio, device->pin_high,
			level == StatusLED_HIGH ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
