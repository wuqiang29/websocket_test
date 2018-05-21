#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include "server_client.h"
#include "xun_bind.h"
#include "xun_commut.h"

static xun_watch_user_info_t g_watch_user_info = {0};
static cJSON* g_xun_dl_bind_msg ;
extern char WatchSID[40];

extern void WebSocket_Send_Data_addlist(void* data_com);
extern int WebSocket_Data_Send(void);

void xun_set_bind_status(xun_bind_status status)
{
    printf("[bind]xun_set_bind_status--(%d),old status--(%d)\n",status,g_watch_user_info.watch_bind_status);
    if((status >= BIND_STATUS_NOT_BIND) &&(status <= BIND_STATUS_BIND_SUCCESS))
    {
        if(status != g_watch_user_info.watch_bind_status)
        {
            g_watch_user_info.watch_bind_status = status;
        }
    }
    else
    {
        printf("[bind]xun_set_bind_status--status fail\n");
    }
}


cJSON* xun_tcp_compose_message_basic(int  cid , int sn)
{
        cJSON* json_msg; 
        int new_sn ;

        printf("[com]bbwatch_tcp_compose_message_basic CID=%d, sn=%d\n" , cid , sn); 

        json_msg = cJSON_CreateObject();
        cJSON_AddNumberToObject( json_msg , KEY_NAME_CID , cid  );
        cJSON_AddStringToObject( json_msg , KEY_NAME_SID ,  WatchSID );
        cJSON_AddStringToObject(json_msg,KEY_NAME_PROTOCAL_VER,PROTOCOL_VER);

        if(0 == sn)
        {
            // UTC used as sn
         //   new_sn = xun_util_get_current_utc();
            //cJSON_AddNumberToObject( json_msg , KEY_NAME_SN ,  new_sn );
        }
        else
        {
            cJSON_AddNumberToObject( json_msg , KEY_NAME_SN ,  sn );
        }

        return json_msg;
}

char* xun_tcp_compose_message( int  cid , cJSON* pl , int sn , char* gid , group_members_t*   target_eids )
{
        cJSON* json_msg = NULL; 
        char* msg_str=NULL;

    //    printf("[com]bbwatch_tcp_compose_message CID=%d pl=%x gid=%s, target_eids=%x  sn=%d\n",
       //     cid, &pl, gid, target_eids , sn ); 

        if( pl == NULL )
        {
            printf("[com] pl is NULL, compose failed\n"); 
            return NULL;
        }

        json_msg = xun_tcp_compose_message_basic( cid  ,  sn );

        if( gid != NULL )
        {
            cJSON_AddStringToObject( json_msg , KEY_NAME_TGID, gid );
        }
        else if( target_eids != NULL  )
        {
            cJSON_AddStringArrayToObject(json_msg ,KEY_NAME_TEID, target_eids->eid_list, target_eids->count );
        }

        cJSON_AddItemToObject( json_msg , KEY_NAME_PL , pl );
        msg_str = cJSON_Print( json_msg );
        cJSON_Delete( json_msg );

        return msg_str;
}


