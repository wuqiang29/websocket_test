#ifndef _MESG_COM_H_
#define _MESG_COM_H_

#include "xun_mesg_ipc.h"

#define XUN_APPID_INVALID		(0x00)

#define XUN_APPID_WEBSOCKET		(0x4001)
#define XUN_APPID_BLUETOOTH		(0x2001)
#define XUN_APPID_TEST				(0x3001)


#define XUN_MAX_TASKS_PER_APP  (8)
#define XUN_MAX_NUM_OF_APP           (32)

#define XUN_TASK_NAME_MAX_LEN          (32)
#define XUN_DEFAULT_STACK_SIZE    (2 * 4096)




typedef enum 
{
    XUN_APPL_STATE_INIT    = 0,
    XUN_APPL_STATE_ACTIVE  = 1,
}XUN_APP_STATE;


/* Task states */
typedef enum 
{
    XUN_TASK_STATE_SENDING      = 0,    // Sending asynchronous message 
    XUN_TASK_STATE_SENDING_S    = 1,    // Sending synchronous message
    XUN_TASK_STATE_WAITING      = 2,    // Waiting for asynchronous message
    XUN_TASK_STATE_WAITING_S    = 3,    // Waiting for synchronous message
    XUN_TASK_STATE_RESPONSING   = 4,    // Responsing synchronous message
    XUN_TASK_STATE_HANDLING     = 5,    // Handling message
    XUN_TASK_STATE_HANDLING_CLI = 6,    // Handling CLI message
    XUN_TASK_STATE_PRINTING     = 7,
    XUN_TASK_STATE_LOCKING      = 8,
    XUN_TASK_STATE_NORMAL       = 9,    // Normal state
    
    XUN_TASK_STATE_UNKNOWN
} XUN_TASK_STATE;



typedef unsigned int XUN_APP_ID;
typedef int XUN_TASK_ID;
#define XUN_TASK_ID_INVALID            ((XUN_TASK_ID)-1)


typedef int INT;

typedef int XUN_APP_PID;

typedef int XUN_MSG_Q_ID;
#define XUN_MSG_ID_INVALID         ((XUN_MSG_Q_ID)-1)

typedef unsigned int UINT;


#define XUN_MSGQKEY(appId, taskIndex) (appId + taskIndex) 
#define XUN_MAX_BYTE_PER_MSG       (1024*64)


typedef int  XUN_LOCK_ID;
#define XUN_LOCK_ID_INVALID ((XUN_LOCK_ID)(-1))


//#define APP_MAX_BYTE_PER_MSG       (1024*128)


#define xun_assert(cond)   if(cond==0)\
                         {\
                             printf("\r\nAssertion Failed:"#cond", file %s, line %d, pid %d\r\n", __FILE__, __LINE__, getpid()); \
							 return 0;\
                         }\
						 else {}
								


typedef int (*XUN_TASK_ENTRY)(void* pPara);
typedef int (*XUN_APP_INIT_PTR)(int type, void* pPara);
typedef int (*XUN_APP_DE_INIT_PTR)(int type, void* pPara);
typedef int (*XUN_APP_MSG_HANDLER_PTR)(void* pData, XUN_MSG_TYPE type, XUN_MSG_PRI pri, XUN_APP_ID srcApp);

typedef struct
{ 
	XUN_TASK_ID taskId;  //Thread ID
	UINT		pid;	//Process ID
	XUN_APP_ID	appId;	//App ID
	char name[32];		//task name
		
	UINT	priority;
	UINT	stackSize;
	
	XUN_TASK_ENTRY	pEntryFn;
	void* pPara;
	UINT	taskNumber;
	XUN_MSG_Q_ID	msgQId;
	UINT	msgQKey;
	
	XUN_TASK_STATE	taskState;
	UINT	taskArg;
	
	//Counters
	UINT	rxMsgs;
	UINT	rxSyncRspMsgs;
	
	UINT	txMsgs;
	UINT	txSyncMsgs;
	UINT	txSyncRspMsgs;
	UINT	txFailedMsgs;
	UINT	txFailedSyncMsgs;
	UINT	txFailedSyncRspMsgs;
	UINT       txSyncNoRspMsgs;
	
	UINT	stackBaseAddr;
	
	char	*msgBuf;
	
}XUN_TASK_INFO_T;

typedef struct{
	char	name[32];      	//app name
	XUN_APP_ID	appId;		//app id self define
	XUN_APP_PID	app_getpid;	//getpid()
	XUN_TASK_ID		taskId;	//pthread_self()
	XUN_APP_STATE	state;
	XUN_MSG_Q_ID	msgQId;		//msgget return code
	
	
	XUN_TASK_INFO_T  tasks[8];
	XUN_LOCK_ID		lockId;
	
	XUN_APP_INIT_PTR	init;
	XUN_APP_MSG_HANDLER_PTR   handler;
	XUN_APP_DE_INIT_PTR		de_init;
	
	UINT stackBaseAddr;
	
}XUN_APP_INFO_T;



#endif