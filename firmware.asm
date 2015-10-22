
firmware.elf：     文件格式 elf32-littlearm


Disassembly of section .vector:

1ff00000 <firmware_vector>:
1ff00000:	ea000005 	b	1ff0001c <firmware_bootasm>
1ff00004:	ea00006b 	b	1ff001b8 <uart_init>
1ff00008:	ea000081 	b	1ff00214 <uart_enable>
1ff0000c:	ea00009d 	b	1ff00288 <uart_spin_puts>
1ff00010:	ea000191 	b	1ff0065c <sd_spin_init_mem_card>
1ff00014:	ea0001f2 	b	1ff007e4 <sd_dma_spin_read>
1ff00018:	ea000213 	b	1ff0086c <sd_dma_spin_write>

Disassembly of section .text:

1ff0001c <firmware_bootasm>:
1ff0001c:	e59fd004 	ldr	sp, [pc, #4]	; 1ff00028 <firmware_bootasm+0xc>
1ff00020:	e1b0b00d 	movs	fp, sp
1ff00024:	eb000022 	bl	1ff000b4 <firmware_bootmain>
1ff00028:	1ff00000 	.word	0x1ff00000

1ff0002c <puthex>:
1ff0002c:	e300c8f4 	movw	ip, #2292	; 0x8f4
1ff00030:	e341cff0 	movt	ip, #8176	; 0x1ff0
1ff00034:	e92d4030 	push	{r4, r5, lr}
1ff00038:	e1a0e000 	mov	lr, r0
1ff0003c:	e8bc000f 	ldm	ip!, {r0, r1, r2, r3}
1ff00040:	e24dd024 	sub	sp, sp, #36	; 0x24
1ff00044:	e28d400c 	add	r4, sp, #12
1ff00048:	e3005908 	movw	r5, #2312	; 0x908
1ff0004c:	e3415ff0 	movt	r5, #8176	; 0x1ff0
1ff00050:	e8a4000f 	stmia	r4!, {r0, r1, r2, r3}
1ff00054:	e1a0300d 	mov	r3, sp
1ff00058:	e8950007 	ldm	r5, {r0, r1, r2}
1ff0005c:	e59c5000 	ldr	r5, [ip]
1ff00060:	e3a0c01c 	mov	ip, #28
1ff00064:	e8a30003 	stmia	r3!, {r0, r1}
1ff00068:	e5c45000 	strb	r5, [r4]
1ff0006c:	e1a04822 	lsr	r4, r2, #16
1ff00070:	e0c320b2 	strh	r2, [r3], #2
1ff00074:	e5c34000 	strb	r4, [r3]
1ff00078:	e1a03c3e 	lsr	r3, lr, ip
1ff0007c:	e28d1020 	add	r1, sp, #32
1ff00080:	e203300f 	and	r3, r3, #15
1ff00084:	e26c201c 	rsb	r2, ip, #28
1ff00088:	e0813003 	add	r3, r1, r3
1ff0008c:	e24cc004 	sub	ip, ip, #4
1ff00090:	e0812142 	add	r2, r1, r2, asr #2
1ff00094:	e37c0004 	cmn	ip, #4
1ff00098:	e5533014 	ldrb	r3, [r3, #-20]	; 0xffffffec
1ff0009c:	e5423020 	strb	r3, [r2, #-32]	; 0xffffffe0
1ff000a0:	1afffff4 	bne	1ff00078 <puthex+0x4c>
1ff000a4:	e1a0000d 	mov	r0, sp
1ff000a8:	eb000076 	bl	1ff00288 <uart_spin_puts>
1ff000ac:	e28dd024 	add	sp, sp, #36	; 0x24
1ff000b0:	e8bd8030 	pop	{r4, r5, pc}

1ff000b4 <firmware_bootmain>:
1ff000b4:	e92d4010 	push	{r4, lr}
1ff000b8:	e3a00001 	mov	r0, #1
1ff000bc:	eb00008a 	bl	1ff002ec <sleep>
1ff000c0:	eb00003c 	bl	1ff001b8 <uart_init>
1ff000c4:	eb000052 	bl	1ff00214 <uart_enable>
1ff000c8:	e3000914 	movw	r0, #2324	; 0x914
1ff000cc:	e3410ff0 	movt	r0, #8176	; 0x1ff0
1ff000d0:	eb00006c 	bl	1ff00288 <uart_spin_puts>
1ff000d4:	eb0000bc 	bl	1ff003cc <sd_init>
1ff000d8:	e3000924 	movw	r0, #2340	; 0x924
1ff000dc:	e3410ff0 	movt	r0, #8176	; 0x1ff0
1ff000e0:	eb000068 	bl	1ff00288 <uart_spin_puts>
1ff000e4:	eb00015c 	bl	1ff0065c <sd_spin_init_mem_card>
1ff000e8:	e3500000 	cmp	r0, #0
1ff000ec:	0a000027 	beq	1ff00190 <firmware_bootmain+0xdc>
1ff000f0:	e3500001 	cmp	r0, #1
1ff000f4:	0a000003 	beq	1ff00108 <firmware_bootmain+0x54>
1ff000f8:	e3000984 	movw	r0, #2436	; 0x984
1ff000fc:	e3410ff0 	movt	r0, #8176	; 0x1ff0
1ff00100:	eb000060 	bl	1ff00288 <uart_spin_puts>
1ff00104:	eafffffe 	b	1ff00104 <firmware_bootmain+0x50>
1ff00108:	e3000964 	movw	r0, #2404	; 0x964
1ff0010c:	e3410ff0 	movt	r0, #8176	; 0x1ff0
1ff00110:	eb00005c 	bl	1ff00288 <uart_spin_puts>
1ff00114:	e3a02000 	mov	r2, #0
1ff00118:	e3a01001 	mov	r1, #1
1ff0011c:	e3a00601 	mov	r0, #1048576	; 0x100000
1ff00120:	eb0001af 	bl	1ff007e4 <sd_dma_spin_read>
1ff00124:	e3500000 	cmp	r0, #0
1ff00128:	1a000014 	bne	1ff00180 <firmware_bootmain+0xcc>
1ff0012c:	e30009a8 	movw	r0, #2472	; 0x9a8
1ff00130:	e3410ff0 	movt	r0, #8176	; 0x1ff0
1ff00134:	eb000053 	bl	1ff00288 <uart_spin_puts>
1ff00138:	e3a03c01 	mov	r3, #256	; 0x100
1ff0013c:	e3403010 	movt	r3, #16
1ff00140:	e5d320fe 	ldrb	r2, [r3, #254]	; 0xfe
1ff00144:	e3520055 	cmp	r2, #85	; 0x55
1ff00148:	1a000002 	bne	1ff00158 <firmware_bootmain+0xa4>
1ff0014c:	e5d330ff 	ldrb	r3, [r3, #255]	; 0xff
1ff00150:	e35300aa 	cmp	r3, #170	; 0xaa
1ff00154:	0a000011 	beq	1ff001a0 <firmware_bootmain+0xec>
1ff00158:	e30009e8 	movw	r0, #2536	; 0x9e8
1ff0015c:	e3410ff0 	movt	r0, #8176	; 0x1ff0
1ff00160:	eb000048 	bl	1ff00288 <uart_spin_puts>
1ff00164:	e3000a00 	movw	r0, #2560	; 0xa00
1ff00168:	e3410ff0 	movt	r0, #8176	; 0x1ff0
1ff0016c:	eb000045 	bl	1ff00288 <uart_spin_puts>
1ff00170:	e3000a0c 	movw	r0, #2572	; 0xa0c
1ff00174:	e3410ff0 	movt	r0, #8176	; 0x1ff0
1ff00178:	eb000042 	bl	1ff00288 <uart_spin_puts>
1ff0017c:	eaffffe0 	b	1ff00104 <firmware_bootmain+0x50>
1ff00180:	e30009bc 	movw	r0, #2492	; 0x9bc
1ff00184:	e3410ff0 	movt	r0, #8176	; 0x1ff0
1ff00188:	eb00003e 	bl	1ff00288 <uart_spin_puts>
1ff0018c:	eaffffdc 	b	1ff00104 <firmware_bootmain+0x50>
1ff00190:	e3000948 	movw	r0, #2376	; 0x948
1ff00194:	e3410ff0 	movt	r0, #8176	; 0x1ff0
1ff00198:	eb00003a 	bl	1ff00288 <uart_spin_puts>
1ff0019c:	eaffffdc 	b	1ff00114 <firmware_bootmain+0x60>
1ff001a0:	e30009d4 	movw	r0, #2516	; 0x9d4
1ff001a4:	e3410ff0 	movt	r0, #8176	; 0x1ff0
1ff001a8:	eb000036 	bl	1ff00288 <uart_spin_puts>
1ff001ac:	e3a03601 	mov	r3, #1048576	; 0x100000
1ff001b0:	e12fff33 	blx	r3
1ff001b4:	eaffffea 	b	1ff00164 <firmware_bootmain+0xb0>

1ff001b8 <uart_init>:
1ff001b8:	e3a03a01 	mov	r3, #4096	; 0x1000
1ff001bc:	e3011fff 	movw	r1, #8191	; 0x1fff
1ff001c0:	e34e3000 	movt	r3, #57344	; 0xe000
1ff001c4:	e3a02028 	mov	r2, #40	; 0x28
1ff001c8:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
1ff001cc:	e3a00003 	mov	r0, #3
1ff001d0:	e583100c 	str	r1, [r3, #12]
1ff001d4:	e3a0e000 	mov	lr, #0
1ff001d8:	e5832000 	str	r2, [r3]
1ff001dc:	e3a0c03e 	mov	ip, #62	; 0x3e
1ff001e0:	e5830000 	str	r0, [r3]
1ff001e4:	e3a02020 	mov	r2, #32
1ff001e8:	e5831014 	str	r1, [r3, #20]
1ff001ec:	e3a00006 	mov	r0, #6
1ff001f0:	e5832004 	str	r2, [r3, #4]
1ff001f4:	e3a01f4a 	mov	r1, #296	; 0x128
1ff001f8:	e5832020 	str	r2, [r3, #32]
1ff001fc:	e5832044 	str	r2, [r3, #68]	; 0x44
1ff00200:	e583e01c 	str	lr, [r3, #28]
1ff00204:	e583c018 	str	ip, [r3, #24]
1ff00208:	e5830034 	str	r0, [r3, #52]	; 0x34
1ff0020c:	e5831000 	str	r1, [r3]
1ff00210:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)

1ff00214 <uart_enable>:
1ff00214:	e3a03a01 	mov	r3, #4096	; 0x1000
1ff00218:	e3a02014 	mov	r2, #20
1ff0021c:	e34e3000 	movt	r3, #57344	; 0xe000
1ff00220:	e5832000 	str	r2, [r3]
1ff00224:	e12fff1e 	bx	lr

1ff00228 <uart_disable>:
1ff00228:	e3a03a01 	mov	r3, #4096	; 0x1000
1ff0022c:	e3a02028 	mov	r2, #40	; 0x28
1ff00230:	e34e3000 	movt	r3, #57344	; 0xe000
1ff00234:	e5832000 	str	r2, [r3]
1ff00238:	e12fff1e 	bx	lr

1ff0023c <uart_spin_getbyte>:
1ff0023c:	e3a01a01 	mov	r1, #4096	; 0x1000
1ff00240:	e34e1000 	movt	r1, #57344	; 0xe000
1ff00244:	e591202c 	ldr	r2, [r1, #44]	; 0x2c
1ff00248:	e3a03a01 	mov	r3, #4096	; 0x1000
1ff0024c:	e34e3000 	movt	r3, #57344	; 0xe000
1ff00250:	e3120002 	tst	r2, #2
1ff00254:	1afffffa 	bne	1ff00244 <uart_spin_getbyte+0x8>
1ff00258:	e5930030 	ldr	r0, [r3, #48]	; 0x30
1ff0025c:	e6ef0070 	uxtb	r0, r0
1ff00260:	e12fff1e 	bx	lr

1ff00264 <uart_spin_putbyte>:
1ff00264:	e3a01a01 	mov	r1, #4096	; 0x1000
1ff00268:	e34e1000 	movt	r1, #57344	; 0xe000
1ff0026c:	e591202c 	ldr	r2, [r1, #44]	; 0x2c
1ff00270:	e3a03a01 	mov	r3, #4096	; 0x1000
1ff00274:	e34e3000 	movt	r3, #57344	; 0xe000
1ff00278:	e3120010 	tst	r2, #16
1ff0027c:	1afffffa 	bne	1ff0026c <uart_spin_putbyte+0x8>
1ff00280:	e5830030 	str	r0, [r3, #48]	; 0x30
1ff00284:	e12fff1e 	bx	lr

1ff00288 <uart_spin_puts>:
1ff00288:	e92d4010 	push	{r4, lr}
1ff0028c:	e1a04000 	mov	r4, r0
1ff00290:	e5d00000 	ldrb	r0, [r0]
1ff00294:	e3500000 	cmp	r0, #0
1ff00298:	08bd8010 	popeq	{r4, pc}
1ff0029c:	ebfffff0 	bl	1ff00264 <uart_spin_putbyte>
1ff002a0:	e5f40001 	ldrb	r0, [r4, #1]!
1ff002a4:	e3500000 	cmp	r0, #0
1ff002a8:	1afffffb 	bne	1ff0029c <uart_spin_puts+0x14>
1ff002ac:	e8bd8010 	pop	{r4, pc}

1ff002b0 <gtc_get_time>:
1ff002b0:	e3a03000 	mov	r3, #0
1ff002b4:	e34f38f0 	movt	r3, #63728	; 0xf8f0
1ff002b8:	e5931204 	ldr	r1, [r3, #516]	; 0x204
1ff002bc:	e5930200 	ldr	r0, [r3, #512]	; 0x200
1ff002c0:	e5932204 	ldr	r2, [r3, #516]	; 0x204
1ff002c4:	e1520001 	cmp	r2, r1
1ff002c8:	1afffffa 	bne	1ff002b8 <gtc_get_time+0x8>
1ff002cc:	e92d0030 	push	{r4, r5}
1ff002d0:	e3a04000 	mov	r4, #0
1ff002d4:	e1a05002 	mov	r5, r2
1ff002d8:	e1844000 	orr	r4, r4, r0
1ff002dc:	e1a00004 	mov	r0, r4
1ff002e0:	e1a01005 	mov	r1, r5
1ff002e4:	e8bd0030 	pop	{r4, r5}
1ff002e8:	e12fff1e 	bx	lr

1ff002ec <sleep>:
1ff002ec:	e3a03000 	mov	r3, #0
1ff002f0:	e34f38f0 	movt	r3, #63728	; 0xf8f0
1ff002f4:	e5932204 	ldr	r2, [r3, #516]	; 0x204
1ff002f8:	e593c200 	ldr	ip, [r3, #512]	; 0x200
1ff002fc:	e5931204 	ldr	r1, [r3, #516]	; 0x204
1ff00300:	e1520001 	cmp	r2, r1
1ff00304:	1afffffa 	bne	1ff002f4 <sleep+0x8>
1ff00308:	e3a01b21 	mov	r1, #33792	; 0x8400
1ff0030c:	e3a03000 	mov	r3, #0
1ff00310:	e34117d7 	movt	r1, #6103	; 0x17d7
1ff00314:	e0010091 	mul	r1, r1, r0
1ff00318:	e92d03f0 	push	{r4, r5, r6, r7, r8, r9}
1ff0031c:	e183600c 	orr	r6, r3, ip
1ff00320:	e0966001 	adds	r6, r6, r1
1ff00324:	e34f38f0 	movt	r3, #63728	; 0xf8f0
1ff00328:	e0a27fc1 	adc	r7, r2, r1, asr #31
1ff0032c:	e5932204 	ldr	r2, [r3, #516]	; 0x204
1ff00330:	e5930200 	ldr	r0, [r3, #512]	; 0x200
1ff00334:	e5931204 	ldr	r1, [r3, #516]	; 0x204
1ff00338:	e1520001 	cmp	r2, r1
1ff0033c:	1afffffa 	bne	1ff0032c <sleep+0x40>
1ff00340:	e3a04000 	mov	r4, #0
1ff00344:	e1570002 	cmp	r7, r2
1ff00348:	e1848000 	orr	r8, r4, r0
1ff0034c:	01560008 	cmpeq	r6, r8
1ff00350:	8afffff5 	bhi	1ff0032c <sleep+0x40>
1ff00354:	e8bd03f0 	pop	{r4, r5, r6, r7, r8, r9}
1ff00358:	e12fff1e 	bx	lr

1ff0035c <usleep>:
1ff0035c:	e3a03000 	mov	r3, #0
1ff00360:	e34f38f0 	movt	r3, #63728	; 0xf8f0
1ff00364:	e5932204 	ldr	r2, [r3, #516]	; 0x204
1ff00368:	e593c200 	ldr	ip, [r3, #512]	; 0x200
1ff0036c:	e5931204 	ldr	r1, [r3, #516]	; 0x204
1ff00370:	e1520001 	cmp	r2, r1
1ff00374:	1afffffa 	bne	1ff00364 <usleep+0x8>
1ff00378:	e3a03e19 	mov	r3, #400	; 0x190
1ff0037c:	e0010093 	mul	r1, r3, r0
1ff00380:	e3a03000 	mov	r3, #0
1ff00384:	e92d03f0 	push	{r4, r5, r6, r7, r8, r9}
1ff00388:	e3a06000 	mov	r6, #0
1ff0038c:	e186600c 	orr	r6, r6, ip
1ff00390:	e34f38f0 	movt	r3, #63728	; 0xf8f0
1ff00394:	e0966001 	adds	r6, r6, r1
1ff00398:	e0a27fc1 	adc	r7, r2, r1, asr #31
1ff0039c:	e5932204 	ldr	r2, [r3, #516]	; 0x204
1ff003a0:	e5930200 	ldr	r0, [r3, #512]	; 0x200
1ff003a4:	e5931204 	ldr	r1, [r3, #516]	; 0x204
1ff003a8:	e1520001 	cmp	r2, r1
1ff003ac:	1afffffa 	bne	1ff0039c <usleep+0x40>
1ff003b0:	e3a04000 	mov	r4, #0
1ff003b4:	e1570002 	cmp	r7, r2
1ff003b8:	e1848000 	orr	r8, r4, r0
1ff003bc:	01560008 	cmpeq	r6, r8
1ff003c0:	8afffff5 	bhi	1ff0039c <usleep+0x40>
1ff003c4:	e8bd03f0 	pop	{r4, r5, r6, r7, r8, r9}
1ff003c8:	e12fff1e 	bx	lr

1ff003cc <sd_init>:
1ff003cc:	e3a03000 	mov	r3, #0
1ff003d0:	e3a02001 	mov	r2, #1
1ff003d4:	e34e3010 	movt	r3, #57360	; 0xe010
1ff003d8:	e1a01003 	mov	r1, r3
1ff003dc:	e5c3202f 	strb	r2, [r3, #47]	; 0x2f
1ff003e0:	e5d1202f 	ldrb	r2, [r1, #47]	; 0x2f
1ff003e4:	e3a03000 	mov	r3, #0
1ff003e8:	e34e3010 	movt	r3, #57360	; 0xe010
1ff003ec:	e3120001 	tst	r2, #1
1ff003f0:	1afffffa 	bne	1ff003e0 <sd_init+0x14>
1ff003f4:	e1a01003 	mov	r1, r3
1ff003f8:	e3042001 	movw	r2, #16385	; 0x4001
1ff003fc:	e1c322bc 	strh	r2, [r3, #44]	; 0x2c
1ff00400:	e1d122bc 	ldrh	r2, [r1, #44]	; 0x2c
1ff00404:	e3a03000 	mov	r3, #0
1ff00408:	e34e3010 	movt	r3, #57360	; 0xe010
1ff0040c:	e3120002 	tst	r2, #2
1ff00410:	0afffffa 	beq	1ff00400 <sd_init+0x34>
1ff00414:	e1d312bc 	ldrh	r1, [r3, #44]	; 0x2c
1ff00418:	e3a02000 	mov	r2, #0
1ff0041c:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
1ff00420:	e3811004 	orr	r1, r1, #4
1ff00424:	e3a0e00f 	mov	lr, #15
1ff00428:	e1c312bc 	strh	r1, [r3, #44]	; 0x2c
1ff0042c:	e3e0cc01 	mvn	ip, #256	; 0x100
1ff00430:	e5c3e029 	strb	lr, [r3, #41]	; 0x29
1ff00434:	e3e00b03 	mvn	r0, #3072	; 0xc00
1ff00438:	e5c32028 	strb	r2, [r3, #40]	; 0x28
1ff0043c:	e3a01c02 	mov	r1, #512	; 0x200
1ff00440:	e1c3c3b4 	strh	ip, [r3, #52]	; 0x34
1ff00444:	e1c303b6 	strh	r0, [r3, #54]	; 0x36
1ff00448:	e1c323b8 	strh	r2, [r3, #56]	; 0x38
1ff0044c:	e1c323ba 	strh	r2, [r3, #58]	; 0x3a
1ff00450:	e1c310b4 	strh	r1, [r3, #4]
1ff00454:	e49df004 	pop	{pc}		; (ldr pc, [sp], #4)

1ff00458 <sd_frame_cmd>:
1ff00458:	e3500c11 	cmp	r0, #4352	; 0x1100
1ff0045c:	0a00002b 	beq	1ff00510 <sd_frame_cmd+0xb8>
1ff00460:	8a000012 	bhi	1ff004b0 <sd_frame_cmd+0x58>
1ff00464:	e3500c07 	cmp	r0, #1792	; 0x700
1ff00468:	0a00003d 	beq	1ff00564 <sd_frame_cmd+0x10c>
1ff0046c:	8a000008 	bhi	1ff00494 <sd_frame_cmd+0x3c>
1ff00470:	e3500c03 	cmp	r0, #768	; 0x300
1ff00474:	0a000034 	beq	1ff0054c <sd_frame_cmd+0xf4>
1ff00478:	9a00002d 	bls	1ff00534 <sd_frame_cmd+0xdc>
1ff0047c:	e3500c05 	cmp	r0, #1280	; 0x500
1ff00480:	0a000019 	beq	1ff004ec <sd_frame_cmd+0x94>
1ff00484:	e3500c06 	cmp	r0, #1536	; 0x600
1ff00488:	1a000027 	bne	1ff0052c <sd_frame_cmd+0xd4>
1ff0048c:	e300063a 	movw	r0, #1594	; 0x63a
1ff00490:	e12fff1e 	bx	lr
1ff00494:	e3500c0a 	cmp	r0, #2560	; 0xa00
1ff00498:	0a000031 	beq	1ff00564 <sd_frame_cmd+0x10c>
1ff0049c:	8a000036 	bhi	1ff0057c <sd_frame_cmd+0x124>
1ff004a0:	e3500b02 	cmp	r0, #2048	; 0x800
1ff004a4:	1a000031 	bne	1ff00570 <sd_frame_cmd+0x118>
1ff004a8:	e300081a 	movw	r0, #2074	; 0x81a
1ff004ac:	e12fff1e 	bx	lr
1ff004b0:	e3500c37 	cmp	r0, #14080	; 0x3700
1ff004b4:	0a00002a 	beq	1ff00564 <sd_frame_cmd+0x10c>
1ff004b8:	9a00000d 	bls	1ff004f4 <sd_frame_cmd+0x9c>
1ff004bc:	e3500c97 	cmp	r0, #38656	; 0x9700
1ff004c0:	0a000012 	beq	1ff00510 <sd_frame_cmd+0xb8>
1ff004c4:	9a000014 	bls	1ff0051c <sd_frame_cmd+0xc4>
1ff004c8:	e3500caa 	cmp	r0, #43520	; 0xaa00
1ff004cc:	0a000024 	beq	1ff00564 <sd_frame_cmd+0x10c>
1ff004d0:	e3500cb3 	cmp	r0, #45824	; 0xb300
1ff004d4:	0a00000d 	beq	1ff00510 <sd_frame_cmd+0xb8>
1ff004d8:	e3500ca9 	cmp	r0, #43264	; 0xa900
1ff004dc:	1a000012 	bne	1ff0052c <sd_frame_cmd+0xd4>
1ff004e0:	e7ed0050 	ubfx	r0, r0, #0, #14
1ff004e4:	e3800002 	orr	r0, r0, #2
1ff004e8:	e12fff1e 	bx	lr
1ff004ec:	e300051b 	movw	r0, #1307	; 0x51b
1ff004f0:	e12fff1e 	bx	lr
1ff004f4:	e3500b06 	cmp	r0, #6144	; 0x1800
1ff004f8:	0a000004 	beq	1ff00510 <sd_frame_cmd+0xb8>
1ff004fc:	8a000014 	bhi	1ff00554 <sd_frame_cmd+0xfc>
1ff00500:	e3500c12 	cmp	r0, #4608	; 0x1200
1ff00504:	0a000001 	beq	1ff00510 <sd_frame_cmd+0xb8>
1ff00508:	e3500c17 	cmp	r0, #5888	; 0x1700
1ff0050c:	1a000006 	bne	1ff0052c <sd_frame_cmd+0xd4>
1ff00510:	e7ed0050 	ubfx	r0, r0, #0, #14
1ff00514:	e380003a 	orr	r0, r0, #58	; 0x3a
1ff00518:	e12fff1e 	bx	lr
1ff0051c:	e3500c86 	cmp	r0, #34304	; 0x8600
1ff00520:	0a00000f 	beq	1ff00564 <sd_frame_cmd+0x10c>
1ff00524:	e3500c8d 	cmp	r0, #36096	; 0x8d00
1ff00528:	0a00000d 	beq	1ff00564 <sd_frame_cmd+0x10c>
1ff0052c:	e7ed0050 	ubfx	r0, r0, #0, #14
1ff00530:	e12fff1e 	bx	lr
1ff00534:	e3500c01 	cmp	r0, #256	; 0x100
1ff00538:	0affffe8 	beq	1ff004e0 <sd_frame_cmd+0x88>
1ff0053c:	e3500c02 	cmp	r0, #512	; 0x200
1ff00540:	1afffff9 	bne	1ff0052c <sd_frame_cmd+0xd4>
1ff00544:	e3800009 	orr	r0, r0, #9
1ff00548:	e12fff1e 	bx	lr
1ff0054c:	e300031b 	movw	r0, #795	; 0x31b
1ff00550:	e12fff1e 	bx	lr
1ff00554:	e3500c19 	cmp	r0, #6400	; 0x1900
1ff00558:	0affffec 	beq	1ff00510 <sd_frame_cmd+0xb8>
1ff0055c:	e3500b0d 	cmp	r0, #13312	; 0x3400
1ff00560:	1afffff1 	bne	1ff0052c <sd_frame_cmd+0xd4>
1ff00564:	e7ed0050 	ubfx	r0, r0, #0, #14
1ff00568:	e380001a 	orr	r0, r0, #26
1ff0056c:	e12fff1e 	bx	lr
1ff00570:	e3500c09 	cmp	r0, #2304	; 0x900
1ff00574:	0afffff2 	beq	1ff00544 <sd_frame_cmd+0xec>
1ff00578:	eaffffeb 	b	1ff0052c <sd_frame_cmd+0xd4>
1ff0057c:	e3500b03 	cmp	r0, #3072	; 0xc00
1ff00580:	0afffff7 	beq	1ff00564 <sd_frame_cmd+0x10c>
1ff00584:	e3500a01 	cmp	r0, #4096	; 0x1000
1ff00588:	1affffe7 	bne	1ff0052c <sd_frame_cmd+0xd4>
1ff0058c:	eafffff4 	b	1ff00564 <sd_frame_cmd+0x10c>

1ff00590 <sd_spin_send_cmd>:
1ff00590:	e92d4010 	push	{r4, lr}
1ff00594:	ebffffaf 	bl	1ff00458 <sd_frame_cmd>
1ff00598:	e3a0c000 	mov	ip, #0
1ff0059c:	e34ec010 	movt	ip, #57360	; 0xe010
1ff005a0:	e59cc024 	ldr	ip, [ip, #36]	; 0x24
1ff005a4:	e31c0001 	tst	ip, #1
1ff005a8:	1a000029 	bne	1ff00654 <sd_spin_send_cmd+0xc4>
1ff005ac:	e31c0002 	tst	ip, #2
1ff005b0:	1a00001f 	bne	1ff00634 <sd_spin_send_cmd+0xa4>
1ff005b4:	e3530001 	cmp	r3, #1
1ff005b8:	e3a0c000 	mov	ip, #0
1ff005bc:	e34ec010 	movt	ip, #57360	; 0xe010
1ff005c0:	e3a0400e 	mov	r4, #14
1ff005c4:	e1cc10b6 	strh	r1, [ip, #6]
1ff005c8:	e3e0e000 	mvn	lr, #0
1ff005cc:	e1cc42be 	strh	r4, [ip, #46]	; 0x2e
1ff005d0:	e3e01b03 	mvn	r1, #3072	; 0xc00
1ff005d4:	e58c2008 	str	r2, [ip, #8]
1ff005d8:	03a02037 	moveq	r2, #55	; 0x37
1ff005dc:	e1cce3b0 	strh	lr, [ip, #48]	; 0x30
1ff005e0:	e1cc13b2 	strh	r1, [ip, #50]	; 0x32
1ff005e4:	0a000002 	beq	1ff005f4 <sd_spin_send_cmd+0x64>
1ff005e8:	e3530002 	cmp	r3, #2
1ff005ec:	03a02027 	moveq	r2, #39	; 0x27
1ff005f0:	13a02001 	movne	r2, #1
1ff005f4:	e3a03000 	mov	r3, #0
1ff005f8:	e34e3010 	movt	r3, #57360	; 0xe010
1ff005fc:	e1c320bc 	strh	r2, [r3, #12]
1ff00600:	e1a01003 	mov	r1, r3
1ff00604:	e1c300be 	strh	r0, [r3, #14]
1ff00608:	ea000001 	b	1ff00614 <sd_spin_send_cmd+0x84>
1ff0060c:	e3130001 	tst	r3, #1
1ff00610:	1a00000b 	bne	1ff00644 <sd_spin_send_cmd+0xb4>
1ff00614:	e1d133b0 	ldrh	r3, [r1, #48]	; 0x30
1ff00618:	e3a02000 	mov	r2, #0
1ff0061c:	e34e2010 	movt	r2, #57360	; 0xe010
1ff00620:	e6ff3073 	uxth	r3, r3
1ff00624:	e3130902 	tst	r3, #32768	; 0x8000
1ff00628:	0afffff7 	beq	1ff0060c <sd_spin_send_cmd+0x7c>
1ff0062c:	e3e00002 	mvn	r0, #2
1ff00630:	e8bd8010 	pop	{r4, pc}
1ff00634:	e3100020 	tst	r0, #32
1ff00638:	0affffdd 	beq	1ff005b4 <sd_spin_send_cmd+0x24>
1ff0063c:	e3e00001 	mvn	r0, #1
1ff00640:	e8bd8010 	pop	{r4, pc}
1ff00644:	e3a03001 	mov	r3, #1
1ff00648:	e3a00000 	mov	r0, #0
1ff0064c:	e1c233b0 	strh	r3, [r2, #48]	; 0x30
1ff00650:	e8bd8010 	pop	{r4, pc}
1ff00654:	e3e00000 	mvn	r0, #0
1ff00658:	e8bd8010 	pop	{r4, pc}

1ff0065c <sd_spin_init_mem_card>:
1ff0065c:	e92d4070 	push	{r4, r5, r6, lr}
1ff00660:	e3a05000 	mov	r5, #0
1ff00664:	e34e5010 	movt	r5, #57360	; 0xe010
1ff00668:	e5953024 	ldr	r3, [r5, #36]	; 0x24
1ff0066c:	e3130801 	tst	r3, #65536	; 0x10000
1ff00670:	0a00004d 	beq	1ff007ac <sd_spin_init_mem_card+0x150>
1ff00674:	e3a00e7d 	mov	r0, #2000	; 0x7d0
1ff00678:	ebffff37 	bl	1ff0035c <usleep>
1ff0067c:	e3a03000 	mov	r3, #0
1ff00680:	e1a02003 	mov	r2, r3
1ff00684:	e1a01003 	mov	r1, r3
1ff00688:	e1a00003 	mov	r0, r3
1ff0068c:	ebffffbf 	bl	1ff00590 <sd_spin_send_cmd>
1ff00690:	e2503000 	subs	r3, r0, #0
1ff00694:	1a000046 	bne	1ff007b4 <sd_spin_init_mem_card+0x158>
1ff00698:	e1a01003 	mov	r1, r3
1ff0069c:	e30021aa 	movw	r2, #426	; 0x1aa
1ff006a0:	e3a00b02 	mov	r0, #2048	; 0x800
1ff006a4:	ebffffb9 	bl	1ff00590 <sd_spin_send_cmd>
1ff006a8:	e3500000 	cmp	r0, #0
1ff006ac:	1a000044 	bne	1ff007c4 <sd_spin_init_mem_card+0x168>
1ff006b0:	e5952010 	ldr	r2, [r5, #16]
1ff006b4:	e30031aa 	movw	r3, #426	; 0x1aa
1ff006b8:	e1520003 	cmp	r2, r3
1ff006bc:	0a000006 	beq	1ff006dc <sd_spin_init_mem_card+0x80>
1ff006c0:	ea00003d 	b	1ff007bc <sd_spin_init_mem_card+0x160>
1ff006c4:	ebffffb1 	bl	1ff00590 <sd_spin_send_cmd>
1ff006c8:	e3500000 	cmp	r0, #0
1ff006cc:	1a00000d 	bne	1ff00708 <sd_spin_init_mem_card+0xac>
1ff006d0:	e5954010 	ldr	r4, [r5, #16]
1ff006d4:	e3540000 	cmp	r4, #0
1ff006d8:	ba00000c 	blt	1ff00710 <sd_spin_init_mem_card+0xb4>
1ff006dc:	e3a03000 	mov	r3, #0
1ff006e0:	e3a00c37 	mov	r0, #14080	; 0x3700
1ff006e4:	e1a02003 	mov	r2, r3
1ff006e8:	e1a01003 	mov	r1, r3
1ff006ec:	ebffffa7 	bl	1ff00590 <sd_spin_send_cmd>
1ff006f0:	e3a02000 	mov	r2, #0
1ff006f4:	e3442030 	movt	r2, #16432	; 0x4030
1ff006f8:	e2503000 	subs	r3, r0, #0
1ff006fc:	e3a00ca9 	mov	r0, #43264	; 0xa900
1ff00700:	e1a01003 	mov	r1, r3
1ff00704:	0affffee 	beq	1ff006c4 <sd_spin_init_mem_card+0x68>
1ff00708:	e3e00004 	mvn	r0, #4
1ff0070c:	e8bd8070 	pop	{r4, r5, r6, pc}
1ff00710:	e1a03000 	mov	r3, r0
1ff00714:	e1a02000 	mov	r2, r0
1ff00718:	e1a01000 	mov	r1, r0
1ff0071c:	e3a00c02 	mov	r0, #512	; 0x200
1ff00720:	ebffff9a 	bl	1ff00590 <sd_spin_send_cmd>
1ff00724:	e3500000 	cmp	r0, #0
1ff00728:	1a000029 	bne	1ff007d4 <sd_spin_init_mem_card+0x178>
1ff0072c:	e3a06000 	mov	r6, #0
1ff00730:	e34e6010 	movt	r6, #57360	; 0xe010
1ff00734:	ea000004 	b	1ff0074c <sd_spin_init_mem_card+0xf0>
1ff00738:	e5965010 	ldr	r5, [r6, #16]
1ff0073c:	e1a05825 	lsr	r5, r5, #16
1ff00740:	e1a05805 	lsl	r5, r5, #16
1ff00744:	e3550000 	cmp	r5, #0
1ff00748:	1a000008 	bne	1ff00770 <sd_spin_init_mem_card+0x114>
1ff0074c:	e3a03000 	mov	r3, #0
1ff00750:	e3a00c03 	mov	r0, #768	; 0x300
1ff00754:	e1a02003 	mov	r2, r3
1ff00758:	e1a01003 	mov	r1, r3
1ff0075c:	ebffff8b 	bl	1ff00590 <sd_spin_send_cmd>
1ff00760:	e3500000 	cmp	r0, #0
1ff00764:	0afffff3 	beq	1ff00738 <sd_spin_init_mem_card+0xdc>
1ff00768:	e3e00006 	mvn	r0, #6
1ff0076c:	e8bd8070 	pop	{r4, r5, r6, pc}
1ff00770:	e1a03000 	mov	r3, r0
1ff00774:	e1a01000 	mov	r1, r0
1ff00778:	e1a02005 	mov	r2, r5
1ff0077c:	e3a00c09 	mov	r0, #2304	; 0x900
1ff00780:	ebffff82 	bl	1ff00590 <sd_spin_send_cmd>
1ff00784:	e2503000 	subs	r3, r0, #0
1ff00788:	1a00000f 	bne	1ff007cc <sd_spin_init_mem_card+0x170>
1ff0078c:	e1a02005 	mov	r2, r5
1ff00790:	e1a01003 	mov	r1, r3
1ff00794:	e3a00c07 	mov	r0, #1792	; 0x700
1ff00798:	ebffff7c 	bl	1ff00590 <sd_spin_send_cmd>
1ff0079c:	e3500000 	cmp	r0, #0
1ff007a0:	1a00000d 	bne	1ff007dc <sd_spin_init_mem_card+0x180>
1ff007a4:	e7e00f54 	ubfx	r0, r4, #30, #1
1ff007a8:	e8bd8070 	pop	{r4, r5, r6, pc}
1ff007ac:	e3e00000 	mvn	r0, #0
1ff007b0:	e8bd8070 	pop	{r4, r5, r6, pc}
1ff007b4:	e3e00001 	mvn	r0, #1
1ff007b8:	e8bd8070 	pop	{r4, r5, r6, pc}
1ff007bc:	e3e00003 	mvn	r0, #3
1ff007c0:	e8bd8070 	pop	{r4, r5, r6, pc}
1ff007c4:	e3e00002 	mvn	r0, #2
1ff007c8:	e8bd8070 	pop	{r4, r5, r6, pc}
1ff007cc:	e3e00007 	mvn	r0, #7
1ff007d0:	e8bd8070 	pop	{r4, r5, r6, pc}
1ff007d4:	e3e00005 	mvn	r0, #5
1ff007d8:	e8bd8070 	pop	{r4, r5, r6, pc}
1ff007dc:	e3e00008 	mvn	r0, #8
1ff007e0:	e8bd8070 	pop	{r4, r5, r6, pc}

1ff007e4 <sd_dma_spin_read>:
1ff007e4:	e92d4010 	push	{r4, lr}
1ff007e8:	e3a04000 	mov	r4, #0
1ff007ec:	e34e4010 	movt	r4, #57360	; 0xe010
1ff007f0:	e5943024 	ldr	r3, [r4, #36]	; 0x24
1ff007f4:	e3130801 	tst	r3, #65536	; 0x10000
1ff007f8:	0a000017 	beq	1ff0085c <sd_dma_spin_read+0x78>
1ff007fc:	e5840000 	str	r0, [r4]
1ff00800:	e3a03001 	mov	r3, #1
1ff00804:	e3a00c12 	mov	r0, #4608	; 0x1200
1ff00808:	ebffff60 	bl	1ff00590 <sd_spin_send_cmd>
1ff0080c:	e3500000 	cmp	r0, #0
1ff00810:	1a000013 	bne	1ff00864 <sd_dma_spin_read+0x80>
1ff00814:	e1a01004 	mov	r1, r4
1ff00818:	ea000001 	b	1ff00824 <sd_dma_spin_read+0x40>
1ff0081c:	e3130002 	tst	r3, #2
1ff00820:	1a000009 	bne	1ff0084c <sd_dma_spin_read+0x68>
1ff00824:	e1d133b0 	ldrh	r3, [r1, #48]	; 0x30
1ff00828:	e3a02000 	mov	r2, #0
1ff0082c:	e34e2010 	movt	r2, #57360	; 0xe010
1ff00830:	e6ff3073 	uxth	r3, r3
1ff00834:	e3130902 	tst	r3, #32768	; 0x8000
1ff00838:	0afffff7 	beq	1ff0081c <sd_dma_spin_read+0x38>
1ff0083c:	e3e03b03 	mvn	r3, #3072	; 0xc00
1ff00840:	e3e00002 	mvn	r0, #2
1ff00844:	e1c233b2 	strh	r3, [r2, #50]	; 0x32
1ff00848:	e8bd8010 	pop	{r4, pc}
1ff0084c:	e3a03002 	mov	r3, #2
1ff00850:	e3a00000 	mov	r0, #0
1ff00854:	e1c233b0 	strh	r3, [r2, #48]	; 0x30
1ff00858:	e8bd8010 	pop	{r4, pc}
1ff0085c:	e3e00000 	mvn	r0, #0
1ff00860:	e8bd8010 	pop	{r4, pc}
1ff00864:	e3e00001 	mvn	r0, #1
1ff00868:	e8bd8010 	pop	{r4, pc}

1ff0086c <sd_dma_spin_write>:
1ff0086c:	e92d4010 	push	{r4, lr}
1ff00870:	e3a04000 	mov	r4, #0
1ff00874:	e34e4010 	movt	r4, #57360	; 0xe010
1ff00878:	e5943024 	ldr	r3, [r4, #36]	; 0x24
1ff0087c:	e3130801 	tst	r3, #65536	; 0x10000
1ff00880:	0a000017 	beq	1ff008e4 <sd_dma_spin_write+0x78>
1ff00884:	e5840000 	str	r0, [r4]
1ff00888:	e3a03002 	mov	r3, #2
1ff0088c:	e3a00c19 	mov	r0, #6400	; 0x1900
1ff00890:	ebffff3e 	bl	1ff00590 <sd_spin_send_cmd>
1ff00894:	e3500000 	cmp	r0, #0
1ff00898:	1a000013 	bne	1ff008ec <sd_dma_spin_write+0x80>
1ff0089c:	e1a01004 	mov	r1, r4
1ff008a0:	ea000001 	b	1ff008ac <sd_dma_spin_write+0x40>
1ff008a4:	e3130002 	tst	r3, #2
1ff008a8:	1a000009 	bne	1ff008d4 <sd_dma_spin_write+0x68>
1ff008ac:	e1d133b0 	ldrh	r3, [r1, #48]	; 0x30
1ff008b0:	e3a02000 	mov	r2, #0
1ff008b4:	e34e2010 	movt	r2, #57360	; 0xe010
1ff008b8:	e6ff3073 	uxth	r3, r3
1ff008bc:	e3130902 	tst	r3, #32768	; 0x8000
1ff008c0:	0afffff7 	beq	1ff008a4 <sd_dma_spin_write+0x38>
1ff008c4:	e3e03b03 	mvn	r3, #3072	; 0xc00
1ff008c8:	e3e00002 	mvn	r0, #2
1ff008cc:	e1c233b2 	strh	r3, [r2, #50]	; 0x32
1ff008d0:	e8bd8010 	pop	{r4, pc}
1ff008d4:	e3a03002 	mov	r3, #2
1ff008d8:	e3a00000 	mov	r0, #0
1ff008dc:	e1c233b0 	strh	r3, [r2, #48]	; 0x30
1ff008e0:	e8bd8010 	pop	{r4, pc}
1ff008e4:	e3e00000 	mvn	r0, #0
1ff008e8:	e8bd8010 	pop	{r4, pc}
1ff008ec:	e3e00001 	mvn	r0, #1
1ff008f0:	e8bd8010 	pop	{r4, pc}
