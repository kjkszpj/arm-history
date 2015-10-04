foo:	foo.c
	arm-unknown-eabi-gcc -Wall -fno-builtin -nostdinc -nostdlib -march=armv7-a -mlittle-endian -I. -I./include -I./include/arch/armv7a-le -I./include/arch/armv7a-le/asm/mach-zynq7000 -DMACH_ZYNQ7000  -c foo.c -o foo.o && arm-unknown-eabi-ld -T preload.ld foo.o -o a.out
clean:
	rm a.out foo.o
