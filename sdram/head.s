

.equ		MEM_CTRL_BASE,			0x48000000
.equ		SDRAM_BASE,				0x30000000

.text
.global _start
_start:
	BL		disable_watch_dog
	BL		memsetup
	BL		copy_steppingstone_to_sdram
	LDR		pc,		=on_sdram
on_sdram:
	LDR		sp,		=0x34000000
	BL		main
halt_loop:
	B		halt_loop

disable_watch_dog:
	MOV		R1,		#0x53000000
	MOV		R2,		#0x0
	STR		R2,		[R1]
	MOV		pc,		lr			@ return
	
copy_steppingstone_to_sdram:
	@ 将steppingstone的4K数据全部复制到SDRAM
	
	MOV		R1,		#0
	LDR		R2,		=SDRAM_BASE
	MOV		R3,		#1024*4
1:
	LDR		R4,		[R1],	#4
	STR		R4,		[R2],	#4
	CMP		R1,		R3
	bne		1b
	MOV		pc,		lr
	
memsetup:
	@ 设置SDRAM
	
	MOV		R1,		#MEM_CTRL_BASE		@ 存储控制寄存器起始地址
	ADRL	R2,		mem_cfg_val			@ 13个值的存储地址
	ADD		R3,		R1,		#52			@ 13 * 4 = 52
	
1:
	LDR		R4,		[R2],	#4
	STR		R4,		[R1],	#4
	CMP		R1,		R3
	bne		1b
	MOV		pc,		lr
	
.align 4
mem_cfg_val:
	@ 13个寄存器设置值
	.long	0x22011110		@ BWSCON
	.long	0x00000700		@ BANKCON0
	.long	0x00000700		@ BANKCON1
	.long	0x00000700		@ BANKCON2
	.long	0x00000700		@ BANKCON3
	.long	0x00000700		@ BANKCON4
	.long	0x00000700		@ BANKCON5
	.long	0x00018005		@ BANKCON6
	.long	0x00018005		@ BANKCON7
	.long	0x008C07A3		@ REFRESH
	.long	0x000000B1		@ BANKSIZE
	.long	0x00000030		@ MRSRB6
	.long	0x00000030		@ MRSRB7
