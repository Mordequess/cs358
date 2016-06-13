#ifndef PEER
#define PEER

#include "contentStructure.h"
#include "mybind.h"
#include "pickIp.h"

#include <iostream>
#include <cstdlib>
#include <strings.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
struct addrinfo {
   int              ai_flags;
   int              ai_family;
   int              ai_socktype;
   int              ai_protocol;
   socklen_t        ai_addrlen;
   struct sockaddr *ai_addr;
   char            *ai_canonname;
   struct addrinfo *ai_next;
};

int getaddrinfo(const char *node, const char *service,
                const struct addrinfo *hints,
                struct addrinfo **res);

void freeaddrinfo(struct addrinfo *res);

const char *gai_strerror(int errcode);

*/

//99.253.145.155

class Peer {
public:
   //std::vector<Peer*> peerlist;
   ContentStructure content;

   int sockfd;
   sockaddr_in serv_addr;
   sockaddr_in leftPeer;
   sockaddr_in rightPeer;

   Peer() {
      // Initialize the network "addpeer"
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0) {
         perror("ERROR opening socket");
         exit(1);
      }
      bzero((char *) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      if (pickServerIPAddr(&serv_addr.sin_addr) < 0) {
         perror("ERROR picking ip");
         exit(1);
      }
      if (mybind(sockfd, &serv_addr) < 0) {
         perror("ERROR on binding");
         exit(1);
      }

      leftPeer = rightPeer = serv_addr;

      //ready
      std::cout << "My ip:" << inet_ntoa(serv_addr.sin_addr) << ":" << serv_addr.sin_port << std::endl;
      begin();
   }

   //this method is called if this peer is not the first
   void addPeerToNetwork(char *ip, int port) {}

   void begin() {
      std::cout << "IT HAS BEGUN" << std::endl;
      for (int i = 0; i < 30; i++) {
         //std::cout << i << std::endl;
      }
      while (true);
   }

};
//Peer::peerlist;

#endif
