/********************************************************************************************************
 * @file    board_cgdk2.h
 *
 * @brief   This is the header file for board_cgdk2
 *
 *******************************************************************************************************/
#ifndef _BOARD_CGDK2_H_
#define _BOARD_CGDK2_H_

#include "version_cfg.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


#define ZCL_MANUFACTURER    "Tuya-TS0001-custom"
#define ZCL_MODEL           "TS0001-custom"

#define RF_TX_POWER_DEF RF_POWER_P10p46dBm


// LED
#define GPIO_LED			GPIO_PD3
#define LED_ON 				1
#define LED_OFF				0

#define PD3_INPUT_ENABLE	0
#define PD3_DATA_OUT		0
#define PD3_OUTPUT_ENABLE	1
#define PD3_FUNC			AS_GPIO

// Button on board
#define ON_BOARD_BUTTON	    GPIO_PB4
#define PB4_INPUT_ENABLE	1
#define PB4_DATA_OUT		0
#define PB4_OUTPUT_ENABLE	0
#define PB4_FUNC			AS_GPIO
#define PULL_WAKEUP_SRC_PB4 PM_PIN_PULLUP_10K

// Button S1
#define S1_BUTTON	        GPIO_PB5
#define PB5_INPUT_ENABLE	1
#define PB5_DATA_OUT		0
#define PB5_OUTPUT_ENABLE	0
#define PB5_FUNC			AS_GPIO
#define PULL_WAKEUP_SRC_PB5 PM_PIN_PULLUP_10K


enum{
    KEY_ON_BOARD = 0x01,
    KEY_S1 = 0x02
};


#define KB_MAP_NORMAL   {\
        {KEY_ON_BOARD,}, \
        {KEY_S1,}, }

#define KB_MAP_NUM      KB_MAP_NORMAL
#define KB_MAP_FN       KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {ON_BOARD_BUTTON,  S1_BUTTON}


// Control S1
#define GPIO_S1_PWR 	    GPIO_PB1
#define PB1_INPUT_ENABLE	0
#define PB1_DATA_OUT		0
#define PB1_OUTPUT_ENABLE	1
#define PB1_FUNC			AS_GPIO

#define PWR_ON              1
#define PWR_OFF             0


#define BAUDRATE            115200
#define	DEBUG_INFO_TX_PIN	GPIO_PA0 //print


// Enable tuya ota

#define ZIGBEE_TUYA_OTA 	1

#define SWITCH_CLUSTERS 1
#define RELAY_CLUSTERS 1


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif // (BOARD == BOARD_CGDK2)