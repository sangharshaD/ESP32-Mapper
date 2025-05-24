
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

void app_main(void)
{
    gpio_output_enable(GPIO_NUM_18);
    gpio_set_level(GPIO_NUM_18, 1);
    printf("Helo");
}
