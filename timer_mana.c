#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "timer_mana.h"

#define time_before(a, b) ((a).tv_sec < (b).tv_sec || ((a).tv_sec == (b).tv_sec && (a).tv_nsec < (b).tv_nsec))
#define FREE_SAFE(pt) do{if((pt)!=NULL){free(pt);(pt)=NULL;}}while(0)

int s_timer_quit = 0;
OS_TIMER_LIGHT_T *s_timer_list;
pthread_cond_t s_timer_list_cond;
pthread_mutex_t s_timer_list_mutex;

static void timer_list_add(OS_TIMER_LIGHT_T*  pt_timer) 
{
	OS_TIMER_LIGHT_T* tmp_list = NULL;
	if(pt_timer == NULL){
		printf("timer_list_add failed \n");
		return;
	}
	
	if(s_timer_list !=NULL)
		{
			if(time_before(pt_timer->abstime,tmp_list->abstime))
				{
					s_timer_list = pt_timer;				
			}else
				{
					for(tmp_list=tmp_list->next;tmp_list!=s_timer_list;tmp_list = tmp_list->next)
						if(time_before(pt_timer->abstime,tmp_list->abstime))
							break;
			}
			pt_timer->previous = tmp_list->previous;
			pt_timer->next = tmp_list;
			
			tmp_list->previous->next = pt_timer;
			tmp_list->previous = pt_timer;
	}
	else		
		{
			s_timer_list = pt_timer;
			pt_timer->next =pt_timer;
			pt_timer->previous = pt_timer;			
	}
	printf("timer_list_add success\n");
}


/*
  *如果是一次性的删除链表重新配置链表，是否内存
  *如果是重复的只需删除链表，重新配置链表即可
  */
static void timer_list_remove(OS_TIMER_LIGHT_T* pt_timer)
{	
	OS_TIMER_LIGHT_T *tmp_list = NULL;
	if(pt_timer == NULL)
		{
			printf("timer_list_add failed \n");
			return;			
	}
//(1)update list
	if(pt_timer->previous = pt_timer)
		{
			s_timer_list = NULL;
		}
	else
		{
			pt_timer->next->previous = pt_timer->previous;
			pt_timer->previous->next = pt_timer->next;
			if(s_timer_list == pt_timer)
				{
					s_timer_list = pt_timer->next;
			}
		}
	pt_timer->previous = pt_timer->next = NULL;

//(2)free memory 
	if(pt_timer->interval.tv_sec==0 && pt_timer->interval.tv_nsec==0)
		{
			if(pt_timer->pt_args!=NULL&&pt_timer->pt_args->ptimer_msg)
				{
					FREE_SAFE(pt_timer->pt_args->ptimer_msg);
					FREE_SAFE(pt_timer->pt_args);
					FREE_SAFE(pt_timer);					
			}
	}
	
}

int _timer_start(unsigned int ui4_delay, TIMER_FLAG_E e_flags, u_os_timer_cb_fct pf_callback, struct _TIMER_CBFCT_ARGS* pt_args)
{
	//printf("_timer_start start \n");
	OS_TIMER_LIGHT_T* pt_timer;
	struct timespec delay;
	if((pf_callback==NULL)||(ui4_delay==0)||((e_flags !=X_TIMER_FLAG_ONCE)&&(e_flags !=X_TIMER_FLAG_REPEAT)))
		{
			return -1;
	}
	delay.tv_sec = ui4_delay/1000;
	delay.tv_nsec = (ui4_delay%1000)*1000000;

	pt_timer = malloc(sizeof(OS_TIMER_LIGHT_T));
	memset(pt_timer,0x00,sizeof(OS_TIMER_LIGHT_T));

	
//	if(pt_timer->previous !=NULL)
//		{
			//timer_list_remove(pt_timer);
//	}

	VERIFY(clock_gettime(CLOCK_MONOTONIC, &pt_timer->abstime)==0);

	pt_timer->abstime.tv_sec += delay.tv_sec;
	pt_timer->abstime.tv_nsec += delay.tv_nsec;

	if(pt_timer->abstime.tv_nsec>1000000000)
		{
			pt_timer->abstime.tv_nsec -= 1000000000;
			pt_timer->abstime.tv_sec++; 
	}
	if(e_flags == X_TIMER_FLAG_ONCE)
		{
			pt_timer->interval.tv_sec = 0;
			pt_timer->interval.tv_nsec = 0;
	}else{
			pt_timer->interval.tv_sec = delay.tv_sec;
			pt_timer->interval.tv_nsec = delay.tv_nsec;	
	}

	pt_timer->pf_callback = pf_callback;
	pt_timer->pt_args = pt_args;
	
	VERIFY(pthread_mutex_lock(&s_timer_list_mutex)==0);
	timer_list_add(pt_timer);
	VERIFY(pthread_cond_signal(&s_timer_list_cond)==0);
	VERIFY(pthread_mutex_unlock(&s_timer_list_mutex)==0);
	return 0;
}


