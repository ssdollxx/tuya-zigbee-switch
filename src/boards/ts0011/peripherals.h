#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

#include "base_components/led.h"
#include "base_components/button.h"
#include "base_components/relay.h"

extern led_t led;
extern relay_t relay1;

extern button_t button_on_board;
extern button_t button_s1;

void periferals_update();


#endif
