#include <string.h>
#include <stdio.h>
#include <openssl/md5.h>

#include "xun_bind.h"
#include "cJSON.h"
#include "xun_commut.h"

#define WATCHN  "78:11:DC:94:03:83"

int ApkSN = 123456;

char  PhoneEID[40] = "PHONEEID";
extern char WatchSID[40];

//extern void xun_bind_handle_bind_req(cJSON* e2e_msg);

void printHex(unsigned char *hex, int length,char* md5)
{
    int i;
	char str1[33]={0};
	char str2[9]={0};
	
    for(i = 0; i < length; i++)
	{
	//	printf("hex[%d] %02X\n",i,hex[i]&0xff);
		//printf("hex[%d] %s\n",i,hex);
		sprintf(str2,"%02X",hex[i]&0xff);
		strcat(str1,str2);		
	}
	strcpy(md5,str1);
	//printf("str1:[%s]\n",md5);	
	//printf("%02x", hex[i]);
}

void standardCall(char *buffer, int length, unsigned char *md5)
{
  //  char *buffer4 = (char *)"78:11:DC:94:03:8374CECB85AE17BB85C56FFA91FE33F6E0";
    unsigned char md5_get[MD5_DIGEST_LENGTH+1]={0};
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx,buffer,length);

    MD5_Final(md5_get,&ctx);
    printHex(md5_get,MD5_DIGEST_LENGTH,md5);	
}

//MD5 output

char*  cJSON_login(void)
{
	cJSON *root=NULL, *fmt=NULL;
	char md5Src[100] = {0};
	char md5Dsc[33] = {0};
	char name[30] = {0};
	int md5Src_len = 0;

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root,"Version","00030000");
	cJSON_AddNumberToObject(root, "SN", 142);
	cJSON_AddNumberToObject(root, "CID", 10211);
	cJSON_AddItemToObject(root, "PL", fmt=cJSON_CreateObject());
	cJSON_AddStringToObject(fmt,"Name", "78:11:DC:94:03:83");

	 sprintf(md5Src, "%s%s", WATCHN,ACCESS_KEY);
	md5Src_len = strlen(md5Src);
	 standardCall(md5Src, md5Src_len, md5Dsc);
//	cJSON_AddStringToObject(fmt,"Name", "865843024562586");	
//	cJSON_AddStringToObject(fmt, "Password", "5F64E333CEDB15AD7182D18FC070F8DB");
	cJSON_AddStringToObject(fmt, "Password", md5Dsc);
	cJSON_AddNumberToObject(fmt, "Type", 200);

	cJSON_AddStringToObject(fmt, "machSerialNo", "940383");
//	cJSON_AddStringToObject(fmt, "machSerialNo", "562586");
	//sendBuff = cJSON_Print(root);
	//sendBuff = cJSON_PrintUnformatted(root);
			
	char *tmp = cJSON_Print(root);	
	cJSON_Delete(root);
	return tmp;
}

char* cJSON_logout(void)
{
	cJSON *root = NULL;
	root = cJSON_CreateObject();
	
	cJSON_AddNumberToObject(root,"CID",10221);
	
	if(WatchSID[0] == '\0'){
		printf("session is null.\n");
		return NULL;
	}
	cJSON_AddStringToObject(root,"SID",WatchSID);
	
	char *tmp = cJSON_Print(root);

	cJSON_Delete(root);
	return tmp;
}

char* cJSON_little_ping(void)
{
	    char tmpSendData[2] = {0};
		char* tmp = (char*)tmpSendData;
		return tmp;;
}


