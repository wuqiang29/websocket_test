/*

app communication

*/

/* VOS Message Definitions */

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/msg.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include "mesg_com.h"
#include "xun_mesg_ipc.h"
#include "xun_action.h"

//struct define app detail information
XUN_APP_INFO_T* global_appInfo=NULL;

XUN_APP_INFO_T* Xun_GetAppInfo(void)
{
	return global_appInfo;
}

INT Xun_DeleteMsgQ(XUN_MSG_Q_ID msgId)
{
	if(msgId == XUN_MSG_ID_INVALID)
		{
			printf("Input parametes invalid, please check !\n");
			return -1;
	}
	if(0 != msgctl(msgId, IPC_RMID, NULL))
		{
			fprintf(stderr,"Xun_DeleteMsgQ Error:%s pid=%d \r\n",strerror(errno),(int)getpid());
			return -1;
	}
	return 0;
}

XUN_MSG_Q_ID App_CreateMsgQ(unsigned int msgKey, unsigned int maxMsg, unsigned int maxMsgLen)
{
	int msgId;
	if ((msgId = msgget(msgKey, S_IRUSR|S_IWUSR|IPC_CREAT)) == -1) 
    { 
        fprintf(stderr,"App_CreateMsgQ Error:%s pid=%d\r\n", strerror(errno), (int)getpid()); 
        return XUN_MSG_ID_INVALID;
    } 
    return msgId;	
}


INT App_SendToMsgQ(unsigned int msgKey, XUN_MSG_HDR_T* pMsg, unsigned int len)
{
    int msgQId;
    unsigned int      tryCounter = 0;
    
    if (NULL == pMsg)
    {
    	printf("pMsg is NULL\n");
        return -1;
    }
    if(len > XUN_MAX_BYTE_PER_MSG)
    {
    	printf("len is more than expected\n");
        return -1;
    }

    msgQId = msgget(msgKey, S_IRUSR|S_IWUSR);
	printf("***msgQId [%d]**\n",msgQId);
    if (XUN_MSG_ID_INVALID == msgQId)
    {
			  perror("get ipc_id error");
			fprintf(stderr,"App_SendToMsgQ Error:%s pid=%d\r\n", strerror(errno), (int)getpid()); 
		printf("msgget failed ***********\n");
        return -1;
    }

    while (1)
    {
		//make sure mesg to be send
        //Wait forever
        printf("msgsnd [%d]\n",msgQId);
        if (0 != msgsnd(msgQId, pMsg, len, IPC_NOWAIT))  
        {
            // queue is full
            if ((EAGAIN == errno) && (tryCounter < 5))
            {
                tryCounter++;
                //APP_TaskDelay(100+100*tryCounter);
				usleep(1000);
                continue;
            }
            else
            {
            	printf("msgsnd !!!failed \n");
                 return -1;
            }
        }
        else
        {
            return 0;
        }
    }

    return 0;
}



INT App_RecvFromMsgQ(XUN_MSG_Q_ID msgId, void* pMsgHdr, unsigned int uiMaxBytes,  int msgType, unsigned int wait)
{
    int size;
    int msg_flag = 0;
    
    if (XUN_MSG_ID_INVALID == msgId)
    {
        return -1;
    }

    if (NULL == pMsgHdr)
    {
        return -1;
    }
    if(uiMaxBytes > XUN_MAX_BYTE_PER_MSG)
    {
        return -1;
    }

    if(0 == wait)
    {
        msg_flag = IPC_NOWAIT;
    }
    else
    {
        msg_flag = 0;
    }
    
    if((size = msgrcv(msgId, pMsgHdr, uiMaxBytes, msgType, msg_flag))<0)  
    {
        return -1;
    }

    if (0 == size)
    {
        return -1;
    }
    
    return 0;
}

INT Xun_AppRecvMsg(XUN_MSG_HDR_T* pMsg)
{
	XUN_APP_INFO_T* appInfo = Xun_GetAppInfo();
	XUN_TASK_INFO_T* taskInfo =  Xun_GetTaskInfo(Xun_GetTaskId(NULL));
	//make decision wait = 0 (IPC_NOWAIT)
	int wait = 1, ret;

	xun_assert(pMsg);

	Xun_SetTaskState(XUN_TASK_STATE_WAITING, (UINT)appInfo->msgQId);

	ret = App_RecvFromMsgQ(appInfo->msgQId, (void*)pMsg, XUN_MAX_BYTE_PER_MSG, -1*APP_MSG_PRI_NORMAL, wait);

	Xun_SetTaskState(XUN_TASK_STATE_NORMAL, 0);

	if(ret==0)
		{
		taskInfo->rxMsgs++;
	}
	return ret;

}

