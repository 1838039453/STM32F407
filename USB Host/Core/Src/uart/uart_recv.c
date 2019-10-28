#include "uart/uart_recv.h"
#include "uart/uart_debug_parse.h"
#include "usart.h"
#include <stdio.h>

static UART_HandleTypeDef *UART_Debug_Handle = &huart1;
static UART_FrameTypeDef UART_Debug_Frame;

void UART_Debug_Init(void)
{
  __HAL_UART_ENABLE_IT(UART_Debug_Handle, UART_IT_IDLE);
  HAL_UART_Receive_DMA(UART_Debug_Handle, UART_Debug_Frame.Buffer, UART_RECV_BUFFER_MAX);
}

void UART_Debug_Process(void)
{
  if(UART_Debug_Frame.Ret)
  {
    UART_Debug_Frame.Ret = 0;
    UART_Debug_Parse(UART_Debug_Frame.Buffer, UART_Debug_Frame.Length);
  }
}

void Uart_Debug_IDLE(UART_HandleTypeDef *huart)
{
  uint32_t temp;

  if(huart == UART_Debug_Handle)
  {
    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
    {
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      HAL_UART_DMAStop(huart);
      temp = huart->hdmarx->Instance->NDTR;
      UART_Debug_Frame.Length = UART_RECV_BUFFER_MAX - temp;
      UART_Debug_Frame.Ret = 1;
      HAL_UART_Receive_DMA(huart, UART_Debug_Frame.Buffer, UART_RECV_BUFFER_MAX);
      UART_Debug_Frame.Buffer[UART_Debug_Frame.Length] = '\0';
    }
  }
}
