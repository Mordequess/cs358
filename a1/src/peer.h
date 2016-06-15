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
#include <stdlib.h>

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

class Peer {
private:
   //std::vector<Peer*> peerlist;
   ContentStructure container;

   int sockfd;
   int numPeers;
   int numContent;

   sockaddr_in my_server_info;
   sockaddr_in leftPeer;
   sockaddr_in rightPeer;

   void init() {
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

      numPeers = 1;
      numContent = 0;
      leftPeer = rightPeer = my_server_info;
   }

   void removePeerCommand(std::string c, int senderSocket) {
      //ship our content off to left
      //TODO: LET US BALANCE THE CONTENTS

      exit(0);
   }

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

   void removeContentCommand(int uniqueId) {
      if (container.removeContent(uniqueId) != 0) {
         //check if left peer was the one contacted by god, if not
         //ask left peer if they have it via non-god message
         //if left peer was contacted by god,
         std::cout << "Error: no such content" << std::endl;
      }
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

   //move
   //changenum
   //getnum

   //this is a message from another peer saying "set me as your x"
   //parse the void* for sockaddr_in data, save as left or right
   //return to that peer saying what your old info was
   void changeNeighbourCommand(int which, char *peerData, int senderSocket) {
      //send "old info" message
      void *response = which ? &rightPeer : &leftPeer;

      unsigned long addr = 0;
      unsigned short port = 0;
      memcpy(&addr, &peerData[0], sizeof(addr));
      memcpy(&port, &peerData[5], sizeof(port));
      std::cout << "The address I got in changeNeighbourCommand was: " << addr << ':' << port << std::endl;

      int bytes_sent = send(senderSocket, (char *)response, sizeof(sockaddr_in), 0); //error check?
      close(senderSocket);

      //store their info
      if (which == LEFT) {
         leftPeer.sin_port = port;
         leftPeer.sin_addr.s_addr = addr;
      std::cout << "1st's leftPeer: " << inet_ntoa(leftPeer.sin_addr) << "--" << htons(leftPeer.sin_port) << std::endl;
      }
      else {
         rightPeer.sin_port = port;
         rightPeer.sin_addr.s_addr = addr;
      std::cout << "1st's rightPeer: " << inet_ntoa(rightPeer.sin_addr) << "--" << htons(rightPeer.sin_port) << std::endl;
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
            removePeerCommand(std::string(message).substr(2), senderSocket);
            break;

         case ADD_CONTENT: // '2'
            addContentCommand(std::string(message).substr(2), senderSocket);
            break;

         case REMOVE_CONTENT: // '3'
            removeContentCommand(atoi(&message[2]));
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

         case CHANGE_NEIGHBOUR: // '8'
std::cout << "side is--" << message[2] << std::endl;
            changeNeighbourCommand(message[2]-'0', &message[4], senderSocket);
            break;

         default:
            std::cerr << "Message: " << message << std::endl;
            exit(0);
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
         if(recv(newSocket, buffer, sizeof(buffer), 0) != 0) {
            std::cout << "Received command: " << buffer << std::endl;
            executeCommand(buffer, newSocket);
            memset(buffer, '\0', 512);
         }
      }
   }

public:
   Peer() {
      init();
      std::cout << inet_ntoa(my_server_info.sin_addr) << " " << ntohs(my_server_info.sin_port) << std::endl;
      //TODO: print here? return as msg?
      run();
   }

   //new peers need to set up neighbours
   Peer(char *ip, int port) {
      init();

      //connect to right
      int sockfd = socket(PF_INET, SOCK_STREAM, 0); //TODO: do some error checking!
      sockaddr_in dest_addr;

      dest_addr.sin_family = AF_INET;
      dest_addr.sin_port = htons(port);
      dest_addr.sin_addr.s_addr = inet_addr(ip);
      memset(&(dest_addr.sin_zero), '\0', 8);  // zero the rest of the struct

      if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
         std::cerr << "Error: no such peer" << std::endl;
         exit(-1);
      } //TODO: don't forget to error check the connect()!


std::cout << "Contructing our message: " << std::endl;

      //send "hey mr right, lets set up neighbours" messages
      char c[12];
      c[0] = CHANGE_NEIGHBOUR;
      c[1] = ':';
      c[2] = LEFT + '0';
      c[3] = ':';
      memcpy(&c[4], &(my_server_info.sin_addr.s_addr), sizeof(my_server_info.sin_addr.s_addr));
      c[8] = ':';
      memcpy(&c[9], &(my_server_info.sin_port), sizeof(my_server_info.sin_port));
      c[11] = '\0';

std::cout << "size s_addr  : " << sizeof(my_server_info.sin_addr.s_addr) << std::endl;
std::cout << "size s_port  : " << sizeof(my_server_info.sin_port) << std::endl;
std::cout << "My s_addr is  : " << my_server_info.sin_addr.s_addr << std::endl;
std::cout << "My sin_port is: " << my_server_info.sin_port << std::endl;
std::cout << "message       : " << c << std::endl;

      int bytes_sent = send(sockfd, c, sizeof(c), 0); //error check?

std::cout << "Sending to: " << inet_ntoa(dest_addr.sin_addr) << ":" << htons(dest_addr.sin_port) << std::endl;


      //receive sockData back, this will be my left
      int byte_count;
      char sockData[sizeof(sockaddr_in)];
      byte_count = recv(sockfd, sockData, sizeof(sockData), 0);
std::cout << "AAAA 3" << std::endl;
         leftPeer.sin_port = ((sockaddr_in *)(sockData))->sin_port;
         leftPeer.sin_addr = ((sockaddr_in *)(sockData))->sin_addr;
         memset(&(leftPeer.sin_zero), '\0', 8);  // zero the rest of the struct
      //try to receive a 0
std::cout << "AAAA 4" << std::endl;
      if (recv(sockfd, sockData, sizeof(sockaddr_in), 0) != 0) {
         std::cerr << "Error: updating neighbours, received too many messages" << std::endl;
      }

      close(sockfd);
      sockfd = socket(PF_INET, SOCK_STREAM, 0);

std::cout << "AAAA 5" << std::endl;
std::cout << "leftPeer: " << inet_ntoa(leftPeer.sin_addr) << "--" << htons(leftPeer.sin_port) << std::endl;
std::cout << "myselfPeer: " << inet_ntoa(my_server_info.sin_addr) << "--" << htons(my_server_info.sin_port) << std::endl;
      //connect and send to left,

      if (connect(sockfd, (struct sockaddr *)&leftPeer, sizeof(struct sockaddr)) < 0) {
         std::cerr << "Error: updating neighbours, could not find left" << std::endl;
         exit(-1);
      }


std::cout << "AAAA 6" << std::endl;

       //send "hey mr left, lets set up neighbours" messages
      c[2] = RIGHT + '0';
std::cout << "AAAA 7" << std::endl;

      bytes_sent = send(sockfd, c, sizeof(c), 0); //error check?

std::cout << "AAAA 8" << std::endl;

      memset(sockData, '\0', sizeof(sockData));

      byte_count = recv(sockfd, sockData, sizeof(sockaddr_in), 0);
      rightPeer.sin_port = ((sockaddr_in *)(sockData))->sin_port;
      rightPeer.sin_addr = ((sockaddr_in *)(sockData))->sin_addr;
      memset(&(rightPeer.sin_zero), '\0', 8);  // zero the rest of the struct
      //try to receive a 0
      if (recv(sockfd, sockData, sizeof(sockaddr_in), 0) != 0) {
         std::cerr << "Error: updating neighbours, received too many messages" << std::endl;
      }

      std::cout << "rightPeer: " << inet_ntoa(rightPeer.sin_addr) << "--" << htons(rightPeer.sin_port) << std::endl;

      std::cout << inet_ntoa(my_server_info.sin_addr) << " " << ntohs(my_server_info.sin_port) << std::endl;
      //TODO: print here? return as msg?
      close(sockfd);
std::cout << "I'M HERE MOTHER LICKER" << std::endl;
      run();
   }

};
//Peer::peerlist;

#endif
