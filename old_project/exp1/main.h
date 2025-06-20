
#include <stdint.h>
typedef struct
{
    uint32_t Id;
    uint32_t DLC;
    uint8_t Data[8];
} CanMsgTypeDef;

typedef struct
{
    CanMsgTypeDef in_mode;
    CanMsgTypeDef in_target_v;
    CanMsgTypeDef in_limit_i;

    CanMsgTypeDef out_actual_i;
    CanMsgTypeDef out_actual_v;
    CanMsgTypeDef out_actual_mode;
    CanMsgTypeDef out_failt_code;

} Custom_DCDC_Msg_TypeDef;