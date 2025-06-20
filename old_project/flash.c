#include <string.h>

#include "flash.h"
#include "diag_application.h"

#define EEPROM_PAGE_SIZE             (4*1024)
#define MAIN_EEPAGE                  5

 uint8_t read_to_eeprom()
 {
 	uint32_t Data = 0;
	uint8_t ReadErrMM = 0;
	uint32_t Address = 0x08000000 + MAIN_EEPAGE * EEPROM_PAGE_SIZE;
  uint32_t  BankSelector = EEPROM_Main_Bank_Select;
	 
	config_un flash; 
	uint8_t sizeArrConf  =  diag_p_size_array_set() + 1; //32bit size
	 
	__disable_irq();
	
/////////////////*READ EEPROM*/////////////////////////////////////////
	for (int i = 0; i < sizeArrConf; i += 1)
	{
	 flash.data32[i] = EEPROM_ReadWord (Address + i*4, BankSelector);  			
	}
	
	diag_parameter_array_flash_set(flash.data32);
	
	uint16_t crc = 0;
	crc = calcCrc16(flash.data8, (sizeArrConf - 1)*4, crc);

	if (crc != flash.data32[sizeArrConf])
			ReadErrMM = 1;
	
////////////////////////////////////////////////////////////////////////
	 __enable_irq();

	return ReadErrMM;
 }

 uint8_t write_to_eeprom()
 {
	uint32_t Data = 0;
	uint8_t WriteErrMM = 0;
	uint32_t Address = 0x08000000 + MAIN_EEPAGE * EEPROM_PAGE_SIZE;
  uint32_t  BankSelector = EEPROM_Main_Bank_Select;
	
	config_un flash; 
	uint8_t sizeArrConf  =  diag_p_size_array_set() + 1; //32bit size 
	uint32_t buff [sizeArrConf];
	 
	diag_parameter_array_flash_get (buff);
	
  memcpy(flash.data8, buff, (sizeArrConf-1)*4);
	 
	__disable_irq();
		
/////////////////* Erase main memory page MAIN_EEPAGE */////////////////
    EEPROM_ErasePage (Address, BankSelector);

    /* Check main memory page MAIN_EEPAGE */
    Data = 0xFFFFFFFF;
    for (int i = 0; i < EEPROM_PAGE_SIZE; i += 4)
    {
        if (EEPROM_ReadWord (Address + i, BankSelector) != Data)
            WriteErrMM = 1;
    }	
		
/////////////////*calculation CRC16*////////////////////////////////////
		uint16_t crc = 0;
    crc = calcCrc16(flash.data8, (sizeArrConf-1)*4, crc);
		flash.data32[sizeArrConf-1] = crc;
		
//////////////////* write EEPROM *//////////////////////////////////////

		 for (int i = 0; i < sizeArrConf; i += 1)
    {
        Data = flash.data32[i]; 	
        EEPROM_ProgramWord (Address + i*4, BankSelector, Data);
    }

		
//////////////////* check EEPROM *//////////////////////////////////////
		 for (int i = 0; i < sizeArrConf; i += 1)
		{
			Data = flash.data32[i]; 	
			
			if (EEPROM_ReadWord (Address + i*4, BankSelector) != Data)
            WriteErrMM =  1;	
  	}
	
		 __enable_irq();
		
    return WriteErrMM;
 }
 
 
 uint16_t calcCrc16(uint8_t *data, uint32_t size, uint16_t crc)
{
    while (size--)
    {
        int i;
        uint8_t byte = *data++;
        for (i = 0; i < 8; ++i)
        {
            uint32_t osum = crc;
            crc <<= 1;
            if (byte & 0x80)
                crc |= 1;
            if (osum & 0x8000)
                crc ^= 0x1021;
            byte <<= 1;
        }
    }
    return crc;
}
