

.text
.global _start
_start:
	LDR		sp,		=4096
	BL		DisableWatchDog
	BL		MemSetup
	BL		Copy_2th_sdram
	BL		CreatePageTable
	BL		MMU_Init
	LDR		sp,		=0xB4000000			@ 重新设置堆栈指针，指向SDRAM顶端
	LDR		pc,		=0xB0004000			@ 跳到SDRAM中继续执行第二部分代码
	
halt_loop:
	BL		halt_loop