XUN_MSG_HDR_T* Xun_AppCreateMsg_Hd(XUN_MSG_HDR_T* sendBuf, XUN_MSG_TYPE type, XUN_MSG_PRI pri, INT sync, UINT len)
{
	XUN_TASK_INFO_T* taskInfo = Xun_GetTaskInfo(Xun_GetTaskId(NULL));

	xun_assert(sendBuf);

	sendBuf->magic = XUN_MSG_HDR_MAGIC;
	sendBuf->len = len;
	sendBuf->priority = pri;
	sendBuf->type = type;
	sendBuf->srcApplId=Xun_GetAppId();
	sendBuf->sync=sync;

	if(taskInfo)
		{
		sendBuf->srcMsgQKey = taskInfo->msgQKey;
	}
	return sendBuf;
}


INT Xun_ResponseSyncMsg(XUN_APP_ID desApp, void* pReqData, XUN_MSG_TYPE type, void* pData, UINT dataLen)
{
	char* sendBuf = NULL;
	XUN_MSG_HDR_T* pHdr = NULL;
	int ret, msgQKey = -1;

	XUN_TASK_INFO_T*  taskInfo = Xun_GetTaskInfo(Xun_GetTaskId(NULL));
	XUN_APP_INFO_T* appInfo = Xun_GetAppInfo();

	if(desApp == Xun_GetAppId())
		{
		fprintf(stderr,"APP %x, do not sendmesg to self\r\n",desApp);
		return 0;
	}

	if(taskInfo)
		{
			if(!taskInfo->msgBuf)
				{
					taskInfo->msgBuf = (char *)malloc(XUN_MAX_BYTE_PER_MSG);
			}
			sendBuf = taskInfo->msgBuf;
	}else
		{
			sendBuf = (char*) malloc(XUN_MAX_BYTE_PER_MSG);
	}
	xun_assert(sendBuf);

	memset(sendBuf, 0, XUN_MAX_BYTE_PER_MSG);

	xun_assert((dataLen+sizeof(XUN_MSG_HDR_T))<=XUN_MAX_BYTE_PER_MSG);

	if(NULL !=pReqData)
		{
			//get src msgq ID
			pHdr = (XUN_MSG_HDR_T*)((char*)pReqData - sizeof(XUN_MSG_HDR_T));
			if((XUN_APPL_MSG_EXEC_CMD_END == type)||(XUN_APPL_MSG_EXEC_CMD_PRINT ==type)||pHdr->sync)
				{
					msgQKey = pHdr->srcMsgQKey;

			}
			else
				{
					msgQKey = XUN_MSGQKEY(desApp, 0);

			}
	}

	//msgQKey = XUN_MSGQKEY(desApp, 0);

	pHdr = Xun_AppCreateMsg_Hd((XUN_MSG_HDR_T*)sendBuf, type, APP_MSG_PRI_SYNC_RESPONSE, 0, dataLen+sizeof(XUN_MSG_HDR_T));

	xun_assert(pHdr);

	if((NULL != pData))
		{
			memcpy(((char*)pHdr) + sizeof(XUN_MSG_HDR_T), pData, dataLen);
	}
	Xun_SetTaskState(XUN_TASK_STATE_RESPONSING, (UINT)msgQKey);

	pHdr->magic = pHdr->priority;

	ret = App_SendToMsgQ(msgQKey,pHdr,pHdr->len);

	Xun_SetTaskState(XUN_TASK_STATE_NORMAL, (UINT)0);

	if(taskInfo !=NULL)
		{
			if(ret == 0)
				{
					taskInfo->txSyncRspMsgs++;
			}else
				{
					taskInfo->txFailedSyncRspMsgs;

			}

	}else
		{
		free(sendBuf);
	}
	
	return ret;
}


