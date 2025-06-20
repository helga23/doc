#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ixxat_c_lib/ixxat_can.h"

#define CLOCK_PER_MS CLOCKS_PER_SEC / 1000

int main()
{
    uint8_t msg_cnt = 0;

    Ixxat_CAN_Msg_t msg;
    Ixxat_CAN_Msg_t msgRx;

    msg.ID = 0x11;
    msg.DLC = 1;
    msg.Data[0] = 0;
    int8_t res = ixxat_can_init(0);
    clock_t last = clock();

    if (res == 0)
    {
        while (1)
        {
            clock_t now = clock();
            if ((now - last) >= 250)
            {
                last = clock();

                ixxat_can_msg_send(0, &msg);

                uint8_t rx_flag = ixxat_can_msg_receive(0, &msgRx);
                if (rx_flag && msgRx.ID == 0x22)
                    if (msgRx.Data[0] == 5)
                    //  memcpy(msg.Data, &last, sizeof(clock_t));
                    {
                        ixxat_can_msg_send(0, &msg);
                        printf("EEEEEEEEEEE");
                      //    break;
                    }

                printf("sent msg\n");
                if (++msg_cnt >= 50)
                    break;
            }
/*
            Ixxat_CAN_Msg_t rx_msg;
            uint8_t rx_flag = ixxat_can_msg_receive(0, &rx_msg);
            if (rx_flag && rx_msg.ID == 100)
                break;
*/
        }
    }

    ixxat_can_deinit(0);
    printf("finish %d\n", msg_cnt);

    return 0;
}


/*


int tmr_ms_check(uint32_t* last_ms_stamp, uint32_t period_ms) {
    
    clock_t last_ms = clock();  // время завершения
    
    if(clock() > *last_ms_stamp + period_ms) {
        *last_ms_stamp = last_ms;
        return 1;
    }  // цикл ожидания времени
    return 0;
}
*/