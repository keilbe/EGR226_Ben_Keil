/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  04/02/2021
*Project :  Lab 10 part 3
*File :  lab 10 part 3. c
*Description : LCD ACD temp sensor
**********************************************************************/
#include "msp432.h"
#include "stdio.h"
#define RS 1     // P4.0 mask
#define RW 2     // P4.1 mask
#define E 4      // P4.2 mask
/*other pins pin layout:
 *P4.4 -> DB4
 *P4.5 -> DB5
 *P4.6 -> DB6
 *P4.7 -> DB7
 *GND -> VSS,V0
 *5V -> VDD, LEDA,
 *5V -> potentiometer -> LEDK -> potentiometer -> ground
 */
void SysTick_Init (void);
void LCD_init(void);
void delay_micro (unsigned microsec);
void delay_ms(unsigned ms);
void pushByte(unsigned data, unsigned char control);
void commandWrite(unsigned char command);
void dataWrite(unsigned data);
void adcsetup(void);
float result;

char line1[] = "Current Temp. is";
char line2[16];
int k = 0;
int h = 16;
int j = 0;
/****| main | *****************************************
* Brief: calls the function it initialize the LCD. It
* then prints out the text from the 4 arrays.
* Inputs: char line1, int k, int h
* Outputs: char line1, char line2, int k, int h
*/
int main(void) {
    LCD_init();
    SysTick_Init ();
    adcsetup();
    while(1) {
        delay_ms(500);
        commandWrite(1);     // clear display
        commandWrite(0x80);  // set cursor at beginning of first line
        for (k=0;k<16;k++){ //prints out first line
    dataWrite (line1[k]);
    }
    ADC14->CTL0 |=1; //start a conversion
    while(!ADC14->IFGR0); // wait until conversion complete
    result = ADC14->MEM[5]; // immediately store value in a variable
    result = (result/163.84);   //convert to celceus
    printf ("Value is:\n\t%0.1f° C\n", result);
    sprintf(line2, "%0.1f° C", result);     //puts entry into an array
    if (result < 100){
        h = 7;
    }
    if (result < 10){
        h = 6;
    }
    commandWrite(0xC0);  // set cursor at beginning of second line
    for (k=0;k<h;k++){
        if (line2[k] == '°'){       //if statement used to correct ascii mismatch
            dataWrite(0xDF);
        }
        else{
dataWrite (line2[k]);
        }
}
        delay_ms(500);
    }
}

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

/****| LCD_init | *****************************************
* Brief: Initilization sequence for the liquid crystal
* display
* Inputs: none
* Outputs: none
*/
void LCD_init(void) {
    P4->DIR = 0xFF;     //make P4 pins output for data and controls
    delay_ms(30);                //initialization sequence
    pushByte(0x30, 0);
    delay_ms(10);
    pushByte(0x30, 0);
    delay_ms(1);
    pushByte(0x30, 0);
    delay_ms(1);
    pushByte(0x20, 0);  //use 4-bit data mode
    delay_ms(1);

    commandWrite(0x56);      //set 4-bit data, 4-line, 5x7 font
    commandWrite(0x06);      //move cursor right after each char
    commandWrite(0x01);      //clear screen, move cursor to home
    commandWrite(0x0F);      //turn on display, cursor blinking
}

/****| pushByte | *****************************************
* Brief: this function clears the lower and upper
* nibbles for two different variables then pulses E,
* causing a character to be printed on the LCD or writing
* a command to the LCD depending on where pushByte has
* been called from.
* Inputs: unsigned data, unsigned char byte
* Outputs: none
*/
void pushByte(unsigned data, unsigned char byte) {
    data &= 0xF0;        //clear lower nibble for byte(command)
    byte &= 0x0F;        //clear upper nibble for data

    P4->OUT = data | byte;       //RS = 0, R/W = 0
    P4->OUT = data | byte | E;   //sets E to 1, sending the data or command to the LCD
    //delay_ms(1);                    //enable for a slow typing looking effect
    P4->OUT = data;                 //clear E
    P4->OUT = 0;                    //resets E to zero
}
/****| commandWrite | *****************************************
* Brief: The purpose of this function is to assign the
* different time intervals to perform various tasks.
* Inputs: unsigned char command
* Outputs: unsigned char command
*/
void commandWrite(unsigned char command) {
    pushByte(command & 0xF0, 0);    //extracts upper nibble
    pushByte(command << 4, 0);      //shifts lower nibble into place

    if (command < 4)
        delay_micro(1640);          //1.64 ms delay
    else
        delay_micro(40);            //40 us delay
}
/****| dataWrite | *****************************************
* Brief: prompts user to enter a 4 digit code then
* calls the nessicary functions.
* Inputs: unsigned data
* Outputs: unsigned data
*/
void dataWrite (unsigned data){
    pushByte(data & 0xF0, RS);    //extracts upper nibble
    pushByte(data << 4, RS);      //shifts lower nibble into place

    delay_micro(1);
}
/****| delay_ms | *****************************************
* Brief: uses the systick timer to create 1 ms delays
* Inputs: unsigned ms
* Outputs: none
*/
void delay_ms(unsigned ms) {
    SysTick->LOAD = (ms*3000 - 1); //1ms delay
    SysTick->VAL = 0; //any write to CVR clears it
    SysTick -> CTRL = 0x5; // enable timer
    while ( (SysTick->CTRL & 0x00010000) == 0); //wait for flag to be SET
    SysTick -> CTRL = 0; //stop timer
}
/****| delay_micro | *****************************************
* Brief: uses the systick timer to create 1 us delays
* Inputs: unsigned microsec
* Outputs: none
*/
void delay_micro (unsigned microsec){
    SysTick -> LOAD = (microsec*3 - 1); // 1 us delay
    SysTick -> VAL = 0; //clears counter
    SysTick -> CTRL = 0x5; //enable timer
    while((SysTick -> CTRL & 0x00010000) == 0);
    SysTick -> CTRL = 0; //stop timer
}
/****| SysTick_Init | *****************************************
* Brief: initilization sequence for the systick timer
* Inputs: none
* Outputs: none
*/
void SysTick_Init (void){
    SysTick -> CTRL = 0; //disable SysTick During step
    SysTick -> LOAD = 0x00FFFFFF; //max reload value
    SysTick -> VAL = 0; //any write to current clears it
    SysTick -> CTRL = 0x00000005; //enable systic, 3MHz, No interupts
}
