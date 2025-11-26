#include <arch/cpu/modes.h>

const char *get_mode_name(enum cpu_mode mode)
{
	switch (mode) {
	case CPU_MODE_USR_RAW:
		return "User";
	case CPU_MODE_FIQ_RAW:
		return "FIQ";
	case CPU_MODE_IRQ_RAW:
		return "IRQ";
	case CPU_MODE_SVC_RAW:
		return "Supervisor";
	case CPU_MODE_ABT_RAW:
		return "Abort";
	case CPU_MODE_UND_RAW:
		return "Undefined";
	case CPU_MODE_SYS_RAW:
		return "System";
	default:
		return "Invalid";
	}
}
