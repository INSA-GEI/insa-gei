/**
 * \file timer.h
 * \brief header for timer and motor driver
 * \author Sebastien DI MERCURIO
 * 
 * timer.h file is header for timer and motor driver
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#define WITH_WDT	1
#define WITHOUT_WDT	0

void TIMERInit(void);

void TIMERStartSystem(char wdt);
char TIMERAckWDT(void);
void TIMERResetWDT(void);

#endif /* __TIMER_H__ */
