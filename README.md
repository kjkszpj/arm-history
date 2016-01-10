# Laksmi
OS on ARM.

![laksmi](http://www.yogeshkumar.myewebsite.com/img/mid/14/diwali-lakshmi-puja-divali-laksmi-pooja-laxmi-wallpaper.jpg)

Don't know if I can get A for this project, after all, project is just 50% of the grade.

---
###	TODO
-   syscalls
-   irq handler
    -   a sgi for cpu1
    -   ppi, private timer for scheduler
-   scheduler
-	context & PCB, TCB things
-   careful interrupt handler
-	IPC
-	alter page table struct and method
-   L2 page
-	vmalloc, malloc, sbrk

---
###	Now Finish
-	bootloader
-	MMU
-	I cache, D cache
-	SCU enable
-	physical pages alloc
-	slb
-	kmalloc
-	TTB1
-	DIV & printf
-   careless interrupt handler

---
###	feature
-	I cache, D cache enable (not fully managed for now)
-	allow TTB1 (can NOT use on other architecture, delete / 兼容)

###	hard feature
-	swap
-	file system
-	vmalloc
-   CPU1
-   L2 cache

###	things that is not implemented here
-	all kinds of flag, cpu info
-	vmalloc?
-	complicated file system
-	GBA simulator
-	screen
-	network
-   PAE(physical address extension)
-	ARM---fast context switch extension?

---
###	Contact me

游沛杰(Peijie You)

Mail: you.piji@gmail.com
