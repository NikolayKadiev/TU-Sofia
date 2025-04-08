
#include "msp430.h"
#include <stdint.h>
#include "lcd_ctrl.h"

void init_adc(void)
{
    //Избери функцията на извод P9.2 като вход на АЦП (A10)
    P9SEL0 |= 0x04;
    P9SEL1 |= 0x04;
    
    ADC12CTL0 &= ~ADC12ENC;     //Забрани преобразуването, ENC = 0
    ADC12CTL0 |= ADC12ON;       //Включи АЦП
    ADC12CTL0 |= ADC12SHT1_15;  //Избери Sample-and-hold време = 512 такта на АЦП-то за 
                                //канали 8 - 23
    ADC12CTL1 |= ADC12SSEL_2;   //Избери MCLK за тактов сигнал
    ADC12CTL1 |= ADC12SHP;      //Използвай sampling таймера
    ADC12CTL1 |= ADC12CONSEQ_0; //Избери режим "един канал - едно измерване"   
    ADC12CTL1 |= ADC12PDIV_0;   //Раздели тактовия сигнал на 1 
    ADC12CTL1 |= ADC12DIV_1;    //Раздели тактовия сигнал на 2 (от втория делител)    
    ADC12CTL1 &= ~ADC12ISSH;    //Не инвертирай sample-and-hold сигнала
    ADC12CTL1 |= ADC12SHS_0;    //Използвай ADC12SC бита като сигнал за начало на sample-
                                //and-hold
    ADC12CTL2 &= ~ADC12DF;      //Формат на резултата - прав код
    ADC12CTL2 &= ~ADC12RES_3;   //Занули битовете за разредността
    ADC12CTL2 |= ADC12RES_2;    //Избери 12-битово преобразуване, Userguide стр. 880
//-------------------------MCTL конфигурация----------------------------------------------------------------------
    ADC12MCTL10 &= ~ADC12DIF;   //Избери single-ended преобразуване
    ADC12MCTL10 |= ADC12VRSEL_0;        //Използвай Vdd за горен праг, Vss - за долен.    
    ADC12MCTL10 |= ADC12INCH_10;        //Избери A10 за входен канал
    ADC12CTL3 |= ADC12CSTARTADD_10;     //Избери регистър ADC12MEM10 да е приемник на 
                                        //резултата от преобразуването
    ADC12MCTL10 |= ADC12EOS;            //Канал А10 е крайният канал за преобразуване
//----------------------------------------------------------------------------------------------------------------------------
    ADC12CTL0 |= ADC12ENC;      //Установи ENC в 1
}

int main(void)
{
    char msg[6];
    volatile uint16_t adc_ress;

    WDTCTL = WDTPW | WDTHOLD;

    PM5CTL0 &= ~LOCKLPM5;

    init_adc();
    lcd_init();

    while(1){    
        ADC12CTL0 |= ADC12SC;
        while(ADC12CTL1 & ADC12BUSY){ }

        adc_ress =  ADC12MEM10;

        sprintf(msg,"%d\n", adc_ress);
        lcd_print(msg, strlen(msg));
        
       __delay_cycles(100000);
    }

    return 0;
}
