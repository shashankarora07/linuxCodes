/* *File:user_test.c
   *Author:Vishal Chopra
   *Date:4 Dec 2016
   *Purpose: This file is to acess BeagleBoneBlack GPIO 

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "ioctl_driver.h"
#include <string.h>
int main(int argc, char *argv[])
{
	int fd;
	struct bbbgpio_ioctl_struct ioctl_struct;
	if( argc < 2){
		fprintf(stderr, "%d Less no of arguments\n", strerror(errno));
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if(fd == -1){
		fprintf(stderr, "%d:Open\n", strerror(errno));
		perror("open");
		return -1;
	}
	printf("Device is open with FD = %d\n", fd);

	memset(&ioctl_struct, 0, sizeof(struct bbbgpio_ioctl_struct));
	//---------------register the gpio pin for Push Button through ioctl number----------------
	ioctl_struct.gpio_number = 115;
	ioctl_struct.gpio_direction = 0;
	if( ioctl( fd, IOCBBBGPIORP, &ioctl_struct ) ){
		perror("ioctl");
	return -1;
	}
	
	//---------------register the gpio pin for LED through ioctl numbe----------------
	ioctl_struct.gpio_number = 49;
	ioctl_struct.gpio_direction = 1;
	if( ioctl( fd, IOCBBBGPIORP, &ioctl_struct ) ){
		perror("ioctl");
	return -1;
	}
	return 0;
}
