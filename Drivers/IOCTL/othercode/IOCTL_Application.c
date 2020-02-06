#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>
#include<asm/ioctl.h>
#include"ioctl.h"

struct DefaultData {
	int Baudrate;
	int StopBits;
	int Parity;
	int ToggleFreq;
};
int main()
{
	unsigned int FileDesc, Result;//, BAUD_RATE = 9600, STP_BITS = 8;
	struct DefaultData ToSend={9800,1, 0, 1};
	//char Ubuff[]="THis is the User Buffer......Sending Data to the Kernel....";
	//char Kbuff[100];	
	FileDesc=open("/dev/IOCTL0",O_RDWR,0777);
	if(FileDesc <0)
	{
		printf("\nError Opening Device\n");	
		exit(1);
	}
	while (1) {
		printf("Enter the new toggle freq in seconds: ");
		scanf ("%d", &ToSend.ToggleFreq);
		//Result = ioctl (FileDesc, SET_BAUD_RATE, BAUD_RATE);
		/*if (Result < 0) {
			printf ("\n IOCTL Error");
			return (-1);
		}
		ioctl (FileDesc, SET_NO_STOP_BITS, STP_BITS);
		ioctl (FileDesc, SET_DIRECTION_WRITE, NULL);
		*/
		//printf ("DEFAULT_DATA: %d", DEFAULT_DATA);
		Result = DEFAULT_DATA;
		ioctl (FileDesc, Result, &ToSend);
	
	//write(FileDesc,Ubuff,sizeof(Ubuff));
	//read(FileDesc,Kbuff,sizeof(Ubuff));
	//printf("\n The Data read from the Kernel is\n>>>> %s <<<<\n",Kbuff);
	}
	close(FileDesc);
}
