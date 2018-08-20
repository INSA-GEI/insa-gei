/**
 * \file dumber.h
 * \brief Header for miscellaneous functions
 * \author Sebastien DI MERCURIO
 * 
 * dumber.h file is header for various functions
 */

#ifndef DUMBER_H_
#define DUMBER_H_

#define STATE_RUNNING	1
#define STATE_DISABLED	0
#define STATE_IDLE		2
#define STATE_POWER_OFF 3

#define BAT_NORMAL		2
#define BAT_LOW_BAT		1
#define BAT_EMPTY		0

void DUMBERInitPeriph(void);
void DUMBERInitMisc(void);
char DUMBERVbatLevel(void);
char DUMBERGetRobotState(void);
void DUMBERSetRobotState(char state);
void DUMBERCallbackActivity(void);
char DUMBERVerifyChecksum(unsigned char *ptr);
#endif /* DUMBER_H_ */