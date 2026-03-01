#include "stm32f411xe.h"
#include <stdint.h>

volatile uint8_t button_pressed = 0;

void EXTI15_10_IRQHandler(void)
{
    if(EXTI->PR & (1 << 13))   // provjeri je li PC13 interrupt
    {
        EXTI->PR |= (1 << 13); // clear pending bit
        button_pressed = 1;    // samo postavimo flag
    }
}

int main(void)
{
    // Enable GPIOA clock za LED
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Enable GPIOC clock za tipku
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    // Enable SYSCFG clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // LED PA5 output
    GPIOA->MODER &= ~(3U << (5*2));
    GPIOA->MODER |=  (1U << (5*2));

    // Button PC13 input
    GPIOC->MODER &= ~(3U << (13*2));

    // Map PC13 to EXTI13
    SYSCFG->EXTICR[3] &= ~(0xF << 4);
    SYSCFG->EXTICR[3] |=  (0x2 << 4); // Port C = 0x2

    // EXTI13 interrupt mask
    EXTI->IMR |= (1 << 13);

    // Falling edge trigger (tipka prema GND)
    EXTI->FTSR |= (1 << 13);

    // NVIC enable EXTI15_10 IRQ
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    while(1)
    {
        if(button_pressed)
        {
            button_pressed = 0;             // reset flag
            GPIOA->ODR ^= (1U << 5);        // toggle LED
        }
    }
}