INT Xun_AppRecvSyncMsg(XUN_MSG_HDR_T* pHr, INT timeout)
{

	XUN_TASK_INFO_T* taskInfo = Xun_GetTaskInfo(Xun_GetTaskId(NULL));

	if(taskInfo == NULL)
		{
			return -1;
	}

	xun_assert(pHr != NULL);

	if(timeout == 0)
		{
			return App_RecvFromMsgQ(taskInfo->msgQId,  pHr, XUN_MAX_BYTE_PER_MSG, APP_MSG_PRI_SYNC_RESPONSE, 0);
	}

	Xun_SetTaskState(XUN_TASK_STATE_WAITING_S , (UINT)taskInfo->msgQId);
	while(timeout > 0)
		{
			if(0 != App_RecvFromMsgQ(taskInfo->msgQId,  pHr, XUN_MAX_BYTE_PER_MSG, APP_MSG_PRI_SYNC_RESPONSE, 0))
				{
					usleep(3*1000);
					timeout -=3;
			}else{
					Xun_SetTaskState(XUN_TASK_STATE_NORMAL, (UINT)0);
					return 0;
			}
	}
	Xun_SetTaskState(XUN_TASK_STATE_NORMAL, (UINT)0);
	return -1;

}



/*
	async sendMsg
*/

INT Xun_AppSendMsg(XUN_APP_ID desApp, XUN_MSG_TYPE type, XUN_MSG_PRI pri, void* data, UINT datalen)
{
	char* send_buf = NULL;
	XUN_MSG_HDR_T* pHdr = NULL;
	int ret = 0;
	XUN_TASK_INFO_T* taskInfo = Xun_GetTaskInfo(Xun_GetTaskId(NULL));
	XUN_APP_INFO_T* appInfo = Xun_GetAppInfo();

	if(taskInfo)
		{
		if(!taskInfo->msgBuf)
			{
				taskInfo->msgBuf = (char*)malloc(XUN_MAX_BYTE_PER_MSG);			
		}
		send_buf = taskInfo->msgBuf; 			
	}else{
			send_buf = (char*)malloc(XUN_MAX_BYTE_PER_MSG);
	}
	xun_assert(send_buf != NULL);

	xun_assert(datalen+sizeof(XUN_MSG_HDR_T)<=XUN_MAX_BYTE_PER_MSG);
	memset(send_buf, 0x00, XUN_MAX_BYTE_PER_MSG);

	pHdr = Xun_AppCreateMsg_Hd((XUN_MSG_HDR_T*)send_buf,  type,  pri, 0, datalen+sizeof(XUN_MSG_HDR_T));

	xun_assert(pHdr!=NULL);

	if((data != NULL )&& (datalen != 0))
		{
			memcpy(((char*)pHdr)+sizeof(XUN_MSG_HDR_T),data,datalen);
	}

	Xun_SetTaskState(XUN_TASK_STATE_SENDING, (UINT)XUN_MSGQKEY(desApp, 0));
	pHdr->magic = pHdr->priority;
	ret = App_SendToMsgQ((UINT)XUN_MSGQKEY(desApp, 0), pHdr, pHdr->len);
	Xun_SetTaskState(XUN_TASK_STATE_NORMAL, (UINT)0);	
		if(taskInfo)
			{
				if(ret == 0)
					{
						printf("App_SendToMsgQ ret = 0 success\n");
						
						taskInfo->txMsgs++;
				}else{
						taskInfo->txFailedMsgs;
				}

		}else{
				free(send_buf);
		}
		return 0;	
}


