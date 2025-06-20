#include <stdint.h>
#include <string.h>

#include "driver_power_modul.h"
#include "ixxat_c_lib/ixxat_can.h"

dcdc_msg_rx_323 pm_msg_rx_323;
dcdc_msg_tx_213 pm_msg_tx_213;
dcdc_msg_rx_313 pm_msg_rx_313;


void pm_init(power_module_t *power_module_t )
{
    pm_msg_rx_323.ID = ID_RX_CUSTUM_MSG_323;
    pm_pm_msg_rx_323.DLC = DLC_RX_CUSTUM_MSG_323;

    pm_msg_tx_213.ID = ID_RX_CUSTUM_MSG_313;
    pm_msg_tx_213.DLC = DLC_RX_CUSTUM_MSG_313;

    pm_msg_rx_313.ID = ID_TX_CUSTUM_MSG_213;
    pm_msg_rx_313.DLC = DLC_TX_CUSTUM_MSG_213;

    memset(power_module_t, 0, sizeof(power_module_t));
}

uint8_t pm_v_set(power_module_t *pm, uint16_t value)
{
    if (value <= 160)
    {
        pm->target_v = value;
        return 1;
    }
    else
        return 0;
}
uint8_t pm_i_set(power_module_t *pm, uint16_t value)
{
    if (value <= 1740)
    {
        pm->target_i = value;
        return 1;
    }
    else
        return 0;
}
uint8_t pm_cmd_set(power_module_t *pm, pm_commands_t cmd)
{
    if ((cmd == pm_start) || (cmd == pm_stop))
    {
        pm->cmd = cmd;
        return 1;
    }
    else
        return 0;
}

uint16_t pm_v_get(power_module_t *pm)
{
    return pm->actual_v;
}
uint16_t pm_i_get(power_module_t *pm)
{
    return pm->actual_i;
}
uint16_t pm_t_get(power_module_t *pm)
{
    return pm->actual_t;
}
pm_statuses_t pm_status_get(power_module_t *pm)
{
    return pm->actual_st;
}
pm_failt_code_union pm_failt_code_get(power_module_t *pm)
{
    return pm->failt_code;
}

void pm_proc(power_module_t *pm)
{

    if (++pm->msg_cnt >= pm_msg_num)
        pm->msg_cnt = 0;

    if (ms_timer_check(&pm->msg_last_stamp, msg_period[pm->msg_cnt]))
    {
        switch (pm->msg_cnt)
        {
        case pm_msg_1 /* constant-expression */:
            /* code */
            msg_1_send();
            break;

        case pm_msg_2:
            msg_2_send();
            break;

        default:
            break;
        }
    }
}

uint8_t pm_can_data_rx(Ixxat_CAN_Msg_t *pRxCan)
{
    data_16to8_union data;
    uint16_t data;

    switch (RxCan->ID)
    {
    case pm_msg_rx_323.ID:

        memcpy(pm_msg_rx_323.data, pRxCan->Data, pRxCan->DLC);

        data = (uint16_t)((uint8_t)(pm_msg_rx_323.data.voltage << 8) + (uint8_t)pm_msg_rx_323.data.voltage) * 10; // factot = 0.1 V -> factot = 0.01 V
        pm_v_get(&pm) = data;

        data = (int16_t)((uint8_t)(pm_msg_rx_323.data.current << 8) + (uint8_t)pm_msg_rx_323.data.current) + 3212.7 * 10; // (factot = 0.1 V) offset = 0 -> offset = -3212.7
        pm_i_get(&pm) = data;

        return 1;

    case pm_msg_rx_313.ID:
        memcpy(pm_msg_rx_313.data, pRxCan->Data, pRxCan->DLC);

        data = (int16_t)(pm_msg_rx_313.data.status);
        pm_status_get(&pm) = data;

        pm_failt_code_union buff;

        buff.code.Flt_Rnk = pm_msg_rx_313.data.Flt_Rnk;
        buff.code.HV_OVP = pm_msg_rx_313.data.HV_OVP;
        buff.code.HV_OCP = pm_msg_rx_313.data.HV_OCP;
        buff.code.LV_OVP = pm_msg_rx_313.data.LV_OVP;
        buff.code.LV_Short = pm_msg_rx_313.data.LV_Short;
        buff.code.OTP = pm_msg_rx_313.data.OTP;
        buff.code.LV_UVP = pm_msg_rx_313.data.LV_UVP;
        buff.code.TempSenseErr = pm_msg_rx_313.data.TempSenseErr;
        buff.code.CAN_TimeOut = pm_msg_rx_313.data.CAN_TimeOut;
        buff.code.RunFail = pm_msg_rx_313.data.RunFail;

        pm_failt_code_get(&pm) = buff.code_16;

        return 1;
        
    default:
        return 0;
    }
}

void msg_send(power_module_t *pm)
{
    uint8_t buf[8];
    dcdc_msg_rx_323_Data_t *pm_msg_rx_213.data = (dcdc_msg_rx_323_Data_t *)buf;

    if (pm->cmd == pm_start)
        pm_msg_rx_213.cmd = START_CUSTUM;

    if (pm->cmd == pm_stop)
        pm_msg_rx_213.cmd = STOP_CUSTUM;

    pm_msg_rx_213.voltage = pm->actual_v;
    pm_msg_rx_213.current = pm->actual_i;

    send_can_msg(msg_ids[pm_msg_1], buf, 8);
}
