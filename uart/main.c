#include "uart.h"

int main(void)
{
	unsigned char c;
	UartInit();//115200bps 8bit 1bit None
	
	while (1)
	{
		c = getc();
		if (isDigit(c) || isLetter(c))
		{
			putc(c+1);
		}
	}
	
	return 0;
}
