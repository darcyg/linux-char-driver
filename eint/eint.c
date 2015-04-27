#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>

#define DEV_NAME "mydriver"
#define DEV_MAJOR 222
#define INT_NUM     IRQ_EINT0

 

int count;

DECLARE_WAIT_QUEUE_HEAD(queue_read);

 

static int phoenix_interrupt(int irq,void *dev_id,struct pt_regs *regs)

{

      count++;      
       wake_up_interruptible(&queue_read);
      return 0;

}

 

int phoenix_open(struct inode *inode,struct file *filp)

{

       int temp;

       //register interrupt 

       s3c2410_gpio_cfgpin(S3C2410_GPF0,0x10);

       temp=request_irq(INT_NUM,phoenix_interrupt,IRQ_TYPE_EDGE_BOTH,DEV_NAME,NULL); 

       if(temp) 

       {

              printk("<0>""request_irq error1:<%d> !\n",temp);

       }

      // set_irq_type(INT_NUM,IRQ_FALLING);

       count=0;

       return 0;

}

int phoenix_read(struct file *filp,char *buffer,size_t length,loff_t *offset)
{    

       if(count==0)
       {

              if(!(filp->f_flags & O_NONBLOCK))

              {

                     interruptible_sleep_on(&queue_read);

              }

              else

                     return 0;

       }

       copy_to_user(buffer,(char*)&count,4);//copy to user   

       count=0;

       return 4;
}

int phoenix_release(struct inode *inode,struct file *filp)

{

       free_irq(INT_NUM,NULL);

       return 0;

}

 

struct file_operations oper_struct=

{

       .owner=THIS_MODULE,

       .open=phoenix_open,

       .read=phoenix_read,

       .release=phoenix_release,

};

 

int phoenix_init(void)

{

       int tmp;

       tmp=register_chrdev(DEV_MAJOR,DEV_NAME,&oper_struct);

       if(tmp<0)

       {

              printk("<0>""module_init error !\n");

              return tmp;

       }

      

       return 0;

}

void phoenix_exit(void)

{

       unregister_chrdev(DEV_MAJOR,DEV_NAME);

}

 

module_init(phoenix_init);

module_exit(phoenix_exit);

 

MODULE_LICENSE("Dual BSD/GPL");

 
