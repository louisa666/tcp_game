#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#define BUFLEN 512
#define MAXLINE 255
char point [9]={'1','2','3','4','5','6','7','8','9'};

char *my_itoa(int num, char *str)
{
        if(str == NULL)
        {
                return NULL;
        }
        sprintf(str, "%d", num);
        return str;
}


//#ifdef MUTEX
pthread_mutex_t lock;
//#endif

void *tcp_work(void *);



int check () 
{
	if (((*point=='X')&&(*(point+1)=='X')&&(*(point+2)=='X'))||((*(point+3)=='X')&&(*(point+4)=='X')&&(*(point+5)=='X'))||((*(point+6)=='X')&&(*(point+7)=='X')&&(*(point+8)=='X'))||((*point=='X')&&(*(point+3)=='X')&&(*(point+6)=='X'))||((*(point+1)=='X')&&(*(point+4)=='X')&&(*(point+7)=='X'))||((*(point+2)=='X')&&(*(point+5)=='X')&&(*(point+8)=='X'))||((*point=='X')&&(*(point+4)=='X')&&(*(point+8)=='X'))||((*(point+2)=='X')&&(*(point+4)=='X')&&(*(point+6)=='X')))
	{ return 1;}
	else if (((*point=='O')&&(*(point+1)=='O')&&(*(point+2)=='O'))||((*(point+3)=='O')&&(*(point+4)=='O')&&(*(point+5)=='O'))||((*(point+6)=='O')&&(*(point+7)=='O')&&(*(point+8)=='O'))||((*point=='O')&&(*(point+3)=='O')&&(*(point+6)=='O'))||((*(point+1)=='O')&&(*(point+4)=='O')&&(*(point+7)=='O'))||((*(point+2)=='O')&&(*(point+5)=='O')&&(*(point+8)=='O'))||((*point=='O')&&(*(point+4)=='O')&&(*(point+8)=='O'))||((*(point+2)=='O')&&(*(point+4)=='O')&&(*(point+6)=='O')))
	{ return 2;}
	if (((*point=='X')||(*point=='O'))&&((*(point+1)=='X')||(*(point+1)=='O'))&&((*(point+2)=='X')||(*(point+2)=='O'))&&((*(point+3)=='X')||(*(point+3)=='O'))&&((*(point+4)=='X')||(*(point+4)=='O'))&&((*(point+5)=='X')||(*(point+5)=='O'))&&((*(point+6)=='X')||(*(point+6)=='O'))&&((*(point+7)=='X')||(*(point+7)=='O'))&&((*(point+8)=='X')||(*(point+8)=='O')))
	{ return 3;}
	return 0;
}
int g_port;

struct s_Data{
	char filename[50];
	char data[MAXLINE];
	int number;
	int flag;
}sendData, recvData;

float rand_us(){
	int a;
	float rez;
	a = rand()%10+rand();
	rez = a*1.0/RAND_MAX;
	return rez;

}

void reaper(int sig)
{
  int status;
  while(wait3(&status, WNOHANG, (struct rusage *)0) >= 0);
}

int MAX (int x, int y)
{
 if (x>y)
 return x;
 else
 return y;
}


int create_udp()
{
  int sockfd;
  struct sockaddr_in servaddr;
  if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
   {
     perror("Error open Socket.");
     exit(1);
   }
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = 0;
  if(bind(sockfd, (struct sockaddr *)& servaddr, sizeof(servaddr)) < 0)
   {
     perror("bind server error");
     exit(2);
   }

	else{
      printf("Server UDP : bind() successful\n");
	socklen_t len = sizeof(servaddr);	
	if (getsockname(sockfd,(struct sockaddr*) &servaddr, &len)==-1)
		perror("getsockname");
	else{
		g_port = ntohs(servaddr.sin_port);
	}
	
	 }
	
  return sockfd;
}

