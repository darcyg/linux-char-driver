#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	int fd;
	fd = open("/dev/mydriver", O_RDWR);
	if (fd < 0)
	{
		printf("open err\n");
		return 0;
	}

	char cmd;
        unsigned char buff[2];
        float temp;
	while (1)
	{
		printf("please input r or q; r-read, q-quit\n");
		scanf("%c", &cmd);
		switch (cmd)
		{
			case 'q':
			scanf("%c",&cmd);
			close(fd);
			return 0;	
			break;
			
			case 'r':
			scanf("%c",&cmd);
			temp = read(fd, buff, 2);
			
			if (0!=temp)
			{
				temp = buff[0] | (buff[1]<<8);
				temp = 3.3*temp/1024;
				printf ("ADC is %f\n", temp);
			}
			else
			{
				printf ("read err\n");
			}
			break;	
			
			default:
			break;
		}
	}
			
	return 0;
}
