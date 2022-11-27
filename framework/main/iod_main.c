#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sys_main.h"
#include "iod_main.h"

static ST_SYS_TIMER sts_timer;

static void iod_monitor_init(void);
static void iod_monitor_main(void);

// 外部公開関数
void iod_init(void) {
    iod_port_init();
    iod_monitor_init();
}

void iod_deinit(void) {
    iod_port_deinit();
}

void iod_reinit(void) {
    iod_port_reinit();
}

void iod_main_1ms(void) {
    iod_port_main_1ms();
    iod_monitor_main();
}

void iod_main_in(void) {
    iod_port_main_in();
}

void iod_main_out(void) {
    iod_port_main_out();
}

// 内部関数
static void iod_monitor_init(void) {
    sys_call_timer_start(&sts_timer);
}

static void iod_monitor_main(void) {
    static uint8_t u8s_count = 0;

    if (sys_call_timer_check(&sts_timer, 1000)) {
        printf("Hello iod_monitor_main. (%d)\n", u8s_count);
        u8s_count++;
        sys_call_timer_start(&sts_timer);
    }
}
