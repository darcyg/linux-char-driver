#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int i;
	int led_no;
	int fd;
	if (argc != 2 || sscanf(argv[1], "%d", &led_no) != 1 || 
	     led_no < 0 || led_no > 1) {
		fprintf(stderr, "Usage: test_led 0|1\n");
		exit(1);
	}
	fd = open("/dev/led_for_loop", 0);
	if (fd < 0) {
		perror("open device led");
		exit(1);
	}
	
	for (i=0; i<4; i++)
	{
		ioctl(fd, 1, i);
	}
		
	printf("now let's go with the led trip, come on\n");
	sleep(3);
	if (led_no)
	{
		for (i=0; i<4; i++)
		{
			ioctl(fd, 0, i);
			sleep(3);
			ioctl(fd, 1, i);
		}		
	}
	else
	{
		for (i=0; i<4; i++)
		{
			ioctl(fd, 0, 3-i);
			sleep(3);
			ioctl(fd, 1, 3-i);
		}		
	}

	printf("performan is over, thank you\n");
	close(fd);
	return 0;
}
