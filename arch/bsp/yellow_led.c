#include <arch/bsp/yellow_led.h>
#include <stddef.h>
#include <arch/bsp/gpio.h>

#define YELLOW_LED	     7u
#define YELLOW_LED_GPF_SHIFT (YELLOW_LED * GPF_BITS)

void init_yellow(void)
{
	GPIO->func[0] = (GPIO->func[0] & ~(GPF_MASK << YELLOW_LED_GPF_SHIFT)) |
			(gpio_output << YELLOW_LED_GPF_SHIFT);
}

void yellow_on(void)
{
	GPIO->set[0] = 1 << YELLOW_LED;
}

void yellow_off(void)
{
	GPIO->clr[0] = 1 << YELLOW_LED;
}
