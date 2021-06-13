/*
 * Configuation settings for the sh7753evb board
 *
 * Copyright (C) 2012 Renesas Solutions Corp.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __SH7753EVB_H
#define __SH7753EVB_H

#undef DEBUG
#define CONFIG_CPU_SH7753	1
#define CONFIG_SH7753EVB	1

#define CONFIG_SYS_TEXT_BASE	0x5ff80000
#define CONFIG_SYS_LDSCRIPT	"board/renesas/sh7753evb/u-boot.lds"

#define CONFIG_CMD_DFL
#define CONFIG_CMD_SDRAM
#define CONFIG_CMD_MD5SUM
#define CONFIG_MD5
#define CONFIG_DOS_PARTITION
#define CONFIG_MAC_PARTITION

#define CONFIG_BAUDRATE		115200
#define CONFIG_BOOTDELAY	3
#define CONFIG_BOOTARGS		"console=ttySC2,115200 root=/dev/nfs ip=dhcp"

#define CONFIG_VERSION_VARIABLE
#undef	CONFIG_SHOW_BOOT_PROGRESS
#define CONFIG_CMDLINE_EDITING
#define CONFIG_AUTO_COMPLETE

/* MEMORY */
#define SH7753EVB_SDRAM_BASE		(0x40000000)
#define SH7753EVB_SDRAM_SIZE		(512 * 1024 * 1024)

#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_CBSIZE		256
#define CONFIG_SYS_PBSIZE		256
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_BARGSIZE		512
#define CONFIG_SYS_BAUDRATE_TABLE	{ 115200 }

/* SCIF */
#define CONFIG_SCIF_CONSOLE	1
#define CONFIG_CONS_SCIF2	1
#undef	CONFIG_SYS_CONSOLE_INFO_QUIET
#undef	CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
#undef	CONFIG_SYS_CONSOLE_ENV_OVERWRITE

#define CONFIG_SYS_MEMTEST_START	(SH7753EVB_SDRAM_BASE)
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + \
					 480 * 1024 * 1024)
#undef	CONFIG_SYS_ALT_MEMTEST
#undef	CONFIG_SYS_MEMTEST_SCRATCH
#undef	CONFIG_SYS_LOADS_BAUD_CHANGE

#define CONFIG_SYS_SDRAM_BASE		(SH7753EVB_SDRAM_BASE)
#define CONFIG_SYS_SDRAM_SIZE		(SH7753EVB_SDRAM_SIZE)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + \
					 128 * 1024 * 1024)

#define CONFIG_SYS_MONITOR_BASE		0x00000000
#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(4 * 1024 * 1024)
#define CONFIG_SYS_BOOTMAPSZ		(8 * 1024 * 1024)

/* FLASH */
#define CONFIG_SYS_NO_FLASH

/* Ether */
#define CONFIG_SH_ETHER			1
#define CONFIG_SH_ETHER_USE_PORT	0
#define CONFIG_SH_ETHER_PHY_ADDR	18
#define CONFIG_SH_ETHER_CACHE_WRITEBACK	1
#define CONFIG_SH_ETHER_USE_GETHER	1
#define CONFIG_PHYLIB
#define CONFIG_BITBANGMII
#define CONFIG_BITBANGMII_MULTI
#define CONFIG_SH_ETHER_PHY_MODE PHY_INTERFACE_MODE_RGMII
#define CONFIG_PHY_VITESSE

#define SH7753EVB_ETHERNET_MAC_BASE_SPI	0x00090000
#define SH7753EVB_SPI_SECTOR_SIZE	(64 * 1024)
#define SH7753EVB_ETHERNET_MAC_BASE	SH7753EVB_ETHERNET_MAC_BASE_SPI
#define SH7753EVB_ETHERNET_MAC_SIZE	17
#define SH7753EVB_ETHERNET_NUM_CH	2
#define CONFIG_BOARD_LATE_INIT

/* SPI */
#define CONFIG_SH_SPI			1
#define CONFIG_SH_SPI_BASE		0xfe002000

/* MMCIF */
#define CONFIG_MMC			1
#define CONFIG_GENERIC_MMC		1
#define CONFIG_SH_MMCIF			1
#define CONFIG_SH_MMCIF_ADDR		0xffcb0000
#define CONFIG_SH_MMCIF_CLK		48000000

/* ENV setting */
#define CONFIG_ENV_IS_EMBEDDED
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SECT_SIZE	(64 * 1024)
#define CONFIG_ENV_ADDR		(0x00080000)
#define CONFIG_ENV_OFFSET	(CONFIG_ENV_ADDR)
#define CONFIG_ENV_OVERWRITE	1
#define CONFIG_ENV_SIZE		(CONFIG_ENV_SECT_SIZE)
#define CONFIG_ENV_SIZE_REDUND	(CONFIG_ENV_SECT_SIZE)
#define CONFIG_EXTRA_ENV_SETTINGS				\
		"netboot=bootp; bootm\0"

/* Board Clock */
#define CONFIG_SYS_CLK_FREQ	48000000
#define CONFIG_SH_TMU_CLK_FREQ CONFIG_SYS_CLK_FREQ
#define CONFIG_SH_SCIF_CLK_FREQ CONFIG_SYS_CLK_FREQ
#define CONFIG_SYS_TMU_CLK_DIV	4
#endif	/* __SH7753EVB_H */
