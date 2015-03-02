#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd;
	unsigned long count_w;
	unsigned long count_r;
	char *buf = "hello there!";
	char readbuf[100];

	fd = open("/dev/memdev", O_RDWR);
	if(fd<0)
	{
		printf("cannot open the device: memdev\n");
		return -1;
	}
	

	count_w = write(fd, buf, 13);
	if(count_w<0)
		printf("cannot write device\n");
	else
		printf("have write %ld\n", count_w);

	memset(readbuf, 0, sizeof(readbuf));	
	if( (count_r = read(fd, readbuf, sizeof(readbuf))) <0)
		printf("cannot read\n");
	else
		printf("have read %ld, the readbuf is %s\n", count_r, readbuf);


	close(fd);
	return 0;
}
