/*
 * File: main.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on October 17, 2021
 */

// MPLAB header libraries
#include <xc.h>

#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// User header files
#include "config.h"
#include "gpio.h"
#include "clocks.h"
#include "timers.h"
#include "UART2.h"

//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled


// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");}

// Global variables
uint16_t delay = 1; // Initializes delay variable
uint16_t turnoff = 1; // Initializes turnoff variable
uint16_t turnon = 0; // Initializes turnon variable
int buttons[3]; // Initializes buttons array
int blink; // Initializes blink variable
int three; // Initializes three variable
int two; // Initializes two variable
int nothing_pressed; // Initializes nothing_pressed variable
char pb; // Initializes pb variable
char pb2; // Initializes pb2 variable

void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void){
    if(IFS1bits.CNIF == 1){
        IOcheck(); // Calls IOcheck() function
    }
    IFS1bits.CNIF = 0; // Clear CN interrupt flag status
}

void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void) {
    IFS0bits.T2IF = 0; // Clear timer 2 interrupt flag
    T2CONbits.TON = 0; // stop timer
    if(turnon == 1){
        LATBbits.LATB8 = 1; // turn on led
    }
    else if(turnoff == 1){
        Idle(); // Put MCU in idle mode - only CPU off
    }
    else{
        LATBbits.LATB8 = !PORTBbits.RB8; // switch to next state
    }
}

//MAIN
void main(void) {
    IOinit(); // Calls IOinit() function
    ConfigCN(); // Calls ConfigCN() function
    while(1)
    {
        if(three == 1){
            NewClk(8); // Change clock to 8 MHz
            Disp2String("\n\r All PBs pressed "); // Displays string
            NewClk(32); // Change clock to 32 kHz
            three = 0; // Sets three to 0
        }
        else if(two == 1){
            NewClk(8); // Change clock to 8 MHz
            Disp2String("\n\r PB"); // Displays string
            XmitUART2(pb, 1); // Displays char
            Disp2String(" and PB"); // Displays string
            XmitUART2(pb2, 1); // Displays char
            Disp2String(" are pressed "); // Displays string
            NewClk(32); // Change clock to 32 kHz
            two = 0; // Sets two to 0
        }
        else if(blink == 1){
            NewClk(8); // Change clock to 8 MHz
            Disp2String("\n\r PB"); // Displays string
            XmitUART2(pb, 1); // Displays char
            Disp2String(" is pressed "); // Displays string
            NewClk(32); // Change clock to 32 kHz
            blink = 0; // Sets blink to 0
        }
        else if(nothing_pressed == 1){
            NewClk(8); // Change clock to 8 MHz
            Disp2String("\n\r Nothing Pressed "); // Displays string
            NewClk(32); // Change clock to 32 kHz
            blink = 0; // Sets blink to 0
        }
        Delay_ms(delay); // Calls Delay_ms() function
    }
    
    return; // returns main() function
}