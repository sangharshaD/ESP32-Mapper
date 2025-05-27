#include "peripherals.h"

// STEPPER PINS
#define STP_IN1 GPIO_NUM_18
#define STP_IN2 GPIO_NUM_19
#define STP_IN3 GPIO_NUM_21
#define STP_IN4 GPIO_NUM_22

// ULTRASONIC SENSOR PINS
#define USS_ECHO GPIO_NUM_17
#define USS_TRIG GPIO_NUM_16

// DC MOTOR DRIVER PINS
#define PWN_IN1 GPIO_NUM_23 // A
#define PWN_IN2 GPIO_NUM_25
#define PWN_IN3 GPIO_NUM_26 // B
#define PWN_IN4 GPIO_NUM_27

#define PWM_FREQ_HZ       1000
#define PWM_RES_BITS      LEDC_TIMER_8_BIT
#define PWM_MODE          LEDC_HIGH_SPEED_MODE

// LEDC CONFIG FOR L298N H BRIDGE DRIVER MODULE
ledc_timer_config_t TIMER = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = PWM_RES_BITS,
        .freq_hz = PWM_FREQ_HZ,
        .clk_cfg = LEDC_AUTO_CLK
};
ledc_channel_config_t CHANNEL1 = { // OUT 1
        .gpio_num = PWN_IN1,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
};
ledc_channel_config_t CHANNEL2 = { // OUT 2
        .gpio_num = PWN_IN2,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
};
ledc_channel_config_t CHANNEL3 = { // OUT 3
        .gpio_num = PWN_IN3,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_2,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
};
ledc_channel_config_t CHANNEL4 = { // OUT 4
        .gpio_num = PWN_IN4,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_3,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
};
 
void setup(void) {
    gpio_output_enable(GPIO_NUM_23); gpio_set_level(GPIO_NUM_23, 1);
    gpio_output_enable(STP_IN4); gpio_set_level(STP_IN4, 0);
    gpio_output_enable(STP_IN3); gpio_set_level(STP_IN3, 0);
    gpio_output_enable(STP_IN2); gpio_set_level(STP_IN2, 0);
    gpio_output_enable(STP_IN1); gpio_set_level(STP_IN1, 0);
    gpio_input_enable(USS_ECHO); // ECHO PIN INPUT
    gpio_output_enable(USS_TRIG); // TRIG PIN OUTPUT
    gpio_set_level(USS_TRIG, 0); // TRIG PIN LOW

    ledc_timer_config(&TIMER);
    ledc_channel_config(&CHANNEL1);
    ledc_channel_config(&CHANNEL2);
    ledc_channel_config(&CHANNEL3);
    ledc_channel_config(&CHANNEL4);
}

void app_main(void)
{
    setup(); 

    while (1) {
        // Rotate stepper motor to obtain boundaries from sensor
        STEPPER_PULSE(STP_IN1, STP_IN2, STP_IN3, STP_IN4);
    }
}
