#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sys_main.h"
#include "iod_main.h"
#include "apl_main.h"

#define TASK_PRIORITY_1MS   ((UBaseType_t)2U)
#define TASK_PRIORITY_5MS   tskIDLE_PRIORITY
#define TASK_STACK_SIZE_1MS (2048)
#define TASK_STACK_SIZE_5MS (4096)
#define TASK_CYCLE_1MS      (1)
#define TASK_CYCLE_5MS      (5)
#define SYS_TIME_MAX        (0xFFFFFFFFFFFFFFFF)

static volatile uint64_t u64s_sys_time;

static void sys_task_1ms(void *);
static void sys_task_5ms(void *);
static void sys_init(void);
static void sys_deinit(void);
static void sys_reinit(void);
static void sys_main_1ms(void);
static void sys_main_5ms(void);
static void sys_time_init(void);
static void sys_time_main(void);

// 外部公開関数
void app_main(void) {
    TickType_t u32a_last_wake_time;
    TaskHandle_t pvda_handle_task_1m;
    TaskHandle_t pvda_handle_task_5m;

    sys_init();
    u32a_last_wake_time = xTaskGetTickCount();
    // 周期 1msのタスクを生成
    xTaskCreatePinnedToCore(
            sys_task_1ms,           // pvTaskCode
            "sys_task_1ms",         // pcName
            TASK_STACK_SIZE_1MS,    // usStackDepth
            &u32a_last_wake_time,   // pvParameters
            TASK_PRIORITY_1MS,      // uxPriority
            &pvda_handle_task_1m,   // pvCreatedTask
            APP_CPU_NUM);           // xCoreID
    // 周期 5msのタスクを生成
    xTaskCreatePinnedToCore(
            sys_task_5ms,           // pvTaskCode
            "sys_task_5ms",         // pcName
            TASK_STACK_SIZE_5MS,    // usStackDepth
            &u32a_last_wake_time,   // pvParameters
            TASK_PRIORITY_5MS,      // uxPriority
            &pvda_handle_task_5m,   // pvCreatedTask
            APP_CPU_NUM);           // xCoreID

    while (true) {
        vTaskDelay(1000 / portTICK_RATE_MS);
        if (false) {
            sys_deinit();
            sys_reinit();
        }
    }
}

void sys_call_timer_start(ST_SYS_TIMER *psta_sys_timer) {
    psta_sys_timer->u64_time = u64s_sys_time;
    psta_sys_timer->bl_state = true;
}

void sys_call_timer_stop(ST_SYS_TIMER *psta_sys_timer) {
    psta_sys_timer->u64_time = SYS_TIME_MAX;
    psta_sys_timer->bl_state = false;
}

bool sys_call_timer_check(ST_SYS_TIMER *psta_sys_timer, uint64_t u64a_wait_time) {
    uint64_t u64a_time_diff;
    bool bla_rcode;

    u64a_time_diff = sys_call_timer_diff(psta_sys_timer, u64a_wait_time);
    // 待ち時間が経過している場合は、trueを設定
    bla_rcode = (u64a_time_diff > 0) ? false : true;

    return bla_rcode;
}

uint64_t sys_call_timer_diff(ST_SYS_TIMER *psta_sys_timer, uint64_t u64a_wait_time) {
    uint64_t u64a_pass_time;
    uint64_t u64a_rtime;

    // タイマーが開始している場合
    if (psta_sys_timer->bl_state) {
        // 経過時間を算出する
        if (u64s_sys_time >= psta_sys_timer->u64_time) {
            u64a_pass_time = u64s_sys_time - psta_sys_timer->u64_time;
        } else {
            u64a_pass_time = SYS_TIME_MAX - (psta_sys_timer->u64_time - u64s_sys_time);
        }
        // 待ち時間との差を算出する（経過後は、0を設定）
        if (u64a_wait_time > u64a_pass_time) {
            u64a_rtime = u64a_wait_time - u64a_pass_time;
        } else {
            u64a_rtime = 0;
        }
    // タイマーが停止している場合は、0を設定
    } else {
        u64a_rtime = 0;
    }

    return u64a_rtime;
}

bool sys_call_timer_isrun(ST_SYS_TIMER *psta_sys_timer) {
    return psta_sys_timer->bl_state;
}

// 内部関数
static void sys_task_1ms(void *pvd_parameters) {
    TickType_t u32a_last_wake_time = *(TickType_t *)pvd_parameters;
    const TickType_t cu32a_frequency = pdMS_TO_TICKS(TASK_CYCLE_1MS);

    while (true) {
        xTaskDelayUntil(&u32a_last_wake_time, cu32a_frequency);
        sys_main_1ms();
    }
}

static void sys_task_5ms(void *pvd_parameters) {
    TickType_t u32a_last_wake_time = *(TickType_t *)pvd_parameters;
    const TickType_t cu32a_frequency = pdMS_TO_TICKS(TASK_CYCLE_5MS);

    while (true) {
        xTaskDelayUntil(&u32a_last_wake_time, cu32a_frequency);
        sys_main_5ms();
    }
}

static void sys_init(void) {
    sys_time_init();
    iod_init();
    apl_init();
}

static void sys_deinit(void) {
    apl_deinit();
    iod_deinit();
}

static void sys_reinit(void) {
    iod_reinit();
    apl_reinit();
}

static void sys_main_1ms(void) {
    sys_time_main();
    iod_main_1ms();
}

static void sys_main_5ms(void) {
    iod_main_in();
    apl_main();
    iod_main_out();
}

static void sys_time_init(void) {
    u64s_sys_time = 0;
}

static void sys_time_main(void) {
    u64s_sys_time++;
}
