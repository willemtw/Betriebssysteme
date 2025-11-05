#include <arch/bsp/yellow_led.h>
#include <stddef.h>
#include <arch/bsp/gpio.h>

static constexpr unsigned int YELLOW_LED	   = 7u;
static constexpr unsigned int YELLOW_LED_GPF_SHIFT = YELLOW_LED * GPF_BITS;

void init_yellow(void)
{
	gpio_port->func[0] = (gpio_port->func[0] & ~(GPF_MASK << YELLOW_LED_GPF_SHIFT)) |
			     (gpio_output << YELLOW_LED_GPF_SHIFT);
}

void yellow_on(void)
{
	gpio_port->set[0] = 1 << YELLOW_LED;
}

void yellow_off(void)
{
	gpio_port->clr[0] = 1 << YELLOW_LED;
}
