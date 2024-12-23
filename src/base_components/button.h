#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "types.h"

typedef void (*ev_button_callback_t)(void*);

typedef struct{
    u8 pressed;
    u8 long_pressed;
	u32 pressed_at_ms;
    u32 long_press_duration_ms;
    ev_button_callback_t on_press;
    ev_button_callback_t on_long_press;
    ev_button_callback_t on_release;
    void *callback_param;
}button_t;


void btn_update(button_t *button, u8 is_pressed);

#endif

