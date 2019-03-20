#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>

int main()
{
		int x, y;

	{char buffer[256];
	int socket1, portNo;
	struct hostent * server;
	char ipv4[32];
	sprintf(ipv4,"127.0.0.1");
	server=gethostbyname(ipv4);
	struct sockaddr_in server_address;
	portNo=6000;
	socket1=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(portNo);
	bcopy( (char *)server->h_addr,
	(char *)&server_address.sin_addr.s_addr,
	server->h_length
	);
	connect(socket1,(struct sockaddr *)&server_address,sizeof(server_address));
	
	
	
		//scanf("%d %d", &x, &y);
		//sprintf(buffer, "%d %d\n", x, y);
		//write(socket1,buffer,strlen(buffer));
		//printf("Message Had Been Sent\n");
		bzero(buffer, 256);
		read(socket1, buffer, 256);
		sscanf(buffer, "%d\n%d", &x,&y);
		printf("%d %d is received from server\n", x,y);
		close(socket1);
	
	}
	
	char buffer[256];
	int socket1, portNo;
	struct hostent * server;
	char ipv4[32];
	sprintf(ipv4,"127.0.0.1");
	server=gethostbyname(ipv4);
	struct sockaddr_in server_address;
	portNo=x;
	socket1=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(portNo);
	bcopy( (char *)server->h_addr,
	(char *)&server_address.sin_addr.s_addr,
	server->h_length
	);
	connect(socket1,(struct sockaddr *)&server_address,sizeof(server_address));
		
	bzero(buffer, 256);
	sprintf(buffer,"%d\n",x-5000);
	write(socket1,buffer,strlen(buffer));
	
	char file;
	while(1)
	{
		char a;//-1 bye 1 read 2 write
		scanf("%c",&a);
		bzero(buffer, 256);
		sprintf(buffer,"%c\n",a);
		write(socket1,buffer,strlen(buffer));
		if(a == '3')
		{
			printf("Socket Closed\n");
			close(socket1);
			break;
		}
		else if(a == '1')
		{
			printf("Reading from Server\n\n");
			while(1){
			bzero(buffer,256);
			read(socket1,buffer,256);
			if(!(strcmp(buffer,"EEENNNDDD\n"))){break;}
			for(int i = 0 ;i<strlen(buffer);i++)
			{
				printf("%c",buffer[i]);
			}	
			}
			printf("\nReading's Ended\n");
		}
		else if(a=='2')
		{
			printf("Writing to Server\n");
			bzero(buffer,256);
			FILE *re = fopen("/home/dayyan/RFC.txt","r");
			char ch;
			int i = 0 ;
			bzero(buffer,256);
			usleep(1);				
			while((fgets(buffer,256,re)))
			{
				//sprintf(buffer,"abas\n");
				//printf("%s",buffer);
				//printf("milad\n");
				write(socket1,buffer,strlen(buffer));
				bzero(buffer,256);				
			} 
			usleep(1);
			sprintf(buffer,"EEENNNDDD");
			write(socket1,buffer,strlen(buffer));
			printf("Writing's Ended\n");
		}
	}


	return 0;
}
