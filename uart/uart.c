
#include "s3c24xx.h"
#include "uart.h"

#define TX0READY		(1<<2)
#define RX0READY		(1)

#define PCLK			50000000		//init 中设置PCLK为50MHz
#define UART_CLK		PCLK
#define UART_BAUD_RATE	115200
#define UART_BRD		((UART_CLK / (UART_BAUD_RATE * 16)) - 1)


void UartInit(void)
{
	GPHCON |= 0xA0;			//GPH2,GPH3 used TXD RXD
	GPHUP = 0x0C;			//GPH2,GPH3 pull on
	ULCON0 = 0x03;			//8N1
	UCON0 = 0x05;			//scan mode  clock used PCLK
	UFCON0 = 0x00;			//Not used FIFO
	UMCON0 = 0x00;			//Not used Flow Control
	UBRDIV0 = UART_BRD;		//115200bps
}

void putc(unsigned char c)
{
	while (!(UTRSTAT0 & TX0READY));
	UTXH0 = c;
}

unsigned char getc(void)
{
	while (!(UTRSTAT0 & RX0READY));
	return URXH0;
}

int isDigit(unsigned char c)
{
	if (c >= '0' && c <= '9')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int isLetter(unsigned char c)
{
	if (c >= 'a' && c <= 'z')
	{
		return 1;
	}
	else if (c >= 'A' && c <= 'Z')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
