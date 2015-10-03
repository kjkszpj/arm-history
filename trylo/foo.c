int foo(int x)
{
	void (*puts)(const char *str) = (void *)0x1ff0025c;
	puts("Hello Kernel!\r\n");
}
