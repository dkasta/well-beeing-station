/*
 * relay.h
 *
 *  Created on: Feb 6, 2021
 *      Author: giova
 */

#ifndef INC_RELAY_H_
#define INC_RELAY_H_

#include "main.h"

typedef struct {
	GPIO_TypeDef *gpio;
	uint16_t pin;
	uint8_t reversed;
} Relay_device_t;

void Relay_Init(Relay_device_t *device);
void Relay_Switch(Relay_device_t *device);
void Relay_SwitchOn(Relay_device_t *device);
void Relay_SwitchOff(Relay_device_t *device);
void Relay_SwitchStatus(Relay_device_t *device, GPIO_PinState status);

#endif /* INC_RELAY_H_ */
