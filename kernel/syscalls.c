void syscall_exit(void)
{
	asm volatile("svc 0" ::: "lr");
}
