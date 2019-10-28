#include <stdio.h>
#include <string.h>
#include "uart/uart_debug_parse.h"
#include "usart.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)  (sizeof(a) / sizeof(a[0]))
#endif

static void UART_Debug_Ifconfig(uint8_t Argc, const char *Argv[]);
static void UART_Debug_Ls(uint8_t Argc, const char *Argv[]);
static void UART_Debug_Ping(uint8_t Argc, const char *Argv[]);

static void UART_Debug_Ifconfig_Ack(void);
static void UART_Debug_Ls_Ack(void);
static void UART_Debug_Ping_Ack(void);

typedef struct
{
  const char *Cmd;
  void (* UART_Debug_ParseCallback)(uint8_t Argc, const char *Argv[]);
  void (* UART_Debug_AckCallback)(void);
} UART_Debug_EntryTypeDef;

static const UART_Debug_EntryTypeDef UART_Debug_Items[] =
{
  {"ifconfig", UART_Debug_Ifconfig, UART_Debug_Ifconfig_Ack},
  {"ls", UART_Debug_Ls, UART_Debug_Ls_Ack},
  {"ping", UART_Debug_Ping, UART_Debug_Ping_Ack},
  {NULL, NULL, (void *) 0},
};

// ifconfig
static void UART_Debug_Ifconfig(uint8_t Argc, const char *Argv[])
{
  if(Argc == 1)
  {
    printf("UART_Debug_Ifconfig Get...\r\n");
  }
  else
  {
    printf("UART_Debug_Ifconfig Set...\r\n");
  }
}

// ls
static void UART_Debug_Ls(uint8_t Argc, const char *Argv[])
{
  if(Argc == 1)
  {
    printf("UART_Debug_Ls Get...\r\n");
  }
  else
  {
    printf("UART_Debug_Ls Set...\r\n");
  }
}

static void UART_Debug_Ping(uint8_t Argc, const char *Argv[])
{
  if(Argc == 1)
  {
    printf("UART_Debug_Ping Get...\r\n");
  }
  else
  {
    printf("UART_Debug_Ping Set...\r\n");
  }
}

static void UART_Debug_Ifconfig_Ack(void)
{

}

static void UART_Debug_Ls_Ack(void)
{

}

static void UART_Debug_Ping_Ack(void)
{

}

#define UART_DEBUG_SEPARATE  " "
static uint8_t UART_Debug_Read_Frame(uint8_t *Buffer, const char *argv[])
{
  uint8_t argc = 0;
  char *token;

  token = strtok((char *)Buffer, UART_DEBUG_SEPARATE);
  while(token)
  {
    if(strcmp(token, UART_DEBUG_SEPARATE) == 0)
    {
      token = strtok(NULL, UART_DEBUG_SEPARATE);
      continue;
    }

    argv[argc++] = token;
    token = strtok(NULL, UART_DEBUG_SEPARATE);
  }

  return argc;
}

void UART_Debug_Parse(uint8_t *Buffer, uint16_t Length)
{
  uint8_t argc;
  const char *argv[UART_RECV_BUFFER_MAX];
  const char *cmd;

  if(Length == 1)
  {
    return;
  }

  argc = UART_Debug_Read_Frame(Buffer, argv);

  cmd = argv[0];

  uint16_t i;
  for(i = 0; i < ARRAY_SIZE(UART_Debug_Items); ++i)
  {
    if((strlen(UART_Debug_Items[i].Cmd) == strlen((const char *) cmd))
        && (memcmp(UART_Debug_Items[i].Cmd, cmd, strlen(cmd)) == 0))
    {
      UART_Debug_Items[i].UART_Debug_ParseCallback(argc, argv);
      UART_Debug_Items[i].UART_Debug_AckCallback();
      break;
    }
  }

  if(i == ARRAY_SIZE(UART_Debug_Items))
  {
    printf("cmd is not found...\r\n");
  }
}
