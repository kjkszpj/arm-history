00100000 <main>:
  100000:	e92d4800 	push	{fp, lr}
  100004:	e28db004 	add	fp, sp, #4
  100008:	e24dd038 	sub	sp, sp, #56	; 0x38
  10000c:	e3a0300c 	mov	r3, #12
  100010:	e3413ff0 	movt	r3, #8176	; 0x1ff0
  100014:	e50b3010 	str	r3, [fp, #-16]
  100018:	e3a03014 	mov	r3, #20
  10001c:	e3413ff0 	movt	r3, #8176	; 0x1ff0
  100020:	e50b3014 	str	r3, [fp, #-20]	; 0xffffffec
  100024:	e30031d6 	movw	r3, #470	; 0x1d6
  100028:	e3403010 	movt	r3, #16
  10002c:	e5933000 	ldr	r3, [r3]
  100030:	e50b3018 	str	r3, [fp, #-24]	; 0xffffffe8
  100034:	e3a03812 	mov	r3, #1179648	; 0x120000
  100038:	e50b301c 	str	r3, [fp, #-28]	; 0xffffffe4
  10003c:	e51b001c 	ldr	r0, [fp, #-28]	; 0xffffffe4
  100040:	e51b3018 	ldr	r3, [fp, #-24]	; 0xffffffe8
  100044:	e5932000 	ldr	r2, [r3]
  100048:	e51b3014 	ldr	r3, [fp, #-20]	; 0xffffffec
  10004c:	e3a01002 	mov	r1, #2
  100050:	e12fff33 	blx	r3
  100054:	e51b301c 	ldr	r3, [fp, #-28]	; 0xffffffe4
  100058:	e2833018 	add	r3, r3, #24
  10005c:	e5933000 	ldr	r3, [r3]
  100060:	e50b3020 	str	r3, [fp, #-32]	; 0xffffffe0
  100064:	e51b301c 	ldr	r3, [fp, #-28]	; 0xffffffe4
  100068:	e283301c 	add	r3, r3, #28
  10006c:	e5933000 	ldr	r3, [r3]
  100070:	e50b3008 	str	r3, [fp, #-8]
  100074:	e51b301c 	ldr	r3, [fp, #-28]	; 0xffffffe4
  100078:	e1d332ba 	ldrh	r3, [r3, #42]	; 0x2a
  10007c:	e14b32b2 	strh	r3, [fp, #-34]	; 0xffffffde
  100080:	e51b301c 	ldr	r3, [fp, #-28]	; 0xffffffe4
  100084:	e1d332bc 	ldrh	r3, [r3, #44]	; 0x2c
  100088:	e14b32b4 	strh	r3, [fp, #-36]	; 0xffffffdc
  10008c:	e51b301c 	ldr	r3, [fp, #-28]	; 0xffffffe4
  100090:	e1a03103 	lsl	r3, r3, #2
  100094:	e51b2008 	ldr	r2, [fp, #-8]
  100098:	e0823003 	add	r3, r2, r3
  10009c:	e50b3008 	str	r3, [fp, #-8]
  1000a0:	e3a03000 	mov	r3, #0
  1000a4:	e54b3009 	strb	r3, [fp, #-9]
  1000a8:	ea00002d 	b	100164 <main+0x164>
  1000ac:	e51b3008 	ldr	r3, [fp, #-8]
  1000b0:	e5933004 	ldr	r3, [r3, #4]
  1000b4:	e50b3028 	str	r3, [fp, #-40]	; 0xffffffd8
  1000b8:	e51b3008 	ldr	r3, [fp, #-8]
  1000bc:	e5933008 	ldr	r3, [r3, #8]
  1000c0:	e50b302c 	str	r3, [fp, #-44]	; 0xffffffd4
  1000c4:	e51b3008 	ldr	r3, [fp, #-8]
  1000c8:	e5933010 	ldr	r3, [r3, #16]
  1000cc:	e50b3030 	str	r3, [fp, #-48]	; 0xffffffd0
  1000d0:	e51b3028 	ldr	r3, [fp, #-40]	; 0xffffffd8
  1000d4:	e54b3031 	strb	r3, [fp, #-49]	; 0xffffffcf
  1000d8:	e51b3028 	ldr	r3, [fp, #-40]	; 0xffffffd8
  1000dc:	e1a034a3 	lsr	r3, r3, #9
  1000e0:	e50b3038 	str	r3, [fp, #-56]	; 0xffffffc8
  1000e4:	e51b201c 	ldr	r2, [fp, #-28]	; 0xffffffe4
  1000e8:	e51b3028 	ldr	r3, [fp, #-40]	; 0xffffffd8
  1000ec:	e0823003 	add	r3, r2, r3
  1000f0:	e2433001 	sub	r3, r3, #1
  1000f4:	e1a034a3 	lsr	r3, r3, #9
  1000f8:	e2833001 	add	r3, r3, #1
  1000fc:	e50b303c 	str	r3, [fp, #-60]	; 0xffffffc4
  100100:	e55b3031 	ldrb	r3, [fp, #-49]	; 0xffffffcf
  100104:	e51b202c 	ldr	r2, [fp, #-44]	; 0xffffffd4
  100108:	e0630002 	rsb	r0, r3, r2
  10010c:	e51b303c 	ldr	r3, [fp, #-60]	; 0xffffffc4
  100110:	e6ff2073 	uxth	r2, r3
  100114:	e51b3038 	ldr	r3, [fp, #-56]	; 0xffffffc8
  100118:	e6ff3073 	uxth	r3, r3
  10011c:	e0633002 	rsb	r3, r3, r2
  100120:	e6ff1073 	uxth	r1, r3
  100124:	e51b201c 	ldr	r2, [fp, #-28]	; 0xffffffe4
  100128:	e51b3028 	ldr	r3, [fp, #-40]	; 0xffffffd8
  10012c:	e0823003 	add	r3, r2, r3
  100130:	e1a02003 	mov	r2, r3
  100134:	e51b3014 	ldr	r3, [fp, #-20]	; 0xffffffec
  100138:	e12fff33 	blx	r3
  10013c:	e15b32b2 	ldrh	r3, [fp, #-34]	; 0xffffffde
  100140:	e1a03123 	lsr	r3, r3, #2
  100144:	e6ff3073 	uxth	r3, r3
  100148:	e1a03103 	lsl	r3, r3, #2
  10014c:	e51b2008 	ldr	r2, [fp, #-8]
  100150:	e0823003 	add	r3, r2, r3
  100154:	e50b3008 	str	r3, [fp, #-8]
  100158:	e55b3009 	ldrb	r3, [fp, #-9]
  10015c:	e2833001 	add	r3, r3, #1
  100160:	e54b3009 	strb	r3, [fp, #-9]
  100164:	e55b3009 	ldrb	r3, [fp, #-9]
  100168:	e6ff3073 	uxth	r3, r3
  10016c:	e15b22b4 	ldrh	r2, [fp, #-36]	; 0xffffffdc
  100170:	e1520003 	cmp	r2, r3
  100174:	8affffcc 	bhi	1000ac <main+0xac>
  100178:	e51b3020 	ldr	r3, [fp, #-32]	; 0xffffffe0
  10017c:	e12fff33 	blx	r3
  100180:	eafffffe 	b	100180 <main+0x180>