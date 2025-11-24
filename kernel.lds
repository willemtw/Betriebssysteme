ENTRY(_start)
SECTIONS
{
	. = 0x00008000;
	.init : { *(.init) }
	.text : { *(.text) }
	.rodata : { *(.rodata) }
	. = ALIGN(1<<20);
	.data : { *(.data) }
	.bss  : { *(.bss)  }

	. = ALIGN(8);
	.stack (NOLOAD) : {
		*(.stack.fiq)
		fiq_stack_top = .;
		*(.stack.irq)
		irq_stack_top = .;
		*(.stack.abt)
		abt_stack_top = .;
		*(.stack.und)
		und_stack_top = .;
		*(.stack.sys)
		sys_stack_top = .;
		*(.stack.svc)
		svc_stack_top = .;
	}
}
