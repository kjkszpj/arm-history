foo:	foo.c
	arm-unknown-eabi-gcc -Wall -fno-builtin -nostdinc -nostdlib -march=armv7-a -mlittle-endian -I. -I./include -I./include/arch/armv7a-le -I./include/arch/armv7a-le/asm/mach-zynq7000 -DMACH_ZYNQ7000  -c foo.c -o foo.o && arm-unknown-eabi-ld -T foo.ld foo.o -o a.out
	
clean:
	rm a.out foo.o
	
load_elf:	load_elf.c
	cd .. && \
	arm-unknown-eabi-gcc -Wall -fno-builtin -nostdinc -nostdlib -march=armv7-a -mlittle-endian -I. -I./include -I./include/arch/armv7a-le -I./include/arch/armv7a-le/asm/mach-zynq7000 -DMACH_ZYNQ7000  -c trylo/load_elf.c -o trylo/load_elf.o && \
	cd trylo/ && \
	arm-unknown-eabi-ld -T load_elf.ld load_elf.o -o load_elf.elf && \
	arm-unknown-eabi-objcopy -O binary -S load_elf.elf temp