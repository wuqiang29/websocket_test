#ifndef _TIMER_MANA_H
#define _TIMER_MANA_H
#include <assert.h>



#define CHECK_ASSERT(ret) do{int y=(int)ret;if(y!=0){assert(0);printf("CHECK_ASSERT in fun %s at line %d ret:%d\n",__FILE__,__LINE__,y);}}while(0)
#define ASSERT(x)		do{int y=(int)(x);assert(y);}while(0)
#define VERIFY(x)		ASSERT(x)
struct _TIMER_CBFCT_ARGS{
	//HANDLE_T h_app;
	char s_name[16];
	void *ptimer_msg;
	unsigned int  msg_size;
};
typedef void (*u_os_timer_cb_fct) (struct _TIMER_CBFCT_ARGS*  pv_tag);

typedef struct os_timer_light
{
    struct os_timer_light *previous;
    struct os_timer_light *next;
    struct timespec abstime;
    struct timespec interval;
    u_os_timer_cb_fct pf_callback;
    struct _TIMER_CBFCT_ARGS *pt_args; 	//@ pt_args will allocate in u_timer_start, and release when time_list_remove.
} OS_TIMER_LIGHT_T;

typedef enum
{
    X_TIMER_FLAG_ONCE   = 1,
    X_TIMER_FLAG_REPEAT
} TIMER_FLAG_E;

typedef struct
{
//	HANDLE_T h_timer;
	unsigned int ui4_delay;
	TIMER_FLAG_E e_flags;	
}TIMER_TYPE_T;


void* TimerProc(void* arg);
int u_timer_start(char* name, TIMER_TYPE_T *p_timer, void* p_msg, size_t msg_size,u_os_timer_cb_fct pf_callback);
int _timer_start(unsigned int ui4_delay, TIMER_FLAG_E e_flags, u_os_timer_cb_fct pf_callback, struct _TIMER_CBFCT_ARGS* pt_args);
#endif
