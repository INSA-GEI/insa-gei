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
#include "scheduler.h"
#include "common.h"

#include "port.h"

/* Tableau de description des taches */
st_TaskInfo *Task_List[MAX_PRIORITY_LEVEL];
u32 TaskStackPointer[MAX_PRIORITY_LEVEL]; 	// Stock les SP en cours des taches non actives 

/* Declaration de la background task */
TaskType BackgroundTask_ID;
st_TaskInfo BackgroundTask_info;
const char BackgroundTask_nom[] = "Background Task";

/* 
 * void Prepare_Task_For_Activation(TaskType TaskID)
 *
 * Cette fonction s'occupe d'initialiser la pile et d'autre info de la tache pour une activation
 */
void Prepare_Task_For_Activation(TaskType TaskID)
{
		/* Elle passe a READY et rentre dans le pool des taches activables*/
		Task_List[TaskID]->state = 		READY;
		Task_List[TaskID]->locksource =	LOCK_SOURCE_NONE;
		Task_List[TaskID]->events = 0; /* Remet a zero les events */
		Task_List[TaskID]->waiting_events = 0; /* Remet a zero les events en attente*/
		Task_List[TaskID]->triggered_events = 0; /* Remet a zero les events declanchés */
		Task_List[TaskID]->elevated_priority = 0; /* Remise à zero de l'elevation de priorité */

		/* On lui recopie la stack initiale */
		FastCopy(Task_List[TaskID]->stack + STACK_SIZE - STARTUP_STACK_SIZE, 
		     	 (u32*)StartupStack, STARTUP_STACK_SIZE);

		/* On met a jour le point d'entrée */
		SET_ENTRY_POINT(TaskID,(u32)(Task_List[TaskID]->entrypoint));
		
		/* On enregistre le pointeur de pile dans TaskStackPointer */
		TaskStackPointer[TaskID] = (u32)(Task_List[TaskID]->stack + STACK_SIZE - STARTUP_STACK_SIZE);
}

/*
 * TaskType DeclareTask(st_TaskInfo *TaskInfo)
 * 
 * Permet d'enregistrer une tache
 */
TaskType DeclareTask(st_TaskInfo *TaskInfo)
{
TaskType TaskID;

	TaskID = TaskInfo->priority;

	if (TaskID >= MAX_TASK_NBR)
	{
		TaskID = E_OS_INVALID_TASK;
	} 
	else
	{
		if (Task_List[TaskID] != 0x0)
		{
			 TaskID = E_OS_LIMIT;
		}
		else
		{
			Task_List[TaskID] = TaskInfo;

#ifdef __DEBUG_FILL_STACK__	
			/* On remplit la pile avec un motif pour tester la consommation qui en est faite */
			FastFill(Task_List[TaskID]->stack, DEBUG_FILL_STACK_PATTERN, STACK_SIZE);
#endif /* __DEBUG_FILL_STACK__ */

			/* Preparation de la stack de la tache pour activation */
			Prepare_Task_For_Activation(TaskID);
		}
	}
	
	return TaskID;
}

/*
 * StatusType GetTaskID(TaskType *TaskID)
 * 
 * Retourne l'identifiant de la tache en cours (running)
 */
StatusType GetTaskID(TaskType *TaskID)
{
	*TaskID = CurrentTask;
	 
	return E_OK;
}

/*
 * StatusType	GetTaskState(TaskType TaskID, TaskStateType *State)
 * 
 * Retourne l'etat de la tache indiqué (running) 
 * ou retourne E_OS_INVALID_TASK si la tache n'existe pas (pas enregistrée)  
 */
StatusType	GetTaskState(TaskType TaskID, TaskStateType *State)
{
	if ((TaskID >= MAX_TASK_NBR) || (Task_List[TaskID] == 0x0)) 
	{
		return E_OS_INVALID_TASK;
	} 
	
	*State = Task_List[TaskID]->state;

	return E_OK;
}

/*
 * StatusType	ActivateTask(TaskType TaskID)
 * 
 * Active une tache (deja enregistrée), c-a-d passe son etat à READY et prepare la tache a étre executée  
 */
StatusType	ActivateTask_Int(u32 Func_ID, TaskType TaskID)
{
u8 Status;

	if (TaskID >= MAX_TASK_NBR)
	{
		return E_OS_INVALID_TASK;
	} 

	Status = E_OK;

	if ((Task_List[TaskID] != 0x0) && (Task_List[TaskID]->state == SUSPENDED))
	{
		/* Preparation de la stack de la tache pour activation */
		Prepare_Task_For_Activation(TaskID);

		/* Determination de la nouvelle tache active */
		//Reschedule();	
	}
	else
	{
		Status = E_OS_LIMIT; // La tache a deja été activée
	}
	return Status;
}

/*
 * StatusType	TerminateTask(void)
 * 
 * Termine la tache et la retire de la liste des taches activables 
 */
StatusType	TerminateTask_Int(u32 Func_ID)
{
u8 Status;

	Status = E_OK;

	if (Task_List[CurrentTask]->state != SUSPENDED)
	{
		/* Elle passe a SUSPENDED (retirée de la liste des taches activables)*/
		Task_List[CurrentTask]->state = SUSPENDED;

		/* Determination de la nouvelle tache active */
		//Reschedule();	
	}
	else
	{
		Status = E_OS_LIMIT; // La tache a deja été suspendue
	}
	return Status;
}

/*
 * StatusType	ChaineTask(TaskType TaskID)
 * 
 * Termine la tache en cours et active la tache indiquée, permettant son execution au plus tôt
 */
StatusType	ChaineTask_Int(u32 Func_ID, TaskType TaskID)
{
	return E_OS_LIMIT;
}

/*
 * StatusType	Schedule(void)
 * 
 * Active la tache de priorité superieur en relachant sa ressource, et passe la tache actuelle à ready
 *
 * TODO: je comprends pas trop ce que doit faire cette fonction -> A finir
 */
StatusType	Schedule_Int(u32 Func_ID)
{
	//Reschedule();

	return E_OK;
}

/*
 * void Task_Init(void)
 * 
 * Initialise la partie du noyau responsable de la gestion des taches
 */
void Task_Init(void)
{
u8 i;

	for(i=0; i<	MAX_TASK_NBR; i++)
	{
		Task_List[i] = 0x0;
	}

	/* Ajout de la background task */
	BackgroundTask_info.taskname = (char*)BackgroundTask_nom;
	BackgroundTask_info.entrypoint = BackgroundTask;
	BackgroundTask_info.priority = 0;  	// Plus faible priorité

	BackgroundTask_ID = DeclareTask(&BackgroundTask_info);
}

/*
 * void BackgroundTask(void)
 * 
 * Tache de fond du system
 *
 * La background task etant declarée comme "weak", 
 * cette definition	permet d'avoir une background task meme si l'utilisateur a oublié d'en fournir une
 */
void BackgroundTask(void)
{
	while (1);
}
