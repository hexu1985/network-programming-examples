#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int acp_sock, recv_sock;
	struct sockaddr_in recv_adr;
	struct sockaddr_in send_adr;  
	int send_adr_size, str_len;
	char buf[BUF_SIZE];
	int result, fd_max;
	
	fd_set read, except, read_copy, except_copy;
	struct timeval timeout;
	
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);  
		exit(1);
	}
	
	acp_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&recv_adr, 0, sizeof(recv_adr));
	recv_adr.sin_family=AF_INET;
	recv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	recv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(acp_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr))==-1)
		error_handling("bind() error");
	if(listen(acp_sock, 5)==-1)
		error_handling("listen() error");
	
	send_adr_size=sizeof(send_adr);
	recv_sock=accept(acp_sock, (struct sockaddr*)&send_adr, &send_adr_size);
    fd_max = acp_sock > recv_sock ? acp_sock : recv_sock;
	FD_ZERO(&read);
	FD_ZERO(&except);
	FD_SET(recv_sock, &read);
	FD_SET(recv_sock, &except);

	while(1)
	{  
		read_copy=read;
		except_copy=except;
		timeout.tv_sec=5;
		timeout.tv_usec=0; 
		
		result=select(fd_max+1, &read_copy, 0, &except_copy, &timeout);

		if(result>0)
		{
			if(FD_ISSET(recv_sock, &except_copy))
			{
				str_len=recv(recv_sock, buf, BUF_SIZE-1, MSG_OOB);
				buf[str_len]=0;
				printf("Urgent message: %s \n", buf);
			}	

			if(FD_ISSET(recv_sock, &read_copy))
			{
				str_len=recv(recv_sock, buf, BUF_SIZE-1, 0);
				if(str_len==0)
				{
					break;
					close(recv_sock);
				}
				else 
				{	   
					buf[str_len]=0;
					puts(buf); 
				}	
			}				
		}
	}
	
	close(acp_sock);
	return 0; 
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
