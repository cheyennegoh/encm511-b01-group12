/*
 * File: gpio.h
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on December 8, 2021
 */

#ifndef IOS_H
#define	IOS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

extern int button_flag;

void IOinit();
// PROMISES: initializes the IO ports

void IOcheck();
// PROMISES: implements the IO checks and buttons for ADC functionality

#endif	/* IOS_H */