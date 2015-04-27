
#include <linux/delay.h>
#include <asm/irq.h>
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


#define DEVICE_NAME "RT5350_DEV"
#define LED_MAJOR  231


#define ADC_CON  0x58000000
#define INT_MASK 0x4a000008

volatile unsigned long *vir_gio_dir;
volatile unsigned long *vir_gio_set;
volatile unsigned long *vir_gio_clear;

int gio_open(struct inode *inode, struct file *filp)
{
//	writel((1<<14)|(25<<6)|(2<<3), vir_adc);
	return 0;
}

int gio_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
/*
	unsigned long dat_tmp;
	*vir_intmask = (*vir_intmask)| 0x80000000;
	*vir_adc     = (*vir_adc) | 0x01;

	while (vir_adc[0] & 0x1);
	while (!(vir_adc[0] & 0x8000));
	
	dat_tmp = readl(vir_adc + 3)&0x3ff;

	copy_to_user(buffer, (unsigned char*)(&dat_tmp), 2);

	printk("<0>""data is %d\n",dat_tmp);
	*vir_intmask = (*vir_intmask)&0x7fffffff;
*/


	return 2;
}




static struct file_operations dev_fops = {
	.owner	=	THIS_MODULE,
	.open   =   gio_open,
	.read	=	gio_read,
};

static int __init dev_init(void)
{
	int ret;
	volatile unsigned long *add_tmp;
	unsigned long dat_tmp;

	ret = register_chrdev(LED_MAJOR, DEVICE_NAME, &dev_fops);
	if (ret < 0)
	{
		printk ("can't register\n");	
		return -1;
	}
	
	//ioremap
	vir_gio_dir = ioremap(0x10000624, 4);
	if (NULL == vir_gio_dir)
		printk ("<0>""vir_gio_dir ioremap err\n");

	vir_gio_set = ioremap(0x1000062c, 4);
	if (NULL == vir_gio_set)
		printk ("<0>""vir_gio_set ioremap err\n");

	vir_gio_clear = ioremap(0x10000630, 4);
	if (NULL == vir_gio_clear)
		printk ("<0>""vir_gio_clear ioremap err\n");	

	
//	writel(dat_tmp|(1<<0)|(1<<1), vir_gio_set);

	vir_gio_clear = ioremap(0x10000630, 4);
	if (NULL == vir_gio_clear)
		printk ("<0>""vir_gio_clear ioremap err\n");	


	dat_tmp = readl(vir_gio_dir);
//	writel(dat_tmp|(((1<<0)|(1<<1))), vir_gio_dir);
	writel(0xffffffff, vir_gio_dir);
	msleep(5);
	writel(0xffffffff, vir_gio_clear);

	
//	add_tmp = ioremap(0x10000620, 4);

//	dat_tmp = readl(add_tmp);
//	ret = writel(dat_tmp&(~((1<<0)|(1<<1))), add_tmp);
//	ret = writel((1<<0)|(1<<1), add_tmp);

	
	printk ("register OK\n");	

	return ret;
}

static void __exit dev_exit(void)
{	
	if (vir_gio_clear != NULL)
		iounmap((void*)vir_gio_clear);
	
	if (vir_gio_dir != NULL)
		iounmap((void*)vir_gio_dir);

	if (vir_gio_set != NULL)
		iounmap((void*)vir_gio_set);
	

	unregister_chrdev(LED_MAJOR, DEVICE_NAME);
	printk ("exit\n");
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("loop zhong");
MODULE_DESCRIPTION("from rt5350 Board");

