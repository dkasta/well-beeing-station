/*
 * RTC.c
 *
 *  Created on: Feb 15, 2021
 *      Author: giova
 */

#include "rtc.h"
#include "debug.h"
#include "controller.h"

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
	Debug_PrintString("Morning Alarm fired.\r\n\n");
	Controller_SetMode(CTRL_MODE_HQ);
}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc) {
	Debug_PrintString("Night Alarm fired.\r\n\n");
	Controller_SetMode(CTRL_MODE_ECO);
}
