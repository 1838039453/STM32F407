#include "led/led.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])
#endif

/* LED active config */
typedef enum
{
  LED_IS_ACTIVE_LOW  = 1,
  LED_IS_ACTIVE_HIGH = 2,
} LED_IS_ACTIVE;

#define LEDX_ACTIVE(GPIOx, GPIO_Pin, Tag) do{           \
  if(LED_IS_ACTIVE_LOW == Tag)                          \
  {                                                     \
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET); \
  }                                                     \
  else                                                  \
  {                                                     \
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);   \
  }                                                     \
} while(0);

#define LEDX_NEGATIVE(GPIOx, GPIO_Pin, Tag) do{         \
  if(LED_IS_ACTIVE_LOW == Tag)                          \
  {                                                     \
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);   \
  }                                                     \
  else                                                  \
  {                                                     \
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET); \
  }                                                     \
} while(0);

typedef struct
{
  GPIO_TypeDef *GPIOx;
  uint16_t Pin;
  uint8_t Tag;
} LED_PortTypeDef;

/* All LED List, add directly, max = 32 */
static LED_PortTypeDef LED_Items[] =
{
  {LED0_GPIO_Port, LED0_Pin, LED_IS_ACTIVE_LOW},
  {LED1_GPIO_Port, LED1_Pin, LED_IS_ACTIVE_LOW},
};

void LED_Active(uint32_t value)
{
  uint32_t i, mask = 1;

  for(i = 0; i < ARRAY_SIZE(LED_Items); ++i)
  {
    if(value & mask)
    {
      LEDX_ACTIVE(LED_Items[i].GPIOx, LED_Items[i].Pin, LED_Items[i].Tag);
    }

    mask <<= 1;
  }
}

void LED_Negative(uint32_t value)
{
  uint32_t i, mask = 1;

  for(i = 0; i < ARRAY_SIZE(LED_Items); ++i)
  {
    if(value & mask)
    {
      LEDX_NEGATIVE(LED_Items[i].GPIOx, LED_Items[i].Pin, LED_Items[i].Tag);
    }

    mask <<= 1;
  }
}

void LED_Toggle(uint32_t value)
{
  uint32_t i, mask = 1;

  for(i = 0; i < ARRAY_SIZE(LED_Items); ++i)
  {
    if(value & mask)
    {
      HAL_GPIO_TogglePin(LED_Items[i].GPIOx, LED_Items[i].Pin);
    }

    mask <<= 1;
  }
}
