#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/twai.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_err.h"
#include "esp_rom_sys.h"

// Ultrasonic sensor pins
#define TRIG_PIN GPIO_NUM_18
#define ECHO_PIN GPIO_NUM_19

// Function to measure distance from ultrasonic sensor
float measure_distance_cm() {
    gpio_set_level(TRIG_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(2));
    gpio_set_level(TRIG_PIN, 1);
    esp_rom_delay_us(10);
    gpio_set_level(TRIG_PIN, 0);

    int64_t start = esp_timer_get_time();
    while (gpio_get_level(ECHO_PIN) == 0) {
        if (esp_timer_get_time() - start > 100000) return -1; // Timeout
    }
    int64_t echo_start = esp_timer_get_time();

    while (gpio_get_level(ECHO_PIN) == 1) {
        if (esp_timer_get_time() - echo_start > 200000) return -1; // Timeout
    }
    int64_t echo_end = esp_timer_get_time();

    float distance = (echo_end - echo_start) / 58.0;
    if (distance < 2.0 || distance > 400.0) return -1; // Out of range
    return distance;
}

void app_main(void) {
    printf("ESP32 CAN Transmit + Ultrasonic Sensor\n");

    gpio_set_direction(TRIG_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);

    // CAN Configuration
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK || twai_start() != ESP_OK) {
        printf("CAN initialization failed!\n");
        return;
    }
    printf("CAN initialized successfully.\n");

    while (1) {
        float distance = measure_distance_cm();
        if (distance < 0) {
            printf("No object detected. Sending 0 cm.\n");
            distance = 0;
        } else {
            printf("Measured Distance: %.2f cm\n", distance);
        }

        twai_message_t message = {
            .identifier = 0x100,
            .extd = 0,
            .rtr = 0,
            .data_length_code = 2,
            .data = {
                (uint8_t)distance,
                (uint8_t)((int)(distance * 10) % 100)
            }
        };

        printf("CAN Sent: ID=0x%" PRIX32 " DLC=%d Distance=%.2f cm | Data =",
               message.identifier,
               message.data_length_code,
               distance);
        for (int i = 0; i < message.data_length_code; i++) {
            printf(" %02X", message.data[i]);
        }
        printf("\n");

        if (twai_transmit(&message, pdMS_TO_TICKS(1000)) != ESP_OK) {
            printf("CAN TX Failed!\n");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
