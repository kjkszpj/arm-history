foo:	foo.c
	arm-unknown-eabi-gcc -Wall -fno-builtin -nostdinc -nostdlib -march=armv7-a -mlittle-endian -I. -I./include -I./include/arch/armv7a-le -I./include/arch/armv7a-le/asm/mach-zynq7000 -DMACH_ZYNQ7000  -c foo.c -o foo.o && arm-unknown-eabi-ld -T foo.ld foo.o -o a.out
	
clean:
	rm a.out foo.o
	
load_elf:	load_elf.c
	arm-unknown-eabi-gcc -Wall -fno-builtin -nostdinc -nostdlib -march=armv7-a -mlittle-endian -I. -I./include -I./include/arch/armv7a-le -I./include/arch/armv7a-le/asm/mach-zynq7000 -DMACH_ZYNQ7000  -c load_elf.c -o load_elf.o && arm-unknown-eabi-ld -T load_elf.ld load_elf.o -o load_elf.elf
	
