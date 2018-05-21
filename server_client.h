#ifndef __SERVER_CLIENT_H__
#define __SERVER_CLIENT_H__
#include "cJSON.h"
char* cJSON_login(void);
char* cJSON_logout(void);
char* cJSON_little_ping(void);
void xun_e2e_phone_to_watch( cJSON* e2e_msg );



#endif
