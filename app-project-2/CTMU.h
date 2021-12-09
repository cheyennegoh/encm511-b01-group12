/*
 * File: CTMU.h
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on December 8, 2021
 */

#ifndef CTMU_H
#define	CTMU_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

void setup(void);
// PROMISES: initializes ADC

float doCTMU();
// PROMISES: unsigned 16-bit integer value from 0 to 1023 from analog-to-digital
// conversion

#endif	/* CTMU_H */