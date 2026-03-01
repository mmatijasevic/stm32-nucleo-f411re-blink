#include "stm32f411xe.h"
#include <stdint.h>

void gpio_init(void);
void exti_init(void);

int main(void)
{
    /* Enable FPU */
    SCB->CPACR |= ((3UL << 20) | (3UL << 22));

    gpio_init();
    exti_init();

    while(1)
    {
        /* CPU sada samo čeka interrupt */
        __WFI();   // Wait For Interrupt (low power + profesionalno)
    }
}

void gpio_init(void)
{
    /* Enable GPIOA and GPIOC clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    (void)RCC->AHB1ENR;  // dummy read (sync)

    /* PA5 as output */
    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (1U << (5 * 2));

    /* PC13 as input */
    GPIOC->MODER &= ~(3U << (13 * 2));
}

void exti_init(void)
{
    /* Enable SYSCFG clock */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /* Map PC13 to EXTI13 */
    SYSCFG->EXTICR[3] &= ~(0xF << 4);
    SYSCFG->EXTICR[3] |=  (0x2 << 4);  // Port C = 0x2

    /* Unmask EXTI13 */
    EXTI->IMR |= (1 << 13);

    /* Falling edge trigger (button active low) */
    EXTI->FTSR |= (1 << 13);

    /* Enable interrupt in NVIC */
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI->PR & (1 << 13))
    {
        EXTI->PR = (1 << 13);  // clear pending flag

        /* SIGURNI toggle bez RMW konflikta */
        if(GPIOA->ODR & (1 << 5))
            GPIOA->BSRR = (1 << (5 + 16));   // reset
        else
            GPIOA->BSRR = (1 << 5);          // set
    }
}
