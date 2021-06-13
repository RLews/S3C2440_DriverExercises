



#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/clk.h>
 
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
 
#include <asm/io.h>
 
#include <asm/arch/regs-nand.h>
#include <asm/arch/nand.h>


struct S3C_NAND_REGS_T {
	unsigned long nfconf  ;
	unsigned long nfcont  ;
	unsigned long nfcmd   ;
	unsigned long nfaddr  ;
	unsigned long nfdata  ;
	unsigned long nfeccd0 ;
	unsigned long nfeccd1 ;
	unsigned long nfeccd  ;
	unsigned long nfstat  ;
	unsigned long nfestat0;
	unsigned long nfestat1;
	unsigned long nfmecc0 ;
	unsigned long nfmecc1 ;
	unsigned long nfsecc  ;
	unsigned long nfsblk  ;
	unsigned long nfeblk  ;
};


static struct nand_chip *s3c_nand;
static struct mtd_info *s3c_mtd;
static struct S3C_NAND_REGS_T *s3c_nand_regs;

static void s3c2440_select_chip(struct mtd_info *mtd, int chipnr)
{
	if (chipnr == -1)
	{
		s3c_nand_regs->nfcont |= (1<<1);
	}
	else
	{
		s3c_nand_regs->nfcont |= ~(1<<1);
	}
}

static void s3c2440_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
	if (ctrl & NAND_CLE)
	{
		s3c_nand_regs->nfcmd = dat;
	}
	else
	{
		s3c_nand_regs->nfaddr = dat;
	}
}

static int s3c2440_dev_ready(struct mtd_info *mtd)
{
	return (s3c_nand_regs->nfstat & (1<<0));
}

static int s3c_nand_init(void)
{
	struct clk *clk;

	/* nand_chip struct */
	s3c_nand = kzalloc(sizeof(struct nand_chip), GFP_KERNEL);

	s3c_nand_regs = ioremap(0x4E000000, sizeof(struct S3C_NAND_REGS_T));

	/* set nand_chip */
	s3c_nand->select_chip = s3c2440_select_chip;
	s3c_nand->cmd_ctrl = s3c2440_cmd_ctrl;
	s3c_nand->IO_ADDR_R = &s3c_nand_regs->nfdata;
	s3c_nand->IO_ADDR_W = &s3c_nand_regs->nfdata;
	s3c_nand->dev_ready = s3c2440_dev_ready;
	s3c_nand->ecc.mode = NAND_ECC_SOFT;

	/* hardware */
	clk = clk_get(NULL, "nand");
	clk_enable(clk);

	/* HCLK=100MHz
	 * TACLS:  发出CLE/ALE之后多长时间才发出nWE信号, 从NAND手册可知CLE/ALE与nWE可以同时发出,所以TACLS=0
	 * TWRPH0: nWE的脉冲宽度, HCLK x ( TWRPH0 + 1 ), 从NAND手册可知它要>=12ns, 所以TWRPH0>=1
	 * TWRPH1: nWE变为高电平后多长时间CLE/ALE才能变为低电平, 从NAND手册可知它要>=5ns, 所以TWRPH1>=0
	 */
#define TACLS    0
#define TWRPH0   1
#define TWRPH1   0
	s3c_nand_regs->nfconf = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);

	s3c_nand_regs->nfcont = (1<<1)|(1);

	/* nand_scan */
	s3c_mtd = kzalloc(sizeof(struct mtd_info), GFP_KERNEL);
	s3c_mtd->owner = THIS_MODULE;
	s3c_mtd->priv = s3c_nand;

	nand_scan(s3c_mtd, 1);

	/* add_mtd_partitions */

	return 0;
}

static void s3c_nand_exit(void)
{
	kfree(s3c_mtd);
	iounmap(s3c_nand_regs);
	kfree(s3c_nand);
}

module_init(s3c_nand_init);
module_exit(s3c_nand_exit);

MODULE_LICENSE("GPL");


