#include <stdio.h>
#include <string.h>


int main()
{

	FILE *fp0 = NULL;
	char buf[4096];

	/*初始化buf*/
	strcpy(buf, "mem is char dev");
	printf("buf: %s\n", buf);

	/*打开设备文件*/
	fp0 = fopen("/dev/memdev0", "r+");
	if(fp0 == NULL)
	{
		printf("Open memdev0 error\n");
		return -1;
	}

	/*写入设备*/
	fwrite(buf, sizeof(buf), 1, fp0);

	/*重新定位文件位置*/
	fseek(fp0, 0, SEEK_SET);

	/*清除buf*/
	strcpy(buf, "buff is NULL");
	printf("buf: %s\n", buf);

	/*读出设备*/
	fread(buf, sizeof(buf), 1, fp0);

	/*检测结果*/
	printf("buf: %s\n", buf);

	return 0;
}
