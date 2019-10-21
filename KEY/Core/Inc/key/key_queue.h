#ifndef __KEY_QUEUE_H
#define __KEY_QUEUE_H

#include <stdint.h>

typedef enum
{
  KEY_QUEUE_EMPTY = 0,
  KEY_QUEUE_FULL  = 1,
  KEY_QUEUE_OK    = 2,
} KEY_Queue_Status;

#define KEY_QUEUE_SIZE  5

typedef struct
{
  uint16_t Front;
  uint16_t Rear;
  uint16_t Size;
  uint8_t Data[KEY_QUEUE_SIZE];
} KEY_QueueTypeDef;

extern KEY_QueueTypeDef KEY_Queue;

uint8_t KEY_Queue_Push(KEY_QueueTypeDef *Queue, uint8_t data);
uint8_t KEY_Queue_Pop(KEY_QueueTypeDef *Queue, uint8_t *data);

#endif /* __KEY_QUEUE_H */
