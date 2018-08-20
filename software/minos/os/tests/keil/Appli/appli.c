#include "stm32f10x_type.h"
#include "kernel.h"

/* Declaration des taches */

TASK(myTask);
TASK(BackgroundTask);

TaskType myTaskID;
TaskType myBackgroundTaskID;

st_TaskInfo myTaskInfo;
st_TaskInfo myBackgroundTaskInfo;

char TaskName[]="myTask"; 
char BackgroundTaskName[]="BackgroundTask";

/* Declaration des resources */
st_ResourceInfo myResInfo;

ResourceType	myResID;

int main (void)
{
StatusType Status;

	/* Init a faire ici	*/
	StartOS(OSDEFAULTAPPMODE);

	myTaskInfo.taskname=TaskName;
	myTaskInfo.entrypoint=myTask;
	myTaskInfo.priority = 12;

	myBackgroundTaskInfo.taskname=BackgroundTaskName;
	myBackgroundTaskInfo.entrypoint=BackgroundTask;
	myBackgroundTaskInfo.priority = 0;

	myTaskID=DeclareTask (&myTaskInfo);
	myBackgroundTaskID=DeclareTask (&myBackgroundTaskInfo);

	myResInfo.initstate = 0;
	myResInfo.resourcetype = RES_BINARY;

	myResID = DeclareResource(&myResInfo);

	Status=ActivateTask(myBackgroundTaskID);

	if (Status!=E_OK)
	{
		/* Un probleme est arrivé */

		while(1);
	}

	while(1);
}

TASK(myTask)
{
	/* Tache vide pour l'instant */

	while (1); /* avec ca, on est sure de ne jamais changer de contexte !!! */
}

TASK(BackgroundTask)
{
StatusType Status;

	Status=ActivateTask(myTaskID);

	if (Status!=E_OK)
	{
		/* Un probleme est arrivé */

		while(1);
	}

	GetResource(myResID);
	ReleaseResource(myResID);

	while (1)
	{
		/* on ne sort jamais de la background task */
	}
}
