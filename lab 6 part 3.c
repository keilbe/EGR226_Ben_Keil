/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  03/06/2021
*Project :  Lab 6 part 3
*File :  lab 6 part 3. c
*Description : kepad controlled program that allows the user to enter a
*code into a keypad and reenter that code and see if the same code has
*been entered.
**********************************************************************/
#include "msp.h"
#include <stdio.h>
void keypad_init (void); // prototype for GPIO initialization
void Systick_delay (void); //prototype for systick delay
void Systick_init (void); //prototype for systick initlization
uint8_t Read_Keypad (void); // prototype for keypad scan subroutine
void Print_Keys (void); // Print Key pressed
void Check_Code (void); //checks if entered code is correct
void Read (void);
uint8_t num, pressed; // GLOBAL!
uint8_t E1, E2, E3, E4; //global
uint8_t C1, C2, C3, C4; //global
unsigned char array[40];
int i = 1;
int K = 1;
int j = 0;

/****| main | *****************************************
* Brief: prompts user to enter a 4 digit code then
* calls the nessicary functions.
* Inputs: pressed
* Outputs: none
*/
void main (void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
    printf("Enter a 4 digit code on the keypad. Press the # key when you have finished entering the code\n");
    keypad_init ( ); // Call initialization functions for functionality
    while ( 1 ){
    pressed = Read_Keypad ( ); // Call Function to read Keypad
    if ( pressed )
    Print_Keys ( );
    __delay_cycles(30000); // 10ms delay through the loop before reading keypad again
    }
}

/****| keypad_init | *****************************************
* Brief: initilizes keypad
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

/****| SysTick_init | *****************************************
* Brief: initilization for systick
* Inputs: none
* Outputs: none
*/
void SysTick_init(void){   //systick initilization function
    SysTick->CTRL = 0; // disable systick during step
    SysTick->LOAD = 0x00FFFFFF; // maximum reload value
    SysTick->VAL = 0; // free to be written over
    SysTick->CTRL = 0x00000005; // enable systick, 3Mhz, no interrupts
}
/****| SysTick_delay | *****************************************
* Brief: delay function for systick
* Inputs: none
* Outputs: none
*/
void SysTick_delay (uint16_t delay){ // Systick delay function
    SysTick->LOAD = ((delay * 3000) - 1); //delay values are equal to 1ms
    SysTick->VAL = 0; // any write to CVR clears it
    while ( (SysTick->CTRL & 0x00010000) == 1); // wait for flag to be SET
}

/****| Read_Keypad | *****************************************
* Brief: reads user input from the keypad into the board and
* stores them in a global variable called num
* Inputs: col, row
* Outputs: num
*/
uint8_t Read_Keypad(void){       //reads keypad entry into the global variable num
    uint8_t col, row;
    for ( col = 0; col < 3; col++ ){
        P4->DIR = 0x00;             // Set Columns to inputs
        P4->DIR |= BIT ( 4+col );   // Set column 3 to output
        P4->OUT &=~ BIT ( 4+col );  // Set column 3 to LOW
        SysTick_delay (10);     // Delay the while loop
        row = P4->IN & 0x0F;    // read all rows
    while ( !(P4IN & BIT0) | !(P4IN & BIT1) | !( P4IN & BIT2) | !( P4IN & BIT3) );
    if (row != 0x0F){
        SysTick_delay (5);    //prevents switch bounce
        if (row != 0x0F){
        break; // if one of the input is low, some key is pressed.
        }
    }
    }
    P2->DIR = 0x00; // Set Columns to inputs
    if ( col == 3)
    return 0;
    if (row == 0x0E) num = col + 1; // key in row 0
    if (row == 0x0D) num = 3 + col +1; // key in row 1
    if (row == 0x0B) num = 6 + col +1; // key in row 2
    if (row == 0x07) num = 9 + col+1; // key in row 3
return 1;
}

/****| Print_Keys | *****************************************
* Brief: prints out what key has been pressed on the keypad.
* If the # key has been pressed it also calls either Read()
* or Check_Keys().
* Inputs: K, num
* Outputs: i, array[i]
*/
void Print_Keys (void){
    if((num == 1)){
        printf("1");
        fflush (stdout);
        array[i] = num;
        i=i+1;
    }
    if((num == 2)){
        printf("2");
        fflush (stdout);
        array[i] = num;
        i=i+1;
    }
    if((num == 3)){
        printf("3");
        fflush (stdout);
        array[i] = num;
        i=i+1;
    }
    if((num == 4)){
        printf("4");
        fflush (stdout);
        array[i] = num;
        i=i+1;
    }
    if((num == 5)){
        printf("5");
        fflush (stdout);
        array[i] = num;
        i=i+1;
    }
    if((num == 6)){
        printf("6");
        fflush (stdout);
        array[i] = num;
        i=i+1;
    }
    if((num == 7)){
        printf("7");
        fflush (stdout);
        array[i] = num;
        i=i+1;
    }
    if((num == 8)){
        printf("8");
        fflush (stdout);
        array[i] = num;
        i=i+1;
    }
    if((num == 9)){
        printf("9");
        fflush (stdout);
        array[i] = num;
        i=i+1;
    }
    if((num == 11)){
        printf("0");
        fflush (stdout);
        array[i] = 0;
        i=i+1;
    }
    if((num == 12)){
        if(i < 5){      //if an insufficent amount of numbers has been entered
        printf("#");
        fflush (stdout);
        printf("\nInvalid entry! Please re-enter code\n");
        i = 1;          //resets i to 1 so the program can tell if less than 4 numbers have been entered.
        }
        if(i >= 5){     //if a sufficent amount of numbers has been entered
        printf("#");
        fflush (stdout);
        if(K==1){
        Read ( );           //calls function to read code
        }
        if(K==2){
        Check_Code ( );     //calls function to check if the re-etered code is correct
        }
    }
    }

}
/****| Read | *****************************************
* Brief: reads in what the previous 4 digits entered
* were and prints them out. It also resets i to 1 and
* sets K to 2.
* Inputs:  array[i]
* Outputs: K, i,
*/
void Read (void){                   //reads in the 4 most recent numbers and sets them as the code
    E4 = array[i-1];                //assigns the 4 most recent numbers to variables that will be used for the code.
    E3 = array[i-2];
    E2 = array[i-3];
    E1 = array[i-4];
    i=1;       //resets i back to 1 to prevent code from messing up when checking for the code.
printf("\nYour code is: %d%d%d%d\n", E1, E2, E3, E4);       //prints out code
printf("Plese re-enter code.\n");
K = 2;                              //allows code to move onto the check code part.
}

/****| Check_Code | *****************************************
* Brief: reads in what the previous 4 digits entered
* were and checks to see if they match the entered code.
* It also resets i to 1 and sets K to 1 if the correct
* code is entered.
* Inputs:  array[i], E1, E2, E3, E4
* Outputs: K, i,
*/
void Check_Code (void){             //this function is for checiking if the code is a match when the user is trying to reenter the code
    C4 = array[i-1];                //assigns the 4 most recent numbers to variables so the code can be checked
    C3 = array[i-2];
    C2 = array[i-3];
    C1 = array[i-4];
    i=1;        //resets i back to 1 to prevent code from messing up when checking for the code if the user enters the wrong code
if((E1 == C1) && (E2 == C2) && (E3 == C3) && (E4 == C4)){       //checks if the code matches
    printf("\ncorrect!\n");
    K=1;       //resets so code can be run again
    printf("\nEnter another 4 digit code in the same way if you wish.\n");
}
else{
    printf("\nIncorrect entry! Please try again.\n");
}
}