int Xun_AppSendSyncMsg(XUN_APP_ID desApp, XUN_MSG_TYPE type, XUN_MSG_PRI pri, void* data, UINT datalen, void* RetData, UINT retDataLen, UINT timeout)
{
	XUN_MSG_HDR_T* pHdr = NULL;
	char* buff = NULL;

	XUN_APP_INFO_T* srcAppInfo = Xun_GetAppInfo();
	XUN_TASK_INFO_T* taskInfo  = Xun_GetTaskInfo(Xun_GetTaskId(NULL));
	int ret = 0;

	if(taskInfo)
		{
			if(taskInfo->msgBuf == NULL )
				{
					taskInfo->msgBuf = (char*)malloc(XUN_MAX_BYTE_PER_MSG);			
			}
			buff = taskInfo->msgBuf;
	}else{
			buff = (char*)malloc(XUN_MAX_BYTE_PER_MSG);
	}

	xun_assert(buff != NULL);
	/*clear sync messages*/
	while(0 == Xun_AppRecvSyncMsg((XUN_MSG_HDR_T*)buff, 0))
		{

	}

	xun_assert(datalen+sizeof(XUN_MSG_HDR_T)<=XUN_MAX_BYTE_PER_MSG);

	memset(buff, 0x00, XUN_MAX_BYTE_PER_MSG);

	pHdr = Xun_AppCreateMsg_Hd((XUN_MSG_HDR_T*) buff,  type,  pri,  1, datalen+sizeof(XUN_MSG_HDR_T));

	xun_assert(pHdr != NULL);
	/*to avoid app to send message to itself*/
	if(desApp == pHdr->srcApplId)
		{
			fprintf(stderr, "Error, sync message can not be sent to yourself\n");
			if(!taskInfo)
				{
					free(buff);
			}
			return -1;
	}

	if((data != NULL )&& (datalen != 0))
		{
			memcpy(((char*)pHdr)+sizeof(XUN_MSG_HDR_T),data,datalen);
	}

	Xun_SetTaskState(XUN_TASK_STATE_SENDING_S, (UINT)XUN_MSGQKEY(desApp, 0));

	pHdr->magic = pHdr->priority;
	ret = App_SendToMsgQ((UINT)XUN_MSGQKEY(desApp, 0), pHdr, pHdr->len);

		if(ret != 0)
			{
	              	Xun_SetTaskState(XUN_TASK_STATE_NORMAL, (UINT)0);
				if(taskInfo)
					{
						taskInfo->txFailedSyncMsgs++;
				}else{
						free(buff);
				}
      		 	 printf("%s:%d, Send sync msg failed, ret %d\r\n", __func__, __LINE__, ret);				
			return -1;
		}
	Xun_SetTaskState(XUN_TASK_STATE_NORMAL, (UINT)0);
	taskInfo->txSyncMsgs++;

	//wait sync response

	if(0 != Xun_AppRecvSyncMsg((XUN_MSG_HDR_T* )buff,  timeout))
		{
			//fprintf(stderr,"\nSync 0x%x, msg no reply, dest [], %s:%d\n"),type, __func__,__LINE__);
			if(taskInfo)
				{
					taskInfo->txSyncNoRspMsgs++;
			}
			else
				{
					free(buff);
			}
			printf("%s: %d Recv sync response failed\r\n",__func__,__LINE__);
			return -1;
	}

	pHdr = (XUN_MSG_HDR_T*)buff;
	if(pHdr->len>=sizeof (XUN_MSG_HDR_T))
		{
			if(RetData)
				{
					memcpy(RetData,(char*)pHdr+sizeof(XUN_MSG_HDR_T),retDataLen);
			}
	}
	if(taskInfo)
		{
			taskInfo->rxSyncRspMsgs++;
	}else{
			free(buff);
	}
	return 0;	
}



void* Xun_Malloc(unsigned int bytes)
{
	void* retVal;
	retVal = malloc(bytes);
	if(NULL==retVal)
	{
		printf("Error:out of memory asking for %d bytes\n",bytes);
	}
	else{
		memset(retVal,0x00,bytes);
	}
	return retVal;
}

void  Xun_Free(void* ptr)
{
	if(NULL != ptr)
	{
		free(ptr);
	}	
}



