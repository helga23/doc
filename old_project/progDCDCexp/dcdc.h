#include <stdint.h>

#include "ixxat_c_lib/ixxat_can.h"


#define TIME_MAIN_CUCLE_MS 500
#define TIME_TX_CUSTOM_CUCLE_MS 500
#define TIME_RX_CUSTOM_CUCLE_MS 500
#define TIME_TX_J1939_MS 500

#define START_CUSTUM 170
#define STOP_CUSTUM 85

#define START 1
#define STOP 2

#define ID_RX_CUSTUM_MSG_323 0x232
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

/*
typedef struct
{
    uint32_t Id;
    uint32_t DLC;
    uint8_t Data[8];
} CanMsgTypeDef;
*/
typedef struct
{
    Ixxat_CAN_Msg_t tx_customMsg213;
    Ixxat_CAN_Msg_t rx_customMsg323;
    Ixxat_CAN_Msg_t rx_customMsg313;
} Custom_DCDC_Msg_TypeDef;

typedef struct
{
    Ixxat_CAN_Msg_t tx_j1939MsgVc;
    Ixxat_CAN_Msg_t tx_j1939MsgOc;
    Ixxat_CAN_Msg_t tx_j1939MsgDM1;
} J1939_DCDC_TypeDef;

typedef struct
{
    unsigned int Flt_Rnk : 2;
    unsigned HV_OVP : 1;
    unsigned HV_OCP : 1;
    unsigned LV_OVP : 1;
    unsigned LV_Short : 1;
    unsigned OTP : 1;
    unsigned LV_UVP : 1;
    unsigned TempSenseErr : 1;
    unsigned CAN_TimeOut : 1;
    unsigned RunFail : 1;
} failt_code_mode_struct;

typedef union
{
    uint16_t code_16;
    failt_code_mode_struct code;
} failt_code_mode_union;

/*
Values Value_DCDC_TypeDef:
    Custom messags:
    { TxCustum Id = VCU_0x213;
    - in_mode [START, STOP];
    - in_target_v [factot = 0.1 V, max = 16 V];
    - in_limit_i [factot = 0.1 A, max = 174 A];
    }

    J1939 messags:
    { RxCustum Id = DCDC2_0x323; -> TxJ1939 Id = DCDC1VC;
    - out_actual_i  [factot = 0.1 A, offset = -3212.7];  // DCDC1VC_LowSideCurrent values from J1939_RX;  range [-3212.7, 3212.7]
    - out_actual_v  [factot = 0.01 V, offset = 0];       // DCDC1VC_LowSideVoltage values from J1939_RX;  range [      0, 642.55]

    RxCustum Id = DCDC1_0x313; -> TxJ1939 Id = DCDC1OS;
    - out_actual_mode [ 1: Init
                        2: Standby
                        3: HV battery connected OK
                        4: Work
                        5: Power off
                        6: Shutdown ] // values from CustomRX

    RxCustum Id = DCDC1_0x313; -> TxJ1939 Id = DM1;
    - out_failt_code [  Flt_Rnk:       2 [0: Normal, 3: Fault];
                        HV_OVP:        1 [0: Normal, 1: Error];
                        HV_OCP:        1 [0: Normal, 1: Error];
                        LV_OVP:        1 [0: Normal, 1: Error];
                        LV_Short:      1 [0: Normal, 1: Error];
                        OTP:           1 [0: Normal, 1: Error];
                        LV_UVP:        1 [0: Normal, 1: Error];
                        TempSenseErr:  1 [0: Normal, 1: Error];
                        CAN_TimeOut:   1 [0: Normal, 1: Error];
                        RunFail:       1 [0: Normal, 1: Error]. ]  //values from CustomRX
    }
*/

typedef struct
{
    uint16_t in_mode;
    uint16_t in_target_v;
    uint16_t in_limit_i;

    uint16_t out_actual_i;
    uint16_t out_actual_v;
    uint16_t out_actual_mode;
    failt_code_mode_union out_failt_code;

} Value_DCDC_TypeDef;

void init_DCDC_Msg(void);

void conversion_value_to_custom_DCDC(uint16_t *pValueMsg, Ixxat_CAN_Msg_t *pCustomMsg);
void conversion_custom_to_value_DCDC(Ixxat_CAN_Msg_t *pCustomMsg, uint16_t *pValueMsg);
void conversion_value_to_j1939_DCDC(uint16_t *pValueMsg, Ixxat_CAN_Msg_t *pJ1939Msg);

void CAN_handler_Receive_DCDC(void);

void set_App_DCDC(uint16_t *pValueMsg, uint16_t data);
uint16_t get_App_DCDC(uint16_t *pValueMsg);

#include <sys/time.h>
int tmr_ms_check(clock_t *last, uint32_t period_ms);