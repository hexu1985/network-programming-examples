#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

int main() {
   printf("sizeof struct sockaddr:%u\n", sizeof(struct sockaddr)); 
   printf("sizeof struct sockaddr_storage:%u\n", sizeof(struct sockaddr_storage)); 
   printf("sizeof struct sockaddr_in:%u\n", sizeof(struct sockaddr_in)); 

   int sockfd =  socket(AF_INET, SOCK_STREAM, 0);

   struct sockaddr_storage addr = {0};
   struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
   saddr->sin_family = AF_INET;
   saddr->sin_addr.s_addr = inet_addr("192.168.1.100");
   saddr->sin_port = htons(1234);
   bind(sockfd, (struct sockaddr *)saddr, sizeof(struct sockaddr_in));

   return 0;
}
