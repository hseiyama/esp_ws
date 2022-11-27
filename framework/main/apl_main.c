#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sys_main.h"
#include "iod_main.h"
#include "apl_main.h"

static ST_SYS_TIMER sts_timer_port;
static ST_SYS_TIMER sts_timer_monitor;

static void apl_port_init(void);
static void apl_port_main(void);
static void apl_monitor_init(void);
static void apl_monitor_main(void);

void apl_init(void) {
    apl_port_init();
    apl_monitor_init();
}

void apl_deinit(void) {
}

void apl_reinit(void) {
}

void apl_main(void) {
    apl_port_main();
    apl_monitor_main();
}

// 内部関数
static void apl_port_init(void) {
    sys_call_timer_start(&sts_timer_port);
}

static void apl_port_main(void) {
    static bool bla_port = false;
    bool bla_btn0;
    bool bla_btn1;

    iod_read_btn0(&bla_btn0);
    iod_read_btn1(&bla_btn1);
    iod_write_led0(bla_btn0);
    iod_write_led1(bla_btn1);
    if (sys_call_timer_check(&sts_timer_port, 2000)) {
        iod_write_port0(bla_port);
        iod_write_port1(!bla_port);
        bla_port = !bla_port;
        sys_call_timer_start(&sts_timer_port);
    }
}

static void apl_monitor_init(void) {
    sys_call_timer_start(&sts_timer_monitor);
}

static void apl_monitor_main(void) {
    static uint8_t u8s_count = 0;

    if (sys_call_timer_check(&sts_timer_monitor, 1000)) {
        printf("Hello apl_main. (%d)\n", u8s_count);
        u8s_count++;
        sys_call_timer_start(&sts_timer_monitor);
    }
}
