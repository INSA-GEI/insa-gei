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
#include "scheduler.h"
#include "task.h"
#include "common.h"

/*
 * void Reschedule(void)
 * 
 * Fonction de recalcul de la tache a executer
 *
 * TODO: possibilite de choisir plusieurs type de scheduler, à la compilation
 */
TaskType Reschedule(void)
{
TaskType TaskID;
int MaxPrio;
TaskType TaskMaxPrio;

	/* On parcourt la table des taches à la recherche d'une tache RUNNING ou READY, 
	 * non bloqués par une source (locksource)
	 * A l'issue de cette fonction, la tache a executer sera indiquée dans CurrentTask 
	 */

 	/* 
	 * La tache 0, la moins prioritaire, correspond à la "background task" d'autres OS
     * En ce sens, elle ne peut pas etre bloquée sur semaphore et doit tjs exister 
	 */

	if (Task_List[CurrentTask]->state == RUNNING) Task_List[CurrentTask]->state = READY;

	MaxPrio =0;
	TaskMaxPrio=0;

	for (TaskID = 1; TaskID < MAX_TASK_NBR; TaskID ++)
	{
		if (Task_List[TaskID] != 0)
		{
			if (Task_List[TaskID]->state == READY)
			{
				if (Task_List[TaskID]->elevated_priority!=0)
				{
					if (Task_List[TaskID]->elevated_priority>MaxPrio)
					{
						MaxPrio = Task_List[TaskID]->elevated_priority;
						TaskMaxPrio = TaskID;	
					}
				}
				else
				{
					if (Task_List[TaskID]->priority>MaxPrio)
					{
						MaxPrio=Task_List[TaskID]->priority;
						TaskMaxPrio = TaskID;
					}
				}
			}	
		}
	}
	
	CurrentTask = TaskMaxPrio;
	Task_List[CurrentTask]->state = RUNNING;

	return CurrentTask;
}

