/*
 * File: UART2.h
 * Author: Cheyenne Goh, Faisal Hossain, Andrine Buiza
 *
 * Submitted on December 8, 2021
 */

#ifndef UART2_H
#define	UART2_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

void InitUART2(void);
void XmitUART2(char, unsigned int);

void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void);
void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void); 

void Disp2Hex(unsigned int);
void Disp2Hex32(unsigned long int);
void Disp2String(char*);
void Disp2Dec(uint32_t);
void Disp2Float(float);


#endif	/* UART2_H */

