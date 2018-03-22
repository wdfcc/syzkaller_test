#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
static int test_dev_major=-1;
static long test_dev_ioctl (struct file* filep,unsigned int cmd ,unsigned long arg)
{

	return 0;
}

static file_operations test_ops
{
	.owner=THIS_MODULE;
	.unlocked_ioctl=test_dev_ioctl;
	.compat_ioctl=test_dev_ioctl;
};
static int __init test_init(void)
{
	test_dev_major=register_chardev(0,"testdev",&test_ops);
	if(test_dev_major<0)
	{
		printk("[*]register error\n");
	}
	return 0;
}

static void __exit test_exit(void)
{

	unregister_chardev(test_dev_major,"testdev");
	printk("[*]unregister_chardev\n");
}
module_init(test_init);
module_exit(test_exit);
MODULE_AUTHOR("TOPSEC");
MODULE_DESCRIPTION("SYZKALLER TEST");
MODULE_LICENSE("GPL V2");
