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

uint16_t delay = 5000;
uint16_t turnoff = 1;
uint16_t turnon = 0;

void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void){
    if(IFS1bits.CNIF == 1){
        IFS0bits.T2IF = 0; // Clear timer 2 interrupt flag
        T2CONbits.TON = 0; // stop timer
        TMR2 = 0; // Sets TMR2 to 0
        turnoff = 0; // Clear turnoff flag
        turnon = 0; // Clear turnon flag
        if ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2) {
            // checks if two or more buttons are pressed
            delay = 1; // Set delay to 1
            turnon = 1; // turnon flag
        }
        else if (PORTAbits.RA2 == 0) {
            delay = 1000; // Set delay to 1 seconds
        }
        else if (PORTBbits.RB4 == 0) {
            delay = 2000; // Set delay to 2 seconds          
        }
        else if (PORTAbits.RA4 == 0) {
            delay = 3000; // Set delay to 3 seconds
        }
        else {
            delay = 5000; // Reset delay
            turnoff = 1; // Set turnoff flag
            LATBbits.LATB8 = 0; // turnoff LED
        }
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
//        LATBbits.LATB8 = 0; // turn off led
        Idle();
    }
    else{
        LATBbits.LATB8 = !PORTBbits.RB8; // switch to next state
    }
}

void Delay_ms(uint16_t time_ms) {
    
    // Change Clock
    // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz
    NewClk(32); // Select an appropriate clock speed
    
    // T2CON Configuration
    T2CONbits.T32 = 0; // Timer2 and Timer3 act as two 16-bit timers
    T2CONbits.TCKPS = 0; // Timer2 Input Clock Prescale Select Bits to 1:1
    T2CONbits.TCS = 0; // Internal clock (Fosc/2)
    T2CONbits.TGATE = 0; // Gated time accumulation enabled
    
    PR2 = time_ms * 16; // Sets PR2 to calculated value
    
    IEC0bits.T2IE = 1; // Enable timer interrupt
    IFS0bits.T2IF = 0; // Clear Timer2 flag
    
    // Timer 2 interrupt configuration
    IPC1bits.T2IP2 = 0x4; // Set priority level 4
    IPC1bits.T2IP1 = 0x5; // Set priority level 5
    IPC1bits.T2IP0 = 0x6; // Set priority level 6
    
    T2CONbits.TON = 1; // Starts 16-bit Timer2
    T2CONbits.TSIDL = 0; // Continue module operation in Idle mode
    
    Idle(); // Put MCU in idle mode - only CPU off
}

//MAIN
void main(void) {
     
    // Initialize IOs for low-power wake-up
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
    TRISBbits.TRISB8 = 0; // Make RB8 as output
    
    TRISAbits.TRISA2 = 1; // Make RA2 as input (PB1)
    TRISAbits.TRISA4 = 1; // Make RA4 as input (PB2)
    TRISBbits.TRISB4 = 1; // Make RB4 as input (PB3)
    
    CNEN1bits.CN0IE = 1; // Enable CN0 bit
    CNEN1bits.CN1IE = 1; // Enable CN1 bit
    CNEN2bits.CN30IE = 1; // Enable CN30 bit
    
    CNPU2bits.CN30PUE = 1; //Pull up enabled in CN30
    CNPU1bits.CN0PUE = 1; // Pull up enabled in CN0
    CNPU1bits.CN1PUE = 1; // Pull up enabled in CN1
    
    IPC4bits.CNIP0 = 0x1; // Set interrupt priority to 1
    IPC4bits.CNIP1 = 0x2; // Set interrupt priority to 2
    IPC4bits.CNIP2 = 0x3; // Set interrupt priority to 3
    
    IFS1bits.CNIF = 0; // Clear CN flag
    IEC1bits.CNIE = 1; // Enable CN interrupt bit
    
    while(1)
    {
        Delay_ms(delay); // Calls Delay_ms() function
    }
    
    
    return;
}
