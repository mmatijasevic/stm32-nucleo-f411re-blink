/*
 * led.c
 *
 *  Created on: 11. tra 2026.
 *      Author: marin
 */


#include "led.h"

#define LED_PIN 5U


void LED_Init(void)
{

	/* Enable GPIOA clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	(void)RCC->AHB1ENR;

	/* PA5 as output */
	GPIOA->MODER &= ~(3U << (LED_PIN * 2));
	GPIOA->MODER |=  (1U << (LED_PIN * 2));

}
void LED_On(void)
{
	GPIOA->BSRR = (1U << LED_PIN);   // LED ON
}
void LED_Off(void)
{
	GPIOA->BSRR = (1U << (LED_PIN + 16)); // LED OFF
}

