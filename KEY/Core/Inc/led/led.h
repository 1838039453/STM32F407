#ifndef __LED_H
#define __LED_H

#include "main.h"

typedef enum
{
  LED0_VALUE = (1 << 0),
  LED1_VALUE = (1 << 1),
} LED_Index;

void LED_Active(uint32_t value);
void LED_Negative(uint32_t value);
void LED_Toggle(uint32_t value);

#endif /* __LED_H */
