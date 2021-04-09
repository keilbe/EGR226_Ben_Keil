/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  04/09/2021
*Project :  Lab 11 part 2
*File :  lab 11 part 2. c
*Description : IR detector and emitter
**********************************************************************/
#include "msp.h"
#include "stdio.h"
#define LED_RED    (1U << 0)    //pin P2.0 internal LED
uint8_t detect10Hz;
volatile uint16_t lastedge, currentedge, period;    //maybe remove this line entirely
void TimerA2Init(void);
void LED (void);
void SysTick_Init (void);

/****| main | *****************************************
* Brief: carries out initilization steps then enters
* an empty while loop
* Inputs: none
* Outputs: none
*/
int main(void){
__disable_irq ( ); // disable global interrupt for initilizations
WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    P2OUT = 0x01;
    P2DIR |= LED_RED;       // P2.0 output
    P2->SEL0 |= BIT5; // TA0.CCI2A input capture pin, second function
    P2->SEL1 &= ~BIT5; // TA0.CCI2A input capture pin, second function
    P2->DIR &= ~ BIT5;
    SysTick_Init();
TIMER_A0->CTL = 0x0214; //SMCLK, UP mode,clock divisor 8, clear TA0R
TIMER_A0->CCTL[2] = 0x4910; //Capture rising edge, Use CCI2A, Enable capture interrupt, Enable capture mode, Synchronous capture
NVIC->ISER[0] = 1 << ((TA0_N_IRQn) & 31); // Enable interrupt in NVIC vector

TimerA2Init();

NVIC_EnableIRQ(PORT2_IRQn); //enable port 2 interrupts
NVIC_SetPriority(TA0_N_IRQn, 3);
NVIC_EnableIRQ(TA0_N_IRQn);

__enable_irq ( ); // Enable global interrupt
while (1){      //empty loop

}
}
/****| TA0_N_IRQHandler | *****************************************
* Brief: function is called if an interrupt is triggered.
* Inputs: none
* Outputs: detect10Hz
*/
void TA0_N_IRQHandler(void){ // Timer A0 CCR2 interrupt service routine
    TIMER_A0->CCTL[2] &=~1; // Clear the TA.2 interrupt flag
        currentedge = SysTick -> VAL;
        period = currentedge - lastedge;
        lastedge = currentedge;

        if ((( 20064 < period ) && ( period < 24440))){ // within 10% of 10Hz period
            detect10Hz = 1;
        }
        else{
            detect10Hz = 0;
        }
        LED();          //calls led function
    TIMER_A0->CCTL[2] &= ~(TIMER_A_CCTLN_CCIFG); // Clear the interrupt flag
}
/****| LED | *****************************************
* Brief: function from within the interrupt function
* and will turn on the led and turn it off if an
* interrupt has not been triggered recently.
* Inputs: detect10Hz
* Outputs: none
*/
void LED (void){
if (detect10Hz == 1){   //if the period is correct the led will be turned on
    P2OUT |= LED_RED;       //turns on red LED
}
else{                   //if the period is wrong the LED will be turned off
    P2OUT &= ~LED_RED;       //turns off red LED
}
__delay_cycles(500000);  //if there is nothing blocking the emitter an interrupt will happen before his delay is over
P2OUT &= ~LED_RED;       //if no interrupt happens this line will be read and the LED will be turned off
}
/****| TimerA2Init | *****************************************
* sets up pin P5.6 to create a PWM signal that can be used to
* create an IR signal
* Inputs: none
* Outputs: none
*/
void TimerA2Init(void){
P5->SEL0 |= BIT6;
P5->SEL1 &=~ BIT6;
P5->DIR |= BIT6;
TIMER_A2->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK
TIMER_A_CTL_MC__UP | // Up mode
TIMER_A_CTL_CLR | //Clear timer
TIMER_A_CTL_ID__8; //clock divisor of 8
TIMER_A2->CCR[0] = 37505; // 10 Hz
TIMER_A2->CCR[1] = 37505 / 2; // 50% duty cycle
TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
}
/****| SysTick_Init | *****************************************
* Brief: initilization sequence for the systick timer
* Inputs: none
* Outputs: none
*/
void SysTick_Init (void){
    SysTick -> CTRL = 0; // disable SysTick During step
    SysTick -> LOAD = 0x00FFFFFF; // max reload value
    SysTick -> VAL = 0; // any write to current clears it
    SysTick -> CTRL = 0x00000005; // enable systic, 3MHz, No interrupts
}
