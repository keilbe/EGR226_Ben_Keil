/**************************************************************************************
* Author: Ben Keil
* Course: EGR 226 - 905
* Date: 01/28/2021
* Project: Lab 2 part 1
* File: lab 2 Keil part 1.c
* Description: resistor color code tool
**************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
    void prompt(int resistor);
    void calcResistorColors(int resistor);
    void getColorBands(char* ch1, char* ch2, char* ch3, char* ch4);
    void calcResistance(char ch1, char ch2, char ch3, char ch4);
    float co1, co2, co3, co4;
    char ch1, ch2, ch3, ch4;

int main(){
/****| main | *****************************************
* Brief: The purpose of the main is to call the prompt
* function, ask the user what they would like to
* calculate, and repeat the code if the user wishes to
* do so. This can be seen in code 1.
* Inputs: user inputs
* outputs: color code
*/
char comB, comOP;
int resistor;
int resistorColors;

do{
prompt(resistor); // calls function to prompt user for a resistance
printf("What would you like to find? Enter C for color code. Enter R for resistance.\n");
scanf(" %c", &comOP);
while (comOP != 'C' && comOP != 'R'){ //error checking
printf("Invalid! Please try again\n");
scanf(" %c", &comOP);
}
if (comOP == 'C'){
calcResistorColors(resistorColors); //calls function to calculate resistor color codes
}
if (comOP == 'R'){
getColorBands(ch1, ch2, ch3, ch4); //calls function to calculate resistance
}
printf("Would you like to do another resistor?\nEnter Y to continue. Enter any other key to stop.\n"); //allows user to repeat code
scanf(" %c", &comB);
}
while (comB == 'y' || comB == 'Y');
}

void prompt(int resistor){
/****| prompt | *****************************************
* Brief: displays resistor color code guide and prompts
* the user to enter a resistance
* Inputs: none
* Outputs: resistor value chart
*/
printf("--------------------Resistor Codes---------------------\n");
 printf("|Character| Color | Band 1 & 2 | Band 3 | Band 4 |\n");
 printf("| K | Black | 0 |*1 |+/- 1%% |\n");
 printf("| N | Brown | 1 |*10 |+/- 2%% |\n");
 printf("| R | Red | 2 |*100 | |\n");
 printf("| O | Orange | 3 |*1,000 | |\n");
 printf("| Y | Yellow | 4 |*10,000 | |\n");
 printf("| G | Green | 5 |*100,000 |+/- 0.5%% |\n");
 printf("| B | Blue | 6 |*1,000,000 |+/- 0.25%%|\n");
 printf("| V | Violet | 7 |*10,000,000|+/- 0.1%% |\n");
 printf("| E | Grey | 8 | |+/- 0.05%%|\n");
 printf("| W | White | 9 | | |\n");
 printf("| D | Gold | | |+/- 5%% |\n");
 printf("| S | Silver | | |+/- 10%% |\n");
 printf("-------------------------------------------------------\n\n");
 return 0;
}

