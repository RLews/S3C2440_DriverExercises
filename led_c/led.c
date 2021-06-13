
#define GPFCON		(*(volatile unsigned long *)0x56000050)
#define GPFDAT		(*(volatile unsigned long *)0x56000054)

int main()
{
	GPFCON = 0x00000100;	//GPF4 Output 
	GPFDAT = 0x00000000;	//GPF4 Output low
	
	return 0;
}