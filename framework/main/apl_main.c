#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sys_main.h"
#include "apl_main.h"

static ST_SYS_TIMER sts_timer;

void apl_init(void) {
    sys_call_timer_start(&sts_timer);
}

void apl_deinit(void) {
}

void apl_reinit(void) {
}

void apl_main(void) {
    static uint8_t u8s_count = 0;

    if (sys_call_timer_check(&sts_timer, 1000)) {
        printf("Hello apl_main. (%d)\n", u8s_count);
        u8s_count++;
        sys_call_timer_start(&sts_timer);
    }
}
