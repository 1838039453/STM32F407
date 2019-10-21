#include "key/key_scan.h"
#include "key/key_process.h"
#include "main.h"

void Module_Test(void)
{
  KEY_Scan();
  KEY_Process();
  HAL_Delay(10);
}
