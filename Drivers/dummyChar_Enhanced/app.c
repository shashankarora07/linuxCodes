#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
//#include <sys/types.h>


int main(int argc, char *argv[])
{
	int fd, n = 0;
	int32_t num = 0;
	char *buff = NULL;
	int val = 0;

	if (argc != 3) {
		perror("Usage: give device file and num bytes read");
		exit(1);
	}

	if ((fd=open(argv[1],O_RDONLY,0)) == -1) {
		perror("open");
		exit(1);
	}
	printf("device file opened : fd=%d\n",fd);
	
	num = atoi(argv[2]);
	if ((num < 0) || (num > INT_MAX)) {
		fprintf(stderr,"%s: number of bytes '%d' invalid.\n", argv[0], num);
		close (fd);
		exit (1);
	}

	buff = malloc(num);
	if (!buff) {
		fprintf(stderr,"%s: out of memory!\n", argv[0]);
		close (fd);
		exit (1);
	}

	n = read(fd,buff,num);
	if (n < 0) {
		perror("read failed");
		free(buff); close(fd);
		exit(1);
	}

	printf("read bytes = %d\n",n);
	//printf(" read daata : %s \n",buff);


	free(buff);
	close(fd);
	exit(0);
}