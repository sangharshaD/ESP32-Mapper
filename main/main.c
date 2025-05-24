
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include "esp_log.h"

void app_main(void)
{
    gpio_input_enable(GPIO_NUM_17);
    gpio_output_enable(GPIO_NUM_16);

    while (1) {
        // Rotate stepper motor to obtain boundaries from sensor
        for (volatile uint16_t i=0; i<360; i++) {
            // Send a 10 microsecond signal to ultrasonic sensor
            gpio_set_level(GPIO_NUM_16, 1); // Set pin 16 HIGH
            esp_rom_delay_us(10); // Delay 10 microseconds to prolong output
            gpio_set_level(GPIO_NUM_16, 0); // Set pin 16 LOW

            volatile uint32_t init_CPU_CYCLES = get_ccount(); // Get current CPU cycles
            // Check for input in pin 17
            while (gpio_get_level(GPIO_NUM_17) != 1) {
                if ((get_ccount() - init_CPU_CYCLES) > 14400) { // 14400000 cycles is 60 microseconds
                    break;
                }
            }
            // 58 centimeters per microsecond
            volatile uint32_t range_cm = 58 * (get_ccount()-init_CPU_CYCLES) / 240;
            printf("&d", range_cm);
            vTaskDelay(pdMS_TO_TICKS(100)); // Delay for testing purposes
        }
    }
}
