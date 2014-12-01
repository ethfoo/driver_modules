#ifndef _MEMDEV_H_
#define _MEMDEV_H_

#include <linux/cdev.h>

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

#endif
