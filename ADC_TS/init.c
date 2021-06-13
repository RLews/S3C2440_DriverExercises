#include "s3c24xx.h"

void disable_watch_dog(void);
void clock_init(void);
void memsetup(void);
void copy_steppingstone_to_sdram(void);
void clean_bss(void);

void disable_watch_dog(void)
{
	WTCON = 0;
}

#define FCLK				200000000
#define HCLK				100000000
#define PCLK				50000000
#define S3C2410_MPLL_200MHZ	((0x5C << 12) | (0x04 << 4) | (0x00))
#define S3C2440_MPLL_200MHZ	((0x5C << 12) | (0x01 << 4) | (0x02))

/*
 * 对于MPLLCON寄存器，[19:12]为MDIV，[9:4]为PDIV，[1:0]为SDIV
 * 有如下计算公式：
 *  S3C2410: MPLL(FCLK) = (m * Fin)/(p * 2^s)
 *  S3C2410: MPLL(FCLK) = (2 * m * Fin)/(p * 2^s)
 *  其中: m = MDIV + 8, p = PDIV + 2, s = SDIV
 * 对于本开发板，Fin = 12MHz
 * 设置CLKDIVN，令分频比为：FCLK:HCLK:PCLK=1:2:4，
 * FCLK=200MHz,HCLK=100MHz,PCLK=50MHz
 */
void clock_init(void)
{
	CLKDIVN = 0x03;
	
	__asm__(
	"mrc	p15, 0, r1, c1, c0, 0\n"		//读出控制寄存器
	"orr	r1, r1, #0xC0000000\n"			//设置为asynchronous bus mode
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

/*
 * 设置存储控制器以使用SDRAM
 */
void memsetup(void)
{
    volatile unsigned long *p = (volatile unsigned long *)MEM_CTL_BASE;

    /* 这个函数之所以这样赋值，而不是像前面的实验(比如mmu实验)那样将配置值
     * 写在数组中，是因为要生成”位置无关的代码”，使得这个函数可以在被复制到
     * SDRAM之前就可以在steppingstone中运行
     */
    /* 存储控制器13个寄存器的值 */
    p[0] = 0x22011110;     //BWSCON
    p[1] = 0x00000700;     //BANKCON0
    p[2] = 0x00000700;     //BANKCON1
    p[3] = 0x00000700;     //BANKCON2
    p[4] = 0x00000700;     //BANKCON3  
    p[5] = 0x00000700;     //BANKCON4
    p[6] = 0x00000700;     //BANKCON5
    p[7] = 0x00018005;     //BANKCON6
    p[8] = 0x00018005;     //BANKCON7
    
                                    /* REFRESH,
                                     * HCLK=12MHz:  0x008C07A3,
                                     * HCLK=100MHz: 0x008C04F4
                                     */ 
    p[9]  = 0x008C04F4;
    p[10] = 0x000000B1;     //BANKSIZE
    p[11] = 0x00000030;     //MRSRB6
    p[12] = 0x00000030;     //MRSRB7
}

void copy_steppingstone_to_sdram(void)
{
	unsigned int *pdwSrc = (unsigned int *)0;
	unsigned int *pdwDest = (unsigned int *)0x30000000;
	
	while (pdwSrc < (unsigned int *)4096)
	{
		*pdwDest = *pdwSrc;
		pdwDest++;
		pdwSrc++;
	}
}

void clean_bss(void)
{
	extern int __bss_start, __bss_end;
	int *p = &__bss_start;
	
	for (; p < &__bss_end; p++)
	{
		*p = 0;
	}
}

int CopyCode2SDRAM(unsigned char *buf, unsigned long start_addr, int size)
{
	extern void nand_read(unsigned char *buf, unsigned long start_addr, int size);
	nand_read(buf, start_addr, size);
	return 0;
}
