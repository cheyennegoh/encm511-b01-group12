/*
 * File: ADC.h
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on November 28, 2021
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

void ADCinit(void);
// PROMISES: initializes ADC

uint16_t do_ADC(void);
// PROMISES: unsigned 16-bit integer value from 0 to 1023 from analog-to-digital
// conversion

#endif	/* ADC_H */