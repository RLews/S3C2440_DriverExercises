
#define LARGER_NAND_PAGE

#define GSTATUS1		(*(volatile unsigned int *)0x560000B0)
#define BUSY			1

#define NAND_SECTOR_SIZE    512
#define NAND_BLOCK_MASK     (NAND_SECTOR_SIZE - 1)

#define NAND_SECTOR_SIZE_LP		2048
#define NAND_BLOCK_MASK_LP		(NAND_SECTOR_SIZE_LP - 1)

typedef unsigned int S3C24X0_REG32;

/* NAND Flash see s3c2410 manual chapter 6 */
typedef struct {
	S3C24X0_REG32	NFCONF;
	S3C24X0_REG32	NFCMD;
	S3C24X0_REG32	NFADDR;
	S3C24X0_REG32	NFDATA;
	S3C24X0_REG32	NFSTAT;
	S3C24X0_REG32	NFECC;
}S3C2410_NAND;

/* NAND Flash see s3c2440 manual chapter 6 */
typedef struct {
	S3C24X0_REG32   NFCONF;
    S3C24X0_REG32   NFCONT;
    S3C24X0_REG32   NFCMD;
    S3C24X0_REG32   NFADDR;
    S3C24X0_REG32   NFDATA;
    S3C24X0_REG32   NFMECCD0;
    S3C24X0_REG32   NFMECCD1;
    S3C24X0_REG32   NFSECCD;
    S3C24X0_REG32   NFSTAT;
    S3C24X0_REG32   NFESTAT0;
    S3C24X0_REG32   NFESTAT1;
    S3C24X0_REG32   NFMECC0;
    S3C24X0_REG32   NFMECC1;
    S3C24X0_REG32   NFSECC;
    S3C24X0_REG32   NFSBLK;
    S3C24X0_REG32   NFEBLK;
}S3C2440_NAND;

typedef struct {
	void (*nand_reset)(void);
	void (*wait_idle)(void);
	void (*nand_select_chip)(void);
	void (*nand_deselect_chip)(void);
	void (*write_cmd)(int cmd);
	void (*write_addr)(unsigned int addr);
	unsigned char (*read_data)(void);
}t_nand_chip;

static S3C2410_NAND * s3c2410nand = (S3C2410_NAND *)0x4E000000;
static S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4E000000;

static t_nand_chip nand_chip;

void NandFlashInit(void);
void NandRead(unsigned char *buf, unsigned long start_addr, int size);

static void nand_reset(void);
static void wait_idle(void);
static void nand_select_chip(void);
static void nand_deselect_chip(void);
static void write_cmd(int cmd);
static void write_addr(unsigned int addr);
static unsigned char read_data(void);

static void s3c2410_nand_reset(void);
static void s3c2410_wait_idle(void);
static void s3c2410_nand_select_chip(void);
static void s3c2410_nand_deselect_chip(void);
static void s3c2410_write_cmd(int cmd);
static void s3c2410_write_addr(unsigned int addr);
static unsigned char s3c2410_read_data(void);

static void s3c2440_nand_reset(void);
static void s3c2440_wait_idle(void);
static void s3c2440_nand_select_chip(void);
static void s3c2440_nand_deselect_chip(void);
static void s3c2440_write_cmd(int cmd);
static void s3c2440_write_addr(unsigned int addr);
static unsigned char s3c2440_read_data(void);

static void delay_10ts(void);

static void delay_10ts(void)
{
	int i;
	for (i = 0; i < 10; i++);
}

static void s3c2410_nand_reset(void)
{
	s3c2410_nand_select_chip();
	s3c2410_write_cmd(0xFF); //reset cmd
	s3c2410_wait_idle();
	s3c2410_nand_deselect_chip();
}

static void s3c2410_wait_idle(void)
{
	volatile unsigned char * p = (volatile unsigned char *)&s3c2410nand->NFSTAT;
	
	while (!(*p & BUSY))
	{
		delay_10ts();
	}
}

static void s3c2410_nand_select_chip(void)
{
	s3c2410nand->NFCONF &= ~(1 << 11);
	delay_10ts();
}

static void s3c2410_nand_deselect_chip(void)
{
	s3c2410nand->NFCONF |= (1 << 11);
}

static void s3c2410_write_cmd(int cmd)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFCMD;
	*p = cmd;
}

static void s3c2410_write_addr(unsigned int addr)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFADDR;
	
	*p = addr & 0xFF;
	delay_10ts();
	*p = (addr >> 9) & 0xFF;
	delay_10ts();
	*p = (addr >> 17) & 0xFF;
	delay_10ts();
	*p = (addr >> 25) & 0xFF;
	delay_10ts();
}

static unsigned char s3c2410_read_data(void)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2410nand->NFDATA;
	return *p;
}


static void s3c2440_nand_reset(void)
{
	s3c2440_nand_select_chip();
	s3c2440_write_cmd(0xFF); // reset cmd
	s3c2440_wait_idle();
	s3c2440_nand_deselect_chip();
}

