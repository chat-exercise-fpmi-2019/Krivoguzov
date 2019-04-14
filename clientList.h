#ifndef CLIENTLIST_H
#define CLIENTLIST_H
#include "server.h"
void push(const char nickname[NICK_SIZE], struct sockaddr_in cli);
Client* find(const char nickname[NICK_SIZE]) ;
Client* delete(const char nickname[NICK_SIZE]);
Client* findByCli(struct sockaddr_in cli);
Client* getHead();
#endif
