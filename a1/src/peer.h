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
#include <unistd.h> //close

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
   ContentStructure container;

   int sockfd;
   int numPeers;
   int numContent;

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

      numPeers = numContent = 0;
      leftPeer = rightPeer = my_server_info;

      //ready
      std::cout << inet_ntoa(my_server_info.sin_addr) << " " << ntohs(my_server_info.sin_port) << std::endl;
      //TODO: print here? return as msg?
      run();
   }

   //this method is called if this peer is not the first
   void addPeerToNetwork(char *ip, int port) {}

   void addContentCommand(std::string c, int senderSocket) {
      int id = numContent++;
      container.addContent(c, id);
      //TODO: tell everybody about incremented contentcount

      //send "unique id" message
      int len, bytes_sent;
      char msg[5];
      len = snprintf(&msg[0], 5, "%d", id);
      bytes_sent = send(senderSocket, msg, len*sizeof(char), 0);
   }

   void lookupContentCommand(int uniqueId, int senderSocket) {
      std::string content = container.lookupContent(uniqueId);
      if (content == "") {
         content = "Error: no such content";
         //TODO look at other peers
      }
      int bytes_sent = send(senderSocket, content.c_str(), content.size(), 0);
      //TODO error check?
   }

   void removeContentCommand(int uniqueId, int senderSocket) {
      if (container.removeContent(uniqueId) != 0) {
         //check if left peer was the one contacted by god, if not
         //ask left peer if they have it via non-god message
         //if left peer was contacted by god,
         std::cout << "Error: no such content" << std::endl;
      }
   }

   int executeCommand(char *message, int senderSocket) {

      // std::string content = message.substr(2);

      switch(message[0]) {
         case ALLKEYS: // 'a'
            std::cout << "Output all keys at node";
            break;

         case ADD_PEER: //'0'
            std::cout << "Adding a peer to the network";
            break;

         case REMOVE_PEER: // '1'
            std::cout << "Removing a peer from the network";
            break;

         case ADD_CONTENT: // '2'
            addContentCommand(std::string(message).substr(2), senderSocket);
            break;

         case REMOVE_CONTENT: // '3'
            removeContentCommand(atoi(&message[2]), senderSocket);
            break;

         case LOOKUP_CONTENT: // '4'
            lookupContentCommand(atoi(&message[2]), senderSocket);
            break;

         case MOVE_CONTENT: // '5'
            std::cout << "Moving content across the network";
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

         default:
            std::cerr << "ERROR: This should not be possible" << std::endl;
      }
   }

   //this method is called after creation by the new processes
   //when we receive a kill command, we run off the end of this method and the process dies
   void run() {

      int newSocket;
      sockaddr_in remoteAddress;
      unsigned int remoteAddressLength = sizeof(sockaddr_in);
      int count = 0 , byte_count;
      char buffer[512] = {'\0'};

      listen(sockfd, 10);
      while (true) {
         newSocket = accept(sockfd, (sockaddr *)&remoteAddress, &remoteAddressLength);
         while (recv(newSocket, buffer, sizeof(buffer), 0) != 0) {
            executeCommand(buffer, newSocket);
            memset(buffer, '\0', 512);
         }
      }
   }
};
//Peer::peerlist;

#endif
