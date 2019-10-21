#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

typedef void (*TIMER_Callback)(void *Param);

typedef struct __TIMER_HandleTypedef
{
  struct __TIMER_HandleTypedef *Next;
  uint32_t Ticks;
  TIMER_Callback Handler;
  void *Param;
} TIMER_HandleTypedef;

void TIMER_Init(void);
void TIMER_Run(void);
void TIMER_Create(TIMER_HandleTypedef *Timer, TIMER_Callback handler, void *Param);
void TIMER_Start(TIMER_HandleTypedef *Timer, uint32_t Timeout);
void TIMER_Cancel(TIMER_HandleTypedef *Timer);


#define TIMER_TIMEOUT_EVT  0x01
uint8_t TIMER_Evt_Wait(void);
void TIMER_Evt_Clear(void);

#endif /* _TIMER_H_ */
