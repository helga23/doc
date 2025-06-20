#ifndef _SDO_H_
#define _SDO_H_

#include <stdint.h>

#define ID_SDO_MASTER 0x601
#define ID_SDO_SLAVE 0x581

#define CSS_READ_FALSH 4 
#define CSS_WRITE_FALSH 2
#define CSS_ALL_WRITE_FALSH 1

#define CSS_OK 6
#define CSS_ERROR 8

typedef struct
{
	uint8_t
		xx : 2,
		n : 2,
		css : 4;
	uint8_t index;
	uint16_t subindex;
	uint32_t data;
} sdo_msg_t;
typedef union
{
	uint32_t data[2];
	sdo_msg_t sdoMsg;
} data_SDO_un;



void sdo_init(void);
uint8_t SDO_MsgHandler(uint32_t MsgID, void *Data, int8_t DataSize);

#endif