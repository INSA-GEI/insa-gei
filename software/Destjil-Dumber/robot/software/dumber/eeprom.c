/**
 * \file eeprom.c
 * \brief Driver for EEPROM driver
 * \author Sebastien DI MERCURIO
 * 
 * eeprom.c file contains the driver for EEPROM peripheral
 */

#include <avr/io.h>
#include "eeprom.h"

#define EEPROM_SIZE 512

/**
 * \brief Structure used for retrieving parameters from EEPROM.
 *
 * This structure is used during initialization for retrieving various parameters from EEPROM, like motor speeds 
 * @see ST_EEPROM
 */
struct ST_EEPROM params;

/**
 * \brief Initialization of EEPROM
 *
 * Prepare EEPROM peripheral for read or write.
 * Nothing to do on ATMEGAxx8 
 */ 
void E2PInit(void)
{
	/* rien de special */
}

/**
 * \brief Read a buffer of data in EEPROM
 *
 * Read data in EEPROM and store them in a buffer 
 * @param[in] offset Offset from the beginning of the EEPROM memory
 * @param[in] length Number of data to read
 * @param[out] buffer Pointer to a buffer large enough for the amount of data to read
 * @return Status of the operation 
 * @see SUCCESS
 * @see ADDR_ERROR 
 */
e2p_status E2PRead(unsigned int offset, unsigned int length, unsigned char *buffer)
{
register int i;

	if (offset+length>=EEPROM_SIZE) return ADDR_ERROR;

	for (i=0; i<length; i++)
	{
		EEAR = (unsigned int)(offset+i);
		EECR = EECR | (1<<EERE);
		buffer[i]=EEDR;
	}

	return SUCCESS;
}

/**
 * \brief Write a buffer of data in EEPROM
 *
 * Write data in EEPROM taken from a buffer 
 * @param[in] offset Offset from the beginning of the EEPROM memory
 * @param[in] length Number of data to read
 * @param[in] buffer Pointer to a buffer large enough for the amount of data to write
 * @return Status of the operation 
 * @see SUCCESS
 * @see ADDR_ERROR 
 */
e2p_status E2PWrite(unsigned int offset, unsigned int length, unsigned char *buffer)
{
register int i;

	if (offset+length>=EEPROM_SIZE) return ADDR_ERROR;

	for (i=0; i<length; i++)
	{
		while (EECR & (1<<EEPE));

		EEAR = (unsigned int)(offset+i);
		EEDR = (unsigned char)buffer[i];
		EECR = EECR | (1<<EEMPE);
		EECR = EECR | (1<<EEPE);
	}

	return SUCCESS;
}
