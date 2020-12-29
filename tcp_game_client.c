#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

char num[50];
char message[] = "Hello";

char point [9]={'1','2','3','4','5','6','7','8','9'};
int check (); 
void show(); 


int check () 
{
	if (((*point=='X')&&(*(point+1)=='X')&&(*(point+2)=='X'))||((*(point+3)=='X')&&(*(point+4)=='X')&&(*(point+5)=='X'))||((*(point+6)=='X')&&(*(point+7)=='X')&&(*(point+8)=='X'))||((*point=='X')&&(*(point+3)=='X')&&(*(point+6)=='X'))||((*(point+1)=='X')&&(*(point+4)=='X')&&(*(point+7)=='X'))||((*(point+2)=='X')&&(*(point+5)=='X')&&(*(point+8)=='X'))||((*point=='X')&&(*(point+4)=='X')&&(*(point+8)=='X'))||((*(point+2)=='X')&&(*(point+4)=='X')&&(*(point+6)=='X')))
	{printf(" \nX wins!");  return 1;}
	else if (((*point=='O')&&(*(point+1)=='O')&&(*(point+2)=='O'))||((*(point+3)=='O')&&(*(point+4)=='O')&&(*(point+5)=='O'))||((*(point+6)=='O')&&(*(point+7)=='O')&&(*(point+8)=='O'))||((*point=='O')&&(*(point+3)=='O')&&(*(point+6)=='O'))||((*(point+1)=='O')&&(*(point+4)=='O')&&(*(point+7)=='O'))||((*(point+2)=='O')&&(*(point+5)=='O')&&(*(point+8)=='O'))||((*point=='O')&&(*(point+4)=='O')&&(*(point+8)=='O'))||((*(point+2)=='O')&&(*(point+4)=='O')&&(*(point+6)=='O')))
	{printf("\nO wins!");  return 1;}
	if (((*point=='X')||(*point=='O'))&&((*(point+1)=='X')||(*(point+1)=='O'))&&((*(point+2)=='X')||(*(point+2)=='O'))&&((*(point+3)=='X')||(*(point+3)=='O'))&&((*(point+4)=='X')||(*(point+4)=='O'))&&((*(point+5)=='X')||(*(point+5)=='O'))&&((*(point+6)=='X')||(*(point+6)=='O'))&&((*(point+7)=='X')||(*(point+7)=='O'))&&((*(point+8)=='X')||(*(point+8)=='O')))
	{printf("\ndraw!"); return 1;}
	return 0;
}

void show() 
{
	int j=0;
	for (int i=0;i<9;i++)
	{
		printf("%c ",*(point+i));
		j++;
		if (j==3) {j=0; printf("\n");}
	}
}

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in addr;
    void sigalarm(int);
	if(argc!=3) {
		printf("Вы забыли указать адрес и порт.\n");
		exit(1);
	}
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
    show();
		while(1)
	{
		scanf ("%s", num);
		
		point[atoi(num)-1] = 'X';
		//show();
		check();
		
		send(sock, num, sizeof(num), 0);
		recv(sock, num, sizeof(num), 0);
		if(num[0] == 'e')
		{
			check();
			break;
		}
		else{point[atoi(num)-1] = 'O';
		 show();
		 check();
		}
	  
	  }
	    
	    

    close(sock);
    return 0;
}


