/*
 * File: main.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on December 10, 2021
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
int button_flag = 0, show_frequency = 0;

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
    
    while(1){
        NewClk(500); // Changes clock to 500 kHz
        if(button_flag == 1){
            ADCinit(11); // Calls ADCinit() function
            uint64_t adc_value = do_ADC(); // Calls do_ADC();
            NewClk(8); // Changes clock to 8 MHz
            Disp2String("\n\rVOLTMETER Voltage ="); // Prints newline and carriage return
            float voltage = (3.25*adc_value)/1023; // Scales adc_value for graph
            Disp2Float(voltage); // Displays adc_value in hexadecimal format
            Disp2String("V"); // Prints newline and carriage return
            Delay_ms(1000); // Delays for 1000 ms
            show_frequency = 1; // Sets show_frequency flag to true
        }
        else if(button_flag == 2){
            ADCinit(5); // Calls ADCinit() function
            uint64_t adc_value = do_ADC(); // Calls do_ADC() function
            NewClk(8); // Changes clock to 8 MHz
            Disp2String("\n\rOHMMETER Resistance ="); // Prints newline and carriage return
            float voltage = (3.25*adc_value)/1023; // Scales adc_value for graph
            uint32_t resistance = (uint32_t)((1000 * voltage)/(3.25 - voltage)); // Scales adc_value for graph
            Disp2Dec(resistance); // Displays adc_value in hexadecimal format
            Disp2String("OHM"); // Prints newline and carriage return
            Delay_ms(1000); // Delays 1000 ms
            show_frequency = 1; // Sets show_frequency flag to true
        }
        else if(button_flag == 3){
            // NewClk(8); // Changes clock to 8 MHz
            float ctmu_value = doCTMU(); // Calls doCTMU function
            NewClk(8); // Changes clock to 8 MHz
            Disp2String("\n\rCapacitance ="); // Prints newline and carriage return
            Disp2Dec(ctmu_value); // Displays CTMU value
            Disp2String("uF"); // Prints newline and carriage return
            Delay_ms(1000); // Delays 1000 ms
            show_frequency = 1; // Sets show_frequency flag to true
        }
        else {
            Idle(); // Puts CPU into idle
        }
  
        if(show_frequency){
            TRISBbits.TRISB15 = 0; // Sets TRISB15 to 0
            REFOCONbits.ROEN = 0; // Sets ROEN to 0
            REFOCONbits.ROSSLP = 0; // Sets ROSSLP to 0
            REFOCONbits.ROSEL = 0; // Sets ROSEL to 0
            REFOCONbits.RODIV = 0b1111; // Sets RODIV to 0b1111
            REFOCONbits.ROEN = 0; // Sets ROEN to 0 
            NewClk(8); // Changes clock to 8 MHz
            Disp2String("\n\rFrequency ="); // Displays frequency
            Disp2Dec(PORTBbits.RB15); // Displays value of RB2
            Disp2String("Hz\n\r"); // Prints newline and carriage return
            show_frequency = 0; // Resets show_frequency flag
        }
    }
    
    return; // Returns main() function
} 
