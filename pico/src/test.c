#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

bool flip = false;

void gpio_callback(uint gpio, uint32_t events) 
{
	printf("Pressed \n");
	if(flip == false) gpio_put(1, 1);
	else gpio_put(1, 0);

	flip = !flip;
}

int main() {
	stdio_init_all();
	gpio_init(1);
	gpio_set_dir(1, GPIO_OUT);

	printf("Beginning program \n");

	gpio_set_irq_enabled_with_callback(0, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

	while(1);
	return 0;
}
