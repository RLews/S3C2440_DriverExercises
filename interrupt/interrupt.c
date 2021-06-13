#include "s3c24xx.h"

#define LED_ALL_OFF			{GPFDAT |= (0x7 << 4);}

void EINT_Handle(void)
{
	unsigned long oft = INTOFFSET;
	//unsigned long val;
	
	switch (oft)
	{
		case 0:
		{
			LED_ALL_OFF;
			GPFDAT &= ~(1<<4);
			break;
		}
		case 2:
		{
			LED_ALL_OFF;
			GPFDAT &= ~(1<<5);
			break;
		}
		case 5:
		{
			LED_ALL_OFF;
			GPFDAT &= ~(1<<6);
			break;
		}
		default:
			break;
	}
	
	if (oft == 5)
	{
		EINTPEND = (1 << 11);//EINT8 23 合用IRQ5
	}
	SRCPND = 1 << oft;
	INTPND = 1 << oft;
}
