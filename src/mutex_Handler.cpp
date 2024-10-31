#include "mutex_Handler.h"


SemaphoreHandle_t Semaphore_Group_A = NULL;
SemaphoreHandle_t Semaphore_Group_B = NULL;
SemaphoreHandle_t Semaphore_Group_C = NULL;

void initializeSemaphores(){
    Semaphore_Group_A = xSemaphoreCreateMutex();
    Semaphore_Group_B = xSemaphoreCreateMutex();
    Semaphore_Group_C = xSemaphoreCreateMutex();

    if( Semaphore_Group_A == NULL )
    {
        while(1){

        }
    }
    if( Semaphore_Group_B == NULL )
    {
        while(1){

        }
    }
    if( Semaphore_Group_C == NULL )
    {
        while(1){

        }
    }
}

bool takeSemaphoreGroup(smphr_groups group){
    switch (group)
    {
    case GROUP_A:
        {
            if(xSemaphoreTake(Semaphore_Group_A, ( TickType_t ) 10) == pdTRUE){
                return true;
            }
        }
        break;
    case GROUP_B:
        {
            if(xSemaphoreTake(Semaphore_Group_B, ( TickType_t ) 10) == pdTRUE){
                return true;
            }
        }
        break;
    case GROUP_C:
        {
            if(xSemaphoreTake(Semaphore_Group_C, ( TickType_t ) 10) == pdTRUE){
                return true;
            }
        }
        break;
    default:
        break;
    }
    return false;
}

void releaseSemaphoreGroup(smphr_groups group){
    switch (group)
    {
    case GROUP_A:
        xSemaphoreGive(Semaphore_Group_A);
        break;
    case GROUP_B:
        xSemaphoreGive(Semaphore_Group_B);
        break;
    case GROUP_C:
        xSemaphoreGive(Semaphore_Group_C);
        break;
    default:
        break;
    }
}