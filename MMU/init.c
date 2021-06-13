/*
 *	init.c 执行一些初始化
 *		属第一部分程序，此时MMU未开启
 */
 
#define WTCON			(*(volatile unsigned long *) 0x53000000)

#define MEM_CTRL_BASE	0x48000000

void DisableWatchDog(void)
{
	WTCON = 0;
}

void MemSetup(void)
{
	unsigned long const mem_cfg_val[] = {
		0x22011110,		//BWSCON
		0x00000700,		//BANKCON0
		0x00000700,		//BANKCON1
		0x00000700,		//BANKCON2
		0x00000700,		//BANKCON3
		0x00000700,		//BANKCON4
		0x00000700,		//BANKCON5
		0x00018005,		//BANKCON6
		0x00018005,		//BANKCON7
		0x008C07A3,		//REFRESH
		0x000000B1,		//BANKSIZE
		0x00000030,		//MRSRB6
		0x00000030,		//MRSRB7
	};
	int i = 0;
	
	volatile unsigned long *p = (volatile unsigned long *)MEM_CTRL_BASE;
	
	for ( ; i < 13; i++)
	{
		p[i] = mem_cfg_val[i];
	}
	
}

void Copy_2th_sdram(void)
{
	unsigned int *pdwSrc = (unsigned int *)2048;
	unsigned int *pdwDest = (unsigned int *)0x30004000;
	
	while (pdwSrc < (unsigned int *)4096)
	{
		*pdwDest = *pdwSrc;
		pdwDest++;
		pdwSrc++;
	}
}

void CreatePageTable(void)
{
	/*
	*  段描述的宏定义
	*/
#define MMU_FULL_ACCESS		(3 << 10)	//访问权限
#define MMU_DOMAIN			(0 << 5)	//属于哪个域
#define MMU_SPECIAL			(1 << 4)	//必须是1
#define MMU_CACHEABLE		(1 << 3)	//cacheable
#define MMU_BUFFERABLE		(1 << 2)	//bufferable
#define MMU_SECTION			(2)			//段描述符
#define MMU_SECDESC			(MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_SECTION)
#define MMU_SECDESC_WB		(MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_CACHEABLE | MMU_BUFFERABLE | MMU_SECTION)
#define MMU_SECTION_SIZE	0x00100000

	unsigned long virtualaddr, physicaladdr;
	unsigned long * mmu_tlb_base = (unsigned long *)0x30000000;
	
	/*
	* Steppingstone的起始物理地址为0， 第一部分程序的起始地址也是0
	* 开启MMU后仍能运行第一部分程序， 将0 - 1M的虚拟地址映射到同样的物理地址
	*/
	virtualaddr = 0;
	physicaladdr = 0;
	*(mmu_tlb_base + (virtualaddr >> 20)) = (physicaladdr & 0xFFF00000) | MMU_SECDESC_WB;
	
	/*
	* 0x56000000是寄存器地址
	* GPBCON和GPBDAT这两个寄存器的物理地址0x56000000和0x56000014 ， 在第二部分程序能以地址0xA0000010， 0xA0000014 来操作GPBCON，GPBDAT
	* 把从0xA0000000开始的1M虚拟地址映射到0x56000000开始的1M物理空间
	*/
	virtualaddr = 0xA0000000;
	physicaladdr = 0x56000000;
	*(mmu_tlb_base + (virtualaddr >> 20)) = (physicaladdr & 0xFFF00000) | MMU_SECDESC;
	
	/*
	* SDRAM 物理地址范围是0x30000000 - 0x33FFFFFF
	* 将虚拟地址0xB0000000 - 0xB3FFFFFF映射到物理地址0x30000000 - 0x33FFFFFF； 总共64M 64个段描述符
	*/
	virtualaddr = 0xB0000000;
	physicaladdr = 0x30000000;
	while (virtualaddr < 0xB4000000)
	{
		*(mmu_tlb_base + (virtualaddr >> 20)) = (physicaladdr & 0xFFF00000) | MMU_SECDESC_WB;
		virtualaddr += 0x100000;
		physicaladdr += 0x100000;
	}
}

void MMU_Init(void)
{
	unsigned long ttb = 0x30000000;
	
__asm__(
	"MOV	R0,		#0\n"
	"MCR	p15,	0,	r0,	c7,	c7,	0\n"		//使无效ICahce和DCache
	"MCR	p15,	0,	r0,	c7,	c10,	4\n"	//drain write buffer on v4
	"MCR	p15,	0,	r0,	c8,	c7,	0\n"		//使无效指令、数据TLB
	
	"MOV	R4,		%0\n"						//R4 = 页表基址
	"MCR	p15,	0,	r4,	c2,	c0,	0\n"		//设置页表基址寄存器
	
	"MVN	R0,		#0\n"						
	"MCR	p15,	0,	r0,	c3,	c0,	0\n"		//域访问寄存器设为0xFFFFFFFF；不进行权限检查
	
	"MCR	p15,	0,	r0,	c1,	c0,	0\n"		//读出寄存器的值
	/*
	* 控制寄存器的低16位含义为：.RVI ..RS B... .CAM
	* R:表示换出Cache中的条目中使用的算法，
	*			0 = Random replacement； 1 = Round robin replacement
	* V:表示异常向量表所在的位置，
	*			0 = Low address = 0x00000000 1 = high address = 0xFFFF0000
	* I：0 = 关闭ICaches；1 = 开启ICahces
	* R、S：用来与页表中的描述符一起确定内存访问权限
	* B：0 = CPU为小字节序；1 = CPU为大字节序
	* C: 0 = 关闭DCaches；1 = 开启DCaches
	* A: 0 = 数据访问时不进行地址对齐检查；1 = 数据访问时进行地址对齐检查
	* M: 0 = 关闭MMU；1 = 开启MMU
	*/
	/* 先清除不需要的位，往下若需要则重新设置 */
	"bic	r0,	r0,	#0x3000\n"		//清除V I 位
	"bic	r0,	r0,	#0x0300\n"		//清除R S 位
	"bic	r0,	r0,	#0x0087\n"		//清除B C A M
	/* 设置需要的位 */
	"ORR	r0,	r0,	#0x0002\n"		//开启对齐检查
	"ORR	r0,	r0,	#0x0004\n"		//开启DCache
	"ORR	r0,	r0,	#0x1000\n"		//开启ICache
	"ORR	r0,	r0,	#0x0001\n"		//使能MMU
	
	"MCR	p15,	0,	r0,	c1,	c0,	0\n"	//将修改的值写入控制寄存器
	:	//无输出
	: "r" (ttb) );
}