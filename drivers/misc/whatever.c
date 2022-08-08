#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "whatever"
#define DUMMY_MSG "lorem ipsum dolor sit amet"
#define BUFLEN 28

static int major_num;
static int open_count = 0;
static char buffer[BUFLEN];

static int device_open(struct inode*, struct file*)
{
	if (open_count)
	{
		return -EBUSY;
	}

	open_count++;
	try_module_get(THIS_MODULE);
	return 0;
}

static int device_release(struct inode* inode, struct file* file)
{
	open_count--;
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t device_read(struct file* file, char* dst, size_t len, loff_t* off)
{
	int bytes_read;
	for (
			bytes_read = 0;
			bytes_read < BUFLEN && bytes_read < len;
			bytes_read++
	    )
	{
		put_user(buffer[bytes_read], dst++);
	}
	return bytes_read;
}

static ssize_t device_write(struct file* file, const char* src, size_t len, loff_t* off)
{
	printk(KERN_ALERT "whatever device is read only\n");
	return -EINVAL;
}

static struct file_operations file_ops =
{
	.read    = device_read,
	.write   = device_write,
	.open    = device_open,
	.release = device_release,
};

static int __init whatever_init(void)
{
	printk(KERN_INFO "hello from whatever\n");

	strncpy(buffer, DUMMY_MSG, BUFLEN);

	major_num = register_chrdev(0, DEVICE_NAME, &file_ops);
	if (major_num < 0)
	{
		printk(KERN_ALERT "unable to register whatever device: %d\n", major_num);
		return major_num;
	}
	else
	{
		printk(KERN_INFO "whatever devices registered with major num: %d\n", major_num);
		return 0;
	}
}

static void __exit whatever_exit(void)
{
	unregister_chrdev(major_num, DEVICE_NAME);
	printk(KERN_INFO "bye from whatever\n");
}

MODULE_VERSION("0.0.1");
MODULE_DESCRIPTION("idk just playin");
MODULE_AUTHOR("Leon Rinkel <leon@rinkel.me>");
MODULE_LICENSE("GPL v2");

module_init(whatever_init);
module_exit(whatever_exit);
