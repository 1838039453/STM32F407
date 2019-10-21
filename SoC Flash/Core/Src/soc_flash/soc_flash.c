#include "soc_flash/soc_flash.h"
#include "main.h"
#include <string.h>

static uint32_t SOC_FLASH_Sector_Number(uint32_t Address)
{
  uint32_t sector = 0;

  if((Address < FLASH_ADDR_SECTOR_1) && (Address >= FLASH_ADDR_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;
  }
  else if((Address < FLASH_ADDR_SECTOR_2) && (Address >= FLASH_ADDR_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;
  }
  else if((Address < FLASH_ADDR_SECTOR_3) && (Address >= FLASH_ADDR_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;
  }
  else if((Address < FLASH_ADDR_SECTOR_4) && (Address >= FLASH_ADDR_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;
  }
  else if((Address < FLASH_ADDR_SECTOR_5) && (Address >= FLASH_ADDR_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;
  }
  else if((Address < FLASH_ADDR_SECTOR_6) && (Address >= FLASH_ADDR_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;
  }
  else if((Address < FLASH_ADDR_SECTOR_7) && (Address >= FLASH_ADDR_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;
  }
  else if((Address < FLASH_ADDR_SECTOR_8) && (Address >= FLASH_ADDR_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;
  }
  else if((Address < FLASH_ADDR_SECTOR_9) && (Address >= FLASH_ADDR_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;
  }
  else if((Address < FLASH_ADDR_SECTOR_10) && (Address >= FLASH_ADDR_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;
  }
  else if((Address < FLASH_ADDR_SECTOR_11) && (Address >= FLASH_ADDR_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;
  }
  else if(Address >= FLASH_ADDR_SECTOR_11)
  {
    sector = FLASH_SECTOR_11;
  }

  return sector;
}

void SOC_FLASH_Erase(uint32_t Address, uint8_t Count)
{
  FLASH_EraseInitTypeDef erase;
  uint32_t error;

  HAL_FLASH_Unlock();

  erase.TypeErase = FLASH_TYPEERASE_SECTORS;
  erase.Sector = SOC_FLASH_Sector_Number(Address);
  erase.NbSectors = Count;
  erase.VoltageRange = FLASH_VOLTAGE_RANGE_3; // VCC=2.7~3.6V

  __HAL_FLASH_DATA_CACHE_DISABLE();

  HAL_FLASHEx_Erase(&erase, &error);

  FLASH_WaitForLastOperation(50000);

  __HAL_FLASH_DATA_CACHE_ENABLE();
  HAL_FLASH_Lock();
}

uint8_t SOC_FLASH_Write(uint32_t Address, uint8_t *Buffer, uint32_t Length)
{
  uint32_t i;
  uint16_t data = 0;

  if(Length == 0)
  {
    return 0;
  }

  HAL_FLASH_Unlock();

  __HAL_FLASH_DATA_CACHE_DISABLE();

  for(i = 0; i < Length; i += 2)
  {
    data = (*(Buffer + i + 1) << 8) + (*(Buffer + i));
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t)(Address + i), data);
  }

  __HAL_FLASH_DATA_CACHE_ENABLE();
  HAL_FLASH_Lock();

  return 0;
}

uint32_t SOC_FLASH_Read(uint32_t Address, uint8_t *Buffer, uint32_t Length)
{
  memcpy(Buffer, (void *)Address, Length);

  return Length;
}
