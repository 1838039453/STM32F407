#include "led/led.h"

void Module_Test(void)
{
  LED_Active(LED0_VALUE);
  LED_Negative(LED1_VALUE);
  HAL_Delay(1000);

  LED_Active(LED1_VALUE);
  LED_Negative(LED0_VALUE);
  HAL_Delay(1000);

  LED_Active(LED0_VALUE);
  LED_Active(LED1_VALUE);
  HAL_Delay(1000);

  LED_Toggle(LED0_VALUE);
  LED_Toggle(LED1_VALUE);
  HAL_Delay(1000);
}
