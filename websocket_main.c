/*
websocket function

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "cJSON.h"
#include "mesg_com.h"
#include "websocket_def.h"
#include "xun_action.h"
#include "server_client.h"
#include "timer_mana.h"
#include "xun_commut.h"


char WatchSID[40]={0};
OBJ_WEBSOCKET g_webs_para;

LOCAL_WEBS_CONN  loc_state_para;
MESG_LIST* recieve_data=NULL;
MESG_LIST*      send_data = NULL;

//pthread_cond_t   login_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t  send_data_mutex;
pthread_mutex_t  recieve_data_mutex;

pthread_mutex_t mutex_Send = PTHREAD_MUTEX_INITIALIZER;/*³õÊ¼»¯»¥³âËø*/  
pthread_cond_t cond_Send = PTHREAD_COND_INITIALIZER;/*³õÊ¼»¯Ìõ¼þ±äÁ¿*/  

//explain the recieve message from websocket server
void cJSON_login_rsp(char *json)
{
	cJSON *json_PL, *json_data, *gid_PL;
  	 int rc,cid;

	json_data = cJSON_Parse(json);
	
	//cJSON_GetObjectItem(json_data, "CID")->valueint;
	//rc = cJSON_GetObjectItem(json_data, "RC")->valueint;

	cid = cJSON_GetObjectItem(json_data,"CID")->valueint;
	

	//if(rc<0){
		//printf("message return rc = %d, something error!\n",rc);
		//return;
	//}
	
	switch(cid){
		case 70012:
		break;
		
		case 10212:
		rc = cJSON_GetObjectItem(json_data, "RC")->valueint;

		if(rc<0){
			printf("message return rc = %d, something error!\n",rc);
			return;
		}
		strcpy(WatchSID, cJSON_GetObjectItem(json_data, "SID")->valuestring);
	
		break;
		
	      	case CID_E2E_DOWN:
                    xun_e2e_phone_to_watch(json_data);
              break;
		
		default:

		break;
	}
	//memset(WatchSID,0,sizeof(WatchSID));

	//json_PL = cJSON_GetObjectItem(json_data, "PL");	
}


int WebSocket_Com_Data(char* in)
{

	int ret = 0;
	MESG_LIST* list_p=NULL;


	list_p = recieve_data;
	
	if(list_p == NULL)
		{
			list_p = (MESG_LIST*)malloc(sizeof(MESG_LIST));		
			memset(list_p ,0x00, sizeof(MESG_LIST));
			list_p->json = in;
			list_p->next = NULL;
			recieve_data = list_p;
			//printf("list_data###%s\n",list_p->json);
			return 0;
	}else {

			do{
				list_p = list_p->next;
				
			}while(list_p);

			list_p = (MESG_LIST*)malloc(sizeof(MESG_LIST));		
			memset(list_p ,0x00, sizeof(MESG_LIST));
			list_p->json = in;
			list_p->next = NULL;
			return 0;			
	}	
}


static int WebSocket_write_back(struct lws* wsi, char* str, UINT str_size)
{
	if (str == NULL || wsi ==NULL)
		{
			printf("parameters is invalid\n");
			return -1;
	}

	INT n;
	UINT len;
	char* out= NULL;

	if(str_size < 1)
		len = strlen(str);
	else
		len = str_size;

	out = (char*)malloc(sizeof(char)*(LWS_SEND_BUFFER_PRE_PADDING + len + LWS_SEND_BUFFER_POST_PADDING));
	// set up buffer
	memcpy(out + LWS_SEND_BUFFER_PRE_PADDING, str, len);

	if(str[0] =='\0' && str[1]=='\0')
		{
			n = lws_write(wsi, out + LWS_SEND_BUFFER_PRE_PADDING, len, LWS_WRITE_PING);		
			free(out);
			return 2;

	}else{

			n = lws_write(wsi, out + LWS_SEND_BUFFER_PRE_PADDING, len, LWS_WRITE_TEXT);

	}
	
	printf("[WebSoc_write_back] %s  lws_write len %d\n",str,n);
	
	free(out);

	return n;
	
}

void WebSocket_Send_Data_addlist(void* data_com)
{
	MESG_LIST* send_list = NULL;
	pthread_mutex_lock(&send_data_mutex);	
	send_list = send_data;
	if(!send_list)
		{
			send_data = (MESG_LIST*)data_com;
			//printf("##send_data##%s\n",send_data->json);	
			pthread_mutex_unlock(&send_data_mutex);	
			return;
	}
	do
	{
		send_list = send_list->next;
	}while(send_list);
	send_list = (MESG_LIST*)data_com;
	pthread_mutex_unlock(&send_data_mutex);	
	return;
}


