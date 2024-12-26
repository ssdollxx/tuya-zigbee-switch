#include "button.h"
#include "tl_common.h"
#include "millis.h"


void btn_update(button_t *button, u8 is_pressed) {
    u32 now = millis();
    if (!button->pressed && is_pressed) {
        printf("Press detected\r\n");
        button->pressed_at_ms = now;
        if (button->on_press != NULL) {
            button->on_press(button->callback_param);
        }
        if (now - button->released_at_ms < button->multi_press_duration_ms) {
            button->multi_press_cnt += 1;
            printf("Multi press detected: %d\r\n", button->multi_press_cnt);
            if (button->on_multi_press != NULL) {
                 button->on_multi_press(button->callback_param, button->multi_press_cnt);
            }
        } else {
            button->multi_press_cnt = 0;
        }
    } else if (button->pressed && !is_pressed) {
        printf("Release detected\r\n");
        button->long_pressed = false;
        button->released_at_ms = now;
        if (button->on_release != NULL) {
            button->on_release(button->callback_param);
        }
    }
    button->pressed = is_pressed;
    if (is_pressed && !button->long_pressed && (button->long_press_duration_ms < (now - button->pressed_at_ms))) {
        button->long_pressed = true;
        printf("Long press detected\r\n");
        if (button->on_long_press != NULL) {
            button->on_long_press(button->callback_param);
        }
    };
}


