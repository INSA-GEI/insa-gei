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
#include "kernel.h"
#include "task.h"
#include "alarm.h"
#include "events.h"

#include "scheduler.h"

struct st_Alarm
{
	st_AlarmInfo *alarminfo;
	u32 counter;
	u32 cycle;
} Alarm_List[MAX_ALARM_NBR];

/* Alarm services declaration */

/*
 * AlarmType	DeclareAlarm(st_AlarmInfo *AlarmInfo)
 * 
 * Rajoute une alarme à la liste
 */
AlarmType	DeclareAlarm(st_AlarmInfo *AlarmInfo)
{
AlarmType index;

	index =0;

	while (index< MAX_ALARM_NBR)
	{
		if (Alarm_List[index].alarminfo == 0)
		{
			Alarm_List[index].alarminfo = AlarmInfo;

			if (Alarm_List[index].alarminfo->type & ALARM_AUTOSTART) 
			{
				Alarm_List[index].alarminfo->state = ALARM_ON;
				Alarm_List[index].counter = Alarm_List[index].alarminfo->increment;
			}
			else Alarm_List[index].alarminfo->state = ALARM_OFF;
		}
		else index++;
	}

	return index;
}

/*
 * StatusType	GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
 * 
 * Recupere les caracteristiques de base de l'alarme indiquée
 */
StatusType	GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
{
	if ((AlarmID>= MAX_ALARM_NBR) || (Alarm_List[AlarmID].alarminfo == 0))
	{
		return E_OS_ID;
	}

	Info = &(Alarm_List[AlarmID].alarminfo->basetype);

	return E_OK;
}

/*
 * StatusType	GetAlarm(AlarmType AlarmID, TickRefType Tick)
 * 
 * Recupere la temps restant (en ticks) avant que le timer expire
 */
StatusType	GetAlarm(AlarmType AlarmID, TickRefType Tick)
{
	if ((AlarmID>= MAX_ALARM_NBR) || (Alarm_List[AlarmID].alarminfo == 0))
	{
		return E_OS_ID;
	}

	if (Alarm_List[AlarmID].alarminfo->state == ALARM_OFF) 
	{
		return E_OS_NOFUNC;
	}

	Tick = &(Alarm_List[AlarmID].counter);

	return E_OK;
}

/*
 * StatusType	SetRelAlarm(AlarmType AlarmID, TickType Increment, TickType Cycle)
 * 
 * Regle et demarre une alarme pour une echeance à une temps T relatif par rapport a l'instant d'activation
 */
StatusType	SetRelAlarm_Int(u32 Func_ID, AlarmType AlarmID, TickType Increment, TickType Cycle)
{
	if ((AlarmID>= MAX_ALARM_NBR) || (Alarm_List[AlarmID].alarminfo == 0)) return E_OS_ID;
	if (Alarm_List[AlarmID].alarminfo->state == ALARM_ON) return E_OS_STATE;
	if (Increment > Alarm_List[AlarmID].alarminfo->basetype.maxallowedvalue) return E_OS_VALUE;
	if ((Cycle < Alarm_List[AlarmID].alarminfo->basetype.mincycle) && (Cycle!=0)) return E_OS_VALUE;

	if (Alarm_List[AlarmID].alarminfo->type & ALARM_CYCLIC)
	{
		if (Cycle!=0)
		{
			Alarm_List[AlarmID].alarminfo->cycle = Cycle;	
		}
		else return E_OS_VALUE;
	}
	else Alarm_List[AlarmID].alarminfo->cycle =0; 

	Alarm_List[AlarmID].alarminfo->increment = Increment;

	return E_OK;
}

/*
 * StatusType	SetAbsAlarm_Int(AlarmType AlarmID, TickType Start, TickType Cycle)
 * 
 * Regle et demarre une alarme pour une echeance à une temps T absolu par rapport a l'instant d'activation
 */
StatusType	SetAbsAlarm_Int(u32 Func_ID, AlarmType AlarmID, TickType Start, TickType Cycle)
{
/* TODO: J'ai du mal a comprendre l'interet de ce service. En attendant mieux, je le redirige sur SetRelAlarm */

	return 	SetRelAlarm_Int(Func_ID, AlarmID, Start, Cycle);
}

/*
 * StatusType	CancelAlarm(AlarmType AlarmID)
 * 
 * Arrete une alarme
 */
StatusType	CancelAlarm_Int(u32 Func_ID, AlarmType AlarmID)
{
	if ((AlarmID>= MAX_ALARM_NBR) || (Alarm_List[AlarmID].alarminfo == 0)) return E_OS_ID;
	if (Alarm_List[AlarmID].alarminfo->state == ALARM_OFF) return E_OS_NOFUNC;

	Alarm_List[AlarmID].alarminfo->state = ALARM_OFF;
	return E_OK;
}

/*
 * void Alarm_Init(void)
 * 
 * Initialise le module Alarme du noyau
 */
void Alarm_Init(void)
{
u8 i;

	for(i=0; i<	MAX_ALARM_NBR; i++)
	{
		Alarm_List[i].alarminfo = 0x0;
	}
}

/*
 * void AlarmTimerTick(register int func_index)
 * 
 * Routine d'IT temps reel systeme
 */
void AlarmTimerTick(register int func_index)
{
u32 index;
TaskType TaskID;

	for (index=0; index < MAX_ALARM_NBR; index ++)
	{
		if (Alarm_List[index].alarminfo != 0)
		{
			if (Alarm_List[index].alarminfo->state != ALARM_OFF)
			{
				Alarm_List[index].counter--;
				
				if (Alarm_List[index].counter == 0)
				{
					if (Alarm_List[index].alarminfo->type & ALARM_CYCLIC)
					{
						if (Alarm_List[index].alarminfo->cycle !=0)
						{
							Alarm_List[index].alarminfo->cycle--;
							Alarm_List[index].counter = Alarm_List[index].alarminfo->increment; 	
						}
						else
						{
							Alarm_List[index].alarminfo->state = ALARM_OFF;	
						}
					}
					else
					{
						Alarm_List[index].alarminfo->state = ALARM_OFF;
					}

					switch (Alarm_List[index].alarminfo->type & ALARM_TRIGGER_MASK)
					{
					case ALARM_TRIGGER_TASK:
						TaskID = Alarm_List[index].alarminfo->TaskID;
						
						if (Task_List[TaskID]->state==SUSPENDED)
						{
							Prepare_Task_For_Activation(TaskID);
						}
						else
						{
							if (Task_List[TaskID]->missingactivations!=0xFFFFFFFF) Task_List[TaskID]->missingactivations++;
						} 
					 	break;
					case ALARM_TRIGGER_CALLBACK:
						if (Alarm_List[index].alarminfo->callbackfunc !=0) (Alarm_List[index].alarminfo->callbackfunc)();
						break;
					case ALARM_TRIGGER_EVENT:
						TaskID = Alarm_List[index].alarminfo->TaskID;

						if (Task_List[TaskID]->state!=SUSPENDED)
						{
							SetEvent_Int(0, TaskID, Alarm_List[index].alarminfo->EventID);
						}
						break;
					}
				} 	
			}
		}
	}
}

