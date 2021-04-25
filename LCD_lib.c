/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  04/16/2021
*Project :  LCD library
*File :  LCD_lib. c
*Description : LCD library for final project
**********************************************************************/
#include "LCD_lib.h"
#include <stdio.h>
#define RS 1     // P9.0 mask
#define RW 2     // P9.1 mask
#define E 4      // P9.2 mask
/*other pins pin layout:
 *P9.4 -> DB4
 *P9.5 -> DB5
 *P9.6 -> DB6
 *P9.7 -> DB7
*/

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

    P7->OUT = data | byte;       //RS = 0, R/W = 0
    P7->OUT = data | byte | E;   //sets E to 1, sending the data or command to the LCD
    //delay_ms(1);                    //enable for a slow typing looking effect
    P7->OUT = data;                 //clear E
    P7->OUT = 0;                    //resets E to zero
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