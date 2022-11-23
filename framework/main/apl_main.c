#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sys_main.h"
#include "apl_main.h"

static ST_SYS_TIMER sts_timer;

void apl_init() {
    sys_call_timer_start(&sts_timer);
}

void apl_deinit() {
}

void apl_reinit() {
}

void apl_main() {
    if (sys_call_timer_check(&sts_timer, 10)) {
        printf("Hello apl_main.\n");
        sys_call_timer_start(&sts_timer);
    }
}
