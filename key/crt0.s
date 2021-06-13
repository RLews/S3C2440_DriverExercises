.text
.global _start
_start:
	LDR		R0,		=0x53000000		@ WATCHDOG 
	MOV		R1,		#0x0			@ 
	STR		R1,		[R0]			@ write 0, disable watchdog
	
	LDR		sp,		=1024*4			@ nand flash 复位后会移动到内部ram, ram size 4K
	BL		main
	
halt_loop:
	B		halt_loop