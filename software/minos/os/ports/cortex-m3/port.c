/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of MinOS.
 *
 * MinOS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * MinOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with MinOS; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "stm32f10x_type.h"
#include "os_config.h"
#include "port.h"

#define __OS_OPTIMISED_FUNCTIONS__

void Task_End_Fallback(void);

#define INITIAL_REG		0xBEEFCAB0
#define INITIAL_PSR		0x01000000
#define INITIAL_LR		0xFFFFFFFD

const u32 StartupStack[] = 
{
	INITIAL_REG+4, INITIAL_REG+5, INITIAL_REG+6, INITIAL_REG+7,		// R4, R5, R6, R7
	INITIAL_REG+8, INITIAL_REG+9, INITIAL_REG+10, INITIAL_REG+11,	// R8, R9, R10, R11
	INITIAL_REG+0, INITIAL_REG+1, INITIAL_REG+2, INITIAL_REG+3,		// R0 (place vide), R1, R2, R3
	INITIAL_REG+12,													// R12
	(u32)Task_End_Fallback,				// Link @ vers Task_End_Fallback
	0,									// Initial PC: à completer avec le point d'entrée de la tache
	INITIAL_PSR							// Initial PSR, seul le bit T (Thumb) est positionné
};

void FastCopy(register u32 *dest, register u32 *src, register u32 len)
{
#ifdef __OS_OPTIMISED_FUNCTIONS__
	while (len!=0)
	{
		*dest= *src;
		dest++;
		src++;
		len--; 
	}
#else
	while (len!=0)
	{
		*dest= *src;
		dest++;
		src++;
		len--; 
	}
#endif
}

void FastFill(register u32 *dest, register u32 pattern, register u32 len)
{
#ifdef __OS_OPTIMISED_FUNCTIONS__
	while (len!=0)
	{
		*dest= pattern;
		dest++;

		len--;
	}
#else
	while (len!=0)
	{
		*dest= pattern;
		dest++;

		len--;
	}
#endif
}

u32 SearchFreeLsb(register u32 field)
{
u32 result;

#ifdef __OS_OPTIMISED_FUNCTIONS__
	asm ("CLZ %0, %1"
		 :"=r"(result)	/* output */
	 	 :"r"(field));		

	result = 32 - result;
#else
	
	result = 0;

	while (result != 32)
	{
		if ((field & (1<<result)) == 0) goto End_SearchFreeLsb;
		else result ++;
	}

End_SearchFreeLsb:

#endif 

	return result;	
}

