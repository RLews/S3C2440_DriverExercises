
#include "s3c24xx.h"

#define GPF4_OUT		(1<<(4*2))
#define GPF5_OUT		(1<<(5*2))
#define GPF6_OUT		(1<<(6*2))

#define GPF4_MSK		(3<<(4*2))
#define GPF5_MSK		(3<<(5*2))
#define GPF6_MSK		(3<<(6*2))

#define GPF0_EINT		(0x2<<(0*2))
#define GPF2_EINT		(0x2<<(2*2))
#define GPG3_EINT		(0x2<<(3*2))

#define GPF0_MSK		(3<<(0*2))
#define GPF2_MSK		(3<<(2*2))
#define GPG3_MSK		(3<<(3*2))


void DisableWatchDog(void)
{
	WTCON = 0;
}

void LedInit(void)
{
	GPFCON &= ~(GPF4_MSK | GPF5_MSK | GPF6_MSK);
	GPFCON |= GPF4_OUT | GPF5_OUT | GPF6_OUT;
}

void IRQInit(void)
{
	//设置相应的GPIO为中断
	GPFCON &= ~(GPF0_MSK | GPF2_MSK);
	GPFCON |= GPF0_EINT | GPF2_EINT;
	
	GPGCON &= ~GPG3_MSK;
	GPGCON |= GPG3_EINT;
	
	EINTMASK &= ~(1<<11);
	
	/*
	* 优先级设置：
	* ARB_SEL0 = 00b，ARB_MODE0 = 0; REQ1 > REQ3 仲裁器无需设置  
	*/
	PRIORITY = (PRIORITY & ((~0x01) | (0x03<<7))) | (0x00 << 7);
	
	INTMSK &= (~(1<<0)) & (~(1<<2)) & (~(1<<5));
}

