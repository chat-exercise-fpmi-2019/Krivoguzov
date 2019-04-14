#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include "server.h"
#include "clientList.h"

#define SA struct sockaddr
Client list; 
int sockfd;
int currentClientNumber = 0;
int authorization(char nickname[NICK_SIZE],struct sockaddr_in cli){  
  push(nickname,cli);
  sendto(sockfd,"response",9,0,(SA *)&cli,sizeof(cli));
  return ++currentClientNumber;
}

ssize_t clientSend(const char* msg,Client* from , Client* to){
  int len = strlen(msg) + NICK_SIZE +5+ 1;
  char* slovo = malloc(len);
  strcat(slovo,"MESG|");
  strcat(slovo, from->nickname);
  strcat(slovo,"|");
  strcat(slovo, msg);
  
  printf("clientSend %s->%s: %s\n",from->nickname, to->nickname,msg);
  int n =  sendto(sockfd,slovo,len,0,(SA *)&to->cli,sizeof(to->cli));  
  sendto(sockfd,"response",9,0,(SA *)&from->cli,sizeof(from->cli));
  free(slovo);
  return 0;
}

void tempBroadcast(const char* msg, Client*from){
  Client* start = getHead()->next;
  for(start;start!=NULL;start = start->next){
    clientSend(msg,from,start);
  }
}

ssize_t parse(char* msg, struct sockaddr_in cli){  
  if(!strncmp(msg,"AUTH|",4)){
     char nickname[NICK_SIZE];
     strcpy(nickname,msg+5);
     printf("AUTHORIZATION %d %s \n", currentClientNumber, nickname);     
     authorization(nickname,cli);
  }
  if(!strncmp(msg,"SEND",4)){
    const char delim[] = "|";    
    strtok(msg, delim);
    //char* fromNick = strtok(NULL, delim);
    char* toNick = strtok(NULL, delim);
    char* msg = strtok(NULL, delim);
    clientSend(msg , findByCli(cli), find(toNick) );
  }
  if(!strncmp(msg,"DISC",4)){
    const char delim[] = "|";    
    strtok(msg, delim);
    //char* fromNick = strtok(NULL, delim);
    char* toNick = strtok(NULL, delim);
    char* msg = strtok(NULL, delim);
    
    tempBroadcast("Disconnected", findByCli(cli));
    delete(findByCli(cli)->nickname);

  }  
}

void work()
{
  char buff[MSG_SIZE];
  int n,clen;
  struct sockaddr_in cli;
  clen=sizeof(cli);
  for(;;)
    {
      bzero(buff,MSG_SIZE);
      recvfrom(sockfd,buff,sizeof(buff),0,(SA *)&cli,&clen);
      printf("From client recieve %s\n",buff);      
      parse(buff,cli);
    }
}

int main()
{

  struct sockaddr_in servaddr;
  sockfd=socket(AF_INET,SOCK_DGRAM,0);
  if(sockfd==-1)
    {
      printf("socket creation failed...\n");
      exit(0);
    }
  else
    printf("Socket successfully created..\n");
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servaddr.sin_port=htons(PORT);
  if((bind(sockfd,(SA *)&servaddr,sizeof(servaddr)))!=0)
    {
      printf("socket bind failed...\n");
      exit(0);
    }
  else
    printf("Socket successfully binded..\n");

  
  work();
  close(sockfd);
}
