/*
 * File: main.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on November 28, 2021
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
#include "ADC.h"
//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled


// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");}

// Global variables
int minutes = 0; // Initializes minutes
int seconds = 0; // Initializes seconds
int inc_min_flag = 0; // Initializes inc_min_flag
int inc_sec_flag = 0; // Initializes inc_sec_flag
int countdown = 0; // Initializes countdown
int is_paused = 0; // Initializes is_paused
int alarm = 0; // Initializes alarm

void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void){
    if(IFS1bits.CNIF == 1){
        IOcheck(); // Calls IOcheck() function
    }
    IFS1bits.CNIF = 0; // Clears CN interrupt flag status
}

void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void) {
    IFS0bits.T2IF = 0; // Clears timer 2 interrupt flag
    T2CONbits.TON = 0; // Stops timer
    if(is_paused && (PORTBbits.RB4 == 0)){
        LATBbits.LATB8 = 0; // Sets RB8 to lo
        minutes = 0; // Resets minutes
        seconds = 0; // Resets seconds
        countdown = 0; // Resets countdown
    }
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void){
    IFS0bits.AD1IF = 0; // Clears ADC1 interrupt flag
}

//MAIN
void main(void) {
    IOinit(); // Calls IOinit() function
    ConfigCN(); // Calls ConfigCN() function
    NewClk(32); // Changes clock to 500 kHz
    while(1){
        alarm = 0; // Resets alarm
        NewClk(8); // Changes clock to 8 MHz
        Disp2String("         "); // Clears alarm message
        Disp2String("\r"); // Displays carriage return
        Disp2Dec(minutes); // Displays minutes
        XmitUART2('m', 1); // Displays m
        XmitUART2(' ', 1); // Displays space
        XmitUART2(':', 1); // Displays :
        Disp2Dec(seconds); // Displays seconds
        XmitUART2('s', 1); // Displays s
        Delay_ms(200); // Delays for 200 ms
        if(countdown) {
            if(seconds == 0) {
                if(minutes == 0) {
                    countdown = 0; // Resets countdown
                    NewClk(8); // Changes clock to 8 MHz
                    Disp2String("\r"); // Displays carriage return
                    Disp2Dec(minutes); // Displays minutes
                    XmitUART2('m', 1); // Displays m
                    XmitUART2(' ', 1); // Displays space
                    XmitUART2(':', 1); // Displays :
                    Disp2Dec(seconds); // Displays seconds
                    XmitUART2('s', 1); // Displays s
                    Disp2String(" -- ALARM"); // Displays alarm message
                    LATBbits.LATB8 = 1; // Sets RB8 to hi
                    Idle(); // Returns to idle
                }
                else {
                    minutes--; // Decrements minutes
                    seconds = 59; // Sets seconds to 59
                }
            }
            else {
                seconds--; // Decrements seconds
            }
            if(is_paused) {
                LATBbits.LATB8 = 0; // Sets RB8 to lo
            }
            else {
                LATBbits.LATB8 = !PORTBbits.RB8; // Toggles RB8 value
                if(!is_paused) {
                    Delay_ms(1000); // Delays for 1000 ms
                }
            }
            
        }
        if(inc_min_flag) {
            LATBbits.LATB8 = 0; // Sets RB8 to lo
            if(minutes == 59){
                minutes = 0; // Resets minutes
            }
            else{
                minutes++; // Increments minutes
            }
            Delay_ms(200); // Delays for 200 ms
        }
        if(inc_sec_flag) {
            LATBbits.LATB8 = 0; // Sets RB8 to lo
            if(seconds == 59){
                seconds = 0; // Resets seconds
            }
            else{
                seconds++; // Increments seconds
            }
            Delay_ms(200); // Delays for 200 ms
        }
        if(is_paused) {
            Delay_ms(3000); // Delays for 3000 ms
        }
    }
    
    return; // Returns main() function
} 