int WebSocket_Send_All(void)
{
	
return 0;

}

/*
* return code, 0 success, -1 state error, -2 data miss
*
*/
int WebSocket_Data_Send(void)
{
	int ret = 0 ,len=0;
	 int send_len=0;
	MESG_LIST *send_list, *send_p1;

	//when operare the list ,please mute first
	pthread_mutex_lock(&send_data_mutex);
	send_list = send_data;
	
	if(send_list == NULL)
		{
			printf("send_data is null \n");
			return 0;
	}
	
	//lws_callback_on_writable(g_webs_para.wsi);
	//sleep(1);

	if(!loc_state_para.writeable)
		{
			printf("websocket state is not writeabled\n");
			return -1;
	}	
	while(send_list)
		{
			//send_len = strlen(send_list->json);
			send_len = send_list->data_len;
			len = WebSocket_write_back(g_webs_para.wsi,send_list->json,send_len);

			send_p1 = send_list;
			send_list = send_list->next;
			
			free(send_p1);
			send_p1 = NULL;	

			if(len != send_len)
				{
					printf("WebSocket_write_back failed ,some data miss\n");
					return -2;
			}			
	}
	send_data = NULL;
	pthread_mutex_unlock(&send_data_mutex);
	
	return 0;
}

static int WebSocket_service_callback(struct lws* wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{

    switch (reason) {

        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("[Main Service] Connect with server success.\n");
            loc_state_para.connected = 1;
            lws_callback_on_writable(g_webs_para.wsi);
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            printf("[Main Service] Connect with server error.\n");
			loc_state_para.destroy = 1;
			loc_state_para.connected = 0;
            break;

        case LWS_CALLBACK_CLOSED:
            printf("[Main Service] LWS_CALLBACK_CLOSED\n");
			loc_state_para.destroy = 1;
			loc_state_para.connected = 0;
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            printf("[Main Service] Client recvived:%s\n", (char *)in);

		//how to deal with  the coming data?
		//first put the date into a list
		//WebSocket_Com_Data(in);
		cJSON_login_rsp((char*)in);
			
            break;

        case LWS_CALLBACK_CLIENT_WRITEABLE :
            printf("[Main Service] On writeable is called. send byebye message\n");
			pthread_mutex_lock(&mutex_Send);
			loc_state_para.writeable = 1;
			pthread_cond_signal(&cond_Send);
			pthread_mutex_unlock(&mutex_Send);			
			
			//when get called ,check list, then if not null ,push the data;			
/*
			char *send_mesg=NULL;
			int ret;
			send_mesg = (char*)malloc(MES_LEN);
			memset(send_mesg,0,MES_LEN);
			//send_mesg=cJSON_logout();
			//cJSON_logout(send_mesg);
			cJSON_session_ping(send_mesg);
						
			printf("****%s****\n",send_mesg);
			ret = websocket_write_back(wsi, send_mesg, 0);
			free(send_mesg);
			if(ret < 0) {
				printf("login message send error ,funciton %s line %d ret =[%d]\n",__func__,__LINE__,ret);
				return 0;
			}			
			*/
            //websocket_write_back(wsi, "Byebye! See you later", -1);
            //writeable_flag = 1;
            break;
		case LWS_CALLBACK_CLIENT_RECEIVE_PONG:

			 printf("[Main Service] Client recieve pong:%s len = %d\n",(char*)in, (int)len);
			 break;

        default:
            break;
    }

    return 0;
}

int WebSocket_Msg_Bat_Get(void)
{
	return 0;
}

static const struct lws_protocols protocols[] = {
	{
		"dumb-increment-protocol",	
		WebSocket_service_callback,
		0,
		10240,            //å¦‚æžœå¤ªå°æŽ¥å—ç©ºé—´å¯èƒ½ä¸å¤Ÿç”?
	},
	{ NULL, NULL, 0, 0 } /* end */
};



void WebSocket_Context_Del(struct lws_context* context)
{
	if(context == NULL)
		{
			printf("please check input parameters ,context is null \n");
			return ;
	}
	printf("WebSocket_Context_Del\n");
	lws_context_destroy(context);
}


static int WebSocket_Service(INT argc)
{

	int ret; 

	if(argc<0)
		{
			printf("please check input parameters ,context is null \n");
			return -1;			
	}
	while(!loc_state_para.destroy) {
		lws_service(g_webs_para.cli_con_info.context, argc);	
		//sleep(1);
		//lws_callback_on_writable(g_webs_para.wsi);
	}
	printf("WebSocket_Service exit!!!\n");
	return 0;
}


static int WebSocket_Connect_Via_Info(LWS_CLI_CON_INFO* Via_Info)
{
	INT ret =0;
	if(Via_Info == NULL)
		{
			printf("Via_Info is NULL, please check input parameters\n");
			return -1;
	}

	memset(&loc_state_para,0x00,sizeof(LOCAL_WEBS_CONN));	

	g_webs_para.wsi= lws_client_connect_via_info(Via_Info);

	if(g_webs_para.wsi  == NULL)
		{
		printf("WebSocket_Connect_Via_Info failed, please check \n");
		return -1;		
	}
	printf("wsi create success !!!\n");
	return 0;
}

void WebSocket_Little_Ping(int argc)
{
	printf("WebSocket_Little_Ping start\n");
	char ping_packet[2] = {0};
	int ret = 0;
	MESG_LIST *data = NULL;
	
	data = (MESG_LIST*) malloc(sizeof(MESG_LIST));
	memset(data, 0x00, sizeof(MESG_LIST));
	data->data_len = 2;
	data->json = ping_packet;

	WebSocket_Send_Data_addlist((void*)data);
	ret = WebSocket_Data_Send();
	if(ret == 0)
		{
			return;
	}
	printf("WebSocket_Little_Ping send failed\n");
}
/*
typedef void (*u_os_timer_cb_fct) (struct _TIMER_CBFCT_ARGS*  pv_tag);

int u_timer_start(char* name, TIMER_TYPE_T *p_timer, void* p_msg, size_t msg_size,u_os_timer_cb_fct pf_callback)
{
	struct _TIMER_CBFCT_ARGS *pargs = malloc(sizeof(struct _TIMER_CBFCT_ARGS));
	if(pargs == NULL) return -1;
	strcpy(pargs->s_name,name);

	if(p_msg !=NULL)
		{
			pargs->ptimer_msg = malloc(msg_size);
			if(pargs->ptimer_msg ==NULL) return -1;
			memset(pargs->ptimer_msg ,0x00,msg_size);
			pargs->msg_size = msg_size;
			memcpy(pargs->ptimer_msg, p_msg, msg_size);
	}else{
			pargs->ptimer_msg = NULL;
			pargs->msg_size = 0;
	}
	return _timer_start(p_timer->ui4_delay, p_timer->e_flags, pf_callback, pargs);
}

*/
void Timer_Little_Ping_Call()
{
	//printf("Timer_Little_Ping_Call start \n");
	TIMER_TYPE_T p_timer ;
	char little_data[2]={0};
	memset(&p_timer,0x00,sizeof(TIMER_TYPE_T));
	p_timer.ui4_delay =240000;
	p_timer.e_flags = X_TIMER_FLAG_REPEAT;

	
	u_timer_start("little_ping", &p_timer, little_data,  2,(u_os_timer_cb_fct)WebSocket_Little_Ping);
}
void timer(int sig)
{
	if(SIGALRM == sig)
		{
			//printf("timer\n");
			WebSocket_Little_Ping(sig);
			alarm(3);
	}
	return;
}

INT WebSocket_Ping(void* argv)
{
	signal(SIGALRM, timer);
	alarm(3);
	return 0;
}


#define WEBSOCKET_WORK   (3)
INT WebSocket_Work(void* argv)
{

	INT ret = 0, times = 0, i;
	for (i =0; i<WEBSOCKET_WORK;i++)
		{
			ret = WebSocket_Connect_Via_Info(&g_webs_para.cli_con_info);
			if(ret != 0 )
				{
					printf("WebSocket_Connect_Via_Info failed,please retry\n");
					times++;
					usleep(100);
					continue;
				}
			break;
	}
	if(times == 3)
		{
			printf("WebSocket_Connect_Via_Info connect failed ,please check\n");
			return -1;
	}

	//service loop
	WebSocket_Service(50);

	//service destroy
	WebSocket_Context_Del(g_webs_para.cli_con_info.context);
	return 0;
}


int WebSocket_DeInit(INT arg)
{
	g_webs_para.ws_init=0;
	return 0;
}

int WebSocket_Init()
{
	int ret;
	struct lws_client_connect_info*  context = NULL;
	XUN_TASK_ID websocket_work, timer_work;
	
	memset(&g_webs_para,0x00,sizeof(OBJ_WEBSOCKET));
	g_webs_para.cont_crt_info.port = CONTEXT_PORT_NO_LISTEN;
	g_webs_para.cont_crt_info.iface = NULL;
	g_webs_para.cont_crt_info.protocols = protocols;
	g_webs_para.cont_crt_info.ssl_cert_filepath = NULL;
	g_webs_para.cont_crt_info.ssl_private_key_filepath = NULL;
	g_webs_para.cont_crt_info.options |=1<<12;
	g_webs_para.cont_crt_info.ssl_ca_filepath = WEBSOCKET_SSL_CA_PATH;	//è¿™é‡Œå¯ä»¥ç”¨å®å®šä¹‰
	//g_webs_para.cont_crt_info.ssl_ca_filepath = NULL;

	while(1)
		{
		context = lws_create_context(&g_webs_para.cont_crt_info);
		if(context==NULL)
		{
			printf("context create failed...,please retry!!!\n");
			usleep(100);
			continue;
		}
		printf("context create success.\n");
		break;
	}

	//memcpy(&g_webs_para.cont_crt_info,context, sizeof(LWS_context));
	g_webs_para.cli_con_info.context = context;
	g_webs_para.cli_con_info.port = WEBSOCKET_SERVER_PORT;
	g_webs_para.cli_con_info.address = WEBSOCKET_SERVER_DOMAIN;
	g_webs_para.cli_con_info.path = WEBSOCKET_SERVER_PATH;
	g_webs_para.cli_con_info.ssl_connection = 2;
	g_webs_para.cli_con_info.host = WEBSOCKET_SERVER_DOMAIN;
	g_webs_para.cli_con_info.origin = NULL;
	g_webs_para.cli_con_info.ietf_version_or_minus_one = -1;
	g_webs_para.cli_con_info.protocol = protocols[0].name;
	pthread_mutex_init(&send_data_mutex, NULL);
	g_webs_para.ws_init = 1;
	printf("%s success...\n",__FUNCTION__);

	//make sure wlan up
	//ifconfig wlan0 |grep "wlan0" >/tmp/wlan_up_test
	websocket_work = Xun_Spawn_Task("websocket_con", XUN_TASK_NORMAL_PRI - 10, XUN_DEFAULT_STACK_SIZE, (XUN_TASK_ENTRY)WebSocket_Work, 0, NULL);	
	//send the first message login

	
//	APP_MSG_PRI_NORMAL
	timer_work = Xun_Spawn_Task("timer_work", XUN_TASK_NORMAL_PRI - 10, XUN_DEFAULT_STACK_SIZE, (XUN_TASK_ENTRY)TimerProc, 0, NULL);
	
	MESG_LIST* tmp=NULL;
	tmp = (MESG_LIST*) malloc(sizeof(MESG_LIST));
	memset(tmp,0x00,sizeof(MESG_LIST));
	tmp->json = cJSON_login();
	tmp->data_len = strlen(tmp->json);
	tmp->next = NULL;

	
	WebSocket_Send_Data_addlist((void*)tmp);
	//printf("%s **********************success...\n",__FUNCTION__);
	//make sure socket have been connected ,and writeabled 
	pthread_mutex_lock(&mutex_Send);
	pthread_cond_wait(&cond_Send,&mutex_Send );
	pthread_mutex_unlock(&mutex_Send);
	WebSocket_Data_Send();
	//WebSocket_Ping(NULL);
	Timer_Little_Ping_Call();
	return 0;
}


int WebSocket_Handle_Msg(void* data, XUN_MSG_TYPE type, XUN_MSG_PRI pri, XUN_APP_ID srcAppl)
{
	pthread_t thread_a;

	int ret;

	switch(type)
		{
			//example  
			case WEBSOCK_MSG_BAT_GET:
				//WebSoc_Msg_Bat_Get();
				break;

			//...
			case LOGOUT:
				printf("recieve pUerData [%s]\n",(char*)data);
					{
						char buffer[32] = "logout response";
						int send_len = strlen(buffer);
						int recode = 666;
						//Xun_AppSendMsg(XUN_APPID_TEST,  LOGIN, APP_MSG_PRI_NORMAL, buffer, send_len);
						Xun_ResponseSyncMsg(XUN_APPID_TEST, data, type, &recode, 4);
				}

				break;


			default:

			break;
				

	}

	return 0;

}



int main(int argc ,char* argv[])
{
	Xun_App_Init(argc, argv, XUN_APPID_WEBSOCKET, "websocket");
	
	Xun_App_Start((XUN_APP_INIT_PTR)WebSocket_Init, 
		NULL, 
		(XUN_APP_MSG_HANDLER_PTR)WebSocket_Handle_Msg,
		(XUN_APP_DE_INIT_PTR)WebSocket_DeInit);
	return 0;
	
}
