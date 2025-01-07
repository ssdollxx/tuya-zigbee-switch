/********************************************************************************************************
 * @file    app_cfg.h
 *
 * @brief   This is the header file for app_cfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/
#ifndef _APP_CFG_H_
#define _APP_CFG_H_

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


/**********************************************************************
 * Version configuration
 */
#include "version_cfg.h"

/**********************************************************************
 * Product Information
 */
/* Debug mode config */
#define	DEBUG_ENABLE					0 // lcd = DeviceSysException

#define	UART_PRINTF_MODE				1
#define USB_PRINTF_MODE         		0

/* PM */
#define PM_ENABLE						0

/* PA */
#define PA_ENABLE						0

#define CLOCK_SYS_CLOCK_HZ  		24000000 //48000000

#define SENSOR_SHTC3_4X	1
#define SENSOR_CHT8305	2
#define SENSOR_SHT30	3

#if BOARD == BOARD_TS0001
	#include "boards/ts0001/board_cfg.h"
#elif BOARD == BOARD_TS0012
	#include "boards/ts0012/board_cfg.h"
#elif BOARD == BOARD_TS0011
	#include "boards/ts0011/board_cfg.h"
#elif BOARD == BOARD_TS0002
	#include "boards/ts0002/board_cfg.h"
#endif

#ifndef ZIGBEE_TUYA_OTA
#define ZIGBEE_TUYA_OTA 	0
#endif


/* Watch dog module */
#define MODULE_WATCHDOG_ENABLE						0

/* UART module */
#define	MODULE_UART_ENABLE							0

#if (ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID || ZBHCI_UART)
	#define ZBHCI_EN								1
#endif


/**********************************************************************
 * ZCL cluster support setting
 */
#define ZCL_POWER_CFG_SUPPORT						0
#define ZCL_ON_OFF_SUPPORT							1
#define ZCL_ONOFF_CONFIGUATION					    1
//#define ZCL_IAS_ZONE_SUPPORT						1
#define ZCL_POLL_CTRL_SUPPORT						0
#define ZCL_GROUP_SUPPORT							0
#define ZCL_OTA_SUPPORT								1
#define TOUCHLINK_SUPPORT							0
#define FIND_AND_BIND_SUPPORT						0
#define REJOIN_FAILURE_TIMER						1

#define GP_SUPPORT_ENABLE				            1



#define VOLTAGE_DETECT_ADC_PIN          0

/**********************************************************************
 * Stack configuration
 */
#include "configs/zb_config.h"
#include "configs/stack_cfg.h"


/**********************************************************************
 * EV configuration
 */
typedef enum{
	EV_POLL_ED_DETECT,
	EV_POLL_HCI,
    EV_POLL_IDLE,
	EV_POLL_MAX,
}ev_poll_e;


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

#endif // _APP_CFG_H_
