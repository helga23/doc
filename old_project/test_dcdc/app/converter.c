
#include "dcdc.h" #include < string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "ixxat_c_lib/ixxat_can.h"

Value_DCDC_TypeDef value_DCDC;
Custom_DCDC_Msg_TypeDef custum_DCDC;
J1939_DCDC_TypeDef j1939_DCDC;

Ixxat_CAN_Msg_t RxCan;
uint8_t maskPGN = 0x3c;

void conversion_value_to_custom_DCDC(uint16_t *pValueMsg, Ixxat_CAN_Msg_t *pCustomMsg) // value ->In_custom,
{
    data_16to8_union data;

    if (pValueMsg == &value_DCDC.in_mode)
    {
        if (*pValueMsg == pm_start)
            pCustomMsg->Data[0] = START_CUSTUM;

        if (*pValueMsg == pm_stop)
            pCustomMsg->Data[0] = STOP_CUSTUM;
    }
    if (pValueMsg == &value_DCDC.in_target_v)
    {
        data.val_8[0] = (uint8_t)*pValueMsg;
        data.val_8[1] = (uint8_t)(*pValueMsg >> 8);
        memcpy(&pCustomMsg->Data[1], data.val_8, 2);
    }
    if (pValueMsg == &value_DCDC.in_limit_i)
    {
        data.val_8[0] = (uint8_t)*pValueMsg;
        data.val_8[1] = (uint8_t)(*pValueMsg >> 8);
        memcpy(&pCustomMsg->Data[3], data.val_8, 2);
    }
}


void conversion_value_to_j1939_DCDC(uint16_t *pValueMsg, Ixxat_CAN_Msg_t *pJ1939Msg) // value ->In_j1939
{
    data_16to8_union data;

    if (pJ1939Msg->ID & maskPGN == j1939_DCDC.tx_j1939MsgVc.ID & maskPGN)
    {
        if (pValueMsg == &value_DCDC.actual_v)
        {
            data.val_16 = *pValueMsg;
            memcpy(&pJ1939Msg->Data[0], data.val_8, 2);
        }

        if (pValueMsg == &value_DCDC.actual_i)
        {
            data.val_16 = *pValueMsg;
            memcpy(&pJ1939Msg->Data[2], data.val_8, 2);
        }
    }
    if (pJ1939Msg->ID & maskPGN == j1939_DCDC.tx_j1939MsgOc.ID & maskPGN)
    {
        switch (value_DCDC.actual_st)
        {
        case 1:
            pJ1939Msg->Data[0] = 1;
            break; // Init
        case 2:
            pJ1939Msg->Data[0] = 2;
            break; // Standby
        case 3:
            pJ1939Msg->Data[0] = 1;
            break; //  HV battery connected OK
        case 4:
            pJ1939Msg->Data[0] = 3;
            break; // Work
        case 5:
            pJ1939Msg->Data[0] = 5;
            break; // Power off
        case 6:
            pJ1939Msg->Data[0] = 13;
            break; // Shutdown
        }
    }
    if (pJ1939Msg->ID & maskPGN == j1939_DCDC.tx_j1939MsgDM1.ID & maskPGN)
    {
        data.val_16 = *pValueMsg;
        memcpy(&pJ1939Msg->Data[2], data.val_8, 2);
    }
}