#include "led.h"
#include "button.h"

int main(void)
{
    /* Enable FPU */
    SCB->CPACR |= ((3UL << 20) | (3UL << 22));

    LED_Init();
    Button_Init();

    while(1)
    {
        /* Button is active LOW */
        if(Button_IsPressed())
        {
           	LED_On();
        }
        else
        {
        	LED_Off();
        }
    }
}
