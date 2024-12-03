#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void configureDWTForRunTimeStats(void);

uint32_t getTimerValue(void);

void main_cpp();

#ifdef __cplusplus
}
#endif
