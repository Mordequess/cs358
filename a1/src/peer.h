#ifndef PEER
#define PEER

#include "contentStructure.h"
#include "mybind.h"
#include "pickIp.h"
#include "operations.h"

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
   sockaddr_in my_server_info;
   sockaddr_in leftPeer;
   sockaddr_in rightPeer;

   Peer() {
      // Initialize the network "addpeer"
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0) {
         perror("ERROR opening socket");
         exit(1);
      }
      bzero((char *) &my_server_info, sizeof(my_server_info));
      my_server_info.sin_family = AF_INET;
      if (pickServerIPAddr(&my_server_info.sin_addr) < 0) {
         perror("ERROR picking ip");
         exit(1);
      }
      if (mybind(sockfd, &my_server_info) < 0) {
         perror("ERROR on binding");
         exit(1);
      }

      numPeers = 0;
      leftPeer = rightPeer = my_server_info;

      //ready
      std::cout << inet_ntoa(my_server_info.sin_addr) << " " << ntohs(my_server_info.sin_port) << std::endl;
      run();
   }

   //this method is called if this peer is not the first
   void addPeerToNetwork(char *ip, int port) {}

   int executeCommand(char *message, int senderSocket) {

      // std::string content = message.substr(2);

      switch(message[0]) {
         case ADD_PEER: //'0'
            std::cout << "Adding a peer to the network";
            break;
         case REMOVE_PEER: // '1'
            std::cout << "Removing a peer from the network";
            break;
         case ADD_CONTENT: // '2'
            std::cout << "Adding a piece of content network";
            break;
         case REMOVE_CONTENT: // '3'
            std::cout << "Removing a piece of content from the network";
            break;
         case MOVE_CONTENT: // '4'
            std::cout << "Moving content across the network";
            break;
         case LOOKUP_CONTENT: // '5'
            std::cout << "Look up content on the network";
            break;
         case CHANGE_NUMPEERS_CONTENT: // '6'
            std::cout << "Change number of peers, content on the network";
            break;
         case GET_NUMPEERS_CONTENT: // '7'
            std::cout << "Retrieve number of peers, content on the network";
            break;
         case CHANGE_NEIGHBOUR_NEIGHBOUR: // '8'
            std::cout << "Change peer neighbours in the network";
            break;
         case ALLKEYS: // 'a'
            std::cout << "Output all keys at node";
            break;
         default:
            std::cout << "This should not be possible" << std::endl;
      }
}

   //this method is called after creation by the new processes
   //when we receive a kill command, we run off the end of this method and the process dies
   void run() {
      listen(sockfd, 10); // set s up to be a server (listening) socket
      std::cout << "listening!" << std::endl;

      int newSocket;
      sockaddr remoteAddress;
      socklen_t remoteAddressLength;
      int count = 0 , byte_count;
      char buffer[512];

      while (true) {
         newSocket = accept(sockfd, &remoteAddress, &remoteAddressLength);
         while (recv(newSocket, buffer, sizeof(buffer), 0) != 0) {
            executeCommand(buffer, newSocket);
         }
      }
   }

};
//Peer::peerlist;

#endif
