#ifndef _XUN_BIND_H
#define _XUN_BIND_H
#include "cJSON.h"


#define BING_TIMEOUT_TIME_CNT	57
#define BING_TIMEOUT_TIME (1000)
#define BING_RESEND_TIME (10000)

#define BIND_DEVICESET_CFG_PATH		"bind.cfg"

#define XUN_MAX_QRSTR_LEN		128
#define BIND_DEVICESET_QR_PATH		"qrcode"

#define  XUN_VERSION_NUM_LEN  40
#define WATCH_VERSION_FILE_PATH	       "version"

//-2:not bind;	-1:binding step 1;0 binding step 2;	1:bind sc
typedef enum 
{
   BIND_STATUS_NOT_BIND = -2,
   BIND_STATUS_BINDING_STEP_1 = -1,
   BIND_STATUS_BINDING_STEP_2 = 0,
   BIND_STATUS_BIND_SUCCESS = 1
}xun_bind_status;


typedef struct 
{
	xun_bind_status watch_bind_status; /* -2:not bind;	-1:binding step 1;0 binding step 2;	1:bind sc */
	int deviceset_is_suc;
}xun_watch_user_info_t;


typedef struct mesg_list{	
	char* json;
	int data_len; 
	struct mesg_list *next;	
}MESG_LIST;

xun_bind_status xun_get_bind_status(void);
void xun_bind_handle_bind_req(cJSON* e2e_msg);

#endif
