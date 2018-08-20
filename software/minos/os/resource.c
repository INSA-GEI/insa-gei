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
#include "resource.h"
#include "common.h"
#include "scheduler.h"
#include "task.h"

struct st_Resource
{
	st_ResourceInfo *resourceinfo;
} Resource_List[MAX_RESOURCE_NBR];

/*
 * ResourceType	DeclareResource(st_ResourceInfo *ResourceInfo)
 * 
 * Cree une nouvelle resource ou renvoi INVALID_RESOURCE si plus aucune resource disponible
 */
ResourceType	DeclareResource(st_ResourceInfo *ResourceInfo, ResourceTasksListType ResourceTasksList)
{
ResourceType index;
int i;
u8 max_priority;

	for (index =0; index < MAX_RESOURCE_NBR; index ++)
	{
		if (Resource_List[index].resourceinfo == 0x0)
		{	
			Resource_List[index].resourceinfo = ResourceInfo;
			Resource_List[index].resourceinfo->counter = 0;
			
			goto End_DeclareResource;	
		}

		index ++;
	}

End_DeclareResource:
	
	if (index>=	MAX_RESOURCE_NBR) return E_OS_LIMIT;
	else 
	{
		/* Recherche le niveau de priorité le + elevé */
		max_priority =0;
		i=0;

		while (ResourceTasksList[i]!=0)
		{
			if (max_priority < Task_List[ResourceTasksList[i]]->priority)
			{
				max_priority = Task_List[ResourceTasksList[i]]->priority;
			}

			i++;
		}

		if (i==0) return E_OS_VALUE;

		Resource_List[index].resourceinfo->priority = max_priority;
	}

	return index;
}

/*
 * StatusType		ClearResourceTasksList(ResourceTasksListType *ResourceTasksList)
 * 
 * Remet à zero une ResourceTasksList
 */
StatusType		ClearResourceTasksList(ResourceTasksListType ResourceTasksList)
{
int i;

	for (i=0; i<MAX_PRIORITY_LEVEL; i++)
	{
		 ResourceTasksList[i] = 0;
	}

	return E_OK;
}

/*
 * StatusType		AddTaskInResourceTasksList (ResourceTasksListType *ResourceTasksList, TaskType Taskid);
 * 
 * Ajoute une tache dans une ResourceTasksList
 */
StatusType		AddTaskInResourceTasksList (ResourceTasksListType ResourceTasksList, TaskType TaskID)
{
int i;

	i=0;

	while (i<MAX_PRIORITY_LEVEL)
	{
		 if (ResourceTasksList[i] == 0)
		 {
		 	ResourceTasksList[i] = TaskID;
			goto End_AddTaskInResourceTasksList;
		 }

		 i++;
	}

End_AddTaskInResourceTasksList:

	if (i>=MAX_PRIORITY_LEVEL) return E_OS_LIMIT;
	else return E_OK;
}

/*
 * StatusType		GetResource(ResourceType ResID)
 * 
 * Prend une resource et bloque la tache en cours si la resource est deja prise
 */

/*
 * TODO: Envisager de passer la fonction en tant que fonction standard
 */
StatusType		GetResource_Int(u32 Func_ID, ResourceType ResID)
{
	if ((ResID>=MAX_RESOURCE_NBR) || (Resource_List[ResID].resourceinfo == 0x0)) return E_OS_ID;

	/* Elevation de la priorité de la tache */
	Task_List[CurrentTask]->elevated_priority = Resource_List[ResID].resourceinfo->priority; 
	Task_List[CurrentTask]->locksource = OCCUPIE_RESOURCE;
	Task_List[CurrentTask]->locksource = ResID;

	return E_OK;
}

/*
 * StatusType		ReleaseResource(ResourceType ResID)
 * 
 * Libere une resource et recherche si autre tache etait bloqué sur cette resource
 */
StatusType		ReleaseResource_Int(u32 Func_ID, ResourceType ResID)
{
	if ((ResID>=MAX_RESOURCE_NBR) || (Resource_List[ResID].resourceinfo == 0x0)) return E_OS_ID;

	/* Liberation de la ressource (retour à la normale pour la tache courante */
	Task_List[CurrentTask]->elevated_priority = 0; 
	Task_List[CurrentTask]->locksource = LOCK_SOURCE_NONE;
	Task_List[CurrentTask]->locksource = 0;

	return E_OK;
}

/*
 * void Resource_Init(void)
 * 
 * Initialisation du module "Resource" du noyau 
 */
void Resource_Init(void)
{
u8 i;

	for(i=0; i<	MAX_RESOURCE_NBR; i++)
	{
		Resource_List[i].resourceinfo = 0x0;
	}
}
