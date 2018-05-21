#ifndef _WEBSOCKET_DEF_H_
#define _WEBSOCKET_DEF_H_


#include <libwebsockets.h>

#define WEBSOCKET_SERVER_DOMAIN			"sw502b-a02.imibaby.net"
//#define WEBSOCKET_SERVER_DOMAIN			"test.imibaby.net"
#define WEBSOCKET_SERVER_PORT				(8255)
#define WEBSOCKET_SERVER_PATH				"/svc/pipe"
#define WEBSOCKET_SSL_CA_PATH			"imibaby.net.pem"



typedef struct lws_context_creation_info	LWS_CONT_CRT_INFO;
typedef struct lws_client_connect_info 		LWS_CLI_CON_INFO;
//typedef struct lws_context					LWS_CONTEXT;
//typedef struct lws							LWS_WSI;
typedef struct lws_protocols				LWS_PROTOCOLS;


typedef struct {
	int connected;
	int destroy;
	int writeable;		
}LOCAL_WEBS_CONN;

/*
typedef struct {
	
struct lws_context_creation_info	cont_crt_info;
struct lws_client_connect_info 	cli_con_info;
//struct lws_context					context;
//struct lws_context	context;
//struct lws						wsi;
int							ws_init;      // 1 represent ok, o nok.
	
}OBJ_WEBSOCKET;

*/
typedef struct {
	
LWS_CONT_CRT_INFO	cont_crt_info;
LWS_CLI_CON_INFO		cli_con_info;
//LWS_CONTEXT 				context;
//struct lws_context	context;
struct lws*							wsi;
int							ws_init;      // 1 represent ok, o nok.
	
}OBJ_WEBSOCKET;

/*
typedef struct {
	
struct lws_context_creation_info	cont_crt_info;
struct lws_client_connect_info 	cli_con_info;
struct lws_context					context;
//struct lws_context	context;
struct lws						wsi;
int							ws_init;      // 1 represent ok, o nok.
	
}OBJ_WEBSOCKET;

*/

#define SERVER_MESG_LEN	(64*1024) 
/*
typedef struct  server_mesg{
	char* mesg_data;
	struct  server_mesg *next;
}SERVER_MESG;
*/
typedef struct mesg_list{	
	char* json;
	int data_len; 
	struct mesg_list *next;	
}MESG_LIST;

#endif
