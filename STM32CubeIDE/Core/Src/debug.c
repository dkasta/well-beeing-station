/*
 * debug.c
 *
 *  Created on: Feb 15, 2021
 *      Author: giova
 */

#include "debug.h"

#include "main.h"

#define USART_BUF_SIZE 100

RTC_DateTypeDef sDate;
RTC_TimeTypeDef sTime;
char uartBuf[USART_BUF_SIZE];

extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart1;

void Debug_PrintString(const char *str) {
	uint8_t len = 0;
	while (str[len++] != '\0')
		;
	HAL_UART_Transmit(&huart1, (uint8_t*) str, len, 100);
}

void Debug_PrintDateTime() {
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

	uartBuf[0] = '\t';
	uartBuf[1] = (sDate.Date / 16) + '0';
	uartBuf[2] = (sDate.Date % 16) + '0';
	uartBuf[3] = '.';
	uartBuf[4] = (sDate.Month / 16) + '0';
	uartBuf[5] = (sDate.Month % 16) + '0';
	uartBuf[6] = '.';
	uartBuf[7] = '2';
	uartBuf[8] = '0';
	uartBuf[9] = (sDate.Year / 16) + '0';
	uartBuf[10] = (sDate.Year % 16) + '0';
	uartBuf[11] = '@';
	uartBuf[12] = (sTime.Hours / 16) + '0';
	uartBuf[13] = (sTime.Hours % 16) + '0';
	uartBuf[14] = ':';
	uartBuf[15] = (sTime.Minutes / 16) + '0';
	uartBuf[16] = (sTime.Minutes % 16) + '0';
	uartBuf[17] = ':';
	uartBuf[18] = (sTime.Seconds / 16) + '0';
	uartBuf[19] = (sTime.Seconds % 16) + '0';
	uartBuf[20] = '\r';
	uartBuf[21] = '\n';
	HAL_UART_Transmit(&huart1, (uint8_t*) uartBuf, 22, 100);

}

void Debug_PrintTempHumid(float temp, float humid) {
	uint8_t len = sprintf(uartBuf, "\tTemp: %.02f°C, Humid: %.02f%%\r\n", temp, humid);
	HAL_UART_Transmit(&huart1, (uint8_t*) uartBuf, len, 100);
}
