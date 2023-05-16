/*
 * button.c
 *
 *  Created on: Feb 8, 2021
 *      Author: giova
 */
#include "button.h"

#include "debug.h"
#include "controller.h"

static inline void userButtonPressed() {
	Controller_mode_t currentMode = Controller_GetMode();
	if (currentMode == CTRL_MODE_OFF) {
		Controller_SetMode(CTRL_MODE_HQ);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); /* Reverse logic for UserLed (GPIO A1) */
		Debug_PrintString("HQ Mode selected\r\n\n");
	} else if (currentMode == CTRL_MODE_HQ) {
		Controller_SetMode(CTRL_MODE_ECO);
		Debug_PrintString("ECO Mode selected\r\n\n");
	} else if (currentMode == CTRL_MODE_ECO) {
		Controller_SetMode(CTRL_MODE_OFF);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); /* Reverse logic for UserLed (GPIO A1) */
		Debug_PrintString("OFF Mode selected\r\n\n");
	}
}

#ifdef RENODE_ENV
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_0) {
		/* GPIO_EXTI Interrupt (Rising Edge) on PIN0 is used to sense button pressure.
		 * With Renode environment it is used directly, because there is no bounce effect.
		 **/
		userButtonPressed();
	} else {
		__NOP();
	}
}

#else

extern TIM_HandleTypeDef htim2;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_0) {
		/* GPIO_EXTI Interrupt (Rising Edge) on PIN0 is used to sense button pressure.
		 * Basically it disable further interrupts and starts TIM2 (that effectively
		 * measures the PIN status after 100ms) to avoid bouncing effect.
		 **/
		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
		HAL_TIM_Base_Start_IT(&htim2);
	} else {
		__NOP();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM2) {
		/* TIM2 Interrupt is used to check PIN0 level after 100ms to avoid bouncing effect.
		 * If it is still HIGH, the button is really being pressed and the current mode is switched.
		 * In any case, it stops and reset itself and re-enable GPIO_EXTI interrupts.
		 **/
		HAL_TIM_Base_Stop_IT(&htim2);
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
			/* Switch to next mode if current is different from
			 * CTRL_MODE_ERR, setting the ON/OFF LED according to it.
			 **/
			userButtonPressed();
		}
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	} else {
		__NOP();
	}
}

#endif

