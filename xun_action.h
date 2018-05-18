#ifndef __XUN_ACTION__H__
#define __XUN_ACTION__H__

XUN_LOCK_ID Xun_CreateLock(UINT initVal, char* name);

INT Xun_Unlock_Op(XUN_LOCK_ID lockId);

INT Xun_Lock_Op(XUN_LOCK_ID lockId, UINT timeout);

INT Xun_DeleteLock(XUN_LOCK_ID lockId);

INT Xun_Lock();

XUN_APP_ID Xun_GetAppId(void);

XUN_TASK_ID Xun_GetTaskId(char* name);

XUN_TASK_INFO_T* Xun_GetTaskInfo(XUN_TASK_ID taskId);

INT Xun_SetTaskState(XUN_TASK_STATE state, UINT arg);

INT Xun_TaskEntry(UINT taskIndex);

XUN_TASK_ID Xun_Spawn_Task(const char* TaskName, UINT priority, UINT stacksize, XUN_TASK_ENTRY pEntryFn, int msqQ, void* pPara);

XUN_APP_INFO_T* Xun_GetAppInfo(void);

INT Xun_DeleteMsgQ(XUN_MSG_Q_ID msgId);

XUN_MSG_Q_ID App_CreateMsgQ(unsigned int msgKey, unsigned int maxMsg, unsigned int maxMsgLen);

INT App_SendToMsgQ(unsigned int msgKey, XUN_MSG_HDR_T* pMsg, unsigned int len);

INT App_RecvFromMsgQ(XUN_MSG_Q_ID msgId, void* pMsgHdr, unsigned int uiMaxBytes,  int msgType, unsigned int wait);

int Xun_App_Start(XUN_APP_INIT_PTR pInitFn, void* pInitPara, XUN_APP_MSG_HANDLER_PTR pMsgHandlerFn, XUN_APP_DE_INIT_PTR pDeinitFn);

int Xun_App_Init(unsigned int argc, char* argv[], XUN_APP_ID appId, const char* appName);

#endif