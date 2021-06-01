//master program. Server IP address & port no needs to be passed as arguments

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "frame.h"

#define buffer_size 255

int main(int argc, char *argv[])
{


    int sockfd, portno, channel,count,pos;
    struct sockaddr_in serv_addr;
    struct hostent *server;   //gives info related to host-host name, ip address

    char buffer[buffer_size];   //buffer for storing data

    if (argc < 3)
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(1);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);//creating socket for communication
    if (sockfd < 0)
    {
        error("ERROR opening socket");
	exit(1);
    }

    server = gethostbyname(argv[1]); //storing server's IP address

    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(1); //terminates if server's IP address is not passed
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

	//server structure

    serv_addr.sin_family = AF_INET;  //IPv4 Domain address
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    serv_addr.sin_port = htons(portno);  //port no

    //connect function

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {    error("ERROR connecting");
	exit(1);
    }

//sending the frame

printf("master: ");

    while(1)
    {
	struct frame *ptr = arguments();   //calls arguments() from args.h

    bzero(buffer,buffer_size);//clears the buffer

	int f1= ptr->start_bit;      //passing start bit
    buffer[0]=f1;

	int f2= ptr->server_addr;      //passing address of server
    buffer[1]=f2;

	int f3 = ptr->function_code;      //passing function_code
	buffer[2]=f3;

	char *f4= ptr->data;      //passing data
	strcat(buffer,f4);

	char *f5= ptr->crc;      //passing crc
	strcat(buffer,f5);

    int f6= ptr->stop_bit;      //passing stop bit
    buffer[18]=f6;

	channel = write(sockfd,buffer,strlen(buffer));//client writes into the buffer

	bzero(buffer,buffer_size);//clears the buffer
    fgets(buffer,buffer_size,stdin);// reads the string to be sent
	channel = write(sockfd,buffer,strlen(buffer));//client writes into the buffer

        if (channel < 0)
        {
             error("ERROR writing to socket");
	     exit(1);
        }

        bzero(buffer,buffer_size); //clears buffer
        channel = read(sockfd,buffer,buffer_size);//reads from the buffer

        if (channel < 0)
        {     error("ERROR reading from socket");
	     exit(1);
        }

	printf("\n");
        printf("Server : %s\n",buffer);
        int cmp = strncmp("Bye" , buffer , 3); //compares the string
        if(cmp == 0)
               break;
    }
    close(sockfd);  //closes the socket
    return 0;
}

