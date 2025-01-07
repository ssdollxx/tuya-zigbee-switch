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


#define ZCL_MANUFACTURER    "Tuya-TS0011-custom"
#define ZCL_MODEL   "TS0011-custom"

#define RF_TX_POWER_DEF RF_POWER_P10p46dBm


// LED
#define GPIO_LED			GPIO_PD7
#define LED_ON 				1
#define LED_OFF				0

#define PD7_INPUT_ENABLE	0
#define PD7_DATA_OUT		0
#define PD7_OUTPUT_ENABLE	1
#define PD7_FUNC			AS_GPIO

// Button on board
#define ON_BOARD_BUTTON	    GPIO_PA0
#define PA0_INPUT_ENABLE	1
#define PA0_DATA_OUT		0
#define PA0_OUTPUT_ENABLE	0
#define PA0_FUNC			AS_GPIO

// Button S1
#define S1_BUTTON	    GPIO_PC2
#define PC2_INPUT_ENABLE	1
#define PC2_DATA_OUT		0
#define PC2_OUTPUT_ENABLE	0
#define PC2_FUNC			AS_GPIO

enum{
    KEY_ON_BOARD = 0x01,
    KEY_S1 = 0x02,
    KEY_S2 = 0x03
};


#define KB_MAP_NORMAL   {\
        {KEY_ON_BOARD,}, \
        {KEY_S1,}, }

#define KB_MAP_NUM      KB_MAP_NORMAL
#define KB_MAP_FN       KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {ON_BOARD_BUTTON,  S1_BUTTON}


// Control S1
#define GPIO_S1_PWR 	    GPIO_PC0
#define PC0_INPUT_ENABLE	0
#define PC0_DATA_OUT		0
#define PC0_OUTPUT_ENABLE	1
#define PC0_FUNC			AS_GPIO


#define PWR_ON              1
#define PWR_OFF             0


#define BAUDRATE            115200
#define	DEBUG_INFO_TX_PIN	GPIO_PB1 //print


// Enable tuya ota

#define ZIGBEE_TUYA_OTA 	1


#define SWITCH_CLUSTERS 1
#define RELAY_CLUSTERS 1


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif // (BOARD == BOARD_CGDK2)