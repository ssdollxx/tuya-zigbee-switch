#ifndef _DEVICE_INIT_H_
#define _DEVICE_INIT_H_

#include "base_components/led.h"
#include "base_components/button.h"
#include "base_components/relay.h"
#include "zigbee/endpoint.h"

extern zigbee_endpoint main_endpoint;

void init_zcl_endpoints();
void init_reporting();

#endif