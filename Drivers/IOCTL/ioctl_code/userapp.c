#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>

#define MAGIC_NO 'x'

#define WR_VALUE _IOW(MAGIC_NO,'a',int32_t*)
#define RD_VALUE _IOR(MAGIC_NO,'b',int32_t*)

int main()
{
	int fd;
	int32_t value = 0, no = 0;

	fd = open("/dev/ioctlDevice",O_RDWR);
	if (fd < 0) {
		perror("Unable to open device file");
		return -1;
	}
	
	printf("Enter value to send\n");
	scanf("%d",&value);
	ioctl(fd, WR_VALUE, (int32_t *)&value);
	printf("writing into driver done\n");
	
	printf("Reading value from driver\n");
	ioctl(fd, RD_VALUE, (int32_t *)&no);
	printf("value is : %d\n",no);
	
	printf("Closing application\n");

	close(fd);

	return 0;
}
