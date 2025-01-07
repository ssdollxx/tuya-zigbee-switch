#include "peripherals.h"
#include "board_cfg.h"
#include "tl_common.h"

led_t led = {
	.pin = GPIO_LED,
	.on_high = LED_ON,
};

relay_t relay1 = {
    .pin = GPIO_S1_PWR,
    .on_high = PWR_ON
};


button_t button_on_board = {
	.long_press_duration_ms = 2000,
	.multi_press_duration_ms = 800,
};

button_t button_s1  = {
	.long_press_duration_ms = 2000,
	.multi_press_duration_ms = 800,
};


button_t *buttons[] = {
	&button_on_board, &button_s1,
};

// Convert key scan updates into proper calls it btn_update
void buttons_update(void) {
	bool pressed[2] = {false, false};

    if(kb_scan_key(0, 1)){
		for (int i = 0; i < kb_event.cnt; i++) {
			pressed[kb_event.keycode[i]-1]= true;
		}
    } else {
		for (int i = 0; i < 2; i++) {
			pressed[i] = buttons[i]->pressed;
		}
	}
	for (int i = 0; i < 2; i++) {
		btn_update(buttons[i], pressed[i]);
	}
}

void periferals_update() {
    led_update(&led);
    buttons_update();
}