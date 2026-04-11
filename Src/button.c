/*
 * header.c
 *
 *  Created on: 11. tra 2026.
 *      Author: marin
 */

#include "button.h"

#define BUTTON_PIN 13U

void Button_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    (void)RCC->AHB1ENR;

	/* PC13 as input */
	GPIOC->MODER &= ~(3U << (BUTTON_PIN * 2));

}

uint8_t Button_IsPressed(void)
{
	return !(GPIOC->IDR & (1U << BUTTON_PIN));
}
