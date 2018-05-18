#include <string.h>
#include <stdio.h>
#include "cJSON.h"

extern char WatchSID[40];
char*  cJSON_login(void)
{
	cJSON *root=NULL, *fmt=NULL;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root,"Version","00030000");
	cJSON_AddNumberToObject(root, "SN", 142);
	cJSON_AddNumberToObject(root, "CID", 10211);
	cJSON_AddItemToObject(root, "PL", fmt=cJSON_CreateObject());
	cJSON_AddStringToObject(fmt,"Name", "865843024562586");
	cJSON_AddStringToObject(fmt, "Password", "5F64E333CEDB15AD7182D18FC070F8DB");
	cJSON_AddNumberToObject(fmt, "Type", 200);
	cJSON_AddStringToObject(fmt, "machSerialNo", "562586");
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
