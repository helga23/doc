#include <stdint.h>
#include <stdio.h>
#include "../app/driver_power_modul.h"

typedef enum {
    SUCCESS = 0,
    FAILED = !SUCCESS
} TestResult_t;


void tests_init() {    

}

void pm_test() {

    // 1 - вызываю функции set и проверяю отправленные данные
    // 2 - передаю массив данных на обработку и проверяю корректность

    TestResult_t res = pm_test1();  
    printf("Test 1 %d\n", res);
    
    res = pm_test2();
    printf("Test 2 %d\n", res);
}

// вызываю функции get и проверяю отправленные данные
TestResult_t pm_test1(power_module_t *pm) {

    TestResult_t result = SUCCESS;
// тестовая выборка 1
    uint16_t v = 50;
    pm_commands_t cmd = pm_enable;

    //
    pm_v_set(pm, v);
    pm_cmd_set(pm, pm_enable);

    for(pm_can_messages_t i = pm_msg_1; i < pm_msg_num; i++)
        pm_proc(pm);
    

    // читаем данные из фалйа(памяти)
    uint32_t rx_id = 0;
    uint8_t data[8];
    receive_can_msg(&rx_id, data, 8);
    dcdc_msg1_t *d = (dcdc_msg1_t*)data;

    if(d->voltage != v)
        result = FAILED;

    receive_can_msg(&rx_id, data, 8);
    dcdc_msg2_t *d2 = (dcdc_msg2_t*)data;


    if(d2->cmd != cmd)
        result = FAILED;

    return result;    
}
