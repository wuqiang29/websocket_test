#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <errno.h>
#include "mesg_com.h"
#include "xun_action.h"


XUN_LOCK_ID Xun_CreateLock(UINT initVal, char* name)
{
	XUN_LOCK_ID lockId = XUN_LOCK_ID_INVALID;
	lockId = semget(IPC_PRIVATE, 1, IPC_CREAT | S_IRUSR | S_IWUSR);

	if(lockId == -1)
		{
		fprintf(stderr,"XUN_Create_Lock Error:%s \r\n", strerror(errno));
		return lockId;
	}
	if(semctl(lockId, 0, SETVAL,initVal)==-1)
		{
		fprintf(stderr,"XUN_Create_Lock Init Error:%s \r\n", strerror(errno));
		return -1;
	}
	return lockId;	
}

INT Xun_Unlock_Op(XUN_LOCK_ID lockId)
{
	
	struct sembuf semLock;
	
	semLock.sem_num = 0;
	semLock.sem_op = 1;
	semLock.sem_flg = 0;
	
	
	if(XUN_LOCK_ID_INVALID==lockId)
	{
		return -1;
	}
	if(-1==semop(lockId,&semLock,1))
	{
		return -1;
	}
	return 0;
}

INT Xun_Lock_Op(XUN_LOCK_ID lockId, UINT timeout)
{
	struct sembuf semLock;
	
	semLock.sem_num = 0;
	semLock.sem_op = -1;   //action 
	semLock.sem_flg = 0; //SEM_UNDO
	
	if(XUN_LOCK_ID_INVALID == lockId)
	{
		return -1;
	}
	
	if(!timeout)
	{
		if(-1==semop(lockId,&semLock,1))
		{
			return -1;
		}
	else{
		
		return -1;
		}	
	}
	return 0;
}

INT Xun_DeleteLock(XUN_LOCK_ID lockId)
{
	if(XUN_LOCK_ID_INVALID == lockId)
		{
			return -1;
	}

	if(-1 == semctl(lockId, 0, IPC_RMID))
		{
			printf("Xun_DeleteLock Error, pid = %d\r\n", getpid());
			return -1;
	}
	return 0;
}

INT Xun_Lock()
{


}


XUN_APP_ID Xun_GetAppId(void)
{
	XUN_APP_INFO_T* appInfo = Xun_GetAppInfo();

	if(appInfo == NULL)
		{
		return XUN_APPID_INVALID;
	}
	return appInfo->appId;
}


XUN_TASK_ID Xun_GetTaskId(char* name)
{
	UINT taskIndex;
	XUN_APP_INFO_T* appInfo = Xun_GetAppInfo();
	if(appInfo == NULL)
		{
		return XUN_TASK_ID_INVALID;
	}
	if(NULL==name)
		{
		
		return (XUN_TASK_ID)pthread_self();
	}else{

		Xun_Lock_Op(appInfo->lockId, 0);

		for(taskIndex =0;taskIndex<XUN_MAX_BYTE_PER_MSG;taskIndex++)
			{
				if(strcmp(appInfo->tasks[taskIndex].name,name)==0)
					Xun_Unlock_Op(appInfo->lockId);
				return appInfo->tasks[taskIndex].taskId;
		}
		Xun_Unlock_Op(appInfo->lockId);
	}
	return XUN_TASK_ID_INVALID;
}


XUN_TASK_INFO_T* Xun_GetTaskInfo(XUN_TASK_ID taskId)
{
	UINT taskIndex;
	XUN_APP_INFO_T* appInfo = Xun_GetAppInfo();
	
	if(XUN_TASK_ID_INVALID==taskId)
	{
		return NULL;		
	}
	xun_assert(appInfo);
	Xun_Lock_Op(appInfo->lockId, 0);
	for(taskIndex=0;taskIndex<XUN_MAX_TASKS_PER_APP;taskIndex++)
	{
		if(appInfo->tasks[taskIndex].taskId == taskId)
			{
			Xun_Unlock_Op(appInfo->lockId );
			return  &appInfo->tasks[taskIndex];
		}		
	}
	Xun_Unlock_Op(appInfo->lockId);
	return NULL;
}

int Xun_SetTaskState(XUN_TASK_STATE state, UINT arg)
{
	XUN_TASK_INFO_T*  taskInfo = Xun_GetTaskInfo(Xun_GetTaskId(NULL));
	int ret =0;
	if(taskInfo)
		{
		taskInfo->taskState = state;
		taskInfo->taskArg = arg;
		ret = 0;
	}
	return ret;
}

