/*
 * File: gpio.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on November 28, 2021
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
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 0;
    TMR2 = 0;
    is_paused = 0;
    if ((PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4) < 2) {}
    else if (PORTAbits.RA2 == 0) {
        inc_min_flag = 1;
    }
    else if (PORTAbits.RA4 == 0) {
        inc_sec_flag = 1;
    }
    else if (PORTBbits.RB4 == 0) {
        pb_3 = 0;
        LATBbits.LATB8 = 0;
        if(countdown){
            countdown = !countdown;
            seconds = seconds + 1;
//            is_paused = 1;
        }
        else{
            countdown = !countdown;
        }
        is_paused = 1;
    }
    else {
        if(inc_min_flag){
            inc_min_flag = 0;
        }
        else if(inc_sec_flag){
            inc_sec_flag = 0;
        }
    }
}