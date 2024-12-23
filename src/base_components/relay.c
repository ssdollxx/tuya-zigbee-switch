#include "relay.h"
#include "tl_common.h"
#include "millis.h"


void relay_init(relay_t *relay) {
    relay_off(relay);
}

void relay_on(relay_t *relay) {
    printf("relay_on\r\n");
    printf("drv_gpio_write %d %d \r\n", relay->pin, relay->on_high);
    drv_gpio_write(relay->pin, relay->on_high);
    relay->on = 1;
}

void relay_off(relay_t *relay) {
    printf("relay_off\r\n");
    printf("drv_gpio_write %d %d \r\n", relay->pin, !relay->on_high);
    drv_gpio_write(relay->pin, !relay->on_high);
    relay->on = 0;
}

void relay_toggle(relay_t *relay) {
	printf("relay addr %d\r\n", relay);
    printf("relay_toggle\r\n");
    if (relay->on) {
        relay_off(relay);
    } else {
        relay_on(relay);
    }
}