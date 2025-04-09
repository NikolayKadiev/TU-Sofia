
#include "msp430.h"
#include "lcd_ctrl.h"


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    PM5CTL0 &= ~LOCKLPM5;

    PJSEL0 = BIT4 | BIT5;                   // Използвай LFXT

    // Инициализзация на LCD сегменти 0 - 21 и 26 - 43
    LCDCPCTL0 = 0xFFFF;
    LCDCPCTL1 = 0xFC3F;
    LCDCPCTL2 = 0x0FFF;


    // Настройка на LFXT 32kHz резонатор
    CSCTL0_H = CSKEY >> 8;                  // Отключи CS регистри
    CSCTL4 &= ~LFXTOFF;                     // Раззреши използването на LFXT
    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Нулиране на LFXT флаг
      SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Тестване на тактовия генератор
    CSCTL0_H = 0;                           // Заключи CS регистрите

    // Инициализация на LCD_C
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP; // използване на ACLK | делител = 1 | пред-делител = 16 | 4-изводен MUX

    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN; // Задай напрежение на дисплея 2,6V | използвай вътрешно опорно нпрежение | позволи използването на капацитивната помпа

    LCDCCPCTL = LCDCPCLKSYNC;        // Позволи синхронизация на тактовит генератор

    LCDCMEMCTL = LCDCLRM;            // Нулиране на LCD паметта

    LCDCCTL0 |= LCDON;              // Включи дисплея

    while(1){
        LCDMEM[9] = 0x80;
        LCDMEM[9+1] = 0x50;

        LCDMEM[5] = 0x9F;
        LCDMEM[5+1] = 0x00;

        LCDMEM[3] = 0xB7;
        LCDMEM[3+1] = 0x00;

        LCDMEM[18] = 0x80;
        LCDMEM[18+1] = 0x50;

        LCDMEM[14] = 0x90;
        LCDMEM[14+1] = 0x50;

        LCDMEM[7] = 0x6C;
        LCDMEM[7+1] = 0x82;

        __no_operation();
        __delay_cycles(1000);
    }

    return 0;
}
