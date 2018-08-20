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

#ifndef __PORT_H__
#define __PORT_H__

#include "os_config.h"
#include "stm_system.h"

#define	ActivateTask_Fct_Id			1
#define TerminateTask_Fct_Id		2
#define ChaineTask_Fct_Id			3
#define Schedule_Fct_Id				4
#define GetResource_Fct_Id			5
#define	ReleaseResource_Fct_Id		6
#define SetRelAlarm_Fct_Id			7
#define SetAbsAlarm_Fct_Id			8
#define CancelAlarm_Fct_Id			9
#define SetEvent_Fct_Id				10
#define ClearEvent_Fct_Id			11
#define WaitEvent_Fct_Id			12

extern const u32 StartupStack[];

#define STARTUP_STACK_SIZE			16 // Exprimé en mots de 32 bits */
#define SET_ENTRY_POINT(tid,ep)		Task_List[tid]->stack[STACK_SIZE-2] = ep	

void FastCopy (register u32 *dest, register u32 *src, register u32 len);
void FastFill(register u32 *dest, register u32 pattern, register u32 len);
u32 SearchFreeLsb(register u32 field);

/* Support des interruptions */
#define EnableAllInterrupts_port(void) 	SYS_ENABLE_GLOBAL_INTERRUPTS()
#define DisableAllInterrupts_port(void)	SYS_DISABLE_GLOBAL_INTERRUPTS()
#define EnableSysInterrupts_port(void) 	SYS_ENABLE_FAULT_INTERRUPTS()
#define DisableSysInterrupts_port(void)	SYS_DISABLE_FAULT_INTERRUPTS()

/* Port_Hook */
void PortHook(u32 Initial_MSP);

#endif /* __PORT_H__ */

