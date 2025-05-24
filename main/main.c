
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "xtensa/core-macros.h"
#include "esp_rom_sys.h"
#include "esp_log.h"

void app_main(void)
{
    gpio_input_enable(GPIO_NUM_17); // Pin 17 is input
    gpio_output_enable(GPIO_NUM_16); // Pin 16 is output

    bool got_echo = false;

    while (1) {
        // Rotate stepper motor to obtain boundaries from sensor
            // Send a 10 microsecond signal to ultrasonic sensor
            gpio_set_level(GPIO_NUM_16, 1); // Set pin 16 HIGH
            esp_rom_delay_us(10); // Delay 10 microseconds to prolong output
            gpio_set_level(GPIO_NUM_16, 0); // Set pin 16 LOW

            // Check for input in pin 17 -- if HIGH, then the sensor has started
            while (gpio_get_level(GPIO_NUM_17) != 1) { printf("-"); }

            uint32_t init_CPU_CYCLES;
            // If ECHO low after 38 milliseconds, no signal found
            if (gpio_get_level(GPIO_NUM_17) == 1) {
                got_echo = true;
                init_CPU_CYCLES = xthal_get_ccount(); // ECHO started
                while (gpio_get_level(GPIO_NUM_17) == 1) {
                    if ((xthal_get_ccount() - init_CPU_CYCLES) > 9120000) { // 38 ms max echo duration
                        printf("NO_SIGNAL");
                        got_echo = false;
                        break;
                    }
                }
            }

            if (got_echo) {
                // 58 centimeters per microsecond
                // uint32_t range_cm = 58 * (xthal_get_ccount()-init_CPU_CYCLES) / 240;
                float time_uS = (xthal_get_ccount() - init_CPU_CYCLES) / 240.0;
                uint32_t distance_cm = (time_uS * 0.0343) / 2;
                printf("%ld", distance_cm);
            } else {
                printf("NO_ECHO");
            }
            vTaskDelay(pdMS_TO_TICKS(100)); // Delay for testing purposes
    }
}
