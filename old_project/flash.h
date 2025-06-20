#ifndef _FLASH_H_
#define _FLASH_H_

#include "MDR32FxQI_eeprom.h"


typedef enum
{
	 settings_DEFAULT = 1,
   settings_from_FLASH
}settings_status_en;

 settings_status_en init_settings();
 uint8_t read_to_eeprom();
 uint8_t write_to_eeprom();
 

 																										
 uint16_t calcCrc16(uint8_t *data, uint32_t size, uint16_t crc);
 
 #endif