#include "lcd_ctrl.h"

// Стойности на позиции на сгментите
const int segment_poss[6] = 
{
        9,      // Сегмент A1 започва от S18 
        5,      // Сегмент A2 започва от S10 
        3,      // Сегмент A3 започва от S6  
        18,     // Сегмент A4 започва от S36 
        14,     // Сегмент A5 започва от S28 
        7      // Сегмент A6 започва от S14
} 

// Стойности на седментите за изобразяване на цифри
const char digit[10][2] =
{
    {0xFC, 0x28},  // "0"
    {0x60, 0x20},  // "1"
    {0xDB, 0x00},  // "2"
    {0xF3, 0x00},  // "3"
    {0x67, 0x00},  // "4"
    {0xB7, 0x00},  // "5"
    {0xBF, 0x00},  // "6"
    {0xE4, 0x00},  // "7"
    {0xFF, 0x00},  // "8"
    {0xF7, 0x00}   // "9"
};

// Стойности на седментите за изобразяване на главни букви
const char alphabetBig[26][2] =
{
    {0xEF, 0x00},  // "A"
    {0xF1, 0x50},  // "B"
    {0x9C, 0x00},  // "C"
    {0xF0, 0x50},  // "D"
    {0x9F, 0x00},  // "E"
    {0x8F, 0x00},  // "F"
    {0xBD, 0x00},  // "G"
    {0x6F, 0x00},  // "H"
    {0x90, 0x50},  // "I"
    {0x78, 0x00},  // "J"
    {0x0E, 0x22},  // "K"
    {0x1C, 0x00},  // "L"
    {0x6C, 0xA0},  // "M"
    {0x6C, 0x82},  // "N"
    {0xFC, 0x00},  // "O"
    {0xCF, 0x00},  // "P"
    {0xFC, 0x02},  // "Q"
    {0xCF, 0x02},  // "R"
    {0xB7, 0x00},  // "S"
    {0x80, 0x50},  // "T"
    {0x7C, 0x00},  // "U"
    {0x0C, 0x28},  // "V"
    {0x6C, 0x0A},  // "W"
    {0x00, 0xAA},  // "X"
    {0x00, 0xB0},  // "Y"
    {0x90, 0x28}   // "Z"
};

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
            if(buff[i] == '.'){
                lcd_put_char(buff[i], segment_poss[i-1]);
            }
            else{
                lcd_put_char(buff[i], segment_poss[i]);
            }
        }
    }
}

void lcd_put_char(char in_char, int position)
{
    if (in_char >= '0' && in_char <= '9')
    {
        // Display digit
        LCDMEM[position] = digit[in_char-48][0];
        LCDMEM[position+1] = digit[in_char-48][1];
    }
    else if (in_char >= 'A' && in_char <= 'Z')
    {
        // Display alphabet
        LCDMEM[position] = alphabetBig[in_char-65][0];
        LCDMEM[position+1] = alphabetBig[in_char-65][1];
    }
    else if (in_char == '.'){
        LCDMEM[position+1] |= 0x01;
    }
    else
    {
        // Display space
        LCDMEM[position] = 0;
        LCDMEM[position+1] = 0;
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
