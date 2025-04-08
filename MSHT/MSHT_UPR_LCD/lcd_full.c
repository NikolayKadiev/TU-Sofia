
#include "msp430.h"
#include "lcd_ctrl.h"


int main(void)
{
    char msg[] = "HELLO";

    WDTCTL = WDTPW | WDTHOLD;

    PM5CTL0 &= ~LOCKLPM5;

    lcd_init();

    lcd_print(msg, strlen(msg));

    while(1){
        __no_operation();
        __delay_cycles(1000);
    }

    return 0;
}
