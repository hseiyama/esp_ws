#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sys_main.h"
#include "iod_main.h"

static ST_SYS_TIMER sts_timer;

// 外部公開関数
void iod_init(void) {
    iod_port_init();
    sys_call_timer_start(&sts_timer);
}

void iod_deinit(void) {
    iod_port_deinit();
}

void iod_reinit(void) {
    iod_port_reinit();
}

void iod_main_1ms(void) {
    static uint8_t u8s_count = 0;

    iod_port_main_1ms();
    if (sys_call_timer_check(&sts_timer, 1000)) {
        printf("Hello iod_main_1ms. (%d)\n", u8s_count);
        u8s_count++;
        sys_call_timer_start(&sts_timer);
    }
}

void iod_main_in(void) {
    iod_port_main_in();
}

void iod_main_out(void) {
    iod_port_main_out();
}
