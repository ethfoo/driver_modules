#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <uaccess.h>

#include "mem_dev.h"

static mem_major = MEMDEV_MAJOR;
static mem_minor = 0;

module_param(mem_major, int, S_IRUGO);

struct mem_dev *mem_devp;	/*设备结构体指针*/

int mem_open(struct inode *inode, struct file *filp)
{
	struct mem_dev *dev;

	/*获取次设备号*/
	int num = MINOR(inode->i_rdev);

	if(num>=MEMDEV_NR_DEVS)
		return -ENODEV;
	dev = &mem_devp[num];

	filp->private_data = dev;

	return 0;
}

int mem_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t mem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos )
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct mem_dev *dev= filp->private_data;/*获得设备结构体指针*/

	/*判断读位置是否有效*/
	if(p >= MEMDEV_SIZE)
		return 0;
	if(count > MEMDEV_SIZE-p)
		count = MEMDEV_SIZE-p;

	/*读数据到用户空间*/
	if(copy_to_user(buf, (void*)(dev->data + p), count))
	{
		ret = -EFAULT;
	}
	else
	{
		*ppos += count;
		ret = count;

		printk(KERN_INFO "read %d bytes from %d\n", count, p);
	}

	return ret;
}

static ssize_t mem_write(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct mem_dev *dev=filp->private_data;

	if(p >= MEMDEV_SIZE)
		return 0;
	if(count > MEMDEV_SIZE-p)
		count = MEMDEV_SIZE-p;

	if(copy_from_user(dev->data + p, buf, count))
	{
		ret = -EFAULT;
	}
	else
	{
		*ppos += count;
		ret = count;

		printk(KERN_INFO "write %d bytes from %d\n", count, p);
	}

	return ret;

}

static loff_t mem_llseek(strcut file *filp, loff_t offset, int whence)
{
	loff_t newpos;

	switch(whence)
	{
		case 0: /*SEEK_SET*/
			newpos = offset;
			break;
		case 1: /*SEEK_CUR*/
			newpos = filp->f_pos + offset;
			break;
		case 2: /*SEEK_END*/
			newpos = MEMDEV_SIZE -1 + offset;
			break;
		default:
			return -EINVAL;
	}

	if((newpos<0) || (newpos>MEMDEV_SIZE))
		return -EINVAL;

	filp->f_pos = newpos;
	return newpos;
}

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
	int i;
	
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
	cdev_init(&mem_dev->cdev, &mem_fops);

	/*注册字符设备*/
	cdev_add(&mem_dev->cdev, devno, 1);

	/*为设备描述结构分配内存*/
	mem_devp = kmalloc(MEMDEV_NR_DEVS * sizeof(struct mem_dev), GFP_KERNEL);
	if(!mem_devp)
	{
		result = -ENOMEM;
		goto fail_malloc;
	}
	memset(mem_devp, 0, sizeof(struct mem_dev));

	/*为设备分配内存*/
	for(i=0; i<MEMDEV_NR_DEVS; i++)
	{
		mem_devp[i].size = MEMDEV_SIZE;
		mem_devp[i].data = kmalloc(MEMDEV_SIZE, GFP_KERNEL);
		memset(mem_devp[i].data, 0, MEMDEV_SIZE);
	}

	return 0;

fail_malloc:
	unregister_chrdev_region(devno, 1);
	return result;

}

static void __exit memdev_exit(void)
{
	/*移除字符设备*/
	cdev_del(&mem_devp->cdev);
	/*释放设备结构体内存*/
	kfree(mem_devp);
	/*释放设备编号*/
	unregister_chrdev_region(MKDEV(mem_major, mem_minor), MEMDEV_NR_DEVS);

}

MODULE_LICENSE("GPL");
module_init(memdev_init);
module_init(memdev_exit);
