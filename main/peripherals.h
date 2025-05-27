
#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "xtensa/core-macros.h"
#include "esp_rom_sys.h"
#include "esp_log.h"

#define STEPPER_DELAY_ms 10

// STEPPER MOTOR PULSE FUNCTION
// 4 STEP COMMAND - IN1:A, IN2:B, IN3:C, IN4:D | AB-BC-CD-DA FOR 1 STEP
void STEPPER_PULSE (gpio_num_t IN1, gpio_num_t IN2, gpio_num_t IN3, gpio_num_t IN4) {
    gpio_set_level(IN1, 1);
    gpio_set_level(IN2, 1);
    gpio_set_level(IN3, 0);
    gpio_set_level(IN4, 0); // AB
    vTaskDelay(pdMS_TO_TICKS(STEPPER_DELAY_ms));

    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 1);
    gpio_set_level(IN3, 1); 
    gpio_set_level(IN4, 0); // BC
    vTaskDelay(pdMS_TO_TICKS(STEPPER_DELAY_ms));

    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 0);
    gpio_set_level(IN3, 1); 
    gpio_set_level(IN4, 1); // CD
    vTaskDelay(pdMS_TO_TICKS(STEPPER_DELAY_ms));

    gpio_set_level(IN1, 1);
    gpio_set_level(IN2, 0);
    gpio_set_level(IN3, 0); 
    gpio_set_level(IN4, 1); // DA
    vTaskDelay(pdMS_TO_TICKS(STEPPER_DELAY_ms));
}

// ULTRASONIC SENSOR ONE-TIME DISTANCE FUNCTION
uint32_t USS_GET_DISTANCE(gpio_num_t ECHO, gpio_num_t TRIG) {
    bool got_echo = false;

    // Send a 10 microsecond signal to ultrasonic sensor
    gpio_set_level(TRIG, 1); // Send HIGH to TRIG
    esp_rom_delay_us(10); 
    gpio_set_level(TRIG, 0); // Stop signal

    // Check for input in ECHO -- if HIGH, then the sensor has started
    while (gpio_get_level(ECHO) != 1) {  }

    uint32_t init_CPU_CYCLES; // ECHO started, start cycle count
    if (gpio_get_level(ECHO) == 1) {
        got_echo = true;
        init_CPU_CYCLES = xthal_get_ccount(); 
        while (gpio_get_level(ECHO) == 1) {
            if ((xthal_get_ccount() - init_CPU_CYCLES) > 9120000) { // 38 ms max echo duration
                ESP_LOGW("ULTRASONIC_SENSOR", "NO_SIGNAL");
                got_echo = false;
                break;
            }
        }
    }

    if (got_echo) {
        float time_uS = (xthal_get_ccount() - init_CPU_CYCLES) / 240.0; // Based on 240 Mhz
        uint32_t distance_cm = (time_uS * 0.0343) / 2; // Speed of sound and round-trip
        return distance_cm;
    } else { ESP_LOGW("ULTRASONIC_SENSOR", "FAILED_MEASUREMENT"); return 0;}
}

// DC MOTOR LEFT DRIVE
void LEFT_DRIVE(gpio_num_t IN1, gpio_num_t IN2, int32_t speed) {
    if (speed > 0) {
        // IN1 = PWM, IN2 = LOW
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, speed);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
        gpio_set_level(IN2, 0);
    } else if (speed < 0) {
        // IN1 = LOW, IN2 = PWM
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, -speed);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
        gpio_set_level(IN1, 0);
    } else {
        gpio_set_level(IN1, 0);
        gpio_set_level(IN2, 0);
    }
}

// DC MOTOR RIGHT DRIVE
void RIGHT_DRIVE(gpio_num_t IN3, gpio_num_t IN4, int32_t speed) {
    if (speed > 0) {
        // IN3 = PWM, IN4 = LOW
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2, speed);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_2);
        gpio_set_level(IN4, 0);
    } else if (speed < 0) {
        // IN3 = LOW, IN4 = PWM
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3, -speed);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_3);
        gpio_set_level(IN3, 0);
    } else {
        gpio_set_level(IN3, 0);
        gpio_set_level(IN4, 0);
    }
}

#endif // PERIPHERALS_H