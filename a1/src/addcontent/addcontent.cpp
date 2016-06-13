/*

addcontent
This takes as argument the ip-address and port of some peer in your system, and a piece of content.
It adds a new piece of content into the system.
You choose a peer to host the content – this is not necessarily the peer that we provide as command-line argument.
The content,  which is provided as command-line argument, is just a string.
This command outputs, to stdout, a unique positive integer key by which this content may be retrieved from your peer-to-peer system.

Exampleusage and output:

$ addcontent 1.2.3.4 50400 "Alias Grace"
12

$ addcontent 3.4.5.6 34232 "Come, Thou Tortoise"
328

$ addcontent 1.2.3.4 50400 "No Great Mischief"
33

$ addcontent 3.4.5.6 50400 "Fifth Business"
Error: no such peer

*/

#include "addcontent.h"

#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int addContent(char *ip, int port, std::string content) {
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

   //send "addcontent" message
   int len, bytes_sent;
   std::string msg = "2:" + content + "\0";
   len = msg.length();
   bytes_sent = send(sockfd, msg.c_str(), len, 0);

   //receive unique id back
   int byte_count;
   char uniqueId[4];
   byte_count = recv(sockfd, uniqueId, sizeof(int), 0);
   //TODO: if byte_count != 4, heart attack

   close(sockfd);
   std::cout << atoi(uniqueId) << std::endl;
   return atoi(uniqueId);
}

int main(int argc, char *argv[]) {
   char *ip;
   int port;
   std::string content;

   switch (argc) {//parse input
      case 4:
         ip = argv[1];
         port = atoi(argv[2]);
         content = argv[3];
         addContent(ip, port, content);
         break;

      default:
         std::cerr << "Usage: addcontent ip port content" << std::endl;
         exit(-1);

   } //end of input switch

}
