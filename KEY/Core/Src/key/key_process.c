#include "key/key_process.h"
#include "key/key_scan.h"
#include "key/key_queue.h"
#include <stdint.h>
#include "led/led.h"

static void KEY0_Callback(void);
static void KEY1_Callback(void);
static void KEY2_Callback(void);
static void KEY_WKUP_Callback(void);

#define KEY0_CMD         1
#define KEY1_CMD         2
#define KEY2_CMD         3
#define KEY_WKUP_CMD     4

typedef struct
{
  uint8_t Cmd;
  void (* KEY_ProcessCallback)(void);
} KEY_HandleTypeDef;

static const KEY_HandleTypeDef KEY_Handle_Items[] =
{
  {KEY0_CMD, KEY0_Callback},
  {KEY1_CMD, KEY1_Callback},
  {KEY2_CMD, KEY2_Callback},
  {KEY_WKUP_CMD, KEY_WKUP_Callback},
  {0xFF, NULL},
};

static void KEY0_Callback(void)
{
  LED_Active(LED0_VALUE | LED1_VALUE);
}

static void KEY1_Callback(void)
{
  LED_Negative(LED0_VALUE | LED1_VALUE);
}

static void KEY2_Callback(void)
{
  LED_Negative(LED1_VALUE);
  LED_Toggle(LED0_VALUE);
}

static void KEY_WKUP_Callback(void)
{
  LED_Negative(LED0_VALUE);
  LED_Toggle(LED1_VALUE);
}

static void KEY_Process_Ext(uint8_t Cmd)
{
  const KEY_HandleTypeDef *Entry;

  for(Entry = KEY_Handle_Items; Entry->KEY_ProcessCallback; ++Entry)
  {
    if(Cmd == Entry->Cmd)
    {
      Entry->KEY_ProcessCallback();
      break;
    }
  }
}

void KEY_Process(void)
{
  uint8_t cmd;

  if(KEY_Queue_Pop(&KEY_Queue, &cmd) == KEY_QUEUE_OK)
  {
    KEY_Process_Ext(cmd);
  }
}
