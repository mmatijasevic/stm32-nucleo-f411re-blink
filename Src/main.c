#include "stm32f411xe.h"
#include <stdint.h>

volatile uint32_t ms_ticks = 0;

/* ================= CLOCK 84 MHz (HSI → PLL) ================= */
void Clock_Init_84MHz(void)
{
    /* 1. Enable HSI (should already be on, but for safety) */
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));

    /* 2. Disable PLL before configuration */
    RCC->CR &= ~RCC_CR_PLLON;
    while(RCC->CR & RCC_CR_PLLRDY);

    /* 3. Configure Flash latency (2 WS for 84 MHz) */
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_2WS;

    /* 4. Configure PLL
       HSI = 16 MHz
       PLLM = 16
       PLLN = 336
       PLLP = 4

       16 / 16 = 1 MHz
       1 * 336 = 336 MHz
       336 / 4 = 84 MHz
    */
    RCC->PLLCFGR =
        (16 << RCC_PLLCFGR_PLLM_Pos) |
        (336 << RCC_PLLCFGR_PLLN_Pos) |
        (2 << RCC_PLLCFGR_PLLP_Pos);   // 10b = /4

    /* 5. Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));

    /* 6. Set APB1 prescaler to /2 (max 42 MHz) */
    RCC->CFGR &= ~RCC_CFGR_PPRE1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

    /* 7. Switch system clock to PLL */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    /* 8. Update SystemCoreClock variable */
    SystemCoreClockUpdate();
}

/* ================= SYSTICK 1ms ================= */
void SysTick_Init(void)
{
    SysTick_Config(SystemCoreClock / 1000);
}

void SysTick_Handler(void)
{
    ms_ticks++;
}

/* ================= DELAY ================= */
void delay_ms(uint32_t delay)
{
    uint32_t start = ms_ticks;
    while((ms_ticks - start) < delay);
}

/* ================= MAIN ================= */
int main(void)
{
    Clock_Init_84MHz();
    SysTick_Init();

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* PA5 as output (LD2 on Nucleo) */
    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (1U << (5 * 2));

    while(1)
    {
        GPIOA->ODR ^= (1U << 5);
        delay_ms(500);
    }
}



