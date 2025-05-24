#include "peripherals.h"

// STEPPER PINS
#define STP_IN1 GPIO_NUM_18
#define STP_IN2 GPIO_NUM_19
#define STP_IN3 GPIO_NUM_21
#define STP_IN4 GPIO_NUM_22

// ULTRASONIC SENSOR PINS
#define USS_ECHO GPIO_NUM_17
#define USS_TRIG GPIO_NUM_16
 
void setup(void) {
    gpio_output_enable(STP_IN4); gpio_set_level(STP_IN4, 0);
    gpio_output_enable(STP_IN3); gpio_set_level(STP_IN3, 0);
    gpio_output_enable(STP_IN2); gpio_set_level(STP_IN2, 0);
    gpio_output_enable(STP_IN1); gpio_set_level(STP_IN1, 0);
    gpio_input_enable(USS_ECHO); // ECHO PIN INPUT
    gpio_output_enable(USS_TRIG); // TRIG PIN OUTPUT
    gpio_set_level(USS_TRIG, 0); // TRIG PIN LOW
}

void app_main(void)
{
    setup(); 

    while (1) {
        // Rotate stepper motor to obtain boundaries from sensor
        STEPPER_PULSE(STP_IN1, STP_IN2, STP_IN3, STP_IN4);
    }
}
