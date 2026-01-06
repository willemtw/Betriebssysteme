#include <user/syscalls.h>
#include <stdint.h>

static inline uint32_t syscall0(enum Syscall sc)
{
	register uint32_t r0 asm("r0") = sc;
	asm volatile("svc #0" : "+r"(r0) : : "r1", "r2", "r3", "r4", "lr", "memory");
	return r0;
}

static inline uint32_t syscall1(enum Syscall sc, uint32_t p1)
{
	register uint32_t r0 asm("r0") = sc;
	register uint32_t r1 asm("r1") = p1;
	asm volatile("svc #0" : "+r"(r0) : "r"(r1) : "r2", "r3", "r4", "lr", "memory");
	return r0;
}

static inline uint32_t syscall2(enum Syscall sc, uint32_t p1, uint32_t p2)
{
	register uint32_t r0 asm("r0") = sc;
	register uint32_t r1 asm("r1") = p1;
	register uint32_t r2 asm("r2") = p2;
	asm volatile("svc #0" : "+r"(r0) : "r"(r1), "r"(r2) : "r3", "r4", "lr", "memory");
	return r0;
}

static inline uint32_t syscall3(enum Syscall sc, uint32_t p1, uint32_t p2, uint32_t p3)
{
	register uint32_t r0 asm("r0") = sc;
	register uint32_t r1 asm("r1") = p1;
	register uint32_t r2 asm("r2") = p2;
	register uint32_t r3 asm("r3") = p3;
	asm volatile("svc #0" : "+r"(r0) : "r"(r1), "r"(r2), "r"(r3) : "r4", "lr", "memory");
	return r0;
}

static inline uint32_t syscall4(enum Syscall sc, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4)
{
	register uint32_t r0 asm("r0") = sc;
	register uint32_t r1 asm("r1") = p1;
	register uint32_t r2 asm("r2") = p2;
	register uint32_t r3 asm("r3") = p3;
	register uint32_t r4 asm("r4") = p4;
	asm volatile("svc #0" : "+r"(r0) : "r"(r1), "r"(r2), "r"(r3), "r"(r4) : "lr", "memory");
	return r0;
}

void syscall_exit(void)
{
	syscall0(SYSCALL_EXIT);
}

void syscall_create_thread(void (*f)(void *), void *args, uint32_t arg_size)
{
	syscall3(SYSCALL_CREATE_THREAD, (uint32_t)f, (uint32_t)args, (uint32_t)arg_size);
}

char syscall_getc(void)
{
	return syscall0(SYSCALL_GETC);
}

void syscall_putc(char c)
{
	syscall1(SYSCALL_PUTC, (uint32_t)c);
}

void syscall_sleep(uint32_t cycles)
{
	syscall1(SYSCALL_SLEEP, cycles);
}

void syscall_kernel_exit(void)
{
	syscall0(SYSCALL_KERNEL_EXIT);
}

void syscall_undefined(void)
{
	syscall0(SYSCALL_UNDEFINED);
}
