#ifndef SERVER_H
#define SERVER_H

#define MSG_SIZE 100
#define PORT 8080
#define NICK_SIZE 20
#define MAX_CLIENTS 10

struct Client{
  char nickname[NICK_SIZE];
  struct sockaddr_in cli;
  struct Client *next;
};
typedef struct Client Client;








#endif
