#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>


#define MEM_CPY_NO_DMA		0
#define MEM_CPY_DMA			1

void print_usage(char *name)
{
	printf("Usage: \n");
	printf("%s <nodma | dma>\n", name);
}

/*
	./dma_test /dev/dma dma
*/
int main(int argc, char ** argv)
{
	int fd;

	if (argc != 3)
	{
		print_usage(argv[0]);
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd < 0)
	{
		printf("can`t open %s \n", argv[1]);
		return -1;
	}
	else
	{
		if (strcmp(argv[2],"dma") == 0)
		{
			while (1)
			{
				ioctl(fd, MEM_CPY_DMA);
			}
		}
		else if (strcmp(argv[2],"nodma") == 0)
		{
			while (1)
			{
				ioctl(fd, MEM_CPY_NO_DMA);
			}
			
		}
		else
		{
			print_usage(argv[0]);
			return -1;
		}

	}

	return 0;
	
}

