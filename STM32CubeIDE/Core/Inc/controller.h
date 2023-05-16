/*
 * controller.h
 *
 *  Created on: Feb 9, 2021
 *      Author: giova
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

typedef enum {
	CTRL_MODE_HQ, CTRL_MODE_ECO, CTRL_MODE_OFF, CTRL_MODE_ERR
} Controller_mode_t;

void Controller_Init(void);
void Controller_Handle();
Controller_mode_t Controller_GetMode();
void Controller_SetMode(Controller_mode_t mode);


#endif /* INC_CONTROLLER_H_ */
