/*
 * File: ADC.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on December 10, 2021
 */

#include "xc.h"
#include "ADC.h"

void ADCinit(int i) {
    if(i == 5){
        AD1CON1bits.ADON = 0; // Turn off ADC, turn on before sampling in do_ADC()
        AD1CON1bits.ADSIDL = 0; // Continue ADC operation in idle mode
        AD1CON1bits.FORM = 0b00; // 0b00: int
        AD1CON1bits.SSRC = 0b111; // Start ADC conversion time set by SAMC has passed
        AD1CON1bits.ASAM = 0; // Next sampling begins when SAMP bit is set
        AD1CON1bits.SAMP = 0; // Set this bit in ADC routine to start sampling
        AD1CON1bits.DONE = 1; // Sets DONE to 0

        AD1CON2bits.VCFG = 0b000; // Vref+=AVDD, Vref=+AVSS
        AD1CON2bits.CSCNA = 0; // Do not scan inputs; use channel selected by CH0SA bits
        AD1CON2bits.SMPI = 0; // Any ADC interrupts occur at completion of each sample/convert seq.
        AD1CON2bits.BUFM = 0; // Buffer configured as 1 16 word buffer
        AD1CON2bits.ALTS = 0; // Always use MUXA input MUX settings

        AD1CON3bits.ADRC = 0; // 0: Use System clk
        AD1CON3bits.SAMC = 0b11111; // Sample time = 31*TAD = 31*1/123e3 = 0.248ms
        AD1CON3bits.ADCS = 0b11; // ADCS conversion clock select bits to 3

        AD1CHSbits.CH0NB = 0; // Ch0 MUXB -ve Input is VR-
        AD1CHSbits.CH0SB = 0b1011; // Positive input is AN5/RA3/pin8 for MUXB
        AD1CHSbits.CH0NA = 0; // Ch0 -ve Input is VR-
        AD1CHSbits.CH0SA = 0b1011; // Positive input is AN5/RA/pin8 for MUXA

        // IO port selection
        TRISBbits.TRISB13 = 1; // Set pin8/RA3/AN5 as input
        TRISAbits.TRISA3 = 0; // Clear RB1 as input
        TRISBbits.TRISB1 = 0; // Clear RB1 as input
        
        AD1PCFG = 0xFFFF; // Set all bits as digital
        AD1PCFGbits.PCFG11 = 0; // Sets bit to analog
        
        AD1CSSL = 0; // Input scan disabled, 0x0000 is default state
        IPC3bits.AD1IP = 7; // INT bit setup
        AD1CON1bits.ADON = 1; // Turn on ADC module
    }
    else{
        AD1CON1bits.ADON = 0; // Turn off ADC, turn on before sampling in do_ADC()
        AD1CON1bits.ADSIDL = 0; // Continue ADC operation in idle mode
        AD1CON1bits.FORM = 0b00; // 0b00: int
        AD1CON1bits.SSRC = 0b111; // Start ADC conversion time set by SAMC has passed
        AD1CON1bits.ASAM = 0; // Next sampling begins when SAMP bit is set
        AD1CON1bits.SAMP = 0; // Set this bit in ADC routine to start sampling
        AD1CON1bits.DONE = 1; // Sets DONE to 1

        AD1CON2bits.VCFG = 0b000; // Vref+=AVDD, Vref=+AVSS
        AD1CON2bits.CSCNA = 0; // Do not scan inputs; use channel selected by CH0SA bits
        AD1CON2bits.SMPI = 0; // Any ADC interrupts occur at completion of each sample/convert seq.
        AD1CON2bits.BUFM = 0; // Buffer configured as 1 16 word buffer
        AD1CON2bits.ALTS = 0; // Always use MUXA input MUX settings

        AD1CON3bits.ADRC = 0; // 0: Use System clk
        AD1CON3bits.SAMC = 0b11111; // Sample time = 31*TAD = 31*1/123e3 = 0.248ms
        AD1CON3bits.ADCS = 0b11; // ADCS conversion clock select bits to 3

        AD1CHSbits.CH0NB = 0; // Ch0 MUXB -ve Input is VR-
        AD1CHSbits.CH0SB = 0b0101; // Positive input is AN5/RA3/pin8 for MUXB
        AD1CHSbits.CH0NA = 0; // Ch0 -ve Input is VR-
        AD1CHSbits.CH0SA = 0b0101; // Positive input is AN5/RA/pin8 for MUXA

        // IO port selection
        TRISAbits.TRISA3 = 1; // Set pin8/RA3/AN5 as input
        TRISBbits.TRISB1 = 0; // Clear RB1 as input
        TRISBbits.TRISB13 = 0; // Clear RA3 as input
        
        AD1PCFG = 0xFFFF; // Set all bits as digital
        AD1PCFGbits.PCFG5 = 0; // Sets bit to analog
        
        AD1CSSL = 0; // Input scan disabled, 0x0000 is default state
        IPC3bits.AD1IP = 7; // INT bit setup
        AD1CON1bits.ADON = 1; // Turn on ADC module
    }
    CTMUCON = 0x0000;
    CTMUICON = 0x0000;
}

uint16_t do_ADC(void) {
    
    uint16_t ADCvalue; // Initializes ADCvalue
    
    /*---------------------- ADC SAMPLING AND CONVERSION ---------------------*/
    AD1CON1bits.SAMP = 1; // Start sampling, conversion starts automatically after SSRC and SAMC settings
    while (AD1CON1bits.DONE == 0) {}
    ADCvalue = ADC1BUF0; // Reads ADCvalue from ADC1BUF0
    AD1CON1bits.SAMP = 0; // Stop sampling
    return (ADCvalue); // Returns ADCvalue
}
