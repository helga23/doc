#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "dcdc.h"
#include "test.h"
#include "ixxat_c_lib/ixxat_can.h"

extern Value_DCDC_TypeDef value_DCDC;
extern Custom_DCDC_Msg_TypeDef custum_DCDC;
extern J1939_DCDC_TypeDef j1939_DCDC;

extern Ixxat_CAN_Msg_t RxCan;
int rx_flag = 0;

int main()
{
    uint8_t msg_cnt = 0;
    uint16_t test[4];

    int8_t res = ixxat_can_init(0);
    clock_t last = clock();
    init_DCDC_Msg();

    if (res == 0)
    {
        while (1)
        {
            clock_t now = clock();
            if ((now - last) >= 250)
            //          if (tmr_ms_check(&last, 250) == 1)
            {
                last = clock();
                /*
                set_App_DCDC(&value_DCDC.in_mode, START);
                set_App_DCDC(&value_DCDC.in_limit_i, 0x4321);
                set_App_DCDC(&value_DCDC.in_target_v, 0x4321);

                conversion_value_to_custom_DCDC(&value_DCDC.in_mode, &custum_DCDC.tx_customMsg213);
                conversion_value_to_custom_DCDC(&value_DCDC.in_target_v, &custum_DCDC.tx_customMsg213);
                conversion_value_to_custom_DCDC(&value_DCDC.in_limit_i, &custum_DCDC.tx_customMsg213);


                                if (tmr_ms_check(now, TIME_TX_CUSTOM_CUCLE_MS) == 1)
                                {
                                    */
                custum_DCDC.tx_customMsg213.Data[0] = 7;
                ixxat_can_msg_send(0, &custum_DCDC.tx_customMsg213);
                test_сustom_tx_DCDC(); // вывод значений сustom_tx
                                       //  }
                                       /*
                                                       //////////////////////////////rx//////////////////////////
                       
                                                       if (tmr_ms_check(now, TIME_RX_CUSTOM_CUCLE_MS) == 1)
                                                       {
                                                           uint8_t rx_flag = ixxat_can_msg_receive(0, &RxCan);
                                                           if (rx_flag == 1)
                                                           {
                                                               CAN_handler_Receive_DCDC();
                                                               test_сustom_Rx_DCDC(); // вывод значений сustom_rx
                       
                                                               conversion_value_to_j1939_DCDC(&value_DCDC.out_actual_i, &j1939_DCDC.tx_j1939MsgVc);
                                                               conversion_value_to_j1939_DCDC(&value_DCDC.out_actual_v, &j1939_DCDC.tx_j1939MsgVc);
                                                               conversion_value_to_j1939_DCDC(&value_DCDC.out_actual_mode, &j1939_DCDC.tx_j1939MsgOc);
                                                               conversion_value_to_j1939_DCDC(&value_DCDC.out_failt_code.code_16, &j1939_DCDC.tx_j1939MsgDM1);
                                                           }
                                                       }
                                                       //////////////////////////////rx//////////////////////////
                       
                       
                                                       if (tmr_ms_check(now, TIME_TX_J1939_MS) == 1)
                                                       {
                                                           ixxat_can_msg_send(0, &j1939_DCDC.tx_j1939MsgVc);
                                                           ixxat_can_msg_send(0, &j1939_DCDC.tx_j1939MsgDM1);
                                                           ixxat_can_msg_send(0, &j1939_DCDC.tx_j1939MsgOc);
                       
                                                           test_struct_out_DCDC(); // вывод значений value
                                                           test_j1939_tx_data();   // вывод значений j1939_tx
                                                       }
                                                       test[0] = get_App_DCDC(&value_DCDC.out_actual_i);
                                                       test[1] = get_App_DCDC(&value_DCDC.out_actual_v);
                                                       test[2] = get_App_DCDC(&value_DCDC.out_actual_mode);
                                                       test[3] = get_App_DCDC(&value_DCDC.out_failt_code.code_16);
                                         */
                printf("sent msg\n");
                if (++msg_cnt >= 50)
                    break;
            }
        }
    }

    ixxat_can_deinit(0);
    printf("finish %d\n", msg_cnt);

    return 0;
}

/*
test_RxCanMsg_DCDC(custum_DCDC.rx_customMsg323.Id);
test_RxCanMsg_DCDC(custum_DCDC.rx_customMsg313.Id);
*/