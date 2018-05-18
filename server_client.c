#include <string.h>
#include <stdio.h>
#include <openssl/md5.h>
#include "cJSON.h"
#include "xun_commut.h"



extern char WatchSID[40];

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
    char *buffer4 = (char *)"78:11:DC:94:03:8374CECB85AE17BB85C56FFA91FE33F6E0";
    unsigned char md5_get[MD5_DIGEST_LENGTH+1]={0};
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx,buffer,length);

    MD5_Final(md5_get,&ctx);
    printHex(md5_get,MD5_DIGEST_LENGTH,md5);	
}

//MD5 output
char* String_Md5(char* pData)
{



}

char*  cJSON_login(void)
{
	cJSON *root=NULL, *fmt=NULL;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root,"Version","00030000");
	cJSON_AddNumberToObject(root, "SN", 142);
	cJSON_AddNumberToObject(root, "CID", 10211);
	cJSON_AddItemToObject(root, "PL", fmt=cJSON_CreateObject());
	cJSON_AddStringToObject(fmt,"Name", "78:11:DC:94:03:83");
//	cJSON_AddStringToObject(fmt,"Name", "865843024562586");	
//	cJSON_AddStringToObject(fmt, "Password", "5F64E333CEDB15AD7182D18FC070F8DB");
	cJSON_AddStringToObject(fmt, "Password", "5F64E333CEDB15AD7182D18FC070F8DB");
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
