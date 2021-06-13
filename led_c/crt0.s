.text
.global _start
_start:
	LDR		R0,		=0x53000000		@ WATCHDOG寄存器
	MOV		R1,		#0x0
	STR		R1,		[R0]			@ write 0, disable watchdog
	LDR		sp,		=1024*4			@ Set stack,  < 4K
	BL		main					@ into main()
	
halt_loop:
	B		halt_loop