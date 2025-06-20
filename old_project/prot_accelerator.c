#include "application_data.h"

#define GAS_CHANNEL_CHECK_MS 500
#define GAS_CHANNEL_CALCULATION_MS 50



extern AppHandle_t *locHandle;

accelerator_t *locGas;

void gas_init_msg(accelerator_t *gas)
{
	locGas = gas;
	gas->conf.max_offset0_CH0_mv =  gas->conf.maxAccel_CH0_mv - gas->conf.minAccel_CH0_mv;
	gas->conf.max_offset0_CH1_mv =  gas->conf.maxAccel_CH1_mv - gas->conf.minAccel_CH1_mv;
}


uint8_t gas_proc(accelerator_t *gas)
{
	if (msTimer_Check(&gas->LastMsgSend_Stamp, GAS_CHANNEL_CALCULATION_MS)) 
	{
		gas->accel_CH0_mv = board_ain_mv_get(num_ain_aps0) - gas->conf.minAccel_CH0_mv;
		gas->accel_CH0_pr = 100 * gas->accel_CH0_mv / (float)gas->conf.max_offset0_CH0_mv;
	}
	
	if (msTimer_Check(&gas->LastMsgSend_Stamp, GAS_CHANNEL_CHECK_MS)) 
	{
		gas->accel_CH1_mv = board_ain_mv_get(num_ain_aps1) - gas->conf.minAccel_CH1_mv;
		gas->accel_CH1_pr = 100 * gas->accel_CH1_mv / (float)gas->conf.max_offset0_CH1_mv;	
			
		uint16_t res = abs(gas->accel_CH0_pr - gas->accel_CH1_pr);
		
		if (res > 10) 
			gas->state = 0; //Devices_Status_Fault; 
		else
			gas->state = 1; //Devices_Status_Work;
	}
	
	return 0;
}





uint16_t gas_pr_get(accelerator_t *gp)
{ 
	return gp->accel_CH0_pr ; 
}







//float gas_PI_percent_get(gas_pedal_t *gp)
//{
//    return gp->PI_percent;
//}

//uint16_t gas_speed_get(gas_pedal_t *gp)
//{
//    return gp->speed;
//}

//void gas_integral_accelerator(gas_pedal_t *gp) // Timer
//{
//    gp->speed += gp->accel * gp->dT;
//    if (gp->accel == 0)
//        gp->speed = 0;
//}


//void Timer_Handler(void)
//{
//    vehicle_PI_linar_speed(locHandle);
//    gas_pedal_integral_accelerator(&locHandle->gas_pedal);
//}