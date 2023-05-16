/*
 * debug.h
 *
 *  Created on: Feb 15, 2021
 *      Author: giova
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

#include <stdio.h>

void Debug_PrintString(const char *str);
void Debug_PrintDateTime();
void Debug_PrintTempHumid(float temp, float humid);

#endif /* INC_DEBUG_H_ */
