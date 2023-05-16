/*
 * RTC.h
 *
 *  Created on: Feb 15, 2021
 *      Author: giova
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "main.h"

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);
void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc);

#endif /* INC_RTC_H_ */
