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
   //static std::vector<Peer*> peerlist;
   ContentStructure content;

   int sockfd;
   sockaddr_in serv_addr; //= new sockaddr_in(AF_INET, 0, addr, 0);
   sockaddr_in leftPeer;
   sockaddr_in rightPeer;

   Peer() {
      std::cout << "we made one!" << std::endl;
            // Initialize the network "addpeer"
      /* First call to socket() function */
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0) {
         perror("ERROR opening socket");
         exit(1);
      }
      bzero((char *) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      if (pickServerIPAddr(&serv_addr.sin_addr) < 0) ; //TODO: fail
      //inet_aton("0.0.0.0", &serv_addr.sin_addr);
      if (mybind(sockfd, &serv_addr) < 0) {
         perror("ERROR on binding");
         exit(1);
      }

      std::cout << "ip  : " << inet_ntoa(serv_addr.sin_addr) << ":" << serv_addr.sin_port << std::endl;

   }

   //only called by addpeer if creating the first peer
   void setupNetwork() {
      leftPeer = rightPeer = serv_addr;
      std::cout << "My address    :" << inet_ntoa(serv_addr.sin_addr) << ":" << serv_addr.sin_port << " at memory " << &serv_addr << std::endl;
      std::cout << "Left address  :" << inet_ntoa(leftPeer.sin_addr) << ":" << leftPeer.sin_port << " at memory " << &leftPeer << std::endl;
      std::cout << "Right address :" << inet_ntoa(rightPeer.sin_addr) << ":" << rightPeer.sin_port << " at memory " << &rightPeer << std::endl;
   }

   void addPeerToNetwork(char *ip, int port) {}


   void begin() {
      std::cout << "im gonna listen" << std::endl;
      listen(sockfd, 10); // set s up to be a server (listening) socket
      std::cout << "listening!" << std::endl;

      sockaddr *incomingAddress;
      socklen_t *sizeOfRequest;
      int count = 0;
      while (true) {
         std::cout << count++ << std::endl;
         int newSocket = accept(sockfd, incomingAddress, sizeOfRequest); //this blocks: how to pass control?
         //parse
         //act
         //respond
      }


   }


};
//Peer::peerlist;

#endif
