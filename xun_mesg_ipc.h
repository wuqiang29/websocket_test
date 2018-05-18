#ifndef _XUN_MESG_IPC_H_
#define _XUN_MESG_IPC_H_




typedef unsigned int  XUN_MSG_TYPE;
#define XUN_MSG_TYPE_INVALID	((XUN_MSG_TYPE)0)
#define XUN_MSG_HDR_MAGIC				(0xaabbccdd)




typedef enum
{
    APP_MSG_PRI_URGENT = 1,
    APP_MSG_PRI_NORMAL,
    APP_MSG_PRI_SYNC_RESPONSE, //sync response 

    APP_MSG_PRI_MAX
} XUN_MSG_PRI;

typedef struct  
{
    long int   magic;
    XUN_MSG_PRI      priority;  	/* Priority must be the first 4-byte */
    unsigned int     srcApplId;		 /* XUN_APP_ID */
    XUN_MSG_TYPE     type;
    unsigned int     srcMsgQKey;
    int              sync;      /* Sync or async message */
    unsigned int     len;       /* the length of the message, including the message hdr */
//	int test;
//	int test1;
} XUN_MSG_HDR_T;

typedef struct
{
	int ret;
}XUN_APP_PING_RSP_T;



//define mesg base number
#define XUN_MSG_BASE	(0x001000)
#define WEBSOCK_MSG_BASE	(0x002000)
#define TEST_MSG_BASE		(0x003000)

//app communication mesg type
#define XUN_APP_MSG_PING_SYNC		(XUN_MSG_BASE+1)
#define XUN_APP_MSG_PING_ASYNC	(XUN_MSG_BASE+2)
#define XUN_APP_MSG_PING_ACK   	(XUN_MSG_BASE+3)

//websocket mesg
#define WEBSOCK_MSG_BAT_GET    		(WEBSOCK_MSG_BASE+1)

#define LOGIN							(TEST_MSG_BASE+1)
#define LOGOUT							(TEST_MSG_BASE+2)
#define LOGOUTRSP						(TEST_MSG_BASE+3)
#define XUN_APPL_MSG_EXEC_CMD_END	(TEST_MSG_BASE+10)
#define XUN_APPL_MSG_EXEC_CMD_PRINT	(TEST_MSG_BASE+11)

// Task priority definition
#define XUN_TASK_HIGH_PRI   (10)
#define XUN_TASK_NORMAL_PRI (20)
#define XUN_TASK_LOW_PRI    (40)


#endif