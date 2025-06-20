#ifndef _IXXAT_CAN_H_
#define _IXXAT_CAN_H_

#include <stdint.h>

typedef struct {
    uint32_t ID;
    uint8_t Data[8];
    uint8_t DLC;
} Ixxat_CAN_Msg_t;

int8_t ixxat_can_init(uint8_t channel_num);
uint8_t ixxat_can_msg_send(uint8_t channel_num, Ixxat_CAN_Msg_t *msg);
uint8_t ixxat_can_msg_receive(uint8_t channel_num, Ixxat_CAN_Msg_t *msg);
int8_t ixxat_can_deinit(uint8_t channel_num);

#endif