int xun_e2e_message(cJSON* e2e_pl, int subAction, int src_sn, group_members_t* target_group)
{
    char* sendBuff = NULL;
    int ret, send_len;	
    MESG_LIST *data = NULL;

    printf("[com]### bb_e2e_message subAction=%d, sn=%d\n", subAction, src_sn);

    if( NULL == e2e_pl  )
    {
        printf("[com] e2e_pl is null\n" );
        return RC_FAIL;
    }

    sendBuff = xun_tcp_compose_message(CID_E2E_UP, e2e_pl, src_sn, NULL, target_group);
    send_len = strlen(sendBuff);
    if( sendBuff != NULL ) 
    {
      //  send_task_node newTask = {0};
        //VMUINT32 sn = bbwatch_util_get_current_utc();
        
     //   PrintLongString( sendBuff );

        //Free sendBuff memory is in delete_one_tcp_task
    //    newTask.msgStatus = BB_NODE_PREPARING;
        //服务器传过来的sn可能为负值，在e2e消息没有回复的情况，就会导致超时检查失效
     //   newTask.sn = xun_util_get_current_utc();
       // newTask.phone_sn = src_sn;
     //   newTask.send_buff = sendBuff;
      //  newTask.send_buff_len = strlen(sendBuff);
      //  newTask.req_cid = CID_E2E_UP;
      //  newTask.rsp_callback = xun_common_cb;
       // newTask.ext_data = NULL;
       // newTask.ext_data_len = 0;

        printf("[com]### subAction= %d\n", subAction);
  //      if(subAction == SUB_ACTION_SET_SHUTDOWN_DEVICE)
   //     {
            //lixin: 远程关机要等到e2e之后再执行关机操作
  //          xun_save_watch_status(WATCH_STATUS_POWER_OFF);
 //           xun_upload_watch_status_info();
 //           newTask.req_callback = xun_low_battery_shutdown_timer;
//            XUN_TRACE_EX("[com]### bb_watch_low_battery_shutdown_timer.addr=%x", xun_low_battery_shutdown_timer);
//        }
//        
//        ret = add_high_level_task(&newTask);
  //      if(ret < 0)
  //      {
 //           if(newTask.send_buff)
  //              BB_MemFree((void*)newTask.send_buff);
   //         if(newTask.ext_data && newTask.ext_free_func)
    //            ((ext_data_free_func)newTask.ext_free_func)((void*)newTask.ext_data);
    //    }


	
	data = (MESG_LIST*) malloc(sizeof(MESG_LIST));
	memset(data, 0x00, sizeof(MESG_LIST));
	data->data_len = send_len;
	data->json = sendBuff;
	WebSocket_Send_Data_addlist(data);
	ret = WebSocket_Data_Send();

        return ret;
    }
    else
    {
        return RC_FAIL;
    }
}


int xun_get_phone_group_eid_list(group_members_t* phone_group , int only_required, char* required_phone_eid)
{
    char filename;
		
//    printf("[com] bbwatch_get_phone_group_eid_list required_phone_eid=%x only_required=%d\n",  (unsigned int)required_phone_eid, only_required);
	if( NULL == phone_group )
	{	
	       printf("[com] phone_group is null\n");
	       return  RC_FAIL;
	}

        if( NULL != required_phone_eid ) 
        {
              printf("[com] only add required eid\n");
		 phone_group->count = 0;
		 //phone_group->eid_list[0] = (VMSTR)BB_MemAlloc(MAX_EID_LEN);
		 phone_group->eid_list[0] = (char*)malloc(MAX_EID_LEN);
		 memset((char*)phone_group->eid_list[0],0x00,MAX_EID_LEN);
		 strcpy( phone_group->eid_list[0] , required_phone_eid );
		 phone_group->count++;
        }
        return RC_SUCCESS;
	
}


int xun_bind_send_accept_ret_to_phone(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int  result = RC_FAIL;
    char* seid = NULL;
    int src_sn = 0;
    cJSON* new_pl , *org_pl;
    int  only_add_required_eid = 0;
    group_members_t  phone_group = {0};
    /*----------------------------------------------------------------*/
    /* Code Body                                                                                               */
    /*----------------------------------------------------------------*/
    printf("[bind]xun_bind_send_accept_ret_to_phone\n" );
    if( NULL == g_xun_dl_bind_msg )
    {
        printf("[bind] no cached e2e down msg\n" );
        return RC_FAIL;
    }
    
    org_pl = cJSON_GetObjectItem( g_xun_dl_bind_msg , KEY_NAME_PL );
    if( NULL == org_pl  )
    {
        printf("[bind] org pl invalid : %s\n" , cJSON_GetErrorPtr( ));
        return RC_FAIL;
    }
    seid = cJSON_GetObjectItem( g_xun_dl_bind_msg , KEY_NAME_SEID )->valuestring;
    printf("[bind] SEID = %s" , seid  );
    src_sn = cJSON_GetObjectItem( g_xun_dl_bind_msg , KEY_NAME_SN )->valueint;
    only_add_required_eid = 1;
    
    if( xun_get_phone_group_eid_list( &phone_group , only_add_required_eid ,  seid ) < RC_SUCCESS )
    {
        printf("[bind] get eid list failed\n");
        return RC_FAIL;
    }
    new_pl = cJSON_Duplicate(org_pl , 1);
    cJSON_AddNumberToObject( new_pl , KEY_NAME_RC , RC_SUCCESS );
    result = xun_e2e_message( new_pl , SUB_ACTION_WATCH_BIND, src_sn , &phone_group );
  //  xun_free_phone_group_eid_list(&phone_group );
    return result;
}

