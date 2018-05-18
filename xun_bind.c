#include "server_client.h"
#include "xun_bind.h"

static xun_watch_user_info_t g_watch_user_info = {0};


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

        if(-1 != g_xun_bind_resend_timer)
        {
            xun_stop_timer(g_xun_bind_resend_timer);
            g_xun_bind_resend_timer = -1;
        }

        if(g_xun_bind_resend_timer < 0)
        {
            g_xun_bind_resend_timer = xun_start_timer(BING_RESEND_TIME, (TIMERPROC_T)xun_bind_resend_accept_ret);
        }
        ret = MMI_TRUE ;
    }
    else
    {
        XUN_TRACE_EX("[bind]xun_bind_send_accept_ret--end data error");
    }

    XUN_TRACE_EX("[bind]xun_bind_send_accept_ret--end--ret=%d",ret);
    return ret;
}


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



xun_bind_status xun_get_bind_status(void)
{
    printf("[bind]xun_get_bind_status--(%d)",g_watch_user_info.watch_bind_status);
    return g_watch_user_info.watch_bind_status;
}

void xun_bind_handle_bind_req( cJSON* e2e_msg )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    xun_bind_status bind_status = BIND_STATUS_NOT_BIND;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    bind_status = xun_get_bind_status();
    pritnf("[bind]xun_bind_handle_bind_req--bind status =%d\n", bind_status);
    if(( BIND_STATUS_NOT_BIND == bind_status )||( BIND_STATUS_BIND_SUCCESS == bind_status ))
    {
     //   xun_release_bind_hldr_res();
	//	g_xun_bind_time_count = BING_TIMEOUT_TIME_CNT;
        //if( xun_cache_dl_bind_msg( e2e_msg ) == NULL )
      //  {
      //      XUN_TRACE_EX("[bind] bbwatch_cache_e2e_msg fail" );
      //      return;
     //   }
       xun_set_bind_status( BIND_STATUS_BINDING_STEP_1) ;
	//#if defined(DIGIT_TONE_SUPPORT) && defined(__XUN_LED_ARRAY_SCR_SUPPORT__)
	//if(xun_key_get_keyevent_count() >= 16){
	//	xun_stop_play_number();
	//}
	//#endif
	//xun_tb_set_current_status ( STATUS_BUSY );
	xun_bind_notify_user_confirm();
	//#if defined(__XUN_UI_SCR_SUPPORT__) || defined(__XUN_MMI_COMMON_APP__)
	//xun_logger_send_event_ind(XUN_LOGGER_BIND_NOTIFICATION_IND, NULL);
	//#endif
    }
}

