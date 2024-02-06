/**
 * ****************************(C) COPYRIGHT 2024 Transight Systems Pvt LTD****************************
 * @file       cellular_task.h
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


#ifndef __CELLULAR_TASK_
#define __CELLULAR_TASK_


typedef enum{
    CELLULAR_EVNT_INCOMING_CALL,
    CELLULAR_EVNT_MSG_RECIEVED,
}ECell_Events;

void cellular_task_func(void *pParameters);



#endif