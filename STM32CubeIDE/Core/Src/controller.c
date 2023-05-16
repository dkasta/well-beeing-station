/*
 * controller.c
 *
 *  Created on: Feb 9, 2021
 *      Author: giova
 */

#include "controller.h"

#include <stdlib.h>

#include "main.h"
#include "dht.h"
#include "relay.h"
#include "statusLED.h"
#include "comfort.h"
#include "debug.h"
#ifndef RENODE_ENV
#include "dwt.h"
#endif

#define CONTROLLER_GPIO GPIOC
#define INIT_CONTROLLER_GPIO() __HAL_RCC_GPIOC_CLK_ENABLE()

/* DHT defines */
#define DHT_IN_PIN GPIO_PIN_4
#define DHT_OUT_PIN GPIO_PIN_5

/* Relays defines*/
#define WIN_PIN GPIO_PIN_6
#define HEAT_PIN GPIO_PIN_7
#define COOL_PIN GPIO_PIN_8
#define HUM_PIN GPIO_PIN_11
#define DEHUM_PIN GPIO_PIN_12

/* Output defines */
#define LEDSTATUS_PIN_LOW GPIO_PIN_0
#define LEDSTATUS_PIN_MEDIUM GPIO_PIN_1
#define LEDSTATUS_PIN_HIGH GPIO_PIN_2

/* DHT sensors */
DHT_device_t dht_in;
DHT_device_t dht_out;

/* Relays devices*/
Relay_device_t win_relay;
Relay_device_t heat_relay;
Relay_device_t cool_relay;
Relay_device_t hum_relay;
Relay_device_t dehum_relay;

/* Output devices */
StatusLED_device_t statusLED_device;

Controller_mode_t current_mode;
extern UART_HandleTypeDef huart1;

void Controller_Init(void) {

	INIT_CONTROLLER_GPIO();

#ifndef RENODE_ENV
	DWT_Delay_Init();
#endif

	/*DHT Init */
	dht_in.gpio = CONTROLLER_GPIO;
	dht_in.pin = DHT_IN_PIN;
	dht_out.gpio = CONTROLLER_GPIO;
	dht_out.pin = DHT_OUT_PIN;

	/*Relays Init */
	win_relay.gpio = CONTROLLER_GPIO;
	win_relay.pin = WIN_PIN;
	win_relay.reversed = 1;
	heat_relay.gpio = CONTROLLER_GPIO;
	heat_relay.pin = HEAT_PIN;
	heat_relay.reversed = 0;
	cool_relay.gpio = CONTROLLER_GPIO;
	cool_relay.pin = COOL_PIN;
	cool_relay.reversed = 1;
	hum_relay.gpio = CONTROLLER_GPIO;
	hum_relay.pin = HUM_PIN;
	hum_relay.reversed = 0;
	dehum_relay.gpio = CONTROLLER_GPIO;
	dehum_relay.pin = DEHUM_PIN;
	dehum_relay.reversed = 0;
	Relay_Init(&win_relay);
	Relay_Init(&heat_relay);
	Relay_Init(&cool_relay);
	Relay_Init(&hum_relay);
	Relay_Init(&dehum_relay);

	/*StatusLED Init */
	statusLED_device.gpio = CONTROLLER_GPIO;
	statusLED_device.pin_low = LEDSTATUS_PIN_LOW;
	statusLED_device.pin_medium = LEDSTATUS_PIN_MEDIUM;
	statusLED_device.pin_high = LEDSTATUS_PIN_HIGH;
	StatusLED_Init(&statusLED_device);
}

Controller_mode_t Controller_GetMode() {
	return current_mode;
}

void Controller_SetMode(Controller_mode_t mode) {
	current_mode = mode;
}

