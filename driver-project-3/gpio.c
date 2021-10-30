/*
 * File: gpio.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on October 17, 2021
 */

#include "xc.h"
#include "gpio.h"
#include "UART2.h"
#include "clocks.h"

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
    blink = 1; // Set blink flag
    three = 0; // Clear three flag
    two = 0; // Clear two flag
    nothing_pressed = 0; // Clear nothing_pressed flag
    if ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2) {
        // checks if two or more buttons are pressed
        if((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) == 0){
            three = 1; // Set three flag to 1
        }
        else{
            two = 1; // Set two flag to 1
            if(PORTAbits.RA2 == 1){
                pb = '2'; // Assign pb char to '2'
                pb2 = '3'; // Assign pb2 char to '3'
            }
            else if(PORTBbits.RB4 == 1){
                pb = '1'; // Assign pb char to '1'
                pb2 = '3'; // Assign pb2 char to '3'
            }
            else{
                pb = '1'; // Assign pb char to '1'
                pb2 = '2'; // Assign pb2 char to '2'
            }
        }
        blink = 0; // Set blink to 0
        delay = 1; // Set delay to 1
        turnon = 1; // turnon flag
    }
    else if (PORTAbits.RA2 == 0) {
        delay = 500; // Set delay  to 0.5 seconds
        pb = '1'; // Assign pb char to '1'
    }
    else if (PORTBbits.RB4 == 0) {
        delay = 2000; // Set delay to 2 seconds
        pb = '2'; // Assign pb char to '2'
    }
    else if (PORTAbits.RA4 == 0) {
        delay = 3000; // Set delay to 3 second
        pb = '3'; // Assign pb char to '3'
    }
    else {
        delay = 1; // Reset delay
        turnoff = 1; // Set turnoff flag
        LATBbits.LATB8 = 0; // turnoff LED
        nothing_pressed = 1; // Set nothing pressed to 1
        blink = 0; // Set blink to 0
    }
}