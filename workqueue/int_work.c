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
#include <linux/workqueue.h>
#include <linux/miscdevice.h>

#define DEV_NAME "mydriver"
#define DEV_MAJOR 222
#define INT_NUM     IRQ_EINT0
 
void work_fun(void*data);
int count;
DECLARE_DELAYED_WORK(work, work_fun);
DECLARE_WAIT_QUEUE_HEAD(queue_read);
 
void work_fun(void* data)
{
	unsigned long temp = s3c2410_gpio_getpin(S3C2410_GPF0);
	
	if (0x1 & temp)
	{
		wake_up_interruptible(&queue_read);
		count++;	
	}
}

static int phoenix_interrupt(int irq,void *dev_id,struct pt_regs *regs)
{
	int temp;
	temp = schedule_delayed_work(&work, 200);
	
	if (0 == temp)
	{
		printk("<0>""work has been in game\n");
	}
	return 0;
}

 

int phoenix_open(struct inode *inode,struct file *filp)
{
        int temp;	

       //register interrupt 
       s3c2410_gpio_cfgpin(S3C2410_GPF0,0x10);
       temp=request_irq(INT_NUM,phoenix_interrupt,IRQ_TYPE_EDGE_RISING,DEV_NAME,NULL); 

       if(temp) 
       {
              printk("<0>""request_irq error1:<%d> !\n",temp);
       }

       
       count=0;
       return 0;
}

int phoenix_read(struct file *filp,char *buffer,size_t length,loff_t *offset)
{    
	 interruptible_sleep_on(&queue_read);
      	 copy_to_user(buffer,(char*)&count,2);//copy to user   

       	 return 4;
}

int phoenix_release(struct inode *inode,struct file *filp)

{

       free_irq(INT_NUM,NULL);

       return 0;

}

struct eint{
	struct cdev cdev;
}eint_dev;

struct file_operations eint_fops=

{

       .owner=THIS_MODULE,

       .open=phoenix_open,

       .read=phoenix_read,

       .release=phoenix_release,

};

void eint_setup_cdev(void)
{
	int err, devno=MKDEV(DEV_MAJOR, 0);
	cdev_init(&eint_dev.cdev, &eint_fops);
	eint_dev.cdev.owner = THIS_MODULE;
	eint_dev.cdev.ops   = &eint_fops;

	err = cdev_add(&eint_dev.cdev, devno, 1);
	if (err)
	{
		printk("add err");
	}
	else
	{
		printk("add ok");
	}
}

 

int phoenix_init(void)

{

 	int tmp;		
	dev_t devno=MKDEV(DEV_MAJOR, 0);	

        tmp=register_chrdev_region(devno, 1, DEV_NAME);
		
       if(tmp<0)
       {
              printk("<0>""module_init error !\n");
              return tmp;
       }

      eint_setup_cdev();
       return 0;

}

void phoenix_exit(void)
{
	cdev_del(&eint_dev.cdev);
	unregister_chrdev_region(MKDEV(DEV_MAJOR, 0), 1);
}


module_init(phoenix_init);
module_exit(phoenix_exit);

 

MODULE_LICENSE("Dual BSD/GPL");


 
