/*

lookupcontent
Given an ip-address and port of a peer, and a key as command-line arguments,
this retrieves the content from your peer-to-peer system and prints it to stdout.

Example usage andoutput:

$ lookupcontent 1.2.3.4 50400 12
Error: no such content

$ lookupcontent 1.2.3.4 50400 328
Come, Thou Tortoise

*/
#include "lookupcontent.h"
#include "../operations.h"

#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

std::string lookupcontent(char *ip, int port, int lookupId) {
   //connect to target
   int sockfd = socket(PF_INET, SOCK_STREAM, 0); //TODO: do some error checking!
   sockaddr_in dest_addr;

   dest_addr.sin_family = AF_INET;
   dest_addr.sin_port = htons(port);
   dest_addr.sin_addr.s_addr = inet_addr(ip);
   memset(&(dest_addr.sin_zero), '\0', 8);  // zero the rest of the struct


   if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
       exit(-1);
   } //TODO: don't forget to error check the connect()!

   //send "lookupcontent" message
   int len, bytes_sent;
   char buffer[30];
   snprintf(&buffer[0], 30, "%d", lookupId);
   std::string msg;
   msg.append(1, LOOKUP_CONTENT);
   msg.append(":");
   msg.append(buffer);
   msg.append("\0");
   len = msg.length();
   bytes_sent = send(sockfd, msg.c_str(), len, 0);

   //receive content back
   int byte_count;
   char content[512];
   byte_count = recv(sockfd, content, sizeof(content), 0);
   //TODO error check with bytecount

   close(sockfd);

   return content;
}

int main(int argc, char *argv[]) {
   char *ip;
   int port;
   int uniqueId;
   std::string content;

   switch (argc) {//parse input
      case 4:
         ip = argv[1];
         port = atoi(argv[2]);
         uniqueId = atoi(argv[3]);
         content = lookupcontent(ip, port, uniqueId);
         std::cout << content << std::endl;
         break;

      default:
         std::cerr << "Usage: lookupcontent ip port key" << std::endl;
         exit(-1);

   } //end of input switch

}



