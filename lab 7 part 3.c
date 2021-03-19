/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  03/12/2021
*Project :  Lab 7 part 3
*File :  lab 7 part 3. c
*Description : LCD screen printer
**********************************************************************/
#include "msp432.h"

#define RS 1     //P4.0 mask
#define RW 2     //P4.1 mask
#define E 4      //P4.2 mask
/*other pins pin layout:
 *P4.4 -> DB4
 *P4.5 -> DB5
 *P4.6 -> DB6
 *P4.7 -> DB7
 *GND -> VSS,V0
 *5V -> VDD, LEDA,
 *5V -> potentiometer -> LEDA
 */
void SysTick_init (void); //prototype for systick initlization
void LCD_init(void);
void delay_micro (unsigned microsec);
void delay_ms(unsigned ms);
void pushByte(unsigned data, unsigned char byte);   //includes pushnibble and pulseEnablePin
void commandWrite(unsigned char command);
void dataWrite(unsigned data);

char line1[] = "LABORATORY OVER                 ";
int k = 0;
int a = 0;
int j = 0;
/****| main | *****************************************
* Brief: calls the function it initilize the LCD then
* prints out a message with a scrolling effect.
* Inputs: char line1[], int k, int a, int j
* Outputs: char line1[], int k
*/
int main(void) {    //writes message then clears the screen then rewrites message as an indexed version.
SysTick_Init ();
LCD_init();
while(1) {
commandWrite(1);     //clear display
commandWrite(0x80);  //set cursor at beginning of first line
    for (a=0;a<16;a++){     //uses the variable a instead of k so it will index through all 16 characters every time
        dataWrite (line1[k]);
        k=k+1;//indexes to next letter
        if (k==32){     //prevents k from indexing to a value outside of the array
            k=0;
        }
    }
    j=j+1;
    k=j;
    if (j>=32){     //prevents j and k from indexing to a value outside of the array
        k=0;
        j=0;
    }
delay_ms(1000);
}
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
/* With 4-bit mode, each command or data is sent twice with upper
 * nibble first then lower nibble.
 */
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