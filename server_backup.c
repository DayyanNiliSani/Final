/*example of TCP socket server
server listens to incoming clients and acceptes maximum 5 sockets per second
server checks accepted socket every 1 second and receives the message
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <pthread.h>
#include <netinet/tcp.h>
pthread_mutex_t lock;
#define maxchild 100
void *threadfunc(void *t)
{
	int tt = *((int*) t);

	
	//printf("in thread %d tt = %d\n" ,tt-5000, tt);
	struct hostent * server;
	int socket1, portNo, clientLength;
	int acceptedSocket;
	
	char ipv4[32];
	sprintf(ipv4,"127.0.0.1");

	server=gethostbyname(ipv4);


	struct sockaddr_in server_address, client_address,sin ;
	clientLength=sizeof(client_address);
	portNo=tt;

	socket1=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(portNo);			//=htons(0)
	server_address.sin_addr.s_addr=INADDR_ANY;
	
	bind ( socket1, (struct sockaddr*) &server_address,sizeof(server_address));

	listen(socket1,maxchild);
	
	
	acceptedSocket = accept( socket1,
	(struct sockaddr * )&client_address,&clientLength);
	printf("Thread %d connected\n",tt);
	char buffer[256];
	bzero(buffer,256);
	char income;
	int incomee;
	read(acceptedSocket,buffer,256);	
	sscanf(buffer,"%d\n",&incomee);
	printf("income = %d\n",incomee);
	if (incomee != tt-5000)
		close(socket1);
	while(1)
	{
		bzero(buffer,256);
		read(acceptedSocket,buffer,256);
		sscanf(buffer,"%c\n",&income);
		//printf("income = %c\n",income);	
		if(income == '3')//bye
		{
			close(socket1);
			printf("Thread %d is Ended\n" , tt);
			break;
		}
		else if(income=='1')//client read
		{
			pthread_mutex_lock(&lock);
			printf("Sending data to Client\n");
			bzero(buffer,256);
			FILE *re = fopen("/home/dayyan/SFS.txt","r");
			char ch;
			int i = 0 ;
			bzero(buffer,256);				
			while((fgets(buffer,256,(FILE *)re)))
			{
				send(acceptedSocket,buffer,strlen(buffer),MSG_DONTWAIT);
				bzero(buffer,256);
			}
			usleep(1);
			int flag = 1; 
			sprintf(buffer,"EEENNNDDD\n");
			send(acceptedSocket,buffer,strlen(buffer),MSG_DONTWAIT);
			
			printf("Sending's Ended\n");
			pthread_mutex_unlock(&lock);	
		
		}
		else if(income == '2')//write
		{
			pthread_mutex_lock(&lock);
			printf("Reading from Client %d\n\n",tt);
			while(1)
			{
				bzero(buffer,256);
				read(acceptedSocket,buffer,256);
		//  printf("%d\n",(strcmp(buffer,"EEENNNDDD")));				
				if(!(strcmp(buffer,"EEENNNDDD\n\n"))){break;}			
				for(int i = 0 ;i<strlen(buffer);i++)
				{
					printf("%c",buffer[i]);
				}
				
			}			
			printf("\nReading's Ended\n");
			
			pthread_mutex_unlock(&lock);
		}
		
	}
}
pthread_t tid[100];
int main(){

	char buffer[256];
	int a=0;
	int  b=0;
	int socket1, portNo, clientLength;
	int acceptedSocket;
	int dports[100];
	for(int i=0;i<100;i++)
	{
		dports[i] = 0;
	}
	struct hostent * server;
	char ipv4[32];
	sprintf(ipv4,"127.0.0.1");

	server=gethostbyname(ipv4);


	struct sockaddr_in server_address, client_address,sin ;
	clientLength=sizeof(client_address);
	portNo=6000;

	socket1=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(portNo);			//=htons(0)
	server_address.sin_addr.s_addr=INADDR_ANY;
	
	bind ( socket1, (struct sockaddr*) &server_address,sizeof(server_address));

	listen(socket1,maxchild);

	socklen_t len = sizeof(server_address);
	getsockname(socket1, (struct sockaddr *)&sin, &len);
	printf("the port is %d\n",ntohs(sin.sin_port));
	time_t t;	
	srand((unsigned) time(&t));
	int tt;
	while(1){
		acceptedSocket = accept( socket1,
		(struct sockaddr * )&client_address,&clientLength);
		printf("connected\n");
		pthread_mutex_lock(&lock);
		bzero(buffer,256);
		//read(acceptedSocket, buffer, 256);
		//sscanf(buffer,"%d %d",&a,&b);
		//printf("received\n");
		//a=a+b;
		int flag = 1;
		while(flag ==1){
		tt = rand()%100;
		if (dports[tt]==0)
			{
				dports[tt] = 1;
			int error = pthread_create(&(tid[tt]),NULL,&threadfunc,(void *)&tt);
				if(error!=0)
					printf("\nERROR\n");
				else	
					//printf("\nNo ERROR\n");				
				tt = 5000 + tt;
				flag = 0;
			}		
		}
		sprintf(buffer,"%d\n%d",tt,tt - 5000);///113 is the token
		write(acceptedSocket,buffer,strlen(buffer));
		printf("sent\n");
		bzero(buffer,256);
		int c= close(acceptedSocket);
		if(c==0){
			printf("seccessfully closed\n");
		}
		pthread_mutex_unlock(&lock);


	}
	close(socket1);
	return 0;
	}
