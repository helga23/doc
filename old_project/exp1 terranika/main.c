#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ixxat_c_lib/ixxat_can.h"

#include "dcdc.h"
#include "test.h"

#define CLOCK_PER_MS CLOCKS_PER_SEC / 1000

extern Value_DCDC_TypeDef value_DCDC;
extern Custom_DCDC_Msg_TypeDef custum_DCDC;
extern J1939_DCDC_TypeDef j1939_DCDC;
extern Ixxat_CAN_Msg_t RxCan;

int main()
{
    uint16_t test[4];
    int8_t res = ixxat_can_init(0);
    clock_t last = clock();

    uint8_t msg_cnt = 0;
    init_DCDC_Msg();

    if (res == 0)
    {
        while (1)
        {
            if (tmr_ms_check(&last, TIME_TX_CUSTOM_CUCLE_MS))
            {
                set_App_DCDC(&value_DCDC.in_mode, START);
                set_App_DCDC(&value_DCDC.in_limit_i, 0x4321);
                set_App_DCDC(&value_DCDC.in_target_v, 0x4321);

                conversion_value_to_custom_DCDC(&value_DCDC.in_mode, &custum_DCDC.tx_customMsg213);
                conversion_value_to_custom_DCDC(&value_DCDC.in_target_v, &custum_DCDC.tx_customMsg213);
                conversion_value_to_custom_DCDC(&value_DCDC.in_limit_i, &custum_DCDC.tx_customMsg213);

                ixxat_can_msg_send(0, &custum_DCDC.tx_customMsg213);

                printf("sent msg\n");
                if (++msg_cnt >= 50)
                    break;

                if (ixxat_can_msg_receive(0, &RxCan))
                    CAN_handler_Receive_DCDC();

                //   }
            }
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