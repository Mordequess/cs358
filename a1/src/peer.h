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
   int numPeers;
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

      numPeers = 0;
      leftPeer = rightPeer = serv_addr;

      //ready
      std::cout << inet_ntoa(serv_addr.sin_addr) << " " << ntohs(serv_addr.sin_port) << std::endl;
      begin();
   }

   //this method is called if this peer is not the first
   void addPeerToNetwork(char *ip, int port) {}

   //this method is called after creation by the new processes
   //when we receive a kill command, we run off the end of this method and the process dies
   void begin() {
      listen(sockfd, 10); // set s up to be a server (listening) socket
      std::cout << "listening!" << std::endl;

      int newSocket;
      sockaddr remoteAddress;
      socklen_t remoteAddressLength;
      int count = 0 , byte_count;
      char buffer[512];

      while (true) {
         newSocket = accept(sockfd, &remoteAddress, &remoteAddressLength); //this blocks: how to pass control?
         std::cout << "WE CONNECTED" << std::endl;

         // all right!  now that we're connected, we can recieve some data!
         byte_count = recv(newSocket, buffer, sizeof(buffer), 0);

         switch(buffer[0] - '0') {
            case 0:
               std::cout << "Adding a peer to the network";
               break;
            case 1:
               std::cout << "Removing a peer from the network";
            case 2:
               std::cout << "Removing a peer from the network";
            case 3:
               std::cout << "Removing a peer from the network";
            case 4:
               std::cout << "Removing a peer from the network";
            default:
               std::cout << "This should not be possible" << std::endl;
         }

         printf("recv()'d %d bytes of data in buf\n", byte_count);
         printf("I got: %s", buffer);
         std::cout << std::endl;

         /*
         while (recv(newSocket, buffer, length, 0) != 0) {
            //parse, act, respond
            std::cout << "buffer said -:" (std::string)(buffer *) << " " << length << std::endl;
            return;

            switch (length) { //TODO: should be buffer?
               default:
                  std::cout << buffer << std::endl;
                  return;
            }
         }
         */
      }
   }

};
//Peer::peerlist;

#endif
