#include "timer/timer.h"
#include <main.h>

TIMER_HandleTypedef htimer;

static void TIMER_Timout_CanllBack(void *Param)
{
  Param = Param;

  /* add your code */

  TIMER_Start(&htimer, 1000);
}

void Module_Test(void)
{
  TIMER_Create(&htimer, TIMER_Timout_CanllBack, NULL);
  TIMER_Start(&htimer, 1000);
}
