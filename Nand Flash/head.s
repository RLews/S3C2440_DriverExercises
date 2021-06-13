@*****************************************************
@ 
@*****************************************************

.text
.global _start
_start:
	LDR		sp,		=4096
	BL		DisableWatchDog
	BL		MemSetup
	BL		NandFlashInit
	
	@ 将Nand flash中地址4096开始的1024字节代码复制到SDRAM中
	LDR		R0,		=0x30000000		@ 目标地址，SDRAM起始地址
	MOV		R1,		#4096
	MOV		R2,		#2048
	BL		NandRead
	
	LDR		sp,		=0x34000000
	LDR		lr,		=halt_loop		@ 设置返回地址
	LDR		pc,		=main			@ BL和B指令只能前后跳转32M范围
	
halt_loop:
	B		halt_loop