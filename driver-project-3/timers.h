/*
 * File: timers.h
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on October 17, 2021
 */

#ifndef TIMERS_H
#define	TIMERS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

void Delay_ms(uint16_t time_ms);
// REQUIRES: user specified time delay in milliseconds
// PROMISES: implements the delay functions used to time the LED blinks

#endif	/* TIMERS_H */