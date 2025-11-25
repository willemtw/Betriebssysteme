#ifndef GPIO_H
#define GPIO_H
#include <stddef.h>

#define GPIO_BASE (0x7E200000 - 0x3F000000)
#define GPF_BITS  3u
#define GPF_MASK  0b111u

enum gpio_func {
	gpio_input  = 0x0,
	gpio_output = 0x1,
};

struct gpio {
	unsigned int func[6];
	unsigned int unused0;
	unsigned int set[2];
	unsigned int unused1;
	unsigned int clr[2];
};
static_assert(offsetof(struct gpio, func) == 0x00);
static_assert(offsetof(struct gpio, set) == 0x1C);
static_assert(offsetof(struct gpio, clr) == 0x28);

#define GPIO ((volatile struct gpio *)GPIO_BASE)

#endif
