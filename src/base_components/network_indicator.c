#include "network_indicator.h"
#include "tl_common.h"
#include "millis.h"


void network_indicator_connected(network_indicator_t *indicator)
{
  led_t **led = indicator->leds;

  while (*led != NULL)
  {
    (*led)->blink_times_left = 0;
    if (indicator->keep_on_after_connect)
    {
      led_on(*led);
    }
    led++;
  }
}

void network_indicator_commission_success(network_indicator_t *indicator)
{
  led_t **led = indicator->leds;

  while (*led != NULL)
  {
    led_blink(*led, 500, 500, 7);
    led++;
  }
}

void network_indicator_not_connected(network_indicator_t *indicator)
{
  led_t **led = indicator->leds;

  while (*led != NULL)
  {
    if ((*led)->blink_times_left != LED_BLINK_FOREVER)
    {
      led_blink(*led, 500, 500, LED_BLINK_FOREVER);
    }
    led++;
  }
}
