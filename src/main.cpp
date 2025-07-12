#include "ch32v00x.h"

#define GPIO_LED_PIN GPIO_Pin_2

GPIO_InitTypeDef gpio_init = 
{
    GPIO_LED_PIN,
    GPIO_Speed_50MHz,
    GPIO_Mode_Out_PP,
};

int main(void)
{
    // Init Delay
    Delay_Init();

    // Init the LED GPIO RCC and GPIO itself
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_Init(GPIOD, &gpio_init);

    // Print Hello World and blink an LED in loop
    bool gpio_state = true;
    for ( ;; )
    {
        GPIO_WriteBit(GPIOD, GPIO_LED_PIN, gpio_state? Bit_SET : Bit_RESET);
        Delay_Ms(500);
        gpio_state = !gpio_state;
    }
}
