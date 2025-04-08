#include "lcd_ctrl.h"


void lcd_init(void)
{
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
}

void lcd_print(char *buff, int len)
{
    for(int i = 0;  i < 6; i++){
        if(i > len){
            break;
        }
        else{
            lcd_put_char(buff[i], segment_poss[i]);
        }
    }
}

void lcd_put_char(char in_char, int position)
{
    if (in_char == ' ')
    {
        // Display space
        LCDMEM[position] = 0;
        LCDMEM[position+1] = 0;
    }
    else if (in_char >= '0' && in_char <= '9')
    {
        // Display digit
        LCDMEM[position] = digit[c-48][0];
        LCDMEM[position+1] = digit[c-48][1];
    }
    else if (in_char >= 'A' && in_char <= 'Z')
    {
        // Display alphabet
        LCDMEM[position] = alphabetBig[in_char-65][0];
        LCDMEM[position+1] = alphabetBig[in_char-65][1];
    }
    else
    {
        // Turn all segments on if character is not a space, digit, or uppercase letter
        LCDMEM[position] = 0xFF;
        LCDMEM[position+1] = 0xFF;
    }
}

void lcd_clear(void)
{
    LCDMEM[pos1] = LCDBMEM[pos1] = 0;
    LCDMEM[pos1+1] = LCDBMEM[pos1+1] = 0;
    LCDMEM[pos2] = LCDBMEM[pos2] = 0;
    LCDMEM[pos2+1] = LCDBMEM[pos2+1] = 0;
    LCDMEM[pos3] = LCDBMEM[pos3] = 0;
    LCDMEM[pos3+1] = LCDBMEM[pos3+1] = 0;
    LCDMEM[pos4] = LCDBMEM[pos4] = 0;
    LCDMEM[pos4+1] = LCDBMEM[pos4+1] = 0;
    LCDMEM[pos5] = LCDBMEM[pos5] = 0;
    LCDMEM[pos5+1] = LCDBMEM[pos5+1] = 0;
    LCDMEM[pos6] = LCDBMEM[pos6] = 0;
    LCDMEM[pos6+1] = LCDBMEM[pos6+1] = 0;

    LCDM14 = LCDBM14 = 0x00;
    LCDM18 = LCDBM18 = 0x00;
    LCDM3 = LCDBM3 = 0x00;
}