#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>
#include <asm/io.h>


#define DEV_NAME "mydriver"

#define ADC_CON  0x58000000
#define INT_MASK 0x4a000008

volatile unsigned long *vir_adc;
volatile unsigned long *vir_intmask;

int ADC_open(struct inode *inode, struct file *filp)
{
	writel((1<<14)|(25<<6)|(2<<3), vir_adc);
	return 0;
}

int ADC_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
	unsigned long dat_tmp;
	*vir_intmask = (*vir_intmask)| 0x80000000;
	*vir_adc     = (*vir_adc) | 0x01;

	while (vir_adc[0] & 0x1);
	while (!(vir_adc[0] & 0x8000));
	
	dat_tmp = readl(vir_adc + 3)&0x3ff;

	copy_to_user(buffer, (unsigned char*)(&dat_tmp), 2);

	printk("<0>""data is %d\n",dat_tmp);
	*vir_intmask = (*vir_intmask)&0x7fffffff;

	return 2;
}




static struct file_operations dev_fops = {
	.owner	=	THIS_MODULE,
	.open   =       ADC_open,
	.read	=	ADC_read,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEV_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret;

	ret = misc_register(&misc);
	printk (DEV_NAME" initialized\n");
	
	//ioremap
	vir_intmask = ioremap(INT_MASK, 4);
	if (NULL == vir_intmask)
		printk ("<0>""vir_intmask ioremap err\n");

	vir_adc = ioremap(ADC_CON, 16);
	if (NULL == vir_adc)
		printk ("<0>""vir_adc ioremap err\n");
		
	return ret;
}

static void __exit dev_exit(void)
{	
	if (vir_intmask != NULL)
		iounmap((void*)vir_intmask);
	
	if (vir_adc != NULL)
		iounmap((void*)vir_adc);

	misc_deregister(&misc);
	printk (DEV_NAME "exit\n");
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("loop zhong");
MODULE_DESCRIPTION("from EmbedSky SKY2440/TQ2440 Board");

