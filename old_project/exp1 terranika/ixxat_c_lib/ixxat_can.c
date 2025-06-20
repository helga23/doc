#include <string.h>
#include "inc/vcinpl.h"

#include "ixxat_can.h"

HANDLE devList;
HANDLE canCtrl[2];
HANDLE dev;
HANDLE chan[2];
CANCAPABILITIES2 sCanCaps; 

static int8_t _can_ctrl_init(uint8_t channel_num);
static int8_t _can_channel_init(uint8_t channel_num);

int8_t ixxat_can_init(uint8_t channel_num)
{

    // devices list
    VCIDEVICEINFO devInfo;
    vciEnumDeviceOpen((PHANDLE)&devList);
    vciEnumDeviceNext(devList, (PVCIDEVICEINFO)&devInfo);

    // device
    if (vciDeviceOpen((REFVCIID)&devInfo.VciObjectId, (PHANDLE)&dev) != 0)
        return -1;

    if (_can_ctrl_init(channel_num) != 0)
        return -1;

    if (_can_channel_init(channel_num) != 0)
        return -1;

    return 0;
}

uint8_t ixxat_can_msg_send(uint8_t channel_num, Ixxat_CAN_Msg_t *msg)
{
    HRESULT hResult;
    CANMSG sCanMsg;
    uint16_t ext_flag = (msg->ID > 0x7ff) ? 1 : 0;

    if (sCanCaps.dwFeatures & CAN_FEATURE_DELAYEDTX)
    {                        // controller supports delayed transmission
        sCanMsg.dwTime = 10; // delay by 10 milliseconds
    }
    else
    {
        sCanMsg.dwTime = 0;
    }

    sCanMsg.dwMsgId = msg->ID;
    sCanMsg.uMsgInfo.Bytes.bType = CAN_MSGTYPE_DATA;
    sCanMsg.uMsgInfo.Bytes.bFlags = 0;
    sCanMsg.uMsgInfo.Bytes.bFlags2 = 0;

    sCanMsg.uMsgInfo.Bytes.bAccept = 0;
    sCanMsg.uMsgInfo.Bytes.bReserved = 0;

    sCanMsg.uMsgInfo.Bytes.bFlags = CAN_MAKE_MSGFLAGS(8, 0, 0, 0, ext_flag);
   
    memcpy(sCanMsg.abData, msg->Data, 8);

  hResult = canChannelSendMessage(chan[channel_num],
              100, // wait for at least 100 milliseconds before the
                   // function returns even if no message has been
                   // written to the transmit FIFO
              &sCanMsg);

  return( hResult );

/*
    HRESULT resTx = canChannelSendMessage(chan[channel_num], INFINITE, &sCanMsg);

    if (resTx == VCI_E_TXQUEUE_FULL)
    {
        LONG result = canChannelWaitTxEvent(chan, INFINITE);

        return (result != 0);
    }

    if (resTx == VCI_E_TIMEOUT)
    {
        LONG result = canChannelWaitTxEvent(chan, INFINITE);

        return (result != 0);
    }
  

    //    if (resTx != 0)
    //     {
    //          LONG result = canChannelWaitTxEvent(chan, INFINITE);

    //         return (result != 0);
    //     }
    if (resTx == VCI_OK)
    {
        return 1;
    }
  */

    /*
        LONG res = canChannelPostMessage(chan[channel_num], &sCanMsg);
        if (res == VCI_E_TXQUEUE_FULL)
        {
            LONG result = canChannelPostMessage(chan, &sCanMsg);
            return (result != 0);
        }
        if (res != 0)
        {
            LONG result = canChannelPostMessage(chan, &sCanMsg);
        }

        if (res == VCI_OK)
        {
            return 1;
        }
        */
}

uint8_t ixxat_can_msg_receive(uint8_t channel_num, Ixxat_CAN_Msg_t *msg)
{
    CANMSG sCanMsg;
    LONG res = canChannelPeekMessage(chan[channel_num], &sCanMsg);
    if (res == VCI_E_RXQUEUE_EMPTY)
        return 0;

    msg->ID = sCanMsg.dwMsgId;
    msg->DLC = sCanMsg.uMsgInfo.Bits.dlc;
    memcpy(msg->Data, sCanMsg.abData, msg->DLC);
    return 1;
}

int8_t ixxat_can_deinit(uint8_t channel_num)
{
    canChannelActivate(chan[channel_num], 0);
    canControlClose(canCtrl[channel_num]);
    vciDeviceClose(dev);
    vciEnumDeviceClose(devList);
}

static int8_t _can_ctrl_init(uint8_t channel_num)
{
    //  CAN Control Init
    if (canControlOpen(dev, channel_num, &canCtrl[channel_num]) != 0)
    {
        return -1;
    }

    if (canControlInitialize(canCtrl[channel_num], CAN_BTMODE_RAW, CAN_BT0_250KB, CAN_BT1_250KB) != 0)
    {
        return -1;
    }

    canControlStart(canCtrl[channel_num], 1);
    return 0;
}

static int8_t _can_channel_init(uint8_t channel_num)
{
    // CAN Channel Init
    if (canChannelOpen(dev, channel_num, 0, &chan[channel_num]) != 0)
        return -1;

    canChannelInitialize(chan[channel_num], 20, 10, 10, 5);
    canChannelActivate(chan[channel_num], 1);

    return 0;
}