void Xun_App_DeInit(INT arg)
{
	XUN_APP_INFO_T* appInfo = Xun_GetAppInfo();

	if(appInfo->de_init)
		{
			(*appInfo->de_init)(0,0);
	}
	Xun_DeleteMsgQ(appInfo->msgQId);
	appInfo->msgQId = XUN_MSG_ID_INVALID;
	exit(0);
}
														
								
/*
APP init
*/
int Xun_App_Init(unsigned int argc, char* argv[], XUN_APP_ID appId, const char* appName)
{
	XUN_APP_INFO_T* appInfo;
	XUN_TASK_INFO_T taskInfo;
	UINT	taskIndex;
	
	
	global_appInfo = Xun_Malloc(sizeof(XUN_APP_INFO_T));
	
	if(global_appInfo==NULL)
	{
		printf("Xun_App_Init failed\n");
		return -1;
	}
	appInfo = global_appInfo;
	
	appInfo->appId = appId;
	strcpy(appInfo->name, appName);
	appInfo->state = XUN_APPL_STATE_INIT;
	appInfo->msgQId = App_CreateMsgQ(XUN_MSGQKEY(appId,0),0,0);
	printf("*********");
	printf("appInfo->msgQId [%d]********\n",appInfo->msgQId );
	
    for (taskIndex = 0; taskIndex < XUN_MAX_TASKS_PER_APP; taskIndex++)
    {
        memset(&appInfo->tasks[taskIndex], 0x00, sizeof(XUN_TASK_INFO_T));
        appInfo->tasks[taskIndex].taskId = XUN_TASK_ID_INVALID;
    }	
	
	if(appInfo->msgQId==XUN_MSG_ID_INVALID)
	{
		printf("App_CreateMsgQ failed\n");
		return -1;
	}
	
	appInfo->stackBaseAddr = ((UINT)&appInfo)+32;

	
	//add main task;
	memset(&taskInfo,0x00,sizeof(XUN_TASK_INFO_T));
	taskInfo.appId=appInfo->appId;
	strcpy(taskInfo.name,appName);
	
	//taskInfo.priority = priority;
	//taskInfo.stackSize = stackSize;
	taskInfo.pEntryFn = NULL;
	taskInfo.pPara = NULL;
	
	taskInfo.taskNumber = 0;
	taskInfo.msgQId = appInfo->msgQId;
	
	taskInfo.msgQKey = XUN_MSGQKEY(appInfo->appId,0);
	taskInfo.msgBuf =(char*)malloc(XUN_MAX_BYTE_PER_MSG);
	
	appInfo->app_getpid = getpid();
	appInfo->taskId = pthread_self();
	taskInfo.taskId = pthread_self();
	taskInfo.pid = getpid();
	taskInfo.stackBaseAddr = appInfo->stackBaseAddr;
	
	memcpy(&(appInfo->tasks[0]),&taskInfo,sizeof(XUN_TASK_INFO_T));

//	how to define app lock ?	
	appInfo->lockId = Xun_CreateLock(1,appInfo->name);
	xun_assert(appInfo->lockId != XUN_LOCK_ID_INVALID);
	
	return 0;
}


int Xun_App_Loop(void)
{
	XUN_APP_INFO_T* appInfo = Xun_GetAppInfo();
	XUN_MSG_HDR_T* pMsg;
	char* pUsrData = NULL;

	static char mesg_buf[XUN_MAX_BYTE_PER_MSG]={0};

	appInfo->state = XUN_APPL_STATE_ACTIVE;

	pMsg = (XUN_MSG_HDR_T*)mesg_buf;

	printf("*********into loop*********\n");
	while(1){
		
	UINT enter_tick=0,exit_tick;

	if(0==Xun_AppRecvMsg(pMsg))
		{

	switch(pMsg->type)	
		{
//do with program mesg
		case XUN_APP_MSG_PING_SYNC:
			
		//	//XUN_APP_PING_RSP_T rsp;
			//rsp.ret = 0;
		//	INT ret = 0;
		//	Xun_ResponseSyncMsg(pMsg->srcApplId,  NULL,  XUN_APP_MSG_PING_ACK, &ret, sizeof(XUN_APP_PING_RSP_T));
			break;

//		case 

		default:
			{
				if(pMsg->len >= sizeof(XUN_MSG_HDR_T));
					{
						pUsrData = (char*)pMsg + sizeof(XUN_MSG_HDR_T);
						int len_all = sizeof(XUN_MSG_HDR_T);
						printf("*************pMsg->len [%d] len_all [%d]**********\n",pMsg->len,len_all);
						printf("*****111********pUsrData[%s]*****111*****\n",pUsrData);
				}

				if(appInfo->handler)
					{
						Xun_SetTaskState(XUN_TASK_STATE_HANDLING , (UINT)pMsg->type);
						(*appInfo->handler)(pUsrData, pMsg->type, pMsg->priority, pMsg->srcApplId);
						Xun_SetTaskState(XUN_TASK_STATE_NORMAL , (UINT)0);
				}
				break;
			}
		
			}
					
	}
	else{

	//recieve mesg failed
	}
	
	}
	return 0;

}


int Xun_App_Start(XUN_APP_INIT_PTR pInitFn, void* pInitPara, XUN_APP_MSG_HANDLER_PTR pMsgHandlerFn, XUN_APP_DE_INIT_PTR pDeinitFn)
{
	XUN_APP_INFO_T* appInfo = Xun_GetAppInfo();

	appInfo->init = pInitFn;
	appInfo->handler = pMsgHandlerFn;
	appInfo->de_init = pDeinitFn;

	if(pInitFn)
		{
		(*pInitFn)(0,pInitPara);//app init function pointer

	}

	//app mainloop
	Xun_App_Loop();

	
	// Deinit app
	Xun_App_DeInit(0);
}


