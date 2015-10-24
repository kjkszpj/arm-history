void foo(void)
{
	void (*puts)(const char *) = (void *)0x1FF0000C;

	puts("Hello from sda2 Kernel!\r\n");
	while (1);
	return;
}
