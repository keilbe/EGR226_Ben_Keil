/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  04/16/2021
*Project :  ADC library
*File :  ADC_lib.c
*Description : ADC library for final project
**********************************************************************/
#include "ADC_lib.h"
#include <stdio.h>
    /****| adcsetup | *****************************************
    * Brief: Setup/initilization function to setup the ADC (analog digital converter).
    * Inputs: none
    * Outputs: none
    */
    void adcsetup(void){
        ADC14->CTL0 = 0x00000010; //power on and disabled during configuration
        ADC14->CTL0 |= 0x04D80300; // S/H pulse mode, MCLCK, 32 sample clocks, sw trigger, /4 clock divide
        ADC14->CTL1 = 0x00000030; // 14-bit resolution
        ADC14->MCTL[5] = 0; // A0 input, single ended, vref=avcc
        P5->SEL1 |= 0x20; // configure pin 5.5 for AO
        P5->SEL0 |= 0x20;
        ADC14->CTL1 |= 0x00050000; //start converting at mem reg 5
        ADC14->CTL0 |= 2; //enable ADC after configuration
    }
    /****| adcCheck | *****************************************
    * Brief: is called periodically and checks the
    * potentiometer to see what value it is at and sends out
    * the value so the LCD brightness can be changed.
    * Inputs: none
    * Outputs: backlight
    */
    void adcCheck(void){
        ADC14->CTL0 |=1; //start a conversion
        while(!ADC14->IFGR0); // wait until conversion complete
        result = ADC14->MEM[5]; // immediately store value in a variable
        result = (result)/16738; //converts result into volts
        backlight =60000 * result;
    }