int create_tcp()
{
  int sockfd;
  struct sockaddr_in servaddr,clientaddr;	
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror("Error open Socket.");
      exit(1);
    }
  bzero(&servaddr, sizeof(servaddr));
  bzero(&clientaddr, sizeof(clientaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr =  INADDR_ANY;
  servaddr.sin_port = htons(g_port);
socklen_t len;
  if(bind(sockfd, (struct sockaddr *)& servaddr, sizeof(servaddr)) < 0)
     {
      perror("bind server error");
      exit(2);
     }	
   else{
      printf("Server TCP : bind() successful\n");

	len = sizeof(servaddr);	
	if (getsockname(sockfd,(struct sockaddr*) &servaddr, &len)==-1)
		perror("getsockname");
	else
		printf("Port number %d\n", ntohs(servaddr.sin_port));

	 }
len = sizeof(servaddr);
  if(getsockname(sockfd, (struct sockaddr *)&servaddr, &len) < -1)
   {
     perror("getsockname error.\n");
     exit(1);
   }
  if(listen(sockfd, 5) < 0)
   {
     perror("Error on listen.\n");
     exit(1);
   }

  return sockfd;
}

void udp_work(int *sock)
{
char buf1[] = "j";
char buf[BUFLEN];
srand(time(NULL));
  struct sockaddr_in client_udp;	
  bzero(&client_udp, sizeof(client_udp));
  int n = -1, count = 1;

  socklen_t clientlen_udp = sizeof(client_udp);
  bzero(&sendData, sizeof(sendData));
  bzero(&recvData, sizeof(recvData));	

   n = recvfrom(*sock, buf, BUFLEN, 0, (struct sockaddr *)&client_udp, &clientlen_udp);
   if(n > 0)
    { 
     if (rand_us()>0.3){
        printf("Received UDP packet from %s\nData: %s\n\n",inet_ntoa(client_udp.sin_addr), buf);
	
	sendto(*sock, buf1, 2, 0, (struct sockaddr*)&client_udp, sizeof(client_udp));
	}	
;
  bzero(&sendData, sizeof(sendData));
  bzero(&recvData, sizeof(recvData));
  }
}


int main(int argc, char **argv)
{
  int newsockfd;
  fd_set rdfs;
  int tsock, usock;
  int fd, ndfs;
  struct sockaddr_in client_tcp;
  int childpid;
  usock = create_udp();
  tsock = create_tcp();
  bzero(&client_tcp, sizeof(client_tcp));
  socklen_t clientlen_tcp;
  bzero(&sendData, sizeof(sendData));
  bzero(&recvData, sizeof(recvData));
  clientlen_tcp = sizeof(client_tcp);
  signal(SIGCHLD, reaper);
  ndfs = MAX(tsock, usock) + 1;
  FD_ZERO(&rdfs);
	pthread_t thread_id;
signal(SIGCHLD, reaper);
  while(1)
      {
      FD_SET(tsock, &rdfs);
      FD_SET(usock, &rdfs);
	
      if(select(ndfs, &rdfs, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 0)
       {
	 perror("Select on error.\n");
	 exit(1);
       }
      if(FD_ISSET(tsock, &rdfs))
       {
	 newsockfd = accept(tsock, (struct sockaddr *) &client_tcp, &clientlen_tcp);
	 if(newsockfd < 0)
         {
	  perror("Error on accept.\n");
	  exit(1);
	 }


pthread_mutex_init (&lock, NULL);

		if( pthread_create( &thread_id , NULL , tcp_work , (void *) &newsockfd) < 0)
		{
		perror("could not create thread");
		return 1;
		}
	puts("TCP client connected");

	if (newsockfd < 0)
		{
		perror("accept failed");
		return 1;
		}

pthread_mutex_destroy(&lock);

		
       
        }
       if(FD_ISSET(usock, &rdfs))
         {
	   udp_work(&usock);
	 }
        FD_ZERO(&rdfs);
  }
}




void *tcp_work(void *socket_desc)
{

	int sock = *(int*)socket_desc;

  int count = 1; int bytes_read = 0;
int Mx = 255;
char data[Mx];
char buf1[] = "1";


char num[50];
char exit[] = "exit";

  struct s_Data recvData;
  bzero(&recvData, sizeof(recvData));


	while(1)
	{ 

		bytes_read = recv(sock, &num, sizeof(num), 0);
		printf("%s", num);
		if(bytes_read > 0)
         { 
		
pthread_mutex_lock (&lock);
num[50] = '\0';
	point[atoi(num)-1] = 'X';
	if (check()>0)
	{
		
		send(sock,&exit,sizeof(exit),0);
	}
	else{
		
		srand(time(0));
		int i;
		i = 1+rand()%9;
		do {
			i = 1+rand()%9;
		}while(point[i-1]=='X'||point[i-1]=='O');
		//itoa(i,num,10);
	//	num = i+'\0';
		point[i-1] = 'O';
     send(sock,my_itoa(i,num),sizeof(num),0);

}


		
pthread_mutex_unlock (&lock);

	 }
		
		else break;
	}
	
return 0;
}

