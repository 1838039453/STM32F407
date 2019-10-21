#ifndef __UART_RECV_H
#define __UART_RECV_H

#include <stdint.h>
#include "usart.h"

void UART_Debug_Init(void);
void UART_Debug_Process(void);
void Uart_Debug_IDLE(UART_HandleTypeDef *huart);

#endif /* _UART_RECV_H_ */
