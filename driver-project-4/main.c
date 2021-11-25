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
int ADC_flag; // Initializes ADC_flag

void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void){
    if(IFS1bits.CNIF == 1){
        IOcheck(); // Calls IOcheck() function
    }
    IFS1bits.CNIF = 0; // Clear CN interrupt flag status
}

void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void) {
    IFS0bits.T2IF = 0; // Clear timer 2 interrupt flag
    T2CONbits.TON = 0; // stop timer
}
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void){
    IFS0bits.AD1IF = 0; // Clears ADC1 interrupt flag
}

//MAIN
void main(void) {
    IOinit(); // Calls IOinit() function
    ConfigCN(); // Calls ConfigCN() function
    ADCinit(); // Calls ADCinit() function
    NewClk(500); // Changes clock to 500 kHz
    while(1){
        if(ADC_flag) {
            uint64_t adc_value = 0; // Resets adc_value variable to 0
            uint64_t current; // Initializes current
            int i; // Initializes i
            for (i = 0; i <= 10000; i++) {
                if((i%1000 == 0)&&(i != 0)){
                    adc_value += current/1000; // Saves 1s average to adc_value
                    current = 0; // resets current variable to 0
                }
                current += do_ADC(); // Adds return value of do_ADC to current
            }
            adc_value = adc_value/10; // Averages sum in adc_value
            NewClk(8); // Changes clock to 8 MHz
            Disp2String("\n\r"); // Prints newline and carriage return
            float graph = (40*adc_value)/1023; // Scales adc_value for graph
            XmitUART2('|', (int)graph); // Displays bar graph
            Disp2Hex(adc_value); // Displays adc_value in hexadecimal format
            NewClk(500); // Changes clock to 500 kHz
            ADC_flag = 0; // Resets ADC_flag to false
            Idle(); // Returns to idle state
        }
        
    }
    
    return; // returns main() function
} 
