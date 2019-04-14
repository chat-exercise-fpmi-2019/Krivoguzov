// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define PORT     8080 
#define MAXLINE 1024 
#define MSG_SIZE 100
void printMenu(){
  printf("1 - send private message\n");
  printf("2 - send broadcasted message\n");
  printf("0 - exit\n");
}
void privateMesg(){};
void broadcastedMesg(){};

void recieveInOtherThread(int sockfd, struct sockaddr * servaddr){
  
    char buffer[MAXLINE]; 
  if(fork()==0){
    int len,n;
    for(;;){
      n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
		   MSG_WAITALL, servaddr, 
		   &len); 
      buffer[n] = '\0'; 
      printf("%s\n", buffer);
    }
  }

}
void menu(){
  char c;
  while(c=getchar()!='0'){
    switch (c){
	case '1':
	  privateMesg();
	  break;
	case 2:
	  broadcastedMesg();
	  break;	    
      }
  }  
}
int main() { 
    int sockfd; 
    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY;  

    char buff[MSG_SIZE];     
    int  len;
    recieveInOtherThread(sockfd,(struct sockaddr *)&servaddr);
    for(;;){
      
      scanf("%s",buff);
      sendto(sockfd, (const char *)buff, strlen(buff), 
	     MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
	     sizeof(servaddr));

    
      printf("message sent \"%s\".\n",buff); 
          
     
    }
  
    close(sockfd); 
    return 0; 
}

