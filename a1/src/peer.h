#ifndef PEER
#define PEER

#include <iostream>
#include <cstdlib>
#include <strings.h>
#include <arpa/inet.h>

#include "contentStructure.h"
#include "mybind.h"

class Peer {
public:
   //static std::vector<Peer*> peerlist;
   ContentStructure s;

   int sockfd;
   sockaddr_in serv_addr; //= new sockaddr_in(AF_INET, 0, addr, 0);

   Peer() {
            // Initialize the network "addpeer"
      /* First call to socket() function */
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0) {
         perror("ERROR opening socket");
         exit(1);
      }
      bzero((char *) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      inet_aton("0.0.0.0", &serv_addr.sin_addr);
      if (mybind(sockfd, &serv_addr) < 0) {
         perror("ERROR on binding");
         exit(1);
      }
   }

   void addPeerToList() {

   }



};
//Peer::peerlist;

#endif
