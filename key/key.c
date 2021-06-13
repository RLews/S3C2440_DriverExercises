
#define GPFCON			(*(volatile unsigned long *)0x56000050)
#define GPFDAT			(*(volatile unsigned long *)0x56000054)
#define GPFUP			(*(volatile unsigned long *)0x56000058)

#define GPGCON			(*(volatile unsigned long *)0x56000060)
#define GPGDAT			(*(volatile unsigned long *)0x56000064)
#define GPGUP			(*(volatile unsigned long *)0x56000068)

/*
	dir:
		00 = input
		01 = output
		10 = EINT
		11 = Reserved
*/
#define SET_GPIOCONFIG(reg,port,dir) \
{\
	reg &= ~(3 << (port << 1));\
	reg |= (dir << (port << 1));\
}

#define SET_GPIO(reg,port) \
{\
	reg |= (1 << port);\
}

#define CLR_GPIO(reg, port) \
{\
	reg &= ~(1 << port);\
}

int main(void)
{
	unsigned long lw_dat;
	
	SET_GPIOCONFIG(GPFCON, 4, 1);
	SET_GPIOCONFIG(GPFCON, 5, 1);
	SET_GPIOCONFIG(GPFCON, 6, 1);
	
	SET_GPIOCONFIG(GPFCON, 0, 0);
	SET_GPIOCONFIG(GPFCON, 2, 0);
	SET_GPIOCONFIG(GPGCON, 3, 0);
	
	while (1)
	{
		lw_dat = GPFDAT;
		if (lw_dat & (1 << 0))	{SET_GPIO(GPFDAT,4);}
		else					{CLR_GPIO(GPFDAT,4);}
		
		if (lw_dat & (1 << 2))	{SET_GPIO(GPFDAT,5);}
		else					{CLR_GPIO(GPFDAT,5);}
		
		lw_dat = GPGDAT;
		if (lw_dat & (1 << 3))	{SET_GPIO(GPFDAT,6);}
		else					{CLR_GPIO(GPFDAT,6);}
	}
	
	return 0;
}