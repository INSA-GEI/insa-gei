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
 
#ifndef __OS_CONFIG_H__
#define __OS_CONFIG_H__

#define STACK_SIZE			128	/* Exprimé en u32 */

#define MAX_PRIORITY_LEVEL	32

#define MAX_TASK_NBR		MAX_PRIORITY_LEVEL
#define MAX_ALARM_NBR		16
#define MAX_RESOURCE_NBR	16
#define MAX_EVENT_NBR		32

#if (MAX_PRIORITY_LEVEL<MAX_TASK_NBR)
	#error Le nombre de niveau de priorité est inferieur au nombre de taches (MAX_PRIORITY_LEVEL < MAX_TASK_NBR)
#endif 

#if (MAX_TASK_NBR <= 1)
	#error Nombre de tache invalide (MAX_TASK_NBR <= 1)
#endif

/* Switch pour le debug du noyau */
#define __DEBUG_FILL_STACK__ 	/* Si defini, les piles des taches sont entierement remplies avec 
                                   le motif DEBUG_FILL_STACK_PATTERN lors de la creation de la tache */
#define DEBUG_FILL_STACK_PATTERN	0xDEADC0DE

#endif /* __OS_CONFIG_H__ */
