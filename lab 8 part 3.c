/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  03/19/2021
*Project :  Lab 8 part 3
*File :  lab 8 part 3. c
*Description : Keypad controlled motor PWM
**********************************************************************/
#include "msp432P401R.h"
#include <stdio.h>

void SysTick_Init (void); //prototype for systick initialization
void delay_ms(uint16_t ms); //prototype for systick millisecond delay function
void keypad_init (void); // prototype for GPIO initialization
uint8_t Read_Keypad (void); // prototype for keypad scan function
uint8_t num, pressed;
int k = 0;
int j = 0;

/****| main | *****************************************
* Brief: prompts user to enter a speed to control a
* motor at and calls the nessicary functions. It then
* uses a while loop to call some more functions as well
* as regulate the speed of the motor.
* Inputs: pressed
* Outputs: none
*/
void main (void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; //stop watchdog timer
    int speed = 30000;
    int DS;
    P2SEL0  |= 0x80;              //Clear P2.7 in P2SEL0
    P2SEL1 &= ~0x80;              //Clear P2.7 in P2SEL1
    P2DIR  |=  0x80; 		      //P2.7 output
    SysTick_Init ( ); //initialize SysTick Timer
    keypad_init ( ); //Call initialization function
    printf("Enter a speed on the keypad. 0 is 100%, * is 0%, 1 is 10%, 2 is 20%, and so on.\n");

    TIMER_A0->CCR[0] = 60000-1; //sets period
    TIMER_A0->CCTL[4] = 0xE0; //CCR4 set/reset mode
    TIMER_A0->CTL = 0x0214; //count up

    while ( 1 ){
        pressed = Read_Keypad ( ); // Call Function to read Keypad
        if ( pressed ){
            if (num == 10){             // * on keypad
                speed = 0;
            }
            if ((num<=9) && (num>0)){   // numbers 1 - 9 on keypad
             speed = num*6000;
            }
            if (num == 11){             // 0 on keypad
                speed = 60000;
            }
            DS = (speed/60000)*100;
            printf("duty cycle = %d\n",DS);
        }
        TIMER_A0->CCR[4] = speed; //sets duty cycle
        __delay_cycles(30000); // 10ms delay through the loop before reading keypad again
        }
}       //end of main

/****| keypad_init | *****************************************
* Brief: initializes keypad
* Inputs: none
* Outputs: none
*/
void keypad_init (void)     //initalize keypad
{
    P4SEL0 &= ~0xFF;
    P4SEL1 &= ~0xFF;
    P4->DIR &= ~(BIT0| BIT1 | BIT2| BIT3);//set keypad Row 0 to P4.0 Row 1 to P4.1 and Row2 to P4.2 and Row3 to P4.3
    P4REN |= (BIT0 | BIT1 | BIT2| BIT3);//enable pull resistor on all rows
    P4OUT |= (BIT0 | BIT1 | BIT2| BIT3);//set up rows with pull-up
    P4DIR &= ~(BIT4 | BIT5| BIT6);//Set keypad columns to inputs Col_2  = P4.6, Col_1 = P4.5, and Col_0 = P4.4
}
/****| Read_Keypad | *****************************************
* Brief: reads user input from the keypad into the board and
* stores them in a global variable named "num".
* Inputs: col, row
* Outputs: num
*/
uint8_t Read_Keypad(void){       //reads keypad entry into the global variable num
    uint8_t col, row;
    for ( col = 0; col < 3; col++ ){
        P4->DIR = 0x00;             // Set Columns to inputs
        P4->DIR |= BIT ( 4+col );   // Set column 3 to output
        P4->OUT &=~ BIT ( 4+col );  // Set column 3 to LOW
        delay_ms (10);     // Delay the while loop
        row = P4->IN & 0x0F;    // read all rows
    while ( !(P4IN & BIT0) | !(P4IN & BIT1) | !( P4IN & BIT2) | !( P4IN & BIT3) );
    if (row != 0x0F){
        delay_ms (10);    //prevents switch bounce
        if (row != 0x0F){
        break; // if one of the input is low, some key is pressed.
        }
    }
    }
    P4->DIR = 0x00; // Set Columns to inputs
    if ( col == 3)
    return 0;
    if (row == 0x0E) num = col + 1; // key in row 0
    if (row == 0x0D) num = 3 + col +1; // key in row 1
    if (row == 0x0B) num = 6 + col +1; // key in row 2
    if (row == 0x07) num = 9 + col+1; // key in row 3
return 1;
}
/****| delay_ms | *****************************************
* Brief: SysTick millisecond delay function.
* Inputs: uint16_t ms
* Outputs: none
*/
void delay_ms(uint16_t ms) {
    SysTick->LOAD = ((ms*3000)-1); //delay values are equal to 1ms
    SysTick->VAL = 0; // any write to CVR clears it
    SysTick -> CTRL = 0x5; // enable timer
    while ( (SysTick->CTRL & 0x00010000) == 0); // wait for flag to be SET
    SysTick -> CTRL = 0; // stop timer
}
/****| SysTick_Init | *****************************************
* Brief: initilization for systick
* Inputs: none
* Outputs: none
*/
void SysTick_Init (void){ //initialization of systic timer
    SysTick -> CTRL = 0; // disable SysTick During step
    SysTick -> LOAD = 0x00FFFFFF; // max reload value
    SysTick -> VAL = 0; // any write to current clears it
    SysTick -> CTRL = 0x00000005; // enable systic, 3MHz, No interrupts
}

