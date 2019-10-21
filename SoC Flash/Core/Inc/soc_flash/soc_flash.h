#ifndef __SOC_FLASH_H
#define __SOC_FLASH_H

#include <stdint.h>

#define FLASH_ADDR_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0,  16 Kbytes */
#define FLASH_ADDR_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1,  16 Kbytes */
#define FLASH_ADDR_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2,  16 Kbytes */
#define FLASH_ADDR_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3,  16 Kbytes */
#define FLASH_ADDR_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4,  64 Kbytes */
#define FLASH_ADDR_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define FLASH_ADDR_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define FLASH_ADDR_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define FLASH_ADDR_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define FLASH_ADDR_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define FLASH_ADDR_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10,128 Kbytes */
#define FLASH_ADDR_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11,128 Kbytes */

void SoC_FLASH_Erase(uint32_t Address, uint8_t Count);
uint8_t SoC_FLASH_Write(uint32_t Address, uint8_t *Buffer, uint32_t Length);
uint32_t SOC_FLASH_Read(uint32_t Address, uint8_t *Buffer, uint32_t Length);

#endif /* __SOC_FLASH_H */
