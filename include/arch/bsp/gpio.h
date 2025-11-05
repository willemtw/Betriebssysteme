#ifndef GPIO_H
#define GPIO_H
#include <stddef.h>

static constexpr unsigned int GPIO_BASE = 0x7E200000 - 0x3F000000;
static constexpr unsigned int GPF_BITS	= 3u;
static constexpr unsigned int GPF_MASK	= 0b111u;

enum gpio_func {
	gpio_input  = 0x0,
	gpio_output = 0x1,
};

struct gpio_t {
	unsigned int func[6];
	unsigned int unused0;
	unsigned int set[2];
	unsigned int unused1;
	unsigned int clr[2];
};
static_assert(offsetof(struct gpio_t, func) == 0x00);
static_assert(offsetof(struct gpio_t, set) == 0x1C);
static_assert(offsetof(struct gpio_t, clr) == 0x28);

static volatile struct gpio_t *const gpio_port = (struct gpio_t *)GPIO_BASE;

#endif
