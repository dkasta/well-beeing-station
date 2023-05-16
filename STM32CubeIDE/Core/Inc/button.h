/*
 * button.h
 *
 *  Created on: Feb 8, 2021
 *      Author: giova
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#ifdef RENODE_ENV
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
#else
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#endif

#endif /* INC_BUTTON_H_ */
