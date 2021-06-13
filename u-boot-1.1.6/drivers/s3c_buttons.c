
#include <common.h>


#ifdef S3C_BUTTONS


#define BUTTON_1		0
#define BUTTON_2		2
#define BUTTON_3		3

#define BUTTON_VALUE_UP		0x01
#define BUTTON_VALUE_DOWN	0x02
#define BUTTON_VALUE_OK		0x04

#define GPFCON			(*(volatile unsigned long *)0x56000050)
#define GPFDAT			(*(volatile unsigned long *)0x56000054)
#define GPFUP			(*(volatile unsigned long *)0x56000058)

#define GPGCON			(*(volatile unsigned long *)0x56000060)
#define GPGDAT			(*(volatile unsigned long *)0x56000064)
#define GPGUP			(*(volatile unsigned long *)0x56000068)

static int key_val = 0;

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

int s3c_button_init(void)
{
	SET_GPIOCONFIG(GPFCON, BUTTON_1, 0);
	SET_GPIOCONFIG(GPFCON, BUTTON_2, 0);
	SET_GPIOCONFIG(GPGCON, BUTTON_3, 0);

	SET_GPIOCONFIG(GPFCON, 4, 1);
	SET_GPIOCONFIG(GPFCON, 5, 1);
	SET_GPIOCONFIG(GPFCON, 6, 1);

	/*  */
	CLR_GPIO(GPFDAT,4);
	SET_GPIO(GPFDAT,5);
	SET_GPIO(GPFDAT,6);
	
	return 0;
}

static void button_heart(void)
{
	static int s_led_flag = 0;/* flash led */

	s_led_flag++;
	if (s_led_flag == 10)
	{
		SET_GPIO(GPFDAT,4);
	}
	else if (s_led_flag == 20)
	{
		s_led_flag = 0;
		CLR_GPIO(GPFDAT,4);
	}
}

/* 
	10ms period
*/
int active_buttons(void)
{
	button_heart();
	if (GPFDAT & (1 << BUTTON_1))
	{
		key_val &= (~BUTTON_VALUE_UP);
	}
	else
	{
		key_val |= BUTTON_VALUE_UP;
	}
	
	if (GPFDAT & (1 << BUTTON_2))
	{
		key_val &= (~BUTTON_VALUE_DOWN);
	}
	else
	{
		key_val |= BUTTON_VALUE_DOWN;
	}
	
	if (GPGDAT & (1 << BUTTON_3))
	{
		key_val &= (~BUTTON_VALUE_OK);
	}
	else
	{
		key_val |= BUTTON_VALUE_OK;
	}

	return key_val;
}


#endif