void xun_e2e_phone_to_watch(cJSON* e2e_msg )
{
    cJSON* PL = NULL;
    int sub_action;
    char* keyPtr = NULL;
    oppo_info_t tmp_oppo_info;
    xun_bind_status bind_status = BIND_STATUS_NOT_BIND;


    printf("[com] bbwatch_tcp_e2e_phone_to_watch\n"); 
    if( NULL == e2e_msg )
    {
        printf("[com] invalid e2e_msg\n");
        return ;
    }

    ApkSN  = cJSON_GetObjectItem(e2e_msg, "SN")->valueint;
    PL = cJSON_GetObjectItem( e2e_msg , KEY_NAME_PL );
    if( NULL == PL )
    {
        printf("[com] not have PL,\n");
        return ;   
    }

    sub_action = cJSON_GetObjectItem( PL ,  KEY_NAME_SUB_ACTION )->valueint;

    if(SUB_ACTION_SERVICE_PUSH_MSG != sub_action)
    {
        cJSON* EIDJ = NULL;
        cJSON* SNJ = NULL;

	EIDJ= cJSON_GetObjectItem(e2e_msg,"SEID");
    if (EIDJ !=NULL)
       { 
       strcpy(PhoneEID, cJSON_GetObjectItem(e2e_msg,"SEID")->valuestring);
        strcpy(tmp_oppo_info.eid, cJSON_GetObjectItem(e2e_msg,"SEID")->valuestring);
       }
          SNJ=cJSON_GetObjectItem(e2e_msg, "SN");
         if (SNJ !=NULL)
        tmp_oppo_info.sn = cJSON_GetObjectItem(e2e_msg, "SN")->valueint;
       // strcpy(tmp_oppo_info.eid, cJSON_GetObjectItem(e2e_msg,"SEID")->valuestring);
    }
    
    printf( "PhoneEID=%s, ApkSN=%d\n" , PhoneEID, ApkSN);

    printf("[com] sub_action= %d\n" , sub_action);

    if(cJSON_GetObjectItem(e2e_msg, "EFID") != NULL){
        printf("[Loc_policy] Safe area msg\n");
        #if  0
        if(1 == cJSON_GetObjectItem(e2e_msg, KEY_NAME_TYPE)->valueint){
            //message for 	enter safety area
            bb_watch_set_safety_area(TRUE);
        }else if(2 == cJSON_GetObjectItem(e2e_msg, KEY_NAME_TYPE)->valueint){
            //message for 	exit safety area
            bb_watch_set_safety_area(FALSE);
        }else{
            XUN_TRACE_EX("[Loc_policy]safe area info error!");
        }
        #endif
    }
    else
    {
        switch( sub_action )
        {
            case SUB_ACTION_SERVICE_PUSH_MSG:
                 //   xun_e2e_action_service_push_msg(PL);          
                break;
        
            case SUB_ACTION_GET_LOCATION:
          //      xun_e2e_action_get_trace_data(&tmp_oppo_info);            
                break;
	     case SUB_ACTION_GET_TRACKING_LOCATION:
	     //    xun_fota_check_if_interrupt_dl();
		// xun_switch_tracking_mode(PL);
		 break;
		case SUB_ACTION_REAL_TIME_LOCATION:
		//	xun_real_time_mode(PL);
			break;
                
            case SUB_ACTION_SET_WATCH_SETTING:
                //bbwatch_e2e_action_watch_setting(e2e_msg);
                break;

            case SUB_ACTION_FORCE_RECORD:
                //record 15s;
                //strcpy(VoiceType, "record");
                //bg_start_record();
               // xun_bg_start_record();
                break;
            //add for background call permit.mayanjun.
            case SUB_ACTION_BACKGROUND_CALL:
                //background call action.
               // xun_do_gbcall_action(e2e_msg);
                break;
                
            case SUB_ACTION_SET_SHUTDOWN_DEVICE:
            {
                //xun_e2e_action_shutdown_msg(e2e_msg);
                break;
            }
            
            case SUB_ACTION_SET_WATCH_BUZZER:
                //bbwatch_e2e_action_watch_play_alert( e2e_msg );
            //    #ifdef __XUN_RGBL_BREATH_EFFECT__
             //   xun_show_rgbl_effect_with_timer(RGBL_FIND_DEVICE, 15000);
             //   #endif
             //   #if defined(__LCT_BUZZ_SUPPORT__) && defined(__MTK_TARGET__)
              //  xun_util_buzz_start_ex(500, 15, 4);
             //   #endif
#if 0
            	{
            		mmi_confirm_property_struct tmp = {0};
            		WCHAR asd[8]= {0};
            		mmi_confirm_property_init ( &tmp, CNFM_TYPE_OK );
            		memset ( tmp.softkey, 0, sizeof ( tmp.softkey ) );
            		tmp.softkey[0].str = 0;
            		tmp.softkey[0].img = ( PS8 ) get_image ( IMG_GLOBAL_POP_BUTTON_UP );
            		tmp.confirm_callback = xun_audio_stop_tone;
            		tmp.f_auto_map_empty_softkey = 0;

            		mmi_asc_to_wcs(asd,"");
            		mmi_zc_confirm_display ( asd, MMI_EVENT_FIND_WATCH, &tmp );
            	}

                xun_audio_play_search_tone(1);
#endif
				
                break;
				
            //watch bind xlk add
    		case SUB_ACTION_WATCH_BIND:
			  /*Modify by zhouhansheng  for 2015-10-29  start*/
                    xun_bind_handle_bind_req(e2e_msg);
    			break;
    		//watch bind xlk add
    		//OTA msg add
    		case SUB_ACTION_WATCH_OTA_MSG:
    			printf("[$$] SUB_ACTION_WATCH_OTA_MSG start" );
    			//bbwatch_e2e_action_watch_ota_msg(e2e_msg);
    			//xun_ota_parse_req_msg(e2e_msg);
    			break;
    		//OTA msg add end

            case SUB_ACTION_REQUEST_CURR_VERSION:
              //  xun_e2e_ota_req_curr_firmware_version_handler(e2e_msg);
                break;
            case SUB_ACTION_SAY_HELLO:
                {
                  //  XUN_TRACE_EX("#### SUB_ACTION_SAY_HELLO~~" );
                   // xun_reset_heartbeat_count();
                }
                break;

			 case SUB_ACTION_SMS_TCP_CONNECT:
			 	//XUN_TRACE_EX("jack: sms connect!!!" );
			//	xun_tcp_reconnect();
			 	break;

			 case SUB_ACTION_STEP_INFO_REQ:
			 	{
				//	BOOL need_req=xun_step_record_is_open();
				// 	XUN_TRACE_EX("jack: step info req,need_req=%d!!!",need_req );
				//	if(need_req)
				//	{
				//		xun_step_info_handle_e2e_req(e2e_msg);
				//	}
			 	}
			 	break;
/*Add by zhouhansheng for 2015-11-12*/
#if 0
            case SUB_ACTION_REQ_SPORT_INFO:
                {
                    if( bb_step_info_is_open()==VM_TRUE )
                    {
                        XUN_TRACE_EX("[sport]SUB_ACTION_REQ_SPORT_INFO~~" );
                        sport_info_report_to_service();
                    }
                    else
                    {
                        XUN_TRACE_EX("[sport]SUB_ACTION_REQ_SPORT_INFO~~step is close" );
                    }
                }
                break;
    		case SUB_ACTION_TEST_LOCATION:
        		{
        			TRACE_LOG_OUT("SUB_ACTION_TEST_LOCATION setting");
        			bb_on_test_location(e2e_msg);		
        		}
                break;
        	case SUB_ACTION_TEST_PING_TIME:
        	    #if defined(__LC_PING_TEST__)
                {
                    memset(pingTEID, 0, sizeof(pingTEID));
                    strcpy(pingTEID, PhoneEID);
	                pingSN = ApkSN;
	                XUN_TRACE_EX( "pingTest pingTEID=%s, pingSN=%d" , pingTEID, pingSN);
                }
                #endif	
        	    break;
#endif
	//XUN_CONTACT,add by mayanjun.20160407
	case SUB_ACTION_CONTACT_INFO:
	{
		//S32 ret = xun_contact_parser_json_data(PL, 1);
		//if(ret >= 0){
			//xun_sort_contact_list();
			//xun_contact_save_list();
	//	}
		break;
	}

#ifdef __XUN_DOWNLOAD_MANAGER__
	case SUB_ACTION_DL_ITEMS_INFO:
	{
		//xun_e2e_dl_parse_item_data(PL,TRUE);
		//xun_dm_download_list_item();
		break;
	}
#endif
	case SUB_ACTION_REQ_BALANCE_INQUIRY:
		{
		//	xun_balance_inquiry_rsp(xun_balance_inquiry(PL),tmp_oppo_info.sn);
		}

		break;

    case SUB_ACTION_FETCH_TODAY_BATLOG_MSG:
    {
       // xun_fetch_today_batlog_req_handler(e2e_msg);
        break;
    }
#ifdef __LC_EFNECE_FEATURE_SUPPORT__			   
		case SUB_ACTION_EFENCE_DATA:
			{
		               // bbwatch_e2e_action_eFence_data(e2e_msg);
			}
			break;
#endif	
#if defined(__XUN_LOC_POLICY_MGR__)
	// just for policy test-- start
	case SUB_ACTION_LOC_POLICY_CFG:
	{
		//xun_recv_loc_policy_cfg_e2e_msg( e2e_msg );
		break;
	}
/*	case SUB_ACTION_GET_TRACKING_LOCATION:
	{
		xun_recv_loc_policy_mode_switch_e2e_msg( e2e_msg );
		break;
	}*/
	case SUB_ACTION_LOC_POLICY_READBACK:
	{
		//xun_recv_loc_policy_readback_e2e_msg( e2e_msg );
		 break;
	}
	// just for policy test-- end
#endif 


             case SUB_ACTION_REQUEST_WIFI_SCAN:
             {
            //	 zdebug("get scan e2e_msg point %d sn %d",e2e_msg,cJSON_GetObjectItem((cJSON *)e2e_msg,KEY_NAME_SN)->valueint);
            // 	e2e_msg=cJSON_Duplicate(e2e_msg,1);
            // 	zdebug("get scan e2e_msg point %d sn %d",e2e_msg,cJSON_GetObjectItem((cJSON *)e2e_msg,KEY_NAME_SN)->valueint);
           //  	cmd_wlan_scan(bb_wlan_scan_handle_e2e_req,(void *)e2e_msg);

             }
             break;
			case SUB_ACTION_REQUEST_WIFI_CONNECT:
			{
		
				#if 0
				if(!cJSON_GetObjectItem(PL, "cmd_id"))
				{
					break;
				}
				e2e_msg=cJSON_Duplicate(e2e_msg,1);
				switch(cJSON_GetObjectItem(PL, "cmd_id")->valueint)
				{
					case 0:
						
						if(cJSON_GetObjectItem(PL, "ssid") && cJSON_GetObjectItem(PL, "wifipassword"))
						{
						//	cmd_wlan_connect(bb_wlan_connect_handle_e2e_req, e2e_msg, 
							cJSON_GetObjectItem(PL, "ssid")->valuestring, 
							cJSON_GetObjectItem(PL, "wifipassword")->valuestring);
						}
						else
						{
							cJSON_Delete(e2e_msg);
						}
						break;
					case 1:
						if (cJSON_GetObjectItem(PL, "prof_id"))
						{
						//	cmd_wlan_connect_by_profid(bb_wlan_connect_handle_e2e_req, e2e_msg,
							cJSON_GetObjectItem(PL, "prof_id")->valueint);
						}
						else
						{
							cJSON_Delete(e2e_msg);	
						}
						break;
						default:
						cJSON_Delete(e2e_msg);
						break;
				}
				break;
#endif
			}
			break;
			case SUB_ACTION_REQUEST_WIFI_STATUS:
				//bb_wlan_status_handle_e2e_req ( e2e_msg );
				break;
			case SUB_ACTION_PROGRESS_WIFI_SAVED_PROF:
				//bb_wlan_saved_prof_handle_e2e_req ( e2e_msg );
				break;
			case SUB_ACTION_REQUEST_WIFI_DISCONNECT:
				//e2e_msg=cJSON_Duplicate(e2e_msg,1);
				//cmd_wlan_disconnect();
				//bb_wlan_disconnect_handle_e2e_req( e2e_msg );
				break;
            default:
                break;
        }
    }
}