INT Xun_TaskEntry(UINT taskIndex)
{
	XUN_APP_INFO_T* AppCfg = Xun_GetAppInfo();
	//xun_assert(AppCfg);

	Xun_Lock_Op(AppCfg->lockId, 0);

	if(AppCfg->tasks[taskIndex].pEntryFn)
		{
			//setpriority(0, 0, (UINT)AppCfg->tasks[taskIndex].priority - 20);
			AppCfg->tasks[taskIndex].pid = getpid();
			AppCfg->tasks[taskIndex].stackBaseAddr = ((UINT)&AppCfg) + 32;
			Xun_Unlock_Op(AppCfg->lockId);
			(*((XUN_TASK_ENTRY)AppCfg->tasks[taskIndex].pEntryFn))(AppCfg->tasks[taskIndex].pPara);
			Xun_Lock_Op(AppCfg->lockId, 0);
	}

	if(AppCfg->tasks[taskIndex].msgBuf) {
		free(AppCfg->tasks[taskIndex].msgBuf);
		AppCfg->tasks[taskIndex].msgBuf = NULL;
	}

	//delete message Q
	Xun_DeleteMsgQ(AppCfg->tasks[taskIndex].msgQId);
	AppCfg->tasks[taskIndex].msgQId = XUN_MSG_ID_INVALID;

	//take exit, release it 
	AppCfg->tasks[taskIndex].taskId = XUN_TASK_ID_INVALID;
	Xun_Unlock_Op(AppCfg->lockId);

	return 0;
}

XUN_TASK_ID Xun_Spawn_Task(const char* TaskName, UINT priority, UINT stacksize, XUN_TASK_ENTRY pEntryFn, int msqQ, void* pPara)
{
	XUN_APP_INFO_T* appInfo = Xun_GetAppInfo();
	XUN_TASK_INFO_T taskInfo ;
	UINT taskIndex;
	pthread_t thread_a;

	xun_assert(appInfo);
	memset(&taskInfo, 0x00, sizeof(XUN_TASK_INFO_T));
	taskInfo.appId = appInfo->appId;
	strcpy(taskInfo.name,TaskName);
	taskInfo.priority = priority;
	taskInfo.stackSize = stacksize;
	taskInfo.taskId = XUN_TASK_ID_INVALID;

	taskInfo.pEntryFn = pEntryFn;
	taskInfo.pPara = pPara;
	taskInfo.taskState = XUN_TASK_STATE_NORMAL;
	taskInfo.taskArg = 0;

	Xun_Lock_Op(appInfo->lockId, 0);
	for(taskIndex = 0; taskIndex < XUN_MAX_TASKS_PER_APP; taskIndex++)
		{
			if(appInfo->tasks[taskIndex].taskId == XUN_TASK_ID_INVALID)
				{
					taskInfo.taskNumber = taskIndex;
					memcpy(&(appInfo->tasks[taskIndex]), &taskInfo, sizeof(XUN_TASK_INFO_T));
					break;
			}
	}

	if(XUN_MAX_TASKS_PER_APP == taskIndex)
		{
			Xun_Unlock_Op(appInfo->lockId);
			return XUN_TASK_ID_INVALID;
	}

	if(msqQ)
		{
			appInfo->tasks[taskIndex].msgQId = App_CreateMsgQ(XUN_MSGQKEY(appInfo->appId, taskIndex),0 ,0);
			if(XUN_MSG_ID_INVALID== appInfo->tasks[taskIndex].msgQId)
				{
					appInfo->tasks[taskIndex].taskId= XUN_TASK_ID_INVALID;
					Xun_Unlock_Op(appInfo->lockId);
					return XUN_TASK_ID_INVALID;
			}
			appInfo->tasks[taskIndex].msgQKey = XUN_MSGQKEY(appInfo->appId, taskIndex);
	}else{
					appInfo->tasks[taskIndex].msgQId = XUN_MSG_ID_INVALID;
					appInfo->tasks[taskIndex].msgQId = -1;
			}
	if(0 != pthread_create(&thread_a, NULL, (void*)Xun_TaskEntry, (void*)taskIndex))
		{
			xun_assert(0);
	}

	appInfo->tasks[taskIndex].taskId = (XUN_TASK_ID)thread_a;
	Xun_Unlock_Op(appInfo->lockId);

	if(0 != pthread_detach(thread_a))
		printf("detach pthread %d failed\n",appInfo->tasks[taskIndex].taskId);
	
	return appInfo->tasks[taskIndex].taskId;
	
}
