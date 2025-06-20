#include "dcdc.h"
#include <stdio.h>

#include <time.h>

#include "ixxat_c_lib/ixxat_can.h"

extern Value_DCDC_TypeDef value_DCDC;
extern Ixxat_CAN_Msg_t RxCan;

extern Custom_DCDC_Msg_TypeDef custum_DCDC;
extern J1939_DCDC_TypeDef j1939_DCDC;

void test_сustom_tx_DCDC(void)
{
    FILE *file;
    file = fopen("custom_tx.txt", "w");

    fprintf(file, "tx_customMsg213 ID = %x,  DLC = %d  \n", custum_DCDC.tx_customMsg213.ID, custum_DCDC.tx_customMsg213.DLC);
    fprintf(file, "mode:  %x \n", custum_DCDC.tx_customMsg213.Data[0]);
    fprintf(file, "target_v: MSB LSB   %x %x \n", custum_DCDC.tx_customMsg213.Data[1], custum_DCDC.tx_customMsg213.Data[2]);
    fprintf(file, "limit_i: MSB LSB   %x %x \n", custum_DCDC.tx_customMsg213.Data[3], custum_DCDC.tx_customMsg213.Data[4]);
    fclose(file);
}
/*
void test_RxCanMsg_DCDC(uint32_t idMsg)
{
    if (idMsg == custum_DCDC.rx_customMsg323.ID)
    {
        RxCan.ID = custum_DCDC.rx_customMsg323.ID;
        RxCan.DLC = custum_DCDC.rx_customMsg323.DLC;
        RxCan.Data[0] = 0x14;
        RxCan.Data[1] = 0xc9;
        RxCan.Data[2] = 0x1a;
        RxCan.Data[3] = 0x78;
    }

    if (idMsg == custum_DCDC.rx_customMsg313.ID)
    {
        RxCan.ID = custum_DCDC.rx_customMsg313.ID;
        RxCan.DLC = custum_DCDC.rx_customMsg313.DLC;
        RxCan.Data[0] = 0x25;
        RxCan.Data[1] = 0x84;
        RxCan.Data[2] = 0x43;
    }

}
*/

void test_сustom_Rx_DCDC(void) // сustom_Rx.txt
{
    FILE *file;
    file = fopen("custom_rx.txt", "w");

    fprintf(file, "rx_customMsg232 ID = %x , DLC = %d \n", custum_DCDC.rx_customMsg323.ID,
            custum_DCDC.rx_customMsg323.DLC);
    fprintf(file, "actual_v:  MSB LSB %x %x \n",
            custum_DCDC.rx_customMsg323.Data[1],
            custum_DCDC.rx_customMsg323.Data[0]);
    fprintf(file, "actual_i: MSB LSB %x %x \n",
            custum_DCDC.rx_customMsg323.Data[3],
            custum_DCDC.rx_customMsg323.Data[2]);

    fclose(file);
}

void test_struct_out_DCDC(void)
{
    FILE *file;
    file = fopen("struct_out.txt", "w");
    fprintf(file, "value_DCDC.in_mode: %x \n", value_DCDC.in_mode);
    fprintf(file, "value_DCDC.in_target_v: %x \n", value_DCDC.in_target_v);
    fprintf(file, "value_DCDC.in_limit_i: %x \n", value_DCDC.in_limit_i);

    fprintf(file, "value_DCDC.out_actual_i: %x \n", value_DCDC.out_actual_i);
    fprintf(file, "value_DCDC.out_actual_v: %x \n", value_DCDC.out_actual_v);
    fprintf(file, "value_DCDC.out_actual_mode: %x \n", value_DCDC.out_actual_mode);
    fprintf(file, "value_DCDC.out_failt_code: %x \n", value_DCDC.out_failt_code);

    fclose(file);
}

void test_j1939_tx_data() // j1939_tx_data.txt
{
    FILE *file;
    file = fopen("j1939_rx_data.txt", "w");
    fprintf(file, "tx_j1939MsgVc: %x %d  \n", j1939_DCDC.tx_j1939MsgVc.ID,
            j1939_DCDC.tx_j1939MsgVc.DLC);
    fprintf(file, "out_actual_v: MSB LSB  %x %x \n",
            j1939_DCDC.tx_j1939MsgVc.Data[1],
            j1939_DCDC.tx_j1939MsgVc.Data[0]);
    fprintf(file, "out_actual_c: MSB LSB  %x %x \n",
            j1939_DCDC.tx_j1939MsgVc.Data[3],
            j1939_DCDC.tx_j1939MsgVc.Data[2]);

    fprintf(file, "tx_j1939MsgOc: %x %d  \n", j1939_DCDC.tx_j1939MsgOc.ID,
            j1939_DCDC.tx_j1939MsgOc.DLC);
    fprintf(file, "out_actual_mode:   %x \n",
            j1939_DCDC.tx_j1939MsgOc.Data[0]);

    fprintf(file, "tx_j1939MsgDM1: %x %d  \n", j1939_DCDC.tx_j1939MsgDM1.ID,
            j1939_DCDC.tx_j1939MsgDM1.DLC);
    fprintf(file, "out_failt_code:   %x %x %x \n",
            j1939_DCDC.tx_j1939MsgDM1.Data[2],
            j1939_DCDC.tx_j1939MsgDM1.Data[3],
            j1939_DCDC.tx_j1939MsgDM1.Data[4]);

    fclose(file);
}

void sendCANtx(Ixxat_CAN_Msg_t *pCustomMsg)
{
}

#define CLOCK_PER_MS CLOCKS_PER_SEC / 1000

int tmr_ms_check(clock_t *last, uint32_t period_ms)
{

    clock_t now = clock(); // / CLOCK_PER_MS // время завершения

    if ((now - *last) >= period_ms)
    {
        *last = clock(); 
        return 1;
    } // цикл ожидания времени
    return 0;

    //  return 1;
}
