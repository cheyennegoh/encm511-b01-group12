/*
 * File: CTMU.c
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on December 10, 2021
 */

#include "xc.h";
#include "CTMU.h"
#include "p24Fxxxx.h"
#include "timers.h"
#define DELAY for(i=0;i<500;i++)
#define ADSCALE 1023 //for unsigned conversion 10 sig bits
#define ADREF 3.25 //Vdd connected to A/D r+

int doCTMU() {
    
    int i, j; //index for loop
    unsigned int Vread = 0; // Initializes Vread
    double VTot = 0; // Initializes VTot
    float Vavg, CTMUCap; // Initializes Vavg and CTMUCap
    
    setupCTMU();
    CTMUCONbits.CTMUEN = 1; // Enable the CTMU
    for (j = 0; j < 10; j++) {
        CTMUCONbits.IDISSEN = 1; // drain charge on the circuit
        DELAY; // wait 125us
        CTMUCONbits.IDISSEN = 0; // end drain of circuit
        AD1CON1bits.SAMP = 1; // Manual sampling start
        CTMUCONbits.EDG1STAT = 1; // Begin charging the circuit
        // using CTMU current source
        DELAY; // wait for 125us
        CTMUCONbits.EDG1STAT = 0; // Stop charging circuit
        IFS0bits.AD1IF = 0; // make sure A/D Int not set
        AD1CON1bits.SAMP = 0; // and begin A/D conv.
        while(!AD1CON1bits.DONE); // Wait for A/D convert complete
        Vread = ADC1BUF0; // Get the value from the A/D
        VTot += Vread; // Add the reading to the total
        IFS0bits.AD1IF = 0; // Clear A/D Interrupt Flag
    }
    Vavg = (VTot/10); // Average of 10 readings
    CTMUCap = 1023/Vavg; // Calculates capacitance
    NewClk(8); // Sets clock to 8 MHz
    CTMUCONbits.CTMUEN = 0; // Sets CTMUEN to 0
    return CTMUCap; // Returns capacitance value
}

void setupCTMU(void) {
    
    AD1CON1bits.ADON = 0; // Turn off ADC, turn on before sampling in do_ADC()
    AD1CON1bits.ADSIDL = 0; // Continue ADC operation in idle mode
    AD1CON1bits.FORM = 0b00; // 0b00: int
    AD1CON1bits.SSRC = 0b0000; // Start ADC conversion time set by SAMC has passed
    AD1CON1bits.ASAM = 0; // Next sampling begins when SAMP bit is set
    AD1CON1bits.SAMP = 0; // Set this bit in ADC routine to start sampling

    AD1CON2bits.VCFG = 0b000; // Vref+=AVDD, Vref=+AVSS
    AD1CON2bits.CSCNA = 0; // Do not scan inputs; use channel selected by CH0SA bits
    AD1CON2bits.SMPI = 0; // Any ADC interrupts occur at completion of each sample/convert seq.
    AD1CON2bits.BUFM = 0; // Buffer configured as 1 16 word buffer
    AD1CON2bits.ALTS = 0; // Always use MUXA input MUX settings

    AD1CON3bits.ADRC = 0; // 0: Use System clk
    AD1CON3bits.SAMC = 0; // Sample time = 31*TAD = 31*1/123e3 = 0.248ms
    AD1CON3bits.ADCS = 0; // ADCS conversion clock select bits to 3
    
    //CTMUCON - CTMU Control register
    CTMUCON = 0x0000; //make sure CTMU is disabled
    CTMUCONbits.CTMUEN = 0; // Sets CTMUEN to 0
    CTMUCONbits.TGEN = 1; // Sets TGEN to 1
    CTMUCONbits.EDG1POL = 1; // Sets EDGPOL to 1
    CTMUCONbits.EDG2POL = 1; // Sets EDG2POL to 1
    
    // CTMU continues to run when emulator is stopped,CTMU continues
    // to run in Idle mode, Time Generation mode enabled, Edges are
    // blocked. No edge sequence order, Analog current source not
    // grounded, trigger output disabled, Edge2 polarity = positive level,
    // Edge2 source = source 0, Edge1 polarity = positive level,
    // Edge1 source = source 0, Set Edge status bits to zero
    
    //CTMUICON - CTMU Current Control Register
    CTMUICON = 0x0000; // Resets CTMUICON
    CTMUICONbits.IRNG0 = 1; // 55uA, Nominal - No Adjustment
    CTMUICONbits.IRNG1 = 1; // 55uA, Nominal - No Adjustment
    
/**************************************************************************/
/* Set up AD Converter ****************************************************/
/**************************************************************************/
    
    // IO port selection
    TRISBbits.TRISB1 = 1; // Set RB1 as input
    TRISBbits.TRISB13 = 0; // Clear RB13 as input
    TRISAbits.TRISA3 = 0; // Clear RA3 as input
    AD1PCFG = 0xFFFF; // Set all bits as digital
    AD1PCFGbits.PCFG3 = 0; // Set only AN3 as as Analog input for ADC
    
    // MUX settings
    AD1CHSbits.CH0NB = 0; // Ch0 MUXB -ve Input is VR-
    AD1CHSbits.CH0SB = 0b0011; // AN3
    AD1CHSbits.CH0NA = 0; // Ch0 MUXA -ve Input is VR-
    AD1CHSbits.CH0SA = 0b0011; //AN3
    
    AD1CON1bits.ADON = 1; // Turn off ADC, turn on before sampling
}