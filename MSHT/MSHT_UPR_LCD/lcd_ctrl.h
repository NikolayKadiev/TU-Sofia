#include "msp430.h"

void lcd_init(void);
void lcd_print(char *buff);
void lcd_put_char(char in_char, int position);
void lcd_clear(void);
