#ifndef _DEVICE_INIT_H_
#define _DEVICE_INIT_H_

#include "base_components/led.h"
#include "base_components/button.h"
#include "base_components/relay.h"
#include "zigbee/endpoint.h"

#include "config_nv.h"

extern led_t status_led;

extern zigbee_endpoint endpoints[10];


void periferals_update();
void parse_config();
void init_reporting();

#endif