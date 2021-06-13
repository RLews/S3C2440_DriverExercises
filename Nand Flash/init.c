
#define WTCON					(*(volatile unsigned long *)0x53000000)

#define MEM_CTRL_BASE			0x48000000

void DisableWatchDog(void);
void MemSetup(void);

void DisableWatchDog(void)
{
	WTCON = 0;
}

void MemSetup(void)
{
	int i = 0;
	unsigned long *p = (unsigned long *)MEM_CTRL_BASE;
	unsigned long const mem_cfg_val[] = {
		0x22011110,//BWSCON
		0x00000700,//BANKCON0
		0x00000700,//BANKCON1
		0x00000700,//BANKCON2
		0x00000700,//BANKCON3
		0x00000700,//BANKCON4
		0x00000700,//BANKCON5
		0x00018005,//BANKCON6
		0x00018005,//BANKCON7
		0x008C07A3,//REFRESH
		0x000000B1,//BANKSIZE
		0x00000030,//MRSRB6
		0x00000030,//MRSRB7
	};
	
	for (i = 0; i < 13; i++)
	{
		p[i] = mem_cfg_val[i];
	}
}