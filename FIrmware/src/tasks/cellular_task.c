/**
 * ****************************(C) COPYRIGHT 2024 Transight Systems Pvt LTD****************************
 * @file       cellular_task.c
 * @brief      
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Jan-19-2024     Rajinas                         rajinasm.transight@gmail.com
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 * ****************************(C) COPYRIGHT 2024 Transight Systems Pvt LTD****************************
 */

  
  #include<stdio.h>
  #include<string.h>
  #include<pthread.h>
  #include "cellular_task.h"
  #include "ts_ipc_p1.h"


  pthread_mutex_t cell_evt_mtx = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t  cellular_evnt_group = PTHREAD_COND_INITIALIZER;
  ECell_Events celular_event;

  extern SIPC_P1_Handle OIPC_P1_Hdle;
  extern SIPC_P1_CallData OIPC_P1_CallData;

  void cellular_task_func(void *pParameters){
    ECell_Events event;
    int i;
    uint8_t response;
    while(1){
        	pthread_mutex_lock(&cell_evt_mtx);
    		
    		pthread_cond_wait(&cellular_evnt_group,&cell_evt_mtx);
            event = celular_event;
            pthread_mutex_unlock(&cell_evt_mtx);
    		switch(event){
                case CELLULAR_EVNT_INCOMING_CALL:
                   

                    printf("\r\n Incoming Call Evnt recieved ");
                    printf("\r\n CTX : ");
                    for(i=0;i<12;i++)
                    {
                        printf("%c", OIPC_P1_CallData.CTX[i]);
                    }
                    printf("\r\n Time : ");
                
                    printf("%ld", OIPC_P1_CallData.Call_Time);
                    

                    printf("\r\n entr call response :");
                    scanf("%d",&response);


                    ipc_p1_event_voice_call_response(&OIPC_P1_Hdle,response);
                    if(response==0){
                    printf("\r\n entr call response :");
                    scanf("%d",&response);

                    ipc_p1_event_voice_call_response(&OIPC_P1_Hdle,response);
                    
                    }
                    break;
                default:
                    break;

            }
    }

  }
