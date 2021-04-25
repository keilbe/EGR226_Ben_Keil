/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  04/16/2021
*Project :  final project main code
*File :  final project code Keil.c
*Description : main code for final project
**********************************************************************/
#include "msp.h"
#include <stdio.h>
#include  "keypad_lib.h";
#include  "LCD_lib.h";
#include  "SysTick_lib.h";
#include  "TimerA_lib.h";
#include  "ADC_lib.h";
#define LED_RED     (1U << 0)
#define LED_GREEN   (1U << 1)
void motor (void);
void door (void);
void LED (void);
void arm (void);
void disarm (void);
void SetupPort2Interrupts(void);
void screensaver(void);

#define RS 1     // P9.0 mask
#define RW 2     // P9.1 mask
#define E 4      // P9.2 mask
int operation = 0; //keeps track of current operation
int doorstate = 0; //0 = closed, 1 = open
int color = 0;
int speed = 0;
int brightness = 0;
int backlight = 0;
int rb = 0;//red brightness, green brightness, blue brightness
int gb = 0;
int bb = 0;
int orb = 0;
int ogb = 0;
int obb = 0;
int dooropenness = 6000;
int note;

int k = 0;
int i = 0;
int K = 0;
int dt;
int a;
int p=0;
float result;
uint8_t num, pressed;
uint8_t E1, E2, E3, E4;
uint8_t C1, C2, C3, C4;
unsigned char array[40];

/****| main | *****************************************
* Brief: displays the main menu and allows the user to
* navagete to different screens from there. It also is
* responsable for calling all the initilizations and
* setting most of the PWMs.
* Inputs: int doorstate, num, backlight, speed,
* dooropenness, rg, bb, gb, note.
* Outputs: none
*/
void main(void){
    __disable_irq ();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    char mline1[] = " select a menu  ";
    char mline2[] = "   (1) Door     ";
    char mline3[] = "   (2) Motor    ";
    char mline4[] = "   (3) Lights   ";
    dt =0;
    P2OUT = 0x0B;   //P2.0 and P2.1 output door LEDs
    P2DIR |= LED_RED;       // P2.0 output
    P2DIR |= LED_GREEN;     // P2.1 output
    //initialization/setup functions
    TA_init();
    keypad_init ();
    SysTick_Init ();
    LCD_init();
    adcsetup();
    SetupPort2Interrupts();

    NVIC_EnableIRQ (PORT2_IRQn);
    NVIC_SetPriority(TA2_0_IRQn, 3);
    NVIC_EnableIRQ(TA2_0_IRQn);

    __enable_irq ( );
while(1){
    operation = 0;
    commandWrite(1);     // clear display
    commandWrite(0x80);  // set cursor at beginning of first line
        for (k=0;k<16;k++){
            dataWrite (mline1[k]);
        }
    commandWrite(0xC0);  // set cursor at beginning of second line
        for (k=0;k<16;k++){
            dataWrite (mline2[k]);
        }
    commandWrite(0x90);  // set cursor at beginning of third line
        for (k=0;k<16;k++){
            dataWrite (mline3[k]);
        }
    commandWrite(0xD0);  // set cursor at beginning of fourth line
        for (k=0;k<16;k++){
            dataWrite (mline4[k]);
        }
            pressed = Read_Keypad ( ); // Call Function to read Keypad
            dt = dt+=SysTick -> VAL;    //records time to go through the previous functions in the while loop
            if (dt>=6000000){   //if 10 seconds has passed without a keypad entry the screensaver function will be called
                screensaver();
            }
            if (pressed){
                dt=0;           //timer for the screensaver is reset
                if (num == 1){
                    door();
                }
                if (num == 2){
                    motor();
                }
                if (num == 3){
                    LED();
                }
            }
            if (doorstate == 0){     //door closed
                P2OUT &= ~LED_GREEN; //turns off green LED
                P2OUT |= LED_RED;    //turns on red LED
            }
            if (doorstate == 1){     //door open
                P2OUT &= ~LED_RED;  //turns off red LED
                P2OUT |= LED_GREEN; //turns on green LED
            }

            adcCheck();
            TIMER_A0->CCR[3] = backlight; //sets LCD duty cycle
            TIMER_A0->CCR[4] = speed; //sets motor duty cycle
            TIMER_A3->CCR[1] = dooropenness; //sets door duty cycle
            TIMER_A1->CCR[2] = rb; //sets red LED duty cycle
            TIMER_A1->CCR[3] = gb; //sets green LED duty cycle
            TIMER_A1->CCR[1] = bb; //sets blue LED duty cycle
            TIMER_A2->CCR[2] = note; //sets speaker duty cycle
            __delay_cycles(30000); // 10ms delay through the loop
    }
}
/****| screensaver | *****************************************
* Brief: displays a scrolling message if the keypad has been
* inactive for long enough.
* Inputs: int backlight, uint8_t pressed
* Outputs: int backlight
*/
void screensaver(void){
    printf("scr\n");
    char line1[] = "sleep mode on, press keypad to exit     ";  //40 charicters
    int m = 0;
    int h = 0;
    int w = 0;

    while (w==0){
        commandWrite(1);     //clear display
        commandWrite(0x80);  //set cursor at beginning of first line
            for (a=0;a<16;a++){     //uses the variable a instead of m so it will index through all 16 characters every time
                dataWrite (line1[m]);
                m=m+1;//indexes to next letter
                if (m==40){     //prevents m from indexing to a value outside of the array
                    m=0;
                }
            }
            h=h+1;
            m=h;
            if (h>=40){     //prevents h and m from indexing to a value outside of the array
                m=0;
                h=0;
            }
            pressed = Read_Keypad ( ); // Call Function to read Keypad
            if (pressed){
                dt=0;
                w=1;
            }
            adcCheck();
            TIMER_A0->CCR[3] = backlight; //sets LCD duty cycle
            delay_ms(500);  //half second delay
    }
}

