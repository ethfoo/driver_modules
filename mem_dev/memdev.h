#ifndef _MEMDEV_H_
#define _MEMDEV_H_

#include <linux/cdev.h>
#include <linux/ioctl.h>

#ifndef MEMDEV_MAJOR
#define MEMDEV_MAJOR 249	/*预设的mem主设备号*/	
#endif

#ifndef MEMDEV_NR_DEVS
#define MEMDEV_NR_DEVS 2	/*设备数*/
#endif

#ifndef MEMDEV_SIZE
#define MEMDEV_SIZE 4096
#endif

struct mem_dev
{
	char *data;
	unsigned long size;
	struct cdev cdev;
};

/*定义幻数*/
#define MEMDEV_IOC_MAGIC 'k'

/*定义命令*/
#define MEMDEV_IOCPRINT    _IO(MEMDEV_IOC_MAGIC, 1)
#define MEMDEV_IOCGETDATA  _IOR(MEMDEV_IOC_MAGIC, 2, int)
#define MEMDEV_IOCSETDATA  _IOW(MEMDEV_IOC_MAGIC, 3, int)

#define MEMDEV_IOC_MAXNR 3

#endif
