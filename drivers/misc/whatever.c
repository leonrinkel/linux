#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init whatever_init(void)
{
	printk(KERN_INFO "hello from whatever\n");
	return 0;
}

static void __exit whatever_exit(void)
{
	printk(KERN_INFO "bye from whatever\n");
}

MODULE_VERSION("0.0.1");
MODULE_DESCRIPTION("idk just playin");
MODULE_AUTHOR("Leon Rinkel <leon@rinkel.me>");
MODULE_LICENSE("GPL v2");

module_init(whatever_init);
module_exit(whatever_exit);
