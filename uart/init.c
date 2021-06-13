
#include "s3c24xx.h"

void DisableWatchDog(void);
void ClockInit(void);
void MemSetup(void);
void CopySteppingstoneToSDRAM(void);

void DisableWatchDog(void)
{
	WTCON = 0;
}

#define S3C2410_MPLL_200MHZ			((0x5C<<12) | (0x04<<4) | (0x00))
#define S3C2440_MPLL_200MHZ			((0x5C<<12) | (0x01<<4) | (0x02))

void ClockInit(void)
{
	CLKDIVN = 0x03;
	//FCLK:HCLK:PCLK = 1:2:4
	
	__asm__(
		"mrc	p15, 0, r1, c1, c0, 0\n"		//读出控制寄存器
		"orr    r1, r1, #0xc0000000\n"          //设置为asynchronous bus mode
		"mcr	p15, 0, r1, c1, c0, 0\n"		//写入控制寄存器
	);
	
	if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
	{
		MPLLCON = S3C2410_MPLL_200MHZ;
	}
	else
	{
		MPLLCON = S3C2440_MPLL_200MHZ;
	}
}

void MemSetup(void)
{
	volatile unsigned long *p = (volatile unsigned long *)MEM_CTRL_BASE;
	
	p[0] = 0x22011110;//BWSCON
	p[1] = 0x00000700;//BANKCON0
	p[2] = 0x00000700;//BANKCON1
	p[3] = 0x00000700;//BANKCON2
	p[4] = 0x00000700;//BANKCON3
	p[5] = 0x00000700;//BANKCON4
	p[6] = 0x00000700;//BANKCON5
	p[7] = 0x00018005;//BANKCON6
	p[8] = 0x00018005;//BANKCON7

	p[9] = 0x008C04F4;//REFRESH HCLK=12MHz:0x008C07A	3; HCLK=100MHz:0x008C04F4
	p[10] = 0x000000B1;//BANKSIZE
	p[11] = 0x00000030;//MRSRB6
	p[12] = 0x00000030;//MRSRB7
}

void CopySteppingstoneToSDRAM(void)
{
	unsigned int * pdwSrc = (unsigned int *)0;
	unsigned int * pdwDest = (unsigned int *)0x30000000;
	
	while (pdwSrc < (unsigned int *)4096)
	{
		*pdwDest = *pdwSrc;
		pdwDest++;
		pdwSrc++;
	}
}