/****| door | *****************************************
* Brief: displays the door menu and allows the user to
* open, close, or arm/disarm the door alarm.
* Inputs: int doorstate, backlight,
* dooropenness, p, uint8_t num, pressed
* Outputs: int dooropenness, doorstate, backlight
*/
void door (void){
    printf("door\n");
    char line1[] = "Select an action";
    char line2[] = "  (1) open      ";
    char line3[] = "  (2) close     ";
    char line4[] = "  (3) arm/disarm";
    dt=0;

    while(1){
        commandWrite(1);     // clear display
        commandWrite(0x80);  // set cursor at beginning of first line
            for (k=0;k<16;k++){
                dataWrite (line1[k]);
            }
        commandWrite(0xC0);  // set cursor at beginning of second line
            for (k=0;k<16;k++){
                dataWrite (line2[k]);
            }
        commandWrite(0x90);  // set cursor at beginning of third line
            for (k=0;k<16;k++){
                dataWrite (line3[k]);
            }
        commandWrite(0xD0);  // set cursor at beginning of fourth line
            for (k=0;k<16;k++){
                dataWrite (line4[k]);
            }
        pressed = Read_Keypad ( ); // Call Function to read Keypad
        dt = dt+=SysTick -> VAL;
        if (dt>=6000000){
            screensaver();
        }
        if (pressed){
            dt=0;
        if (num == 1){
            dooropenness = 3000;    //opens door
            doorstate = 1;          //green led
            main();
        }
        if (num == 2){
            dooropenness = 6000;    //closes door
            doorstate = 0;          //red led
            main();
        }
        if (num == 3){
            if (p==0){
            arm();
            }
            if (p==1){
            disarm();
            }
        }
        }
        adcCheck();
        TIMER_A0->CCR[3] = backlight; //sets LCD duty cycle
    }
}
/****| motor | *****************************************
* Brief: displays the motor menu and allows the user to
* set the motor to a desired speed.
* Inputs: int backlight, uint8_t num, pressed
* Outputs: int speed, backlight
*/
void motor (void){
    printf("motor\n");
    char line1[] = " Enter a speed  ";
    char line2[] = "   from 0-9     ";
    dt=0;
    while (1){
        commandWrite(1);     // clear display
        commandWrite(0x80);  // set cursor at beginning of first line
            for (k=0;k<16;k++){
                dataWrite (line1[k]);
            }
        commandWrite(0xC0);  // set cursor at beginning of second line
            for (k=0;k<16;k++){
                dataWrite (line2[k]);
            }
        pressed = Read_Keypad ( ); // Call Function to read Keypad
        dt = dt+=SysTick -> VAL;
        if (dt>=6000000){
            screensaver();
        }
        if ( pressed ){
            dt=0;
            if ((num<=9) && (num>0)){   // numbers 1 - 9 on keypad
             speed = num*6000;
             main();
            }
            if (num == 11){             // 0 on keypad
                speed = 0;
            main();
            }
      }
        adcCheck();
        TIMER_A0->CCR[3] = backlight; //sets LCD duty cycle
        __delay_cycles(30000); // 10ms delay through the loop before reading keypad again
   }
}


