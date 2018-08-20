/**
 * \file eeprom.h
 * \brief header for EEPROM driver
 * \author Sebastien DI MERCURIO
 * 
 * eeprom.h file is header for EEPROM driver
 */

#ifndef _EEPROM_H_
#define _EEPROM_H_

/**
 * \brief Status type for EEPROM functions 
 */ 
typedef unsigned char e2p_status;

enum
{
	SUCCESS=0, /**< Return when EEPROM functions succeed */
	ADDR_ERROR /**< Return when EEPROM functions failed due to invalid address */
};

struct ST_EEPROM
{
	unsigned char eepromVer;
	unsigned char motorLeftNormal;
	unsigned char motorLeftTurbo;
	unsigned char motorRightNormal;
	unsigned char motorRightTurbo;
};

extern struct ST_EEPROM params;

/*
 * E2PInit
 * EEPROM Initialization
 */
void E2PInit(void);

/*
 * E2PRead
 * Read data in EEPROM
 */
e2p_status E2PRead(unsigned int offset, unsigned int length, unsigned char *buffer);

/*
 * E2PWrite
 * Write data in EEPROM 
 */
e2p_status E2PWrite(unsigned int offset, unsigned int length, unsigned char *buffer);

#endif /* _EEPROM_H_ */

 
