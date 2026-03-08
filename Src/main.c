#include <stdint.h>
#include "stm32f411xe.h"

/*
--------------------------------------------------------
Sinusna tablica (32 sample)
Vrijednosti su 0–1000 jer je PWM period 1000
--------------------------------------------------------
*/
uint16_t sine_table[32] =
{
500,598,691,777,853,915,962,990,
1000,990,962,915,853,777,691,598,
500,402,309,223,147,85,38,10,
0,10,38,85,147,223,309,402
};

/*
--------------------------------------------------------
GPIO konfiguracija
PA5 = TIM2_CH1 (LED na Nucleo ploči)
--------------------------------------------------------
*/
void gpio_init(void)
{
    /* uključi clock za GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* PA5 -> Alternate function */
    GPIOA->MODER &= ~(3 << (5*2));
    GPIOA->MODER |=  (2 << (5*2));

    /* odaberi AF1 (TIM2) */
    GPIOA->AFR[0] &= ~(0xF << (5*4));
    GPIOA->AFR[0] |=  (1 << (5*4));
}

/*
--------------------------------------------------------
TIM2 konfiguracija za PWM
--------------------------------------------------------
*/
void timer2_pwm_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 84 - 1;
    TIM2->ARR = 1000;

    TIM2->CCR1 = 500;

    TIM2->CCMR1 |= (6 << 4);
    TIM2->CCMR1 |= (1 << 3);

    TIM2->CCER |= TIM_CCER_CC1E;

    TIM2->CR1 |= TIM_CR1_ARPE;

    /* DMA request na compare event */
    TIM2->DIER |= TIM_DIER_CC1DE;

    /* force update */
    TIM2->EGR |= TIM_EGR_UG;

    TIM2->CR1 |= TIM_CR1_CEN;
}

/*
--------------------------------------------------------
DMA konfiguracija
DMA1 Stream5 Channel3 -> TIM2_CH1
--------------------------------------------------------
*/
void dma_init(void)
{
    /* uključi DMA1 clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    /* disable stream prije konfiguracije */
    DMA1_Stream5->CR &= ~DMA_SxCR_EN;
    while(DMA1_Stream5->CR & DMA_SxCR_EN);

    /*
    odredi perifernu adresu

    destination = CCR1
    */
    DMA1_Stream5->PAR = (uint32_t)&TIM2->CCR1;

    /*
    memory address = sine table
    */
    DMA1_Stream5->M0AR = (uint32_t)sine_table;

    /*
    broj elemenata
    */
    DMA1_Stream5->NDTR = 32;

    /*
    channel 3 = TIM2_CH1
    */
    DMA1_Stream5->CR |= (3 << 25);

    /*
    memory increment
    */
    DMA1_Stream5->CR |= DMA_SxCR_MINC;

    /*
    circular mode
    */
    DMA1_Stream5->CR |= DMA_SxCR_CIRC;

    /*
    memory size = 16 bit
    peripheral size = 16 bit
    */
    DMA1_Stream5->CR |= (1 << 11);
    DMA1_Stream5->CR |= (1 << 13);

    /*
    direction = memory → peripheral
    */
    DMA1_Stream5->CR |= (1 << 6);

    /*
    enable DMA
    */
    DMA1_Stream5->CR |= DMA_SxCR_EN;
}

/*
--------------------------------------------------------
MAIN
--------------------------------------------------------
*/
int main(void)
{
    gpio_init();
    timer2_pwm_init();
    dma_init();

    while(1)
    {
        /*
        CPU ne radi ništa
        sav posao rade periferije
        */
    }
}
