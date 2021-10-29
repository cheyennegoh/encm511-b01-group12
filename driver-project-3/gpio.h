/*
 * File: gpio.h
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on October 17, 2021
 */

#ifndef IOS_H
#define	IOS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

extern uint16_t turnon, turnoff, delay;
extern char pb;
extern int blink;

void IOinit();
// PROMISES: initializes the IO ports and placed

void IOcheck();
// PROMISES: implements the IO checks and LED blinking functions

#endif	/* IOS_H */