/****| LED | *****************************************
* Brief: displays the LED color menu, allows the user
* to select a color, displays the brightness menu and
* then allows the user to select a brightness.
* Inputs: int backlight, uint8_t num, pressed
* Outputs: int rb, bb, gb, backlight
*/
void LED (void){
    printf("led\n");
    char cline1[] = " Select a color ";
    char cline2[] = "   (1) Red      ";
    char cline3[] = "   (2) Green    ";
    char cline4[] = "   (3) Blue     ";

    char bline1[] = "    Select a    ";
    char bline2[] = "Brightness 0-100";
    char bline3[] = "Press * if done ";
    dt=0;
    int i = 0;
    char brig[] = "00000000000";
    color = 0;  //sets/resets color to 0
    while (color == 0){
        commandWrite(1);     // clear display
        commandWrite(0x80);  // set cursor at beginning of first line
            for (k=0;k<16;k++){
                dataWrite (cline1[k]);
            }
        commandWrite(0xC0);  // set cursor at beginning of second line
            for (k=0;k<16;k++){
                dataWrite (cline2[k]);
            }
        commandWrite(0x90);  // set cursor at beginning of third line
            for (k=0;k<16;k++){
                dataWrite (cline3[k]);
            }
        commandWrite(0xD0);  // set cursor at beginning of fourth line
            for (k=0;k<16;k++){
                dataWrite (cline4[k]);
            }
            pressed = Read_Keypad ( ); // Call Function to read Keypad
            dt = dt+=SysTick -> VAL;
            if (dt>=6000000){
                screensaver();
            }
            if(pressed){
                dt=0;
                if ((num>=1) && (num<=3)){
                color = num;
            }
            }
            adcCheck();
            TIMER_A0->CCR[3] = backlight; //sets LCD duty cycle
    }   //end of while loop
    dt=0;
    while (1){
        commandWrite(1);     // clear display
        commandWrite(0x80);  // set cursor at beginning of first line
            for (k=0;k<16;k++){
                    dataWrite (bline1[k]);
            }
        commandWrite(0xC0);  // set cursor at beginning of second line
            for (k=0;k<16;k++){
                    dataWrite (bline2[k]);
            }
        commandWrite(0x90);  // set cursor at beginning of third line
            for (k=0;k<16;k++){
                    dataWrite (bline3[k]);
            }
          pressed = Read_Keypad ( ); // Call Function to read Keypad
          dt = dt+=SysTick -> VAL;
          if (dt>=6000000){
              screensaver();
          }
          if (pressed){
                dt=0;
            if ((num != 10) && (num != 12)){
                if (num == 11){
                    num = 0;
                }
                brig[i] = num;      //stores entries in an array
                i++;
            }
            if ((num == 10) && (i != 0)){           //user is done entering numbers and has pressed *
                if (i==1){
                brightness = brig[0];
                }
                if (i==2){
                brightness = (brig[0]*10)+brig[1];
                }
                if (i==3){
                brightness = (brig[0]*100)+(brig[1]*10)+brig[2];
                }
                if(brightness>100){
                    brightness = 100;
                }
                printf("%d\n", brightness);
                if (color == 1){
                    rb = brightness*600;
                }
                if (color == 2){
                    gb = brightness*600;
                }
                if (color == 3){
                    bb = brightness*600;
                }
            main();
            }
            }
          adcCheck();
          TIMER_A0->CCR[3] = backlight; //sets LCD duty cycle
    }
}
/****| arm | *****************************************
* Brief: displays the arm menu and allows the user to
*  arm the door alarm.
* Inputs: int doorstate, backlight, dooropenness,
* uint8_t num, pressed
* Outputs: int backlight, p, note, uint8_t E1, E2,
* E3, E4
*/
void arm (void){
    char line1[] = "Enter a 4 digit ";
    char line2[] = "    password    ";
    char line3[16];
    dt=0;


    int i = 1;
    printf("\nenter a code.\n");
    while ( 1 ){
        commandWrite(1);     // clear display
        commandWrite(0x80);  // set cursor at beginning of first line
            for (k=0;k<16;k++){
                dataWrite (line1[k]);
            }
        commandWrite(0xC0);  // set cursor at beginning of second line
            for (k=0;k<16;k++){
                dataWrite (line2[k]);
            }
        __delay_cycles(30000); // 10ms delay through the loop before reading keypad again
    pressed = Read_Keypad ( ); // Call Function to read Keypad
    dt = dt+=SysTick -> VAL;
    if (dt>=6000000){
        screensaver();
    }
    if ( pressed ){
    dt=0;
    if((num != 10) && (num != 12)){     //checking for valid keypad entry
        if (num==11){
            num=0;
        }
        array[i] = num;
        i=i+1;
    }
    if (i==5){
        E4 = array[i-1];                //assigns the 4 most recent numbers to variables that will be used for the code.
        E3 = array[i-2];
        E2 = array[i-3];
        E1 = array[i-4];
        i=1;       //resets i back to 1 to prevent code from messing up when checking for the code.
        printf("%d%d%d%d\n", E1, E2, E3, E4);
        sprintf(line3, "      %d%d%d%d      ", E1, E2, E3, E4);     //puts entry into an array
            commandWrite(0x90);  // set cursor at beginning of third line
            for (k=0;k<16;k++){
                dataWrite (line3[k]);
            }
            __delay_cycles(3000000); // 1s delay so user can see the code they entered
            p=1;
            note = 4000;
        main();     //calls main so the note can be played
    }
    }
    adcCheck();
    TIMER_A0->CCR[3] = backlight; //sets LCD duty cycle
}
}
/****| disarm | *****************************************
* Brief: displays the disarm menu and allows the user to
* disarm the door alarm.
* Inputs: int doorstate, backlight, dooropenness,
* uint8_t num, pressed, E1, E2, E3, E4
* Outputs: int backlight, note, p
*/
void disarm (void){
    char line1[] = "reenter password";
    char line2[] = "Incorrect entry!";
    char line3[] = "Please try again";
    char line4[] = "    correct!    ";
    dt=0;


    int i = 1;
    while ( 1 ){
        commandWrite(1);     // clear display
        commandWrite(0x80);  // set cursor at beginning of first line
            for (k=0;k<16;k++){
                dataWrite (line1[k]);
            }
        __delay_cycles(30000); // 10ms delay through the loop before reading keypad again
    pressed = Read_Keypad ( ); // Call Function to read Keypad
    dt = dt+=SysTick -> VAL;
    if (dt>=6000000){
        screensaver();
    }
    if ( pressed ){
    dt=0;
    if((num != 10) && (num != 12)){
        if (num==11){
            num=0;
        }
        array[i] = num;
        i=i+1;
    }
    if (i==5){          //once 4 numbers have been entered it auto enters them in
        C4 = array[i-1];                //assigns the 4 most recent numbers to variables so the code can be checked
        C3 = array[i-2];
        C2 = array[i-3];
        C1 = array[i-4];
        i=1;        //resets i back to 1 to prevent code from messing up when checking for the code if the user enters the wrong code
        printf("%d%d%d%d\n", C1, C2, C3, C4);
    if((E1 == C1) && (E2 == C2) && (E3 == C3) && (E4 == C4)){       //checks if the code matches
        commandWrite(1);     // clear display
        commandWrite(0x80);  // set cursor at beginning of first line
            for (k=0;k<16;k++){
                dataWrite (line4[k]);   //prints correct! to the lcd screen
            }
            printf("\ncorrect!\n");
            p=0;
            note = 0;
            __delay_cycles(3000000); // 1s delay
        main();
    }
    else{
        printf("\nIncorrect entry! Please try again.\n");
        commandWrite(1);     // clear display
        commandWrite(0x80);  // set cursor at beginning of first line
            for (k=0;k<16;k++){
                dataWrite (line2[k]);
            }
            commandWrite(0xC0);  // set cursor at beginning of second line
            for (k=0;k<16;k++){
                dataWrite (line3[k]);
            }
            __delay_cycles(3000000); // 1s delay
        disarm();   //re-enters itself
    }
    }
    }
    adcCheck();
    TIMER_A0->CCR[3] = backlight; //sets LCD duty cycle
}
}
    /****| SetupPort2Interrupts | *****************************************
    * Brief: initialization for the button interrupt ports
    * Inputs: none
    * Outputs: none
    */
    void SetupPort2Interrupts(void){
        P2->SEL0 &=~ 0x30; //P2.4- P2.5 Input, Pull Up Resistor
        P2->SEL1 &=~ 0x30;
        P2->DIR &=~ 0x30;
        P2->REN |= 0x30;
        P2->OUT |= 0x30; //Input, Pull Up Resistor
        P2->IES |= 0x30; //Set pin interrupt to trigger for P2.4-P2.5
        P2->IFG = 0;
        P2->IE = 0x30;      //enable interrupt P2.4 - P2.5
    }
    /****| PORT2_IRQHandler | *****************************************
    * Brief: Detects which button has been pressed and either turns on
    * the motor or turns the LEDs on/off.
    * Inputs: int rb, bb, gb, orb, obb, ogb
    * Outputs: int speed, rb, bb, gb, orb, obb, ogb
    */
    void PORT2_IRQHandler(void){
        if (P2->IFG & 0x10){   //If P2.4 had an interrupt
            __delay_cycles(600000);         //switch bounce
                if (P2->IFG & 0x10){
        speed = 0; //sets duty cycle to 0
        printf("motor stop\n");
        P2->IFG &= ~BIT4;
                }
            P2->IFG &= ~BIT4;   //Clear flag
    }
    if (P2->IFG & 0x20){ //If P2.5 had an interrupt
        __delay_cycles(600000);
            if ((P2->IFG & 0x20)){
                if ((rb == 0) && (gb == 0) && (bb == 0)){
                    rb = orb;     //resets to old value
                    gb = ogb;
                    bb = obb;
                }
                else{
                    orb = rb;     //saves previous values in a temporary variable
                    ogb = gb;
                    obb = bb;
                    rb = 0;     //turns off lights
                    gb = 0;
                    bb = 0;
                }
                printf("lights onoff\n");
        P2->IFG &= ~BIT5;
            }
            P2->IFG &= ~BIT5;
    }
    }