void calcResistorColors(int resistorColors){
/****| calcResistorColors | *****************************************
* Brief: This function takes the user input and error checks
* it. It then finds the number of digits in the number as
* well as determines what the first two digits are. Once all
* of this has been determined the function prints out the
* appropriate color code for the resistor.
* Inputs: user inputs
* outputs: color code
*/
    int i;
    int resistorValue;
    int status;
    int temp;
    float number;
    int bandThree;
    int count = 0;
    int numOne;
    int numTwo;
    float numThree;

printf("Please enter a resistance\n");
status = scanf("%d", &resistorValue);

while (resistorValue > 100000000 || resistorValue < 0){ //error checking for resistor value
    while((temp=getchar()) != EOF && temp != '\n');
    printf("Invalid entry! please try again\n");
    status = scanf("%d", &resistorValue);
}
printf("Please enter a tolerance\n");
scanf(" %f", &numThree);
while (numThree != 1 && numThree != 2 && numThree != .5 && numThree != .25 && numThree != .1 && numThree != .05 && numThree != 5 && numThree != 10){  //error checks inputted tolerance
        while((temp=getchar()) != EOF && temp != '\n');
    printf("Invalid! Please try again\n");
    scanf(" %f", &numThree);
}
number = resistorValue;
while (number >= 1){                                     //finds number of digits by dividing by 10 in order to easily determine the multiplier and which digits are the first and second ones
    number = (number / 10);
    count = count + 1;
}
numOne = number*10;                                     //finds first digit
numTwo = ((number*100) - (numOne*10));                 //finds second digit

if(resistorValue < 10){                                 //special case for single digit numbers
    numOne = 0;
    numTwo = resistorValue;
}
if (numOne == 9){                                       //first digit
    printf("White");
}
if (numOne == 8){
    printf("Grey");
}
if (numOne == 7){
    printf("Violet");
}
if (numOne == 6){
    printf("Blue");
}
if (numOne == 5){
    printf("Green");
}
if (numOne == 4){
    printf("Yellow");
}
if (numOne == 3){
    printf("Orange");
}
if (numOne == 2){
    printf("Red");
}
if (numOne == 1){
    printf("Brown");
}
if (numOne == 0){
    printf("Black");
}
printf(" - ");
if (numTwo == 9){                                      //second digit
    printf("White");
}
if (numTwo == 8){
    printf("Grey");
}
if (numTwo == 7){
    printf("Violet");
}
if (numTwo == 6){
    printf("Blue");
}
if (numTwo == 5){
    printf("Green");
}
if (numTwo == 4){
    printf("Yellow");
}
if (numTwo == 3){
    printf("Orange");
}
if (numTwo == 2){
    printf("Red");
}
if (numTwo == 1){
    printf("Brown");
}
if (numTwo == 0){
    printf("Black");
}

printf(" - ");
count = count-2;
if (count <= 0){                //special case for single digit numbers
    count = 0;
}
if (count == 8){                                    //third color (multiplier)
    printf("Grey");
}
if (count == 7){
    printf("Violet");
}
if (count == 6){
    printf("Blue");
}
if (count == 5){
    printf("Green");
}
if (count == 4){
    printf("Yellow");
}
if (count == 3){
    printf("Orange");
}
if (count == 2){
    printf("Red");
}
if (count == 1){
    printf("Brown");
}
if (count == 0){
    printf("Black");
}
printf(" - ");
if (numThree == 5){                             //tolerance
    printf("Gold");
}
if (numThree == 10){
    printf("Silver");
}
if (numThree == .05){
    printf("Grey");
}
if (numThree == .1){
    printf("Violet");
}
if (numThree == .25){
    printf("Blue");
}
if (numThree == .5){
    printf("Green");
}
if (numThree == 2){
    printf("Brown");
}
if (numThree == 1){
    printf("Black");
}
printf("\n");
return 0;
}

void getColorBands(char* ch1, char* ch2, char* ch3, char* ch4){      //collect character inputs
/****| getColorBands | *****************************************
* Brief: This function exists to take the user inouts for the
* character code, error check them, and then call a function to
* calculate the resistance and tolerance.
* Inputs: user inputs
* Outputs: ch1, ch2, ch3, ch4 (character codes)
*/
printf("enter first charicter code\n");            //first digit
scanf(" %c", &ch1);
while (ch1 != 'K' && ch1 != 'N' && ch1 != 'R' && ch1 != 'O' && ch1 != 'Y' && ch1 != 'G' && ch1 != 'B' && ch1 != 'V' && ch1 != 'E' && ch1 != 'W'){ //error checking to see if input is valid
printf("Invalid! Please try again\n");
scanf(" %c", &ch1);
}
printf("enter second charicter code\n");           //second digit
scanf(" %c", &ch2);
while (ch2 != 'K' && ch2 != 'N' && ch2 != 'R' && ch2 != 'O' && ch2 != 'Y' && ch2 != 'G' && ch2 != 'B' && ch2 != 'V' && ch2 != 'E' && ch2 != 'W'){
printf("Invalid! Please try again\n");
scanf(" %c", &ch2);
}
printf("enter third charicter code\n");             //multiplier
scanf(" %c", &ch4);
while (ch4 != 'K' && ch4 != 'N' && ch4 != 'R' && ch4 != 'O' && ch4 != 'Y' && ch4 != 'G' && ch4 != 'B' && ch4 != 'V' && ch4 != 'D' && ch4 != 'S'){
printf("Invalid! Please try again\n");
scanf(" %c", &ch4);
}
printf("enter fourth charicter code\n");            //tolerance
scanf(" %c", &ch3);
while (ch3 != 'K' && ch3 != 'N' && ch3 != 'G' && ch3 != 'B' && ch3 != 'V' && ch3 != 'E' && ch3 != 'D' && ch3 != 'S'){
printf("Invalid! Please try again\n");
scanf(" %c", &ch3);
}
calcResistance(ch1, ch2, ch3, ch4);                 //calls calcResistance function
}

