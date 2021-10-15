/*
 * File:   main.c
 * Author: Rushi V
 *
 * Created on September 26, 2020, 9:17 PM
 */

// MPLAB header libraries
#include <xc.h>

#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// User header files
//#include "IOs.h"
#include "ChangeClk.h"


//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled


// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //

uint16_t delay = 1;
uint16_t state1 = 0;
uint16_t state2 = 0;

void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void){
    if(IFS1bits.CNIF == 1){
        if ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2) {
            delay = 1;
            state1 = 1;
            state2 = 1;
        }
        else if (PORTAbits.RA2 == 0) {
            delay = 1000;
            state1 = 1;
            state2 = 0;
        }
        else if (PORTBbits.RB4 == 0) {
            delay = 2000;
            state1 = 1;
            state2 = 0;            
        }
        else if (PORTAbits.RA4 == 0) {
            delay = 3000;
            state1 = 1;
            state2 = 0;
        }
        else {
            delay = 1;
            state1 = 0;
            state2 = 0;
        }
        IFS0bits.T2IF = 0; // Clear timer 2 interrupt flag
        T2CONbits.TON = 0; // stop timer
        TMR2 = 0;
    }
    IFS1bits.CNIF = 0;
}

void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void) {
    IFS0bits.T2IF = 0; // Clear timer 2 interrupt flag
    T2CONbits.TON = 0; // stop timer
    // TMR2flag = 1; // global variable created by user
}

void Delay_ms(uint16_t time_ms) {
    
    // configure clock
    NewClk(32); // Select an appropriate clock speed
    
    // T2CON Configuration
    T2CONbits.T32 = 0; // Timer2 and Timer3 act as two 16-bit timers
    T2CONbits.TCKPS = 0;
    T2CONbits.TCS = 0; // Internal clock (Fosc/2)
    T2CONbits.TGATE = 0; // Gated time accumulation enabled
    
    PR2 = (int)time_ms * 16;
    
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
    
    // Timer 2 interrupt configuration
    IPC1bits.T2IP2 = 0x4;
    IPC1bits.T2IP1 = 0x5;
    IPC1bits.T2IP0 = 0x6;
    
    T2CONbits.TON = 1; // Starts 16-bit Timer2
    T2CONbits.TSIDL = 0; // Continue module operation in Idle mode
    
    Idle();
}

//MAIN
void main(void) {
     
    // Change Clock
    // NewClk(32); // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz
     
    // Initialize IOs for low-power wake-up
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
    TRISBbits.TRISB8 = 0;
    
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA4 = 1;
    TRISBbits.TRISB4 = 1;
    
    CNEN1bits.CN0IE = 1;
    CNEN1bits.CN1IE = 1;
    CNEN2bits.CN30IE = 1;
    
    CNPU2bits.CN30PUE = 1;
    CNPU1bits.CN0PUE = 1;
    CNPU1bits.CN1PUE = 1;
    
    IPC4bits.CNIP0 = 0x1;
    IPC4bits.CNIP1 = 0x2;
    IPC4bits.CNIP2 = 0x3;
    
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    
    while(1)
    {
        LATBbits.LATB8 = state1;
        Delay_ms(delay);
        LATBbits.LATB8 = state2;
        Delay_ms(delay);
    }
    
    
    return;
}
