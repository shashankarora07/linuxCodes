/* client thread will receving the file  */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<linux/in.h>

#define MAXLINE 4096
#define SERV_PORT 3000

int main(int argc, char **argv)
{
	struct sockaddr_in servaddr;
	int sockfd, nbytes;
	char buf_send[100], buf_recv[100];
	char filebuffer[1000], frev_buf[1000];
	FILE *fp;
	
	
	if (argc != 2) {
		perror("Usage TCPClient <IP address of the server>");
		exit(1);
	}
	
	//Create a socket for the client
	sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd == -1) {
        perror("error in creating client socket");
        goto out;
    }

	//preparation of socket address
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = inet_addr(argv[1]); //convers IPV4 notation into binary data in network byte order
	
	//Connection of the client to the socket
	if (connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0) {
		perror("Problem in connecting to the server");
		exit(1);
	} else {
		printf("\nSuccessfully connected to the server\n"); 
	}
	
	/* code to download file from server (if exist) */
	
	nbytes = recv(sockfd,buf_recv,sizeof(buf_recv),0);
	buf_recv[nbytes] = '\0';
	printf("%s\n",buf_recv);

	scanf("%s",buf_send);
	send(sockfd,buf_send,strlen(buf_send),0);
	
	printf("\nWaiting for server response.. \n");
	printf("Server checking File exits or not\n");
	nbytes = recv(sockfd,buf_recv,sizeof(buf_recv),0);
	buf_recv[nbytes] = '\0';
	printf("%s\n",buf_recv);

	printf("\nReceiving the file content\n");
	nbytes = recv(sockfd,frev_buf,sizeof(frev_buf),0);
	frev_buf[nbytes] = '\0';
	
	fp = fopen("received_file.txt",'w');
	fputs(filebuffer,fp);
	fclose(fp);
	close(sockfd);
		
	
	return 0;
 out:
	close(sockfd);
	return -1;
}
