添加字符设备驱动方法：
make
sudo insmod memdev.ko

手动添加设备文件：
 (1)首先查看未使用的设备号:	cat /proc/devices
 (2)然后创建字符设备文件：	mknod /dev/memdev c 149 0


