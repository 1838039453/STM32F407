#include "timer/timer.h"
#include "tim.h"

static struct
{
  TIMER_HandleTypedef *Next;
} TIMER_Run_Head;

static volatile uint32_t timer_ex_ticks;
static uint32_t timer_ex_ticks_run;
static uint8_t timer_evt_val;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == htim2.Instance)
  {
    ++timer_ex_ticks;
    timer_evt_val = TIMER_TIMEOUT_EVT;
  }
}

void TIMER_Init(void)
{
  TIMER_Run_Head.Next = NULL;
}

void TIMER_Run(void)
{
  TIMER_HandleTypedef *timer;
  TIMER_HandleTypedef *timer_done;

  timer_done = NULL;

  while(timer_ex_ticks_run != timer_ex_ticks)
  {
    timer = TIMER_Run_Head.Next;

    if(timer)
    {
      if(timer->Ticks)
      {
        ++timer_ex_ticks_run;
        timer->Ticks--;
      }

      while(timer && (0 == timer->Ticks))
      {
        TIMER_Run_Head.Next = timer->Next;
        timer->Next = timer_done;
        timer_done = timer;
        timer = TIMER_Run_Head.Next;
      }
    }
    else
    {
      ++timer_ex_ticks_run;
    }
  }

  while(timer_done)
  {
    timer = timer_done;
    timer_done = timer->Next;
    timer->Next = NULL;
    timer->Handler(timer->Param);
  }
}

void TIMER_Create(TIMER_HandleTypedef *Timer, TIMER_Callback Handler, void *Param)
{
  Timer->Handler = Handler;
  Timer->Param = Param;
}

void TIMER_Start(TIMER_HandleTypedef *Timer, uint32_t Timeout)
{
  TIMER_HandleTypedef *p;
  uint32_t ticks = (Timeout + 9) / 10;

  TIMER_Cancel(Timer);

  p = (TIMER_HandleTypedef *)&TIMER_Run_Head;

  while(p->Next)
  {
    if(ticks <= p->Next->Ticks)
    {
      p->Next->Ticks -= ticks;
      break;
    }
    else
    {
      ticks -= p->Next->Ticks;  // ticks could 0
    }
    p = p->Next;
  }
  Timer->Ticks = ticks;
  Timer->Next = p->Next;
  p->Next = Timer;
}

void TIMER_Cancel(TIMER_HandleTypedef *Timer)
{
  TIMER_HandleTypedef *p;

  if(NULL == Timer)
  {
    return;
  }

  p = (TIMER_HandleTypedef *)&TIMER_Run_Head;

  while(p->Next)
  {
    if(p->Next == Timer)
    {
      if(Timer->Next)
      {
        Timer->Next->Ticks += Timer->Ticks;
      }
      p->Next = Timer->Next;
      Timer->Next = NULL;
      break;
    }
    p = p->Next;
  }
}

//====================================================================================
uint8_t TIMER_Evt_Wait(void)
{
  return timer_evt_val;
}

void TIMER_Evt_Clear(void)
{
  timer_evt_val = 0;
}



