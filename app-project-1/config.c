/*
 * File: config.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on November 28, 2021
 */

#include "xc.h"
#include "config.h"

void ConfigCN() {
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
}