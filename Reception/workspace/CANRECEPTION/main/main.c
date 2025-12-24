#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/twai.h"
#include "esp_err.h"
#include <inttypes.h>

void app_main(void) {
    printf("==== ESP32 CAN Receiver ====\n");

    // ===== CAN Configuration =====
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        GPIO_NUM_21,   // TX pin (mandatory, even if not used)
        GPIO_NUM_22,   // RX pin (connect to MCP2551 RXD)
        TWAI_MODE_NORMAL
    );
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();  // Match transmitter speed
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL(); // Accept all IDs

    // ===== Install and Start CAN =====
    esp_err_t err;
    err = twai_driver_install(&g_config, &t_config, &f_config);
    if (err != ESP_OK) {
        printf("CAN install failed: %s\n", esp_err_to_name(err));
        return;
    }

    err = twai_start();
    if (err != ESP_OK) {
        printf("CAN start failed: %s\n", esp_err_to_name(err));
        return;
    }

    printf("CAN Receiver Ready. Waiting for messages...\n");

    // ===== Main Loop =====
    while (1) {
        twai_message_t rx_message;
        err = twai_receive(&rx_message, pdMS_TO_TICKS(2000)); // 2 second timeout

        if (err == ESP_OK) {
            printf("\n[CAN RECEIVED] ID: 0x%" PRIX32 ", DLC: %d, Data:",
                   rx_message.identifier, rx_message.data_length_code);
            for (int i = 0; i < rx_message.data_length_code; i++) {
                printf(" %02X", rx_message.data[i]);
            }

            // Interpret as distance if correct ID
            if (rx_message.identifier == 0x100 && rx_message.data_length_code == 2) {
                float distance = rx_message.data[0] + (rx_message.data[1] / 100.0);
                if (distance == 0) {
                    printf(" -> No Object Detected");
                } else {
                    printf(" -> Distance: %.2f cm", distance);
                }
            }

            printf("\n");

        } else if (err == ESP_ERR_TIMEOUT) {
            printf("No CAN message received in the last 2 seconds.\n");
        } else {
            printf("CAN receive error: %s\n", esp_err_to_name(err));
        }
    }
}
