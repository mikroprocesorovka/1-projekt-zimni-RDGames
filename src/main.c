#include "stm8s.h"
#include "stm8_hd44780.h"
#include <stdio.h>
#define _ISOC99_SOURCE
#define _GNU_SOURCE
#include "delay.h"

void tim2_setup(void){
     TIM2_TimeBaseInit(TIM2_PRESCALER_8, 40000); 
    //TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_OC1Init(                // inicializujeme kanál 1 (TM2_CH1)
        TIM2_OCMODE_PWM1,        // režim PWM1
        TIM2_OUTPUTSTATE_ENABLE, // Výstup povolen (TIMer ovládá pin)
        2000,                    // výchozí hodnota šířky pulzu (střídy) 1056/1600 = 66%
        TIM2_OCPOLARITY_HIGH      // Polarita LOW protože LED rozsvěcím 0 (spol. anoda)
     );

    // ošetření nežádoucích jevů při změně PWM
    TIM2_OC1PreloadConfig(ENABLE);

    TIM2_Cmd(ENABLE);
}

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    lcd_init(); //inicializace LCD

    GPIO_Init(GPIOC,((GPIO_Pin_TypeDef)(GPIO_PIN_1 | GPIO_PIN_2)),GPIO_MODE_IN_PU_NO_IT); 
    TIM1_DeInit();
    TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP, 500, 16); 
    TIM1_EncoderInterfaceConfig(TIM1_ENCODERMODE_TI12,TIM1_ICPOLARITY_FALLING,TIM1_ICPOLARITY_FALLING);
   TIM1_Cmd(ENABLE);

   tim2_setup();

}


int main(void)
{

    unsigned int present_value = 0x0000;
    unsigned int previous_value = 0x0001;

    char text[16];
    setup();

    while (1) {
        

//získávání aktuální hodnoty z enkoderu
        present_value = TIM1_GetCounter();

        if(present_value != previous_value)
        {
            lcd_gotoxy(0, 0);
            sprintf(text,"regulace PWM %u ",present_value);
            lcd_puts(text);
            TIM2_SetCompare1(present_value*6 + 2000);
        }
        previous_value = present_value;

    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
