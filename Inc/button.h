/*
 * button.h
 *
 *  Created on: 11. tra 2026.
 *      Author: marin
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32f411xe.h"
#include<stdint.h>

void Button_Init(void);
uint8_t Button_IsPressed(void);



#endif /* BUTTON_H_ */
