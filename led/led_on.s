

.text
.global _start
_start:
	LDR		R0,		=0x56000050		@ R0设置为GPFCON寄存器
	MOV		R1,		#0x00000100
	STR		R1,		[R0]
	LDR		R0,		=0x56000054		@ R0设置为GPFDAT寄存器
	MOV		R1,		#0x00000000
	STR		R1,		[R0]
	
MAIN_LOOP:
	B		MAIN_LOOP
	