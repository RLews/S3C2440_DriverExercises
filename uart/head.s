

.extern main
.text
.global _start
_start:
Reset:
	LDR		sp,		=4096
	BL		DisableWatchDog
	BL		ClockInit
	BL		MemSetup
	BL		CopySteppingstoneToSDRAM
	LDR		pc,		=on_sdram
on_sdram:
	LDR		sp,		=0x34000000
	LDR		lr,		=halt_loop
	LDR		pc,		=main
halt_loop:
	B		halt_loop
	