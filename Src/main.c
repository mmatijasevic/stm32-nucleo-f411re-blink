#include "stm32f411xe.h"
#include <stdint.h>

void delay(volatile uint32_t count)
{
    while(count--);
}

int main(void)
{
    SCB->CPACR |= ((3UL << 20) | (3UL << 22));

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    (void)RCC->AHB1ENR;

    /* PA5 output */
    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (1U << (5 * 2));

    /* PC13 input */
    GPIOC->MODER &= ~(3U << (13 * 2));

    while(1)
    {
        if(!(GPIOC->IDR & (1U << 13)))   // tipka pritisnuta
        {
            delay(100000);  // debounce delay

            // provjeri ponovno
            if(!(GPIOC->IDR & (1U << 13)))
            {
                GPIOA->BSRR = (1U << 5);  // LED ON
            }
        }
        else
        {
            GPIOA->BSRR = (1U << (5 + 16));  // LED OFF
        }
    }
}
