#include "key/key_scan.h"
#include "key/key_queue.h"
#include "led/led.h"
#include "main.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])
#endif

/* config key active electric level */
typedef enum
{
  KEY_IS_ACTIVE_INIT,
  KEY_IS_ACTIVE_LOW,
  KEY_IS_ACTIVE_HIGH,
} KEY_IS_ACTIVE;

#define KEY_READ(GPIOx, Pin)  HAL_GPIO_ReadPin(GPIOx, Pin)
static uint8_t KEY_IS_Pressed(uint8_t Index);

/* key state */
typedef enum
{
  KEY_STATE_INIT,
  KEY_STATE_UP,
  KEY_STATE_DOWN,
  KEY_STATE_LONG,
  KEY_STATE_AUTO,
} key_state_t;

#define KEY_FILTER_TIME 2   // 20ms
#define KEY_LONG_TIME   100 // 1S judge long press
#define KEY_REPEAT_TIME 100 // 1S repeat press

typedef struct
{
  GPIO_TypeDef *GPIOx;
  uint16_t Pin;
  uint8_t Tag;

  uint8_t State;
  uint8_t Last;
  uint8_t Count;
  uint16_t LongTime;
  uint16_t LongCount;
  uint8_t RepeatSpeed;
  uint8_t RepeatCount;
  uint8_t (* Press_Callback)(uint8_t Index);
} KEY_PortTypeDef;

/*  All KEY List, add directly, only modify front three items */
static KEY_PortTypeDef KEY_Port_Items[] =
{
  {KEY0_GPIO_Port, KEY0_Pin, KEY_IS_ACTIVE_LOW, KEY_STATE_INIT, KEY_STATE_INIT, KEY_FILTER_TIME, KEY_LONG_TIME, 0, KEY_REPEAT_TIME, 0, KEY_IS_Pressed},
  {KEY1_GPIO_Port, KEY1_Pin, KEY_IS_ACTIVE_LOW, KEY_STATE_INIT, KEY_STATE_INIT, KEY_FILTER_TIME, KEY_LONG_TIME, 0, KEY_REPEAT_TIME, 0, KEY_IS_Pressed},
  {KEY2_GPIO_Port, KEY2_Pin, KEY_IS_ACTIVE_LOW, KEY_STATE_INIT, KEY_STATE_INIT, KEY_FILTER_TIME, KEY_LONG_TIME, 0, KEY_REPEAT_TIME, 0, KEY_IS_Pressed},
  {WK_UP_GPIO_Port, WK_UP_Pin, KEY_IS_ACTIVE_HIGH, KEY_STATE_INIT, KEY_STATE_INIT, KEY_FILTER_TIME, KEY_LONG_TIME, 0, KEY_REPEAT_TIME, 0, KEY_IS_Pressed},
};

static uint8_t KEY_IS_Pressed(uint8_t Index)
{
  if(KEY_IS_ACTIVE_LOW == KEY_Port_Items[Index].Tag)
  {
    if(KEY_READ(KEY_Port_Items[Index].GPIOx, KEY_Port_Items[Index].Pin) == 0)
    {
      return 1;
    }
  }
  else if(KEY_IS_ACTIVE_HIGH == KEY_Port_Items[Index].Tag)
  {
    if(KEY_READ(KEY_Port_Items[Index].GPIOx, KEY_Port_Items[Index].Pin) == 1)
    {
      return 1;
    }
  }

  return 0;
}

static void KEY_Scan_Ext(KEY_PortTypeDef *Entry, uint8_t Index)
{
  switch(Entry->State)
  {
  case KEY_STATE_INIT:
  case KEY_STATE_UP:
  {
    Entry->State = KEY_STATE_DOWN;
    break;
  }

  case KEY_STATE_DOWN:
  {
    if(Entry->LongTime > 0)
    {
      if(Entry->LongCount < Entry->LongTime)
      {
        if(++Entry->LongCount >= Entry->LongTime)
        {
          Entry->State = KEY_STATE_LONG;
          KEY_Queue_Push(&KEY_Queue, (Index + 1));
        }
      }
    }

    break;
  }

  case KEY_STATE_LONG:
  {
    if(Entry->RepeatSpeed > 0)
    {
      if(++Entry->RepeatCount >= Entry->RepeatSpeed)
      {
        Entry->RepeatCount = 0;
        KEY_Queue_Push(&KEY_Queue, (Index + 1));
      }
    }

    break;
  }
  }

  Entry->Last = Entry->State;
}

void KEY_Scan(void)
{
  uint8_t i;

  for(i = 0; i < ARRAY_SIZE(KEY_Port_Items); ++i)
  {
    KEY_PortTypeDef *Entry = &KEY_Port_Items[i];

    if(Entry->Press_Callback(i))
    {
      if(Entry->Count < KEY_FILTER_TIME)
      {
        ++Entry->Count;
      }
      else
      {
        KEY_Scan_Ext(Entry, i);
      }
    }
    else
    {
      if(Entry->Count > KEY_FILTER_TIME)
      {
        Entry->Count = KEY_FILTER_TIME;
      }
      else if(Entry->Count > 0)
      {
        --Entry->Count;
      }
      else
      {
        if(KEY_STATE_DOWN == Entry->Last)
        {
          KEY_Queue_Push(&KEY_Queue, (i + 1));
        }
        Entry->Last = KEY_STATE_UP;
        Entry->State = KEY_STATE_UP;
      }

      Entry->LongCount = 0;
      Entry->RepeatCount = 0;
    }
  }
}
