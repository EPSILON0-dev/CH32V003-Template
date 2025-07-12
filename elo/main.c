#include <stdbool.h>
#include "ch32v00x_gpio.h"
#include "debug.h"

GPIO_InitTypeDef config_d2_pin = { GPIO_Pin_2, GPIO_Speed_50MHz, GPIO_Mode_Out_PP }; 
int main(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_Init(GPIOD, &config_d2_pin);
    Delay_Init();

    bool state = false;
    for (;;) 
    { 
        GPIO_WriteBit(GPIOD, GPIO_Pin_2, state);
        Delay_Ms(500);
        state ^= 1;
    }
}
