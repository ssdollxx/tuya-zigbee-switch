#include "led.h"
#include "tl_common.h"
#include "millis.h"


void led_init(led_t *led) {
    led_off(led);
}

void led_update(led_t *led){
    u32 now = millis();
    u32 time_carry = now - led->last_update;
    led->last_update = now;
    if (led->blink_times_left == 0) {  
        return;
    }

    while (time_carry >= led->blink_switch_counter) {
        time_carry -= led->blink_switch_counter;
        if (led->on) {
            led->on = 0;
            drv_gpio_write(led->pin, !led->on_high);
            led->blink_switch_counter = led->blink_time_off;
        } else {
            led->on = 1;
            drv_gpio_write(led->pin, led->on_high);
            led->blink_switch_counter = led->blink_time_on;
            if (led->blink_times_left != LED_BLINK_FOREVER) {
                led->blink_times_left--;
                if (led->blink_times_left == 0) {
                    return;
                }
            }
        }
    }
    led->blink_switch_counter -= time_carry;
}


void led_on(led_t *led) {
    drv_gpio_write(led->pin, led->on_high);
    led->on = 1;
    led->blink_times_left = 0;
}

void led_off(led_t *led) {
    drv_gpio_write(led->pin, !led->on_high);
    led->on = 0;
    led->blink_times_left = 0;
}

void led_blink(led_t *led, u16 on_time_ms, u16 off_time_ms, u16 times) {
    drv_gpio_write(led->pin, led->on_high);
    led->blink_time_on = on_time_ms;
    led->blink_time_off = off_time_ms;
    led->blink_times_left = times;
    led->blink_switch_counter = on_time_ms;
    led->last_update = millis();
}