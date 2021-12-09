/*
 * File: timers.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on December 8, 2021
 */

#include "xc.h"
#include "timers.h"
#include "clocks.h"

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

