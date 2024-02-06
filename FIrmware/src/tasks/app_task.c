/**
 * ****************************(C) COPYRIGHT 2024 Transight Systems Pvt LTD****************************
 * @file       app_task.c
 * @brief      
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Jan-25-2024     Rajinas                         rajinasm.transight@gmail.com
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 * ****************************(C) COPYRIGHT 2024 Transight Systems Pvt LTD****************************
 */


#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include "ts_vlt_app_0022.h"
#include "app_task.h"

extern SApp0022Handle OApp0022Handle;

pthread_mutex_t app_evt_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  app_evnt_group = PTHREAD_COND_INITIALIZER;
uint32_t app_0022_event;

void app_task_func(void* pParameters){
    EApp0022Errors err_code;
    uint32_t event;
    while(1){
        pthread_mutex_lock(&app_evt_mtx);
        pthread_cond_wait(&app_evnt_group,&app_evt_mtx);
        event = app_0022_event;
        
        pthread_mutex_unlock(&app_evt_mtx);
   
        if((event & APP_0022_NRML_PKT_TMR)!=0){
            
            err_code = app_0022_event_hdlr(&OApp0022Handle,APP_0022_EVT_NRL_PKT_TMR);
            if(ERR_APP_0022_NO_ERROR != err_code){
                fprintf(stderr,"app_0022_evnt_hndlr err:%d\n\r",err_code);
            }
        }
        if((event & APP_0022_HLT_PKT_TMR)!=0){
            err_code = app_0022_event_hdlr(&OApp0022Handle,APP_0022_EVT_HLT_PKT_TMR);
            if(ERR_APP_0022_NO_ERROR != err_code){
                fprintf(stderr,"app_0022_evnt_hndlr err:%d\n\r",err_code);
            }
        }
    
    
    }
}
