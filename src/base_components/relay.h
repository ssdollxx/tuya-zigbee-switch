#ifndef _RELAY_H_
#define _RELAY_H_

#include "types.h"

typedef struct{
    u32 pin;
    u8 on_high;
    u8 on;
} relay_t;


/**
 * @brief      Initialize relay (set initial state)
 * @param	   *relay - Relay to use
 * @return     none
 */
void relay_init(relay_t *relay);


/**
 * @brief      Enable the relay
 * @param	   *relay - Relay to use
 * @return     none
 */
void relay_on(relay_t *relay);

/**
 * @brief      Disable the relay
  * @param	   *relay - Relay to use
 * @return     none
 */
void relay_off(relay_t *relay);

/**
 * @brief      Close the relay
  * @param	   *relay - Relay to use
 * @return     none
 */
void relay_toggle(relay_t *relay);

#endif