void calcResistance(char ch1, char ch2, char ch3, char ch4){ //assigns approved inputs to numbers
/****| calcResistance | *****************************************
* Brief: This function exists to assign the provided
* character codes to number values, calculate the resistance,
* and provide the user with the resistance and the tolerance
* Inputs: ch1, ch2, ch3, ch4 (character codes)
* Outputs: rVal (resistance), co3 (tolerance)
*/
if (ch1 == 'K'){        //the first character is used to find the first digit
co1 = 0;
}
if (ch1 == 'N'){
co1 = 1;
}
if (ch1 == 'R'){
co1 = 2;
}
if (ch1 == 'O'){
co1 = 3;
}
if (ch1 == 'Y'){
co1 = 4;
}
if (ch1 == 'G'){
co1 = 5;
}
if (ch1 == 'B'){
co1 = 6;
}
if (ch1 == 'V'){
co1 = 7;
}
if (ch1 == 'E'){
co1 = 8;
}
if (ch1 == 'W'){
co1 = 9;
}
if (ch2 == 'K'){        //the second character is used to find the second digit
co2 = 0;
}
if (ch2 == 'N'){
co2 = 1;
}
if (ch2 == 'R'){
co2 = 2;
}
if (ch2 == 'O'){
co2 = 3;
}
if (ch2 == 'Y'){
co2 = 4;
}
if (ch2 == 'G'){
co2 = 5;
}
if (ch2 == 'B'){
co2 = 6;
}
if (ch2 == 'V'){
co2 = 7;
}
if (ch2 == 'E'){
co2 = 8;
}
if (ch2 == 'W'){
co2 = 9;
}
if (ch4 == 'K'){      //the third character is used to assign the multiplier
co4 = 1;
}
if (ch4 == 'N'){
co4 = 10;
}
if (ch4 == 'R'){
co4 = 100;
}
if (ch4 == 'O'){
co4 = 1000;
}
if (ch4 == 'Y'){
co4 = 10000;
}
if (ch4 == 'G'){
co4 = 100000;
}
if (ch4 == 'B'){
co4 = 1000000;
}
if (ch4 == 'V'){
co4 = 10000000;
}
if (ch4 == 'D'){
co4 = .1;
}
if (ch4 == 'S'){
co4 = .01;
}
if (ch3 == 'K'){       //the fourth character is used to assign a tolerance
co3 = 1;
}
if (ch3 == 'N'){
co3 = 2;
}

if (ch3 == 'G'){
co3 = .5;
}
if (ch3 == 'B'){
co3 = .25;
}
if (ch3 == 'V'){
co3 = .1;
}
if (ch3 == 'E'){
co3 = .05;
}
if (ch3 == 'D'){
co3 = 5;
}
if (ch3 == 'S'){
co3 = 10;
}
float rVal;
rVal = ((co1*10)+(co2))*co4;                     //calculates resistor value
printf("Resistor value is: %f\n", rVal);          //prints resistor value
printf("Tolerance value is +/- %f\n", co3);        //prints tolerance value
}

