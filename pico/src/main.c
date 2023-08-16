#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/binary_info.h"
#include <inttypes.h>

short BUTTON_PIN 	= 0;
short LED_PIN 		= 1;

// Flags
bool LED_ON 		= false;
bool BUTTON_HANDLED	= false;

// GLOBALS
absolute_time_t 	pressed_ts;

void gpio_released_callback(uint gpio, uint32_t events)
{
	absolute_time_t timestamp = get_absolute_time(); 
	
	if( events & GPIO_IRQ_EDGE_FALL )
	{
		pressed_ts = timestamp;
		printf("PUSHED %lld \n", timestamp);

		LED_ON = true;		// We flag the LED to turn on, since the button is pressed
	}

	if ( events & GPIO_IRQ_EDGE_RISE )
	{
		uint64_t delay_microseconds = absolute_time_diff_us( pressed_ts, timestamp );
		uint32_t delay_ms = delay_microseconds / 1000;	// A delay in milliseconds


		if (delay_ms < 10) return;		// A software debouncer to ignore delays of less than 10ms
		
		printf("RELEASED %lld DELAY %ld\n", timestamp, delay_ms );
		LED_ON = false;		// We flag the LED to switch off
	}
	
	BUTTON_HANDLED = false;		// New state around the LED requires the normal thread loop to update the physical LED output
}

int main() {
	stdio_init_all();

	// Initialising the LED_PIN GPIO to act as a digital output
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	printf("Beginning program \n");

	// Upon the BUTTON_PIN being lifted or dropped, a callback will run to handle that press 
	gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_released_callback);

	while(true)
	{
		// Handles when a button has been pushed and a new update to the physical LED state is needed
		if(BUTTON_HANDLED == false)
		{
			if(LED_ON == true) gpio_put(LED_PIN, 1);		// If the LED is off, we turn it on
			else gpio_put(LED_PIN, 0);				// Otherwise it's turned on

			LED_ON = !LED_ON;	// Toggle the flag for if the LED is on
			BUTTON_HANDLED = true;	// Reset flag for if the button was handled
		} 
	}

	return 0;
}
