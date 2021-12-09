/*
 * File: main.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on December 8, 2021
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
#include "CTMU.h"
//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled


// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");}

// Global variables
int button_flag = 0;

void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void){
    if(IFS1bits.CNIF == 1){
        IOcheck(); // Calls IOcheck() function
    }
    IFS1bits.CNIF = 0; // Clears CN interrupt flag status
}

void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void) {
    IFS0bits.T2IF = 0; // Clears timer 2 interrupt flag
    T2CONbits.TON = 0; // Stops timer
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void){
    IFS0bits.AD1IF = 0; // Clears ADC1 interrupt flag
}

//MAIN
void main(void) {
    IOinit(); // Calls IOinit() function
    ConfigCN(); // Calls ConfigCN() function
    NewClk(500); // Changes clock to 500 kHz
    while(1){
        NewClk(500); // Changes clock to 500 kHz
        if(button_flag == 1){
            ADCinit(11); // Calls ADCinit() function
            uint64_t adc_value = do_ADC();
            NewClk(8); // Changes clock to 8 MHz
            Disp2String("\n\rVOLTMETER Voltage ="); // Prints newline and carriage return
            float voltage = (3.25*adc_value)/1023; // Scales adc_value for graph
            Disp2Float(voltage); // Displays adc_value in hexadecimal format
            Disp2String("V"); // Prints newline and carriage return
            Delay_ms(1000);
        }
        else if(button_flag == 2){
            ADCinit(5); // Calls ADCinit() function
            uint64_t adc_value = do_ADC();
            NewClk(8); // Changes clock to 8 MHz
            Disp2String("\n\rOHMMETER Resistance ="); // Prints newline and carriage return
            float voltage = (3.25*adc_value)/1023; // Scales adc_value for graph
            uint32_t resistance = (uint32_t)((1000 * voltage)/(3.25 - voltage)); // Scales adc_value for graph
            Disp2Dec(resistance); // Displays adc_value in hexadecimal format
            Disp2String("OHM"); // Prints newline and carriage return
            Delay_ms(1000);
        }
        else if(button_flag == 3){
            float ctmu_value = doCTMU();
            NewClk(8); // Changes clock to 8 MHz
            Disp2String("\n\rCapacitance ="); // Prints newline and carriage return
            Disp2Float(ctmu_value);
            Disp2String("uF"); // Prints newline and carriage return
            Delay_ms(1000);
        }
    }
    
    return; // Returns main() function
} 
