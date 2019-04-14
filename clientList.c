#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include "server.h"

Client* head = NULL;

void push(const char nickname[NICK_SIZE], struct sockaddr_in cli) {
   //create a link
   Client *link = (struct Client*) malloc(sizeof(struct Client));
   
   strcpy(link->nickname, nickname);
   link->cli = cli;
        
   link->next = head;
   head = link;
}
Client* getHead(){
  return head;
}
Client* find(const char nickname[NICK_SIZE]) {

   struct Client* current = head;

   if(head == NULL) {
      return NULL;
   }

   while(strcmp(current->nickname, nickname)) {
      if(current->next == NULL) {
         return NULL;
      } else {
         current = current->next;
      }
   }      	
   return current;
}

int clientCompare(struct sockaddr_in client1, struct sockaddr_in client2) {
    if(strncmp
       ((char *) &client1.sin_addr.s_addr, (char *) &client2.sin_addr.s_addr,
        sizeof(unsigned long)) == 0) {
        if(strncmp((char *) &client1.sin_port, (char *) &client2.sin_port, sizeof(unsigned short))
           == 0) {
            if(strncmp
               ((char *) &client1.sin_family, (char *) &client2.sin_family,
                sizeof(unsigned short)) == 0) {

                return 0;
            }
        }
    }
    return 1;
}
Client* findByCli(struct sockaddr_in cli){
   struct Client* current = head;

   if(head == NULL) {
      return NULL;
   }

   while(clientCompare(current->cli, cli)) {
      if(current->next == NULL) {
         return NULL;
      } else {
         current = current->next;
      }
   }      	
   return current;
}

  

Client* delete(const char nickname[NICK_SIZE]) {

   Client* current = head;
   Client* previous = NULL;
	
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(strcmp(current->nickname, nickname)) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}
