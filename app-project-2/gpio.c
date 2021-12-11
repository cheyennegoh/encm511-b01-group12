/*
 * File: gpio.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on December 10, 2021
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
    if ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2) {}
    else if (PORTAbits.RA2 == 0) {
        button_flag = 1;
    }
    else if (PORTAbits.RA4 == 0) {
        button_flag = 2;
    }
    else if (PORTBbits.RB4 == 0) {
        button_flag = 3;
    }
    else {
        button_flag = 0;
    }
}