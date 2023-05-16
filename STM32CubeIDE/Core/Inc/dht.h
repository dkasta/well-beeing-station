/*
 * dht.h
 *
 *  Created on: Feb 6, 2021
 *      Author: giova
 */

#ifndef INC_DHT_H_
#define INC_DHT_H_

#include "main.h"

typedef struct {
	float temp;
	float humid;
} DHT_data_t;

typedef struct {
	GPIO_TypeDef *gpio;
	uint16_t pin;
} DHT_device_t;


void DHT_GetData(DHT_device_t *device, DHT_data_t *data);


#endif /* INC_DHT_H_ */
