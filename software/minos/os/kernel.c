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
#include "stm32f10x_lib.h"
#include "os_config.h"
#include "kernel.h"

#include "task.h"
#include "alarm.h"
#include "interrupts.h"
#include "resource.h"
#include "events.h"
#include "common.h"

#include "scheduler.h"

#ifdef __WITH_EVENTS__
#include "events.h"
#endif /* __WITH_EVENTS__ */

void OS_Start_Switch(void);

AppModeType	GetActiveApplicationMode(void)
{
	return OSDEFAULTAPPMODE;
}

void InitOS(void)
{
	/* Initialisation des interruptions */
	Interrupt_Init();
	/* A la fin de l'init des IT, les interruptions doivent etre desactivées */

	/* Initialisation des ressources communes */
	Common_Init();

	/* Initialisation des taches */
	Task_Init();

	/* Initialisation des resources */
	Resource_Init();

	/* Initialisation des alarms */
	Alarm_Init();

	/* Initialisation des events */
	Events_Init();
}

void StartOS(AppModeType Mode)
{

	/* Mode n'est pas utilisé pour l'instant */
 
	OS_Start_Switch(); 	/* demarrage du scheduler et lancement de l'OS avec la tache la + prioritaire */
}

void ShutdownOS(StatusType Error)
{
	/* TODO: Trouver un truc a faire ici ! */
}