cJSON* xun_cache_dl_bind_msg( cJSON* src_msg )
{
    printf("[bind]bbwatch_cache_e2e_msg\n");
    if( src_msg )
    {        
        printf("[bind]save cached data\n");
        g_xun_dl_bind_msg = cJSON_Duplicate(src_msg, 1);
        return g_xun_dl_bind_msg;
    }

    return NULL;	
}


int  xun_bind_send_accept_ret()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    //send bind suc e2e msg 
    int ret = -1;
    int send_ret = RC_FAIL ;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/      
    printf("[bind]xun_bind_send_accept_ret --start\n");
    send_ret = xun_bind_send_accept_ret_to_phone();

    if(RC_SUCCESS == send_ret)
    {
        xun_set_bind_status( BIND_STATUS_BINDING_STEP_2) ;

     //   if(-1 != g_xun_bind_resend_timer)
 //       {
   //         xun_stop_timer(g_xun_bind_resend_timer);
  //          g_xun_bind_resend_timer = -1;
  //      }

 //       if(g_xun_bind_resend_timer < 0)
 //       {
  //          g_xun_bind_resend_timer = xun_start_timer(BING_RESEND_TIME, (TIMERPROC_T)xun_bind_resend_accept_ret);
  //      }
        ret = 1 ;
    }
    else
    {
        printf("[bind]xun_bind_send_accept_ret--end data error\n");
    }

    printf("[bind]xun_bind_send_accept_ret--end--ret=%d\n",ret);
    return ret;
}





xun_bind_status xun_get_bind_status(void)
{
    printf("[bind]xun_get_bind_status--(%d)\n",g_watch_user_info.watch_bind_status);
    return g_watch_user_info.watch_bind_status;
}

void xun_bind_handle_bind_req(cJSON* e2e_msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                                                         */
    /*----------------------------------------------------------------*/
    xun_bind_status bind_status = BIND_STATUS_NOT_BIND;
    int send_ret = RC_FAIL;
    /*----------------------------------------------------------------*/
    /* Code Body                                                                                               */
    /*----------------------------------------------------------------*/
	 g_watch_user_info.watch_bind_status = bind_status;
    bind_status = xun_get_bind_status();
    printf("[bind]xun_bind_handle_bind_req--bind status =%d\n", bind_status);
    if((BIND_STATUS_NOT_BIND == bind_status)||(BIND_STATUS_BIND_SUCCESS == bind_status))
    {
       //xun_release_bind_hldr_res();
	//g_xun_bind_time_count = BING_TIMEOUT_TIME_CNT;
        if(xun_cache_dl_bind_msg(e2e_msg) == NULL)
        {
            printf("[bind] bbwatch_cache_e2e_msg fail\n");
            return;
        }
        xun_set_bind_status(BIND_STATUS_BINDING_STEP_1) ;
	 //#if defined(DIGIT_TONE_SUPPORT) && defined(__XUN_LED_ARRAY_SCR_SUPPORT__)
	 //if(xun_key_get_keyevent_count() >= 16){
	 //	xun_stop_play_number();
	 //}
	 //#endif
	 //xun_tb_set_current_status ( STATUS_BUSY );
        printf("[bind]xun_bind_send_accept_ret --start\n");
   	 send_ret = xun_bind_send_accept_ret_to_phone();
	 if(RC_SUCCESS == send_ret)
	 	{
			 xun_set_bind_status(BIND_STATUS_BINDING_STEP_2) ;
	 }else
	 	{
       	 printf("[bind]xun_bind_send_accept_ret--end data error\n");
	 }
	// xun_bind_send_accept_ret();
	//XUN_TRACE_EX("[bind]xun_bind_send_accept_ret--end--ret=%d",ret);
	//xun_bind_notify_user_confirm();
	//#if defined(__XUN_UI_SCR_SUPPORT__) || defined(__XUN_MMI_COMMON_APP__)
	//xun_logger_send_event_ind(XUN_LOGGER_BIND_NOTIFICATION_IND, NULL);
	//#endif
    }
}

