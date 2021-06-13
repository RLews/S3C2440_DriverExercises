

.extern main
.text
.global _start
_start:
	B		Reset		@ 中断向量。程序中只使用Reset和HandleIRQ。
	
@ 0x04: 未定义指令终止模式向量地址
HandleUndef:
	B		HandleUndef
	
@ 0x08: 管理模式向量地址
HandleSWI:
	B		HandleSWI
	
@ 0x0C: 指令预取终止异常向量地址
HandlePrefetAbort:
	B		HandlePrefetAbort
	
@ 0x10:	数据访问终止导致的异常向量地址
HandleDataAbort:
	B		HandleDataAbort
	
@ 0x14: 保留
HandleNotUsed:
	B		HandleNotUsed
	
@ 0x18: 中断模式向量地址
	B		HandleIRQ
	
@ 0x1C: 快中断模式的向量地址
HandleFIQ:
	B		HandleFIQ
	
Reset:
	LDR		sp,		=4096
	BL		DisableWatchDog
	
	MSR		cpsr_c,	#0xD2		@ 进入中断模式
	LDR		sp,		=3072
	
	MSR		cpsr_c,	#0xD3		@ 进入管理模式
	LDR		sp,		=4096		@ 复位后CPU处于管理模式。
	
	BL		LedInit
	BL		IRQInit
	MSR		cpsr_c,	#0x53		@ 开IRQ中断
	
	LDR		lr,		=halt_loop
	LDR		pc,		=main
	
halt_loop:
	BL		halt_loop
	
HandleIRQ:
	SUB		lr,		lr,		#4		@ 计算返回地址
	STMDB	sp!,	{ R0-R12, lr }	@ 保存使用到的寄存器。此时的sp是中断模式的sp
	
	LDR		lr,		=int_return		@ 设置调用ISR返回地址
	LDR		pc,		=EINT_Handle	@ 调用中断函数
int_return:
	LDMIA	sp!,	{ R0-R12, pc }^	@ 中断返回， ^表示将spsr的值复制到cpsr
	
