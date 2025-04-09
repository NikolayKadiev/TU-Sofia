#include "msp430.h"
#include "string.h"

void lcd_init(void);
void lcd_print(char *buff, int len);
void lcd_put_char(char in_char, int position);
void lcd_clear(void);
