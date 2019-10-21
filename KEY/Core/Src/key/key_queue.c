#include "key/key_queue.h"

KEY_QueueTypeDef KEY_Queue = {0};

uint8_t KEY_Queue_Push(KEY_QueueTypeDef *Queue, uint8_t Data)
{
  if(((Queue->Rear % KEY_QUEUE_SIZE) == Queue->Front) && (Queue->Size == KEY_QUEUE_SIZE))
  {
    return KEY_QUEUE_FULL;
  }

  Queue->Data[Queue->Rear] = Data;
  Queue->Rear = (Queue->Rear + 1) % KEY_QUEUE_SIZE;
  Queue->Size++;

  return KEY_QUEUE_OK;
}

uint8_t KEY_Queue_Pop(KEY_QueueTypeDef *Queue, uint8_t *Data)
{
  if((Queue->Front == Queue->Rear) && (Queue->Size == 0))
  {
    return KEY_QUEUE_EMPTY;
  }

  *Data = Queue->Data[Queue->Front];
  Queue->Front = (Queue->Front + 1) % KEY_QUEUE_SIZE;
  Queue->Size--;

  return KEY_QUEUE_OK;
}
