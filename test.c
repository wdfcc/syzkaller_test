#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/ioctl.h>
#include<linux/device.h>
#include<linux/slab.h>
#define DEV_NAME "topsec"

#define TOPSECIO 0XEC

#define TOPSEC_GET_VERSION _IO(TOPSECIO,0X1)
#define TOPSEC_UAF _IO(TOPSECIO,0X2)
#define TOPSEC_OUT_OF_BOUND _IO(TOPSECIO,0X3)

#define CURRENT_VERSION 1

static int major=-1;
static struct class * topsec_class;
static int test_dev_open(struct inode* inodep,struct file * filep)
{

	printk("[*]topsec_dev open\n");
	return 0;
}
static long test_dev_ioctl (struct file* filep,unsigned int cmd ,unsigned long arg)
{
	char *p=NULL;
	int  *buffer=NULL;
	int *a;
	printk("[*]ioctl\n");
	if(_IOC_TYPE(cmd)!=TOPSECIO)
	{
		return -EINVAL;
	}
	switch(cmd){
		case TOPSEC_GET_VERSION:
			printk("[*]get current version\n");
			return CURRENT_VERSION;
			break;
		case TOPSEC_UAF:
			p=kzalloc(8,GFP_KERNEL);
			if(p==NULL)
			{
				printk("[*]alloc error\n");
			}
			kfree(p);
			*p=1;
			break;
		case TOPSEC_OUT_OF_BOUND:
			buffer=kzalloc(256,GFP_KERNEL);
			a=buffer[arg];
			kfree(buffer);

	}

	return 0;
}

static struct file_operations test_ops=
{
	.owner=THIS_MODULE,
	.open = test_dev_open,
	.unlocked_ioctl=test_dev_ioctl,
	.compat_ioctl=test_dev_ioctl
};
static int __init test_init(void)
{

	major=register_chrdev(0,DEV_NAME,&test_ops);
	if(major<0)
	{
		printk("[*]register error\n");
		return -EINVAL;
	}
	topsec_class=class_create(THIS_MODULE,"topsec_test");
	if(IS_ERR(topsec_class))
	{
		unregister_chrdev(major,DEV_NAME);
		printk("[*]err class_create\n");
		return -EINVAL;
	}

	device_create(topsec_class,NULL,MKDEV(major,0),NULL,"topsec_test%d",0);

	printk("[*]register success!\n");

return 0;	
}

static void __exit test_exit(void)
{


	device_destroy(topsec_class,MKDEV(major,0));
	class_destroy(topsec_class);
	unregister_chrdev(major,DEV_NAME);
	printk("[*]unregister_chardev\n");
}
module_init(test_init);
module_exit(test_exit);
MODULE_AUTHOR("TOPSEC");
MODULE_DESCRIPTION("SYZKALLER TEST");
MODULE_LICENSE("GPL");
