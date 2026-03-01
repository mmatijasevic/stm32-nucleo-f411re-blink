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


    /* Enable SYSCFG clock */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /* Map PC13 to EXTI13 */
    SYSCFG->EXTICR[3] &= ~(0xF << 4);  // clear bits
    SYSCFG->EXTICR[3] |=  (0x2 << 4);  // Port C = 0x2

    /* Unmask EXTI13 */
    EXTI->IMR |= (1 << 13);

    /* Falling edge trigger (button is active low) */
    EXTI->FTSR |= (1 << 13);

    /* Enable interrupt in NVIC */
    NVIC_EnableIRQ(EXTI15_10_IRQn);



    while(1)
    {

    }
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI->PR & (1 << 13))
    {
        EXTI->PR = (1 << 13);  // clear pending flag

        GPIOA->ODR ^= (1 << 5);  // toggle LED
    }
}
