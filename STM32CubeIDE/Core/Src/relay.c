/*
 * relay.c
 *
 *  Created on: Feb 6, 2021
 *      Author: giova
 */

#include "relay.h"

void Relay_Init(Relay_device_t *device) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = device->pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(device->gpio, &GPIO_InitStruct);

	/* Switch off at init */
	HAL_GPIO_WritePin(device->gpio, device->pin,
			device->reversed ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void Relay_Switch(Relay_device_t *device) {
	HAL_GPIO_TogglePin(device->gpio, device->pin);
}

void Relay_SwitchStatus(Relay_device_t *device, GPIO_PinState status) {
	HAL_GPIO_WritePin(device->gpio, device->pin, status);
}

void Relay_SwitchOn(Relay_device_t *device) {
	HAL_GPIO_WritePin(device->gpio, device->pin,
			device->reversed ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Relay_SwitchOff(Relay_device_t *device) {
	HAL_GPIO_WritePin(device->gpio, device->pin,
			device->reversed ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

