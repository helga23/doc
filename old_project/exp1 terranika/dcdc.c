#include "dcdc.h"
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include "ixxat_c_lib/ixxat_can.h"

Value_DCDC_TypeDef value_DCDC;
Custom_DCDC_Msg_TypeDef custum_DCDC;
J1939_DCDC_TypeDef j1939_DCDC;

Ixxat_CAN_Msg_t RxCan;
uint8_t maskPGN = 0x3c;

void init_DCDC_Msg()
{
  custum_DCDC.rx_customMsg323.ID = ID_RX_CUSTUM_MSG_323;
  custum_DCDC.rx_customMsg323.DLC = DLC_RX_CUSTUM_MSG_323;

  custum_DCDC.rx_customMsg313.ID = ID_RX_CUSTUM_MSG_313;
  custum_DCDC.rx_customMsg313.DLC = DLC_RX_CUSTUM_MSG_313;

  custum_DCDC.tx_customMsg213.ID = ID_TX_CUSTUM_MSG_213;
  custum_DCDC.tx_customMsg213.DLC = DLC_TX_CUSTUM_MSG_213;

  j1939_DCDC.tx_j1939MsgVc.ID = ID_TX_J1939_MSG_VC;
  j1939_DCDC.tx_j1939MsgVc.DLC = DLC_TX_J1939_MSG_VC;

  j1939_DCDC.tx_j1939MsgOc.ID = ID_TX_J1939_MSG_OC;
  j1939_DCDC.tx_j1939MsgOc.DLC = DLC_TX_J1939_MSG_OC;

  j1939_DCDC.tx_j1939MsgDM1.ID = ID_TX_J1939_MSG_DM1;
  j1939_DCDC.tx_j1939MsgDM1.DLC = DLC_TX_J1939_MSG_DM1;
}

void conversion_value_to_custom_DCDC(uint16_t *pValueMsg, Ixxat_CAN_Msg_t *pCustomMsg) // value ->In_custom,
{
  data_16to8_union data;

  if (pValueMsg == &value_DCDC.in_mode)
  {
    if (*pValueMsg == START)
      pCustomMsg->Data[0] = START_CUSTUM;

    if (*pValueMsg == STOP)
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

void conversion_custom_to_value_DCDC(Ixxat_CAN_Msg_t *pCustomMsg, uint16_t *pValueMsg) //  Out_custom -> value
{
  uint16_t data;

  if (pCustomMsg->ID  == custum_DCDC.rx_customMsg323.ID)
  {
    if (pValueMsg == &value_DCDC.out_actual_v)
    {
      data = (uint16_t)((pCustomMsg->Data[0] << 8) + pCustomMsg->Data[1]) * 10; // factot = 0.1 V -> factot = 0.01 V
    }

    if (pValueMsg == &value_DCDC.out_actual_i)
    {
      data = (int16_t)((pCustomMsg->Data[2] << 8) + pCustomMsg->Data[3]) + 3212.7 * 10; // (factot = 0.1 V) offset = 0 -> offset = -3212.7
    }
  }
  if (pCustomMsg->ID == custum_DCDC.rx_customMsg313.ID)
  {
    if (pValueMsg == &value_DCDC.out_actual_mode)
    {
      data = pCustomMsg->Data[0];
      data = (int16_t)((pCustomMsg->Data[0] & 0b11110000) >> 4);
    }

    if (pValueMsg == &value_DCDC.out_failt_code.code_16)
    {
      data = (int16_t)((((pCustomMsg->Data[0] & 0b00001100) >> 2) << 8) +
                       ((pCustomMsg->Data[1] & 0b00111111) << 3) +
                       ((pCustomMsg->Data[2] & 0b00001000) >> 2) +
                       ((pCustomMsg->Data[2] & 0b00000011)));
    }
  }

  *pValueMsg = data;
}

void conversion_value_to_j1939_DCDC(uint16_t *pValueMsg, Ixxat_CAN_Msg_t *pJ1939Msg) // value ->In_j1939
{
  data_16to8_union data;

  if (pJ1939Msg->ID & maskPGN == j1939_DCDC.tx_j1939MsgVc.ID & maskPGN)
  {
    if (pValueMsg == &value_DCDC.out_actual_v)
    {
      data.val_16 = *pValueMsg;
      memcpy(&pJ1939Msg->Data[0], data.val_8, 2);
    }

    if (pValueMsg == &value_DCDC.out_actual_i)
    {
      data.val_16 = *pValueMsg;
      memcpy(&pJ1939Msg->Data[2], data.val_8, 2);
    }
  }
  if (pJ1939Msg->ID & maskPGN == j1939_DCDC.tx_j1939MsgOc.ID & maskPGN)
  {
    switch (value_DCDC.out_actual_mode)
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

void CAN_handler_Receive_DCDC() // Rx -> custom -> value
{
  if (custum_DCDC.rx_customMsg323.ID == RxCan.ID)
   // if (custum_DCDC.rx_customMsg323.DLC == RxCan.DLC)
    {
      memcpy(custum_DCDC.rx_customMsg323.Data, RxCan.Data, RxCan.DLC);

      conversion_custom_to_value_DCDC(&custum_DCDC.rx_customMsg323, &value_DCDC.out_actual_v);
      conversion_custom_to_value_DCDC(&custum_DCDC.rx_customMsg323, &value_DCDC.out_actual_i);
    }

  if (custum_DCDC.rx_customMsg313.ID  == RxCan.ID)
//    if (custum_DCDC.rx_customMsg313.DLC == RxCan.DLC)
    {
      memcpy(custum_DCDC.rx_customMsg313.Data, RxCan.Data, RxCan.DLC);

      conversion_custom_to_value_DCDC(&custum_DCDC.rx_customMsg313, &value_DCDC.out_actual_mode);
      conversion_custom_to_value_DCDC(&custum_DCDC.rx_customMsg313, &value_DCDC.out_failt_code.code_16);
    }
}

void set_App_DCDC(uint16_t *pValueMsg, uint16_t data)
{
  *pValueMsg = data;
}

uint16_t get_App_DCDC(uint16_t *pValueMsg)
{
  if (pValueMsg == &value_DCDC.out_actual_i)
    return value_DCDC.out_actual_i;

  if (pValueMsg == &value_DCDC.out_actual_v)
    return value_DCDC.out_actual_v;

  if (pValueMsg == &value_DCDC.out_actual_mode)
    return value_DCDC.out_actual_mode;

  if (pValueMsg == &value_DCDC.out_failt_code.code_16)
    return value_DCDC.out_failt_code.code_16;
}
