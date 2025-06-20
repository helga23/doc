#ifndef DEVICES_H_
#define DEVICES_H_

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define DEVICE_TIMEOUT 3000


typedef enum
{ 
	Devices_Status_Work = 1,
	Devices_Status_Fault,
}Devices_State_en;



typedef struct
{
	int16_t left_effect;
	int16_t	right_effect;
}effect_on_board_t;




// Function which all subscribers have to implement

///void Devices_addSb_CanHandler (void);




#endif 