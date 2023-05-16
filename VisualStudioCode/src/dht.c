/*
 * dht.c
 *
 *  Created on: Feb 6, 2021
 *      Author: giova
 */

#include "dht.h"

#ifdef RENODE_ENV

#define DEFAULT_TEMP 25
#define DEFAULT_HUMID 30

float temp = DEFAULT_TEMP;
float humid = DEFAULT_HUMID;

void DHT_GetData(DHT_device_t *device, DHT_data_t *data) {
	data->temp = temp;
	data->humid = humid;
}

#else

#include "dwt.h"

/* define DTH type, among TYPE_DHT11 and TYPE_DHT22 */
#define TYPE_DHT11

uint8_t rh_byte1, rh_byte2, temp_byte1, temp_byte2;
uint16_t checksum;
uint8_t presence = 0;

void DHT_Init(DHT_device_t *device) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = device->pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(device->gpio, &GPIO_InitStruct);

	HAL_GPIO_WritePin(device->gpio, device->pin, GPIO_PIN_RESET);

#if defined(TYPE_DHT11)
	DWT_Delay_us(18000);
#elif defined(TYPE_DHT22)
    DWT_DelayUs(1200);
#endif

	HAL_GPIO_WritePin(device->gpio, device->pin, GPIO_PIN_SET);
	DWT_Delay_us(20);

	GPIO_InitStruct.Pin = device->pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(device->gpio, &GPIO_InitStruct);
}

uint8_t DHT_CheckResponse(DHT_device_t *device) {
	uint8_t response = 0;
	DWT_Delay_us(40);

	if (HAL_GPIO_ReadPin(device->gpio, device->pin) == GPIO_PIN_RESET) {
		DWT_Delay_us(80);
		response =
				HAL_GPIO_ReadPin(device->gpio, device->pin) == GPIO_PIN_SET ? 1 : 0;
	}
	while (HAL_GPIO_ReadPin(device->gpio, device->pin) == GPIO_PIN_SET)
		; // wait for the pin to go low

	return response;
}

uint8_t DHT_ReadByte(DHT_device_t *device) {
	uint8_t i = 0;
	for (uint8_t j = 0; j < 8; j++) {
		while (HAL_GPIO_ReadPin(device->gpio, device->pin) == GPIO_PIN_RESET)
			;                                     // wait for the pin to go high
		DWT_Delay_us(40);                          // wait for 40 us
		if (HAL_GPIO_ReadPin(device->gpio, device->pin) == GPIO_PIN_RESET) // if the pin is low
				{
			i &= ~(1 << (7 - j)); // write 0
		} else
			i |= (1 << (7 - j)); // if the pin is high, write 1
		while (HAL_GPIO_ReadPin(device->gpio, device->pin) == GPIO_PIN_SET)
			; // wait for the pin to go low
	}
	return i;
}

void DHT_GetData(DHT_device_t *device, DHT_data_t *data) {
	DHT_Init(device);
	presence = DHT_CheckResponse(device);
	rh_byte1 = DHT_ReadByte(device);
	rh_byte2 = DHT_ReadByte(device);
	temp_byte1 = DHT_ReadByte(device);
	temp_byte2 = DHT_ReadByte(device);
	checksum = DHT_ReadByte(device);

	if (checksum == (rh_byte1 + rh_byte2 + temp_byte1 + temp_byte2)) {

#if defined(TYPE_DHT11)
		data->temp = temp_byte1;
		data->humid = rh_byte1;
#elif defined(TYPE_DHT22)
        data->temp = ((temp_byte1 << 8) | temp_byte2);
        data->humid = ((rh_byte1 << 8) | rh_byte2);
#endif
	}
}
#endif

