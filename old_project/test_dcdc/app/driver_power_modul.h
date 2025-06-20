#ifndef _MODULE_POWER_H_
#define _MODULE_POWER_H_

#define TIME_MAIN_CUCLE_MS 500
#define TIME_TX_CUSTOM_CUCLE_MS 500
#define TIME_RX_CUSTOM_CUCLE_MS 500
#define TIME_TX_J1939_MS 500

#define START_CUSTUM 170
#define STOP_CUSTUM 85

#define ID_RX_CUSTUM_MSG_323 0x323
#define DLC_RX_CUSTUM_MSG_323 5

#define ID_RX_CUSTUM_MSG_313 0x313
#define DLC_RX_CUSTUM_MSG_313 7

#define ID_TX_CUSTUM_MSG_213 0x213
#define DLC_TX_CUSTUM_MSG_213 5

#define ID_TX_J1939_MSG_VC 0xf11406
#define DLC_TX_J1939_MSG_VC 8

#define ID_TX_J1939_MSG_OC 0xf11305
#define DLC_TX_J1939_MSG_OC 8

#define ID_TX_J1939_MSG_DM1 0xfbda0f
#define DLC_TX_J1939_MSG_DM1 8

typedef union
{
    uint8_t val_8[2];
    uint16_t val_16;
} data_16to8_union;


typedef enum
{
    pm_start = 0,
    pm_stop,
} pm_commands_t;

typedef enum
{
    pm_init = 1,
    pm_standby,
    pm_connected_OK;
    pm_Work;
    pm_power_off;
    pm_shutdown;
}
pm_statuses_t;

typedef struct
{
    unsigned Flt_Rnk : 2;
    unsigned HV_OVP : 1;
    unsigned HV_OCP : 1;
    unsigned LV_OVP : 1;
    unsigned LV_Short : 1;
    unsigned OTP : 1;
    unsigned LV_UVP : 1;
    unsigned TempSenseErr : 1;
    unsigned CAN_TimeOut : 1;
    unsigned RunFail : 1;
} pm_failt_code_struct;

typedef union
{
    uint16_t code_16;
    pm_failt_code_struct code;
} pm_failt_code_union;

typedef struct
{
    /// in////
    uint16_t cmd;
    uint16_t target_v;
    uint16_t limit_i;
    /// out////
    uint16_t actual_i;
    uint16_t actual_v;
    uint16_t actual_t;
    uint16_t actual_st;
    uint16_t failt_code;
} power_module_t;


typedef struct
{
    uint8_t cmd;
    uint16_t voltage;
    uint16_t current;
} dcdc_msg_tx_213_Data_t;

typedef struct
{
    uint32_t ID;
    uint8_t DLC;
    dcdc_msg_tx_213_Data_t data;
} dcdc_msg_tx_213;

typedef struct
{   uint16_t voltage;
    uint16_t current;
    uint8_t temperature;
} dcdc_msg_rx_323_Data_t;

typedef struct
{
    uint32_t ID;
    uint8_t DLC;
    dcdc_msg_rx_323_Data_t data;
} dcdc_msg_rx_323;

typedef struct
{ 
    unsigned o : 2;
    unsigned Flt_Rnk : 2;
    unsigned status : 4;
    unsigned HV_OVP : 1;
    unsigned HV_OCP : 1;
    unsigned LV_OVP : 1;
    unsigned LV_Short : 1;
    unsigned OTP : 1;
    unsigned LV_UVP : 1;
    unsigned o : 2;
    unsigned TempSenseErr : 1;
    unsigned CAN_TimeOut : 1;
    unsigned o : 1;
    unsigned RunFail : 1;
} dcdc_msg_rx_313_Data_t;

typedef struct
{
    uint32_t ID;
    uint8_t DLC;
    dcdc_msg_rx_313_Data_t data;
} dcdc_msg_rx_313;



void pm_init(power_module_t *pm);

uint8_t pm_v_set(power_module_t *pm, uint16_t value);
uint8_t pm_i_set(power_module_t *pm, uint16_t value);
uint8_t pm_cmd_set(power_module_t *pm, pm_commands_t cmd);

uint8_t pm_can_data_rx(Ixxat_CAN_Msg_t *pRxCan)

uint16_t pm_v_get(power_module_t *pm);
uint16_t pm_i_get(power_module_t *pm);
uint16_t pm_t_get(power_module_t *pm);
pm_statuses_t pm_status_get(power_module_t *pm);
pm_failt_code_union pm_failt_code_get(power_module_t *pm);

void pm_proc(power_module_t *pm);

#endif
