#include <stdio.h>
#include "esp_task_wdt.h"
#include "driver/gpio.h"

#define STACK_SIZE (2048)
static uint8_t ucParameterToPass;
static TaskHandle_t xHandle = NULL;

#define STACK_SIZE2 (2048)
static StaticTask_t xTaskBuffer;
static StackType_t xStack[STACK_SIZE2];
static TaskHandle_t xHandle2 = NULL;

static void vTaskCode( void *);
static void vTaskCode2( void *);

static void vTaskCode(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(500);
    uint32_t u32a_level;
    int s32a_sw1;
    int s32a_sw2;
    uint8_t u8a_count = 0;

    gpio_set_direction(GPIO_NUM_19, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_21, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_22, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_INPUT);

    xLastWakeTime = xTaskGetTickCount();
    while (true) {
        s32a_sw1 = gpio_get_level(GPIO_NUM_23);
        s32a_sw2 = gpio_get_level(GPIO_NUM_22);
        u32a_level = u8a_count % 2;
        if (s32a_sw1 == 0) {
            gpio_set_level(GPIO_NUM_21, 1);
            gpio_set_level(GPIO_NUM_19, 1);
        } else if (s32a_sw2 == 0) {
            gpio_set_level(GPIO_NUM_21, u32a_level);
            gpio_set_level(GPIO_NUM_19, u32a_level);
        } else {
            gpio_set_level(GPIO_NUM_21, u32a_level);
            gpio_set_level(GPIO_NUM_19, !u32a_level);
        }

        printf("vTaskCode %d.\n", u8a_count);
        u8a_count++;
        xTaskDelayUntil(&xLastWakeTime, xFrequency);
        //esp_rom_delay_us(500000);
        //esp_task_wdt_reset();
    }
}

static void vTaskCode2(void *pvParameters) {
    uint8_t u8a_count = 0;
    while (true) {
        printf("vTaskCode2 %d.\n", u8a_count);
        u8a_count++;
        vTaskDelay(500 / portTICK_PERIOD_MS);
        //esp_rom_delay_us(500000);
        //esp_task_wdt_reset();
    }
}

void app_main(void)
{
    uint8_t u8a_count = 0;

    xTaskCreatePinnedToCore(vTaskCode,
            "NAME",
            STACK_SIZE,
            &ucParameterToPass,
            tskIDLE_PRIORITY,
            &xHandle,
            APP_CPU_NUM);
//            PRO_CPU_NUM);

    xHandle2 = xTaskCreateStaticPinnedToCore(
            vTaskCode2,    // Function that implements the task.
            "NAME2",            // Text name for the task.
            STACK_SIZE2,        // Stack size in bytes, not words.
            &ucParameterToPass, // Parameter passed into the task.
            tskIDLE_PRIORITY,   // Priority at which the task is created.
            xStack,             // Array to use as the task's stack.
            &xTaskBuffer,       // Variable to hold the task's data structure.
            APP_CPU_NUM);       // Specify the task's core affinity

    while (true) {
        printf("step 1000ms.(%d)\n", u8a_count);
        u8a_count++;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        //esp_rom_delay_us(1000000);
        //esp_task_wdt_reset();
    }
}
