#ifndef _LED_H_
#define _LED_H_

#include "types.h"

typedef struct{
    u32 pin;
    u8 on_high;
    u8 on;
    u16 blink_times_left;
    u16 blink_time_on;
    u16 blink_time_off;
    u32 blink_switch_counter;
    u32 last_update;
} led_t;


/**
 * @brief      Initialize led (set initial state)
 * @param	   *led - Led to use
 * @return     none
 */
void led_init(led_t *led);

/**
 * @brief      Led update function, used for blinking support
 * @param	   *led - Led to use
 * @return     none
 */
void led_update(led_t *led);

/**
 * @brief      Turn on led, canceling any blinking
 * @param	   *led - Led to use
 * @return     none
 */
void led_on(led_t *led);

/**
 * @brief      Turn off led, canceling any blinking
 * @param	   *led - Led to use
 * @return     none
 */
void led_off(led_t *led);


#define LED_BLINK_FOREVER 0xFFFF

/**
 * @brief      Start led blinking, will go to off when finished 
 * @param	   *led - Led to use
 *             on_time_ms - Time led should be on in milliseconds
 *             off_time_ms - Time led should be off in milliseconds
 *             times - Times to repeat blink before returning to fixed state,
 *                     0xFFFF - blink forever
 * @return     none
 */
void led_blink(led_t *led, u16 on_time_ms, u16 off_time_ms, u16 times);


#endif
