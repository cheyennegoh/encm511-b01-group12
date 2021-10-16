/*
 * File: gpio.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on October 17, 2021
 */

#include "xc.h"
#include "gpio.h"

void IOinit() {
    // Initialize IOs for low-power wake-up
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
    TRISBbits.TRISB8 = 0; // Make RB8 as output
    
    TRISAbits.TRISA2 = 1; // Make RA2 as input (PB1)
    TRISAbits.TRISA4 = 1; // Make RA4 as input (PB2)
    TRISBbits.TRISB4 = 1; // Make RB4 as input (PB3)
}

void IOcheck(){
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