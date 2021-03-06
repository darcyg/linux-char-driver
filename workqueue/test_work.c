#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	int fd;
	int ret;
	int temp;
	char buf[2];
	char c;

	fd = open("/dev/int_work", O_RDWR);
	
	if (fd<0)
	{
		printf("open int_work err\n");
		return -1;
	}

	while(1)
	{
		scanf("%c", &c);
		if ('q' == c)
		{
			close(fd);
			return 0;
		}	
		printf("read ready\n");
		ret = read(fd, buf, 2);
		if (ret != 0)
		{
			temp = buf[0] | (buf[1]<<8);
			printf("count is %d\n", temp);
		}
	}

	return 0;
}