static void s3c2440_wait_idle(void)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFSTAT;
	while (!(*p & BUSY))
	{
		delay_10ts();
	}
}

static void s3c2440_nand_select_chip(void)
{
	s3c2440nand->NFCONT &= ~(1 << 1);
	delay_10ts();
}

static void s3c2440_nand_deselect_chip(void)
{
	s3c2440nand->NFCONT |= (1 << 1);
}

static void s3c2440_write_cmd(int cmd)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFCMD;
	*p = cmd;
}

static void s3c2440_write_addr(unsigned int addr)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;
	
	*p = addr & 0xFF;
	delay_10ts();
	*p = (addr >> 9) & 0xFF;
	delay_10ts();
	*p = (addr >> 17) & 0xFF;
	delay_10ts();
	*p = (addr >> 25) & 0xFF;
	delay_10ts();
}

static void s3c2440_write_addr_lp(unsigned int addr)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;
	int col,page;
	
	col = addr & NAND_BLOCK_MASK_LP;
	page = addr / NAND_SECTOR_SIZE_LP;
	
	*p = col & 0xFF;				//colunm address A0 - A7
	delay_10ts();
	*p = (col >> 8) & 0x0F;			//colunm address A8 - A11
	delay_10ts();
	*p = page & 0xFF;				//row address A12 - A19
	delay_10ts();
	*p = (page >> 8) & 0xFF;		//row address A20 - A27
	delay_10ts();
	*p = (page >> 16) & 0x03;		//row address A28 - A29
	delay_10ts();
}


static unsigned char s3c2440_read_data(void)
{
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFDATA;
	return *p;
}

static void nand_reset(void)
{
	nand_chip.nand_reset();
}

static void wait_idle(void)
{
	nand_chip.wait_idle();
}

static void nand_select_chip(void)
{
	nand_chip.nand_select_chip();
	delay_10ts();
}

static void nand_deselect_chip(void)
{
	nand_chip.nand_deselect_chip();
}

static void write_cmd(int cmd)
{
	nand_chip.write_cmd(cmd);
}

static void write_addr(unsigned int addr)
{
	nand_chip.write_addr(addr);
}

static unsigned char read_data(void)
{
	return nand_chip.read_data();
}

void NandFlashInit(void)
{
#define TACLS		0
#define TWRPH0		3
#define TWRPH1		0

	if ( (GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002) )
	{
		nand_chip.nand_reset = s3c2410_nand_reset;
		nand_chip.wait_idle = s3c2410_wait_idle;
		nand_chip.nand_select_chip = s3c2410_nand_select_chip;
		nand_chip.nand_deselect_chip = s3c2410_nand_deselect_chip;
		nand_chip.write_cmd = s3c2410_write_cmd;
		nand_chip.write_addr = s3c2410_write_addr;
		nand_chip.read_data = s3c2410_read_data;
		
		s3c2410nand->NFCONF = (1 << 15)|(1 << 12)|(1 << 11)|(TACLS << 8)|(TWRPH0 << 4)|(TWRPH1 << 0);
		
	}
	else
	{
		nand_chip.nand_reset = s3c2440_nand_reset;
		nand_chip.wait_idle = s3c2440_wait_idle;
		nand_chip.nand_select_chip = s3c2440_nand_select_chip;
		nand_chip.nand_deselect_chip = s3c2440_nand_deselect_chip;
		nand_chip.write_cmd = s3c2440_write_cmd;
	#ifdef LARGER_NAND_PAGE
		nand_chip.write_addr = s3c2440_write_addr_lp;
	#else
		nand_chip.write_addr = s3c2440_write_addr;
	#endif
		nand_chip.read_data = s3c2440_read_data;
		//设置时序
		s3c2440nand->NFCONF = (TACLS << 12)|(TWRPH0 << 8)|(TWRPH1 << 4);
		//使能NAND Flash 控制器 初始化ECC 禁止片选
		s3c2440nand->NFCONT = (1<<4)|(1<<1)|(1<<0);
	}
	nand_reset();
}

void NandRead(unsigned char *buf, unsigned long start_addr, int size)
{
	int i,j;
	
#ifdef LARGER_NAND_PAGE
	if ( (start_addr & NAND_BLOCK_MASK_LP) || (size & NAND_BLOCK_MASK_LP) )
	{
		return;//地址或长度不对齐
	}
#else
	if ( (start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK) )
	{
		return;
	}
#endif
	nand_select_chip();
	
	for (i = start_addr; i < (start_addr + size); )
	{
		write_cmd(0);		//READ0 cmd
		write_addr(i);		//write address
	#ifdef LARGER_NAND_PAGE
		write_cmd(0x30);
	#endif
		wait_idle();
		
	#ifdef LARGER_NAND_PAGE
		for (j = 0; j < NAND_SECTOR_SIZE_LP; j++, i++)
		{
	#else
		for (j = 0; j < NAND_SECTOR_SIZE; j++, i++)
		{
	#endif
			*buf = read_data();
			buf++;
		}
	}
	
	nand_deselect_chip();
	
	return;
}