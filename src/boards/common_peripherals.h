#ifndef _COMMON_PERIPHERALS_H_
#define _COMMON_PERIPHERALS_H_

#include "base_components/led.h"
#include "base_components/button.h"
#include "base_components/relay.h"


extern led_t led;

extern button_t button_on_board;

void periferals_update();


#endif