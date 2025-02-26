#include "../../inc/Helpers/mutex_Handler.h"

SemaphoreHandle_t Semaphore_Group_A = NULL;
SemaphoreHandle_t Semaphore_Group_B = NULL;
SemaphoreHandle_t Semaphore_Group_C = NULL;
SemaphoreHandle_t Semaphore_EPS     = NULL;
SemaphoreHandle_t Mutex_SMC     = NULL;


StaticSemaphore_t Semaphore_Group_A_buffer;
StaticSemaphore_t Semaphore_Group_B_buffer;
StaticSemaphore_t Semaphore_Group_C_buffer;
StaticSemaphore_t Semaphore_EPS_buffer;
StaticSemaphore_t Mutex_SMC_buffer;


void initializeSemaphores() {
    Semaphore_Group_A = xSemaphoreCreateMutexStatic(&Semaphore_Group_A_buffer);
    Semaphore_Group_B = xSemaphoreCreateMutexStatic(&Semaphore_Group_B_buffer);
    Semaphore_Group_C = xSemaphoreCreateMutexStatic(&Semaphore_Group_C_buffer);
    Semaphore_EPS     = xSemaphoreCreateBinaryStatic(&Semaphore_EPS_buffer);
    Mutex_SMC  = xSemaphoreCreateMutexStatic(&Mutex_SMC_buffer);


    configASSERT(Semaphore_Group_A);
    configASSERT(Semaphore_Group_B);
    configASSERT(Semaphore_Group_C);
    configASSERT(Semaphore_EPS);
    configASSERT(Mutex_SMC);

}

/**
 * @brief Function that handles the acquisition of Semaphores for the different available Groups.
 * !important : Do not use this function from an ISR
 * 
 * @param group Semaphore GROUP A,B or C 
 * @return true 
 * @return false 
 */
bool takeSemaphoreGroup(smphr_groups group) {
    switch (group) {
        case GROUP_A: {
            if (xSemaphoreTake(Semaphore_Group_A, (TickType_t) 10) == pdTRUE) {
                return true;
            }
        } break;
        case GROUP_B: {
            if (xSemaphoreTake(Semaphore_Group_B, (TickType_t) 10) == pdTRUE) {
                return true;
            }
        } break;
        case GROUP_C: {
            if (xSemaphoreTake(Semaphore_Group_C, (TickType_t) 10) == pdTRUE) {
                return true;
            }
        } break;
        default:
            break;
    }
    return false;
}

void releaseSemaphoreGroup(smphr_groups group) {
    switch (group) {
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

bool takeSemaphoreEPS_ISR() {
    return xSemaphoreTake(Semaphore_EPS, (TickType_t) 100) == pdTRUE;
}

void releaseSemaphoreEPS_ISR() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(Semaphore_EPS, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

bool takeMutexSMC() {
    return xSemaphoreTake(Mutex_SMC, (TickType_t) portMAX_DELAY) == pdTRUE;
}

void releaseMutexSMC() {
    xSemaphoreGive(Mutex_SMC);
}
