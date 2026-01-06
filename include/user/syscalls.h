/*
* Syscall calling convention:
* - The number of the syscall to execute shall be passed in r0.
* - The svc instruction immediate is ignored.
* - Parameters of system calls are to be passed in r1 - r4.
* - The syscall may or may not return a value in r0.
* - Some syscalls may write to pointers passed as arguments.
* - Any syscall may overwrite r0 - r4.
*/

#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stdint.h>

#define SYSCALL_EXIT_NR		 0
#define SYSCALL_CREATE_THREAD_NR 1
#define SYSCALL_GETC_NR		 2
#define SYSCALL_PUTC_NR		 3
#define SYSCALL_SLEEP_NR	 4
#define SYSCALL_KERNEL_EXIT_NR	 5
#define SYSCALL_UNDEFINED_NR	 6

enum Syscall {
	SYSCALL_EXIT	      = 0,
	SYSCALL_CREATE_THREAD = 1,
	SYSCALL_GETC	      = 2,
	SYSCALL_PUTC	      = 3,
	SYSCALL_SLEEP	      = 4,
	SYSCALL_KERNEL_EXIT   = 5,
	SYSCALL_UNDEFINED     = 6,
};

void syscall_exit(void);
void syscall_create_thread(void (*f)(void *), void *args, uint32_t arg_size);
char syscall_getc(void);
void syscall_putc(char c);
void syscall_sleep(uint32_t cycles);
void syscall_undefined(void);
void syscall_kernel_exit(void);

#endif // SYSCALLS_H