void Controller_Handle() {
	DHT_data_t dht_data_in;
	DHT_data_t dht_data_out;
	comfort_level_t comfort;
	GPIO_PinState win_status = GPIO_PIN_RESET;
	GPIO_PinState heat_status = GPIO_PIN_RESET;
	GPIO_PinState cool_status = GPIO_PIN_RESET;
	GPIO_PinState hum_status = GPIO_PIN_RESET;
	GPIO_PinState dehum_status = GPIO_PIN_RESET;
	StatusLED_level_t led_level = StatusLED_OFF;
	float temp_delta;
	float hum_delta;
	int temp_c;
	int hum_c;
	int temp_sc;
	int hum_sc;

	if (current_mode == CTRL_MODE_OFF || current_mode == CTRL_MODE_ERR) {
		led_level = StatusLED_OFF;
		Debug_PrintString("Comfort Station is OFF.\r\n");
	} else {
		Debug_PrintString("Comfort Station is ON:\r\n");
		Debug_PrintDateTime();
		DHT_GetData(&dht_in, &dht_data_in);
		Debug_PrintTempHumid(dht_data_in.temp, dht_data_in.humid);

		comfort = Comfort_GetIndex(dht_data_in.temp, dht_data_in.humid);
		if (comfort == COMFORTABLE) {
			led_level = StatusLED_HIGH;
			Debug_PrintString("\tEnvironment is Comfortable\r\n");
		}

		else {
			if (comfort == STILL_COMFORTABLE) {
				Debug_PrintString("\tEnvironment is Still Comfortable\r\n");
				led_level = StatusLED_MEDIUM;
			} else {
				Debug_PrintString("\tEnvironment is Not Comfortable\r\n");
				led_level = StatusLED_LOW;
			}

			temp_c = comfort_temp_is_comf(dht_data_in.temp);
			hum_c = comfort_hum_is_comf(dht_data_in.humid);
			if (current_mode == CTRL_MODE_HQ) {
				Debug_PrintString("\tMode: High Quality\r\n");
				// enable all devices which help going toward comfortable
				if (temp_c > 0) {
					cool_status = GPIO_PIN_SET;
					Debug_PrintString("\tCooling System activated\r\n");
				} else if (temp_c < 0) {
					heat_status = GPIO_PIN_SET;
					Debug_PrintString("\tHeating System activated\r\n");
				}
				if (hum_c > 0) {
					dehum_status = GPIO_PIN_SET;
					Debug_PrintString("\tDehumidifier activated\r\n");
				} else if (hum_c < 0) {
					hum_status = GPIO_PIN_SET;
					Debug_PrintString("\tHumidifier activated\r\n");
				}
			} else if (current_mode == CTRL_MODE_ECO) {
				DHT_GetData(&dht_out, &dht_data_out);
				temp_delta = dht_data_out.temp - dht_data_in.temp;
				hum_delta = dht_data_out.humid - dht_data_in.humid;
				Debug_PrintString("\tMode: ECO\r\n");
				if (((temp_c * temp_delta) <= 0)
						&& ((hum_c * hum_delta) <= 0)) {
					// open windows when both deltas are convenient
					// (opposite signs) or do not cause any damage
					// (zero)
					win_status = GPIO_PIN_SET;
					Debug_PrintString("\tWindows System activated\r\n");
				} else if (comfort == UNCOMFORTABLE) {
					temp_sc = comfort_temp_is_still_comf(dht_data_in.temp);
					hum_sc = comfort_hum_is_still_comf(dht_data_in.temp,
							dht_data_in.humid);

					if ((temp_sc > 0) || ((hum_c > 0) && (temp_sc == 0))) {
						// activate dehumidifier if humidity is
						// critically wet or temperature is not
						// critical and humidity is wet
						dehum_status = GPIO_PIN_SET;
						Debug_PrintString("\tDehumidifier activated\r\n");
					} else if ((hum_sc < 0)
							|| ((hum_c < 0) && (temp_sc == 0))) {
						// activate humidifier if humidity is
						// critically dry or temperature is not
						// critical and humidity is dry
						hum_status = GPIO_PIN_SET;
						Debug_PrintString("\tHumidifier activated\r\n");
					}

					// activate cooler/heater if temperature is critical
					if (temp_sc > 0) {
						cool_status = GPIO_PIN_SET;
						Debug_PrintString("\tCooling System activated\r\n");
					} else if (temp_sc < 0) {
						heat_status = GPIO_PIN_SET;
						Debug_PrintString("\tHeating System activated\r\n");
					}
				}
			}
		}
	}
	Relay_SwitchStatus(&win_relay, win_status);
	Relay_SwitchStatus(&heat_relay, heat_status);
	Relay_SwitchStatus(&cool_relay, cool_status);
	Relay_SwitchStatus(&hum_relay, hum_status);
	Relay_SwitchStatus(&dehum_relay, dehum_status);
	StatusLED_setLevel(&statusLED_device, led_level);
	Debug_PrintString("\r\n");
}

