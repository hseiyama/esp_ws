#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "iod_main.h"
#include "apl_main.h"

#define GPIO_LED0               GPIO21_PORT
#define GPIO_LED1               GPIO19_PORT
#define GPIO_BTN0               GPIO23_PORT
#define GPIO_BTN1               GPIO22_PORT
#define GPIO_PORT0              GPIO18_PORT
#define GPIO_PORT1              GPIO5_PORT
#define GPIO_BIT_MASK_INPUT     ((1ULL<<GPIO_BTN0) | (1ULL<<GPIO_BTN1))
#define GPIO_BIT_MASK_OUTPUT    ((1ULL<<GPIO_LED0) | (1ULL<<GPIO_LED1) | (1ULL<<GPIO_PORT0) | (1ULL<<GPIO_PORT1))
#define ESP_INTR_FLAG_DEFAULT (0)

static void IRAM_ATTR iod_port_intr_port(void *);

// 外部公開関数
void iod_port_init(void) {
    const gpio_config_t csta_gpio_config_input = {
        GPIO_BIT_MASK_INPUT,    // pin_bit_mask
        GPIO_MODE_INPUT,        // mode
        GPIO_PULLUP_ENABLE,     // pull_up_en
        GPIO_PULLDOWN_DISABLE,  // pull_down_en
        GPIO_INTR_NEGEDGE       // intr_type
    };
    const gpio_config_t csta_gpio_config_output = {
        GPIO_BIT_MASK_OUTPUT,   // pin_bit_mask
        GPIO_MODE_OUTPUT,       // mode
        GPIO_PULLUP_DISABLE,    // pull_up_en
        GPIO_PULLDOWN_DISABLE,  // pull_down_en
        GPIO_INTR_DISABLE       // intr_type
    };
    gpio_config(&csta_gpio_config_input);
    gpio_config(&csta_gpio_config_output);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(GPIO_BTN0, iod_port_intr_port, (void*)GPIO_BTN0);
    gpio_isr_handler_add(GPIO_BTN1, iod_port_intr_port, (void*)GPIO_BTN1);
}

void iod_port_deinit(void) {
}

void iod_port_reinit(void) {
}

void iod_port_main_1ms(void) {
}

void iod_port_main_in(void) {
}

void iod_port_main_out(void) {
}

void iod_read_btn0(bool *pbla_value) {
    *pbla_value = (bool)gpio_get_level(GPIO_BTN0);
}

void iod_read_btn1(bool *pbla_value) {
    *pbla_value = (bool)gpio_get_level(GPIO_BTN1);
}

void iod_write_led0(bool bla_value) {
    gpio_set_level(GPIO_LED0, bla_value);
}

void iod_write_led1(bool bla_value) {
    gpio_set_level(GPIO_LED1, bla_value);
}

void iod_write_port0(bool bla_value) {
    gpio_set_level(GPIO_PORT0, bla_value);
}

void iod_write_port1(bool bla_value) {
    gpio_set_level(GPIO_PORT1, bla_value);
}

// 内部関数
static void IRAM_ATTR iod_port_intr_port(void *pvd_arg) {
    uint8_t u8a_gpio_num = (uint8_t)(uint32_t)pvd_arg;
    apl_port_intr(u8a_gpio_num);
}
