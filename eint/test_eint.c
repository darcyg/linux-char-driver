#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
int main()

{

       //open

       int fd;

       fd=open("/dev/eint", O_RDWR);

       if(fd<0)

       {

              printf("open dev error!\n");

              return -1;

       }

      

       char cmd;

       unsigned char buf[2];

       int temp;

       while(1)

       {

              printf("*****r-read,q-exit*****\ncmd:");

              scanf("%c",&cmd);

              switch(cmd)

              {

                     case 'r':

                            scanf("%c",&cmd);

                            break;

                     case 'q':

                            close(fd);

                            return 0;

                     default:

                            scanf("%c",&cmd);

                            continue;

              }

              temp=read(fd,buf,2);

              if(temp!=0)
	      {
		     temp = buf[0] | (buf[1]<<8);		   	
                     printf("app count=%d\n",  temp);
	      }	
       }

       //close dev

       close(fd);

       return 0;

}