int u_timer_start(char* name, TIMER_TYPE_T *p_timer, void* p_msg, size_t msg_size,u_os_timer_cb_fct pf_callback)
{
//	printf("u_timer_start start \n");
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

int os_timer_init (void)
{
    pthread_condattr_t condattr;

    VERIFY(pthread_condattr_init(&condattr) == 0);
    VERIFY(pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC) == 0);
    VERIFY(pthread_cond_init(&s_timer_list_cond, &condattr) == 0);
    VERIFY(pthread_mutex_init(&s_timer_list_mutex, NULL) == 0);

    return 0;
}

void* TimerProc(void* arg)
{
	OS_TIMER_LIGHT_T* pt_timer;
	struct timespec abstime;
	int ret;

	//printf("*********TimerProc**********\n");
	struct _TIMER_CBFCT_ARGS* pargs = NULL;

	os_timer_init();
	
	VERIFY(pthread_mutex_lock(&s_timer_list_mutex)==0);

	while(!s_timer_quit)
		{
			while(s_timer_list == NULL)
				{
					//printf("*****************pthread_cond_wait start****************\n");
					ret = pthread_cond_wait(&s_timer_list_cond,&s_timer_list_mutex);
					//printf("*****************pthread_cond_wait end****************\n");
					VERIFY(ret == 0);
			}

			VERIFY(clock_gettime(CLOCK_MONOTONIC, &abstime)==0);

			while(s_timer_list && time_before(s_timer_list->abstime, abstime))
				{
					pt_timer = s_timer_list;	

					if(pt_timer->pf_callback)
						{
						pt_timer->pf_callback(pt_timer->pt_args);
					}
			//how to free pt_timer
			//timer_list_remove(pt_timer);

			if(pt_timer->interval.tv_sec == 0 && pt_timer->interval.tv_nsec ==0)
				{
//(1) 若果是重复的只删除链表，重新配置链表即可
//(2)一次性timer,删除链表重新配置链表，是否内存
					
							timer_list_remove(pt_timer);
			}else
			//if(pt_timer->interval.tv_sec !=0 && pt_timer->interval.tv_nsec !=0)
				{
					timer_list_remove(pt_timer);
					pt_timer->abstime.tv_sec += pt_timer->interval.tv_sec;
					pt_timer->abstime.tv_nsec += pt_timer->interval.tv_nsec;
					if(pt_timer->abstime.tv_nsec >= 1000000000)
						{
							pt_timer->abstime.tv_nsec -=1000000000;
							pt_timer->abstime.tv_sec++;
					}
					timer_list_add(pt_timer);
			}
				//VERIFY(pthread_mutex_unlock(&s_timer_list_mutex) == 0);
				}
				//VERIFY(pthread_mutex_lock(&s_timer_list_mutex) == 0);			
			if(s_timer_list !=NULL)
				{
					//printf("*****************pthread_cond_timedwait start****************\n");
					ret = pthread_cond_timedwait(&s_timer_list_cond, &s_timer_list_mutex, &s_timer_list->abstime);
					VERIFY(ret ==0 || ret == ETIMEDOUT) ;
			}
	}
    VERIFY(pthread_mutex_unlock(&s_timer_list_mutex) == 0);
    return NULL;	

}

