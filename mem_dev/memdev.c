#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>

#include "mem_dev.h"

static mem_major = MEMDEV_MAJOR;
static mem_minor = 0;

module_param(mem_major, int, S_IRUGO);

struct mem_dev mem_dev;	/*设备结构体*/

static const struct file_operations mem_fops =
{
	.owner = THIS_MODULE,
	.llseek = mem_llseek,
	.read = mem_read,
	.write = mem_write,
	.open = mem_open,
	.release = mem_release,
};

static int __init memdev_init(void)
{
	int result;
	
	/*************分配设备号****************/
	dev_t devno = MKDEV(mem_major, mem_minor);

	/*静态申请设备号,如果没有指定MEMDEV_MAJOR，
	 *会使用在.h文件中define的MEMDEV_MAJOR
	 */
	if(mem_major)
	{
		result = register_chrdev_region(devno, MEMDEV_NR_DEVS, "memdev");
	}
	else	/*动态分配设备号，需要传入的mem_major为0，
		  分配好的设备号会存储到devno中*/
	{
		result = alloc_chrdev_region(&devno, 0, MEMDEV_NR_DEVS, "memdev");
		mem_major = MAJOR(devno);
	}

	if(result<0)
	{
		return result;	
	}

	/**************注册字符设备*************/

	/*初始化cdev结构*/	
	cdev_init(&mem_dev.cdev, &mem_fops);

	/*注册字符设备*/
	cdev_add(&mem_dev.cdev, devno, 1);

	return 0;
}

static void __exit memdev_exit(void)
{

}
/*this is a test for git*/

MODULE_LICENSE("GPL");
module_init(memdev_init);
module_init(memdev_exit);
