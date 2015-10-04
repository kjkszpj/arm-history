int foo(int x)
{
	void (*puts)(const char *) = (void *)0x1FF0000C;

	puts("FW: Hello kernel\r\n");
	while (1);
	return 0;
}
