#include <stdint.h>
#include "stm32f411xe.h"

/*
PWM fade using DMA
NUCLEO-F411RE
LED: PA5 (TIM2_CH1)
*/

uint16_t pwm_table[] = {
0,50,100,150,200,250,300,350,400,450,
500,550,600,650,700,750,800,850,900,950,
1000,
950,900,850,800,750,700,650,600,550,
500,450,400,350,300,250,200,150,100,50
};

int main(void)
{

/* CLOCKS */

RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;


/* GPIO PA5 → AF1 (TIM2_CH1) */

GPIOA->MODER &= ~(3 << (5*2));
GPIOA->MODER |=  (2 << (5*2));

GPIOA->AFR[0] &= ~(0xF << (5*4));
GPIOA->AFR[0] |=  (1 << (5*4));


/* TIMER CONFIG */

TIM2->PSC = 84 - 1;
TIM2->ARR = 20000;

TIM2->CCMR1 &= ~(7 << TIM_CCMR1_OC1M_Pos);
TIM2->CCMR1 |=  (6 << TIM_CCMR1_OC1M_Pos);

TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

TIM2->CCER |= TIM_CCER_CC1E;

TIM2->CR1 |= TIM_CR1_ARPE;


/* initial duty */

TIM2->CCR1 = pwm_table[0];


/* DMA CONFIG */

DMA1_Stream5->CR = 0;

/* Channel 3 */

DMA1_Stream5->CR |= (3 << DMA_SxCR_CHSEL_Pos);

/* memory → peripheral */

DMA1_Stream5->CR |= DMA_SxCR_DIR_0;

/* increment memory */

DMA1_Stream5->CR |= DMA_SxCR_MINC;

/* circular mode */

DMA1_Stream5->CR |= DMA_SxCR_CIRC;

/* 16-bit transfers */

DMA1_Stream5->CR |= DMA_SxCR_PSIZE_0;
DMA1_Stream5->CR |= DMA_SxCR_MSIZE_0;


/* addresses */

DMA1_Stream5->PAR  = (uint32_t)&TIM2->CCR1;
DMA1_Stream5->M0AR = (uint32_t)pwm_table;

/* transfer count */

DMA1_Stream5->NDTR = sizeof(pwm_table)/sizeof(uint16_t);


/* enable DMA request on compare */

TIM2->DIER |= TIM_DIER_CC1DE;


/* start DMA */

DMA1_Stream5->CR |= DMA_SxCR_EN;


/* start timer */

TIM2->CR1 |= TIM_CR1_CEN;


while(1)
{
}

}
