#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<linux/in.h>

#define MAXLINE 4096
#define SERV_PORT 3000
#define LISTENQ 8

void fileTransfer(int newfd)
{
	char buf_recv[100], buf_send[100];
	char fbuffer[1000], filebuffer[1000];
	char nbytes, len;

	printf("Requesting client to send a filename\n");
	sprintf(buf_send,"Please enter the filename:");
	send(newfd,buf_send,strlen(buf_send),0);

	printf("filename received from client: \n");
	len = strlen(buf_recv);
	nbytes = recv(newfd,buf_recv,len,0);
	buf_recv[nbytes] = '\0';
	printf("%s\n",buf_recv);
	
	printf("\nNow checking file: %s exists or not..\n",buf_recv);
	if ((fp = fopen(buf_recv,'r')) == NULL) {
		sprintf(buf_send,"File could not be found!");
		exit(0);
	} else {
		printf("File found.\n");
		sprintf(buf_send,"File found\n");
		send(newfd,buf_send,strlen(buf_send),0);
	}
	
	printf("Sending the file content to client.. \n");
	while(!feof(fp)) {
		fgets(fbuffer,1000,fp);
		if (feof(fp))
			break;
		strcat(filebuffer,fbuffer);
	}
	fclose(fp);
	send(newfd,filebuffer,strlen(filebuffer),0);
	close(newfd);
	printf("[Server] connection closed. Waiting for new connection..\n");
}

void *tFunc(int *arg)
{
	struct sockaddr_in cAddr;
	int sockfd = (int)arg;
	int nSockfd;
	socklen_t addrlen;
	addrlen = sizeof(cAddr);

	while(1){
		nSockfd = accept(sockfd,&cAddr,&addrlen);
		printf("\nserver now connected with nSockfd = %d\n",nSockfd);
		fileTransfer(nSockfd);
		close(nSockfd);
	}

}

int main(int argc, char **argv)
{
	int sockfd, i = 0, thRet = 0;
	struct sockaddr_in servaddr;
	pthread_t t[10];
	pthread_attr_t attr;
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd == -1) {
		perror("error in creating socket");
		goto out;
	}
	
	//prepration of socket address
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if (bind(sockfd,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("assigning a name to a socket failed");
		goto out;
	}

	if (listen(sockfd,LISTENQ) < 0) {
		perror("listen socket failed");
		goto out;
	}
	
	thRet = pthread_attr_init(&attr) {
	if (thRet == -1) {
		perror("pthread_attr_init failed");
		goto out;
	}
	
	thRet = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED) {
	if (thRet == -1) {
		perror("pthread_attr_setdetachstate failed");
		goto out;
	}

	printf("Server running for connections.....\n");
	
	for(i = 0; i < 10; i++) {
		printf("\n threadID = t[%d]\n",i);
		pthread_create(&t[i],&attr,tFun,(void *)sockfd);
		pause();
	}

	return 0;
 out: 
	close(sockfd);
	return -1;
}


