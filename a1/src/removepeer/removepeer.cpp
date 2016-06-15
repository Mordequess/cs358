/*

removepeer
This takes as argument the ip-address and port associated with a peer.
It causes the peer to be removed from the system.
All the content managed by the peer must be redistributed to the other peers before this peer disappears.
See also the "load-balancing requirement" below.
If this is the only peer in the system, then all the content vanishes with it.

Example usage and output:

$ removepeer 1.2.3.4 50400

$ removepeer 3.4.5.6 12013

$ removepeer 3.4.5.6 34232

$ removepeer 3.4.5.6 34232
Error: no such peer

*/

#include "removepeer.h"
#include "../operations.h"
#include <stdlib.h>

#include <netdb.h>

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


void removePeer(char *ip, int port) {
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


   int len, bytes_sent;
   char c[2] = {REMOVE_PEER, '\0'};

   char p[4];
   snprintf(&p[0], 4, "%d", port);
   std::string msg = std::string(c) + ":" + std::string(ip) + ":" + std::string(p) + "\0";
   std::cout << "MESSAGE : " << msg << std::endl;
   len = msg.length();
   bytes_sent = send(sockfd, msg.c_str(), len, 0);

   close(sockfd);
}


int main(int argc, char *argv[]) {
   switch (argc) {//parse input
      case 3:
         removePeer(argv[1], atoi(argv[2]));
         //Pass this request to the designated peer
         //if the peer to be removed is me:
         //int nextPeer = i%(number of peers in system - 1)
            //for (int i = 0; i < s.size(); i++) {
               //if nextPeer = this one, skip it
               //while nextPeer.s.size() > ceil(c/p), skip this peer
               //nextPeer.push_back(s.at(i));
            //}
            //for i in size of other peers, remove this peer from their lists
            //delete this peer by closing connections
         break;
      default:
         std::cerr << "Usage: removepeer ip port " << std::endl;
         exit(-1);

   } //end of input switch

}
