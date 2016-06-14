/*

removecontent
This removes some content that is hosted by a peer in your system.
The ip-address and port of some peer, and the key associated with the content are provided as command-line arguments.
As with addcontent, the peer provided at the command-line is not necessarily the peer that hosts the content.

Example usage and output:

$ removecontent 1.2.3.4 50400 44
Error: no such content

$ removecontent 3.4.5.6 12013 12

*/

#include "removecontent.h"
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

void removecontent(char *ip, int port, int lookupId) {
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

   //send "removecontent" message
   int len, bytes_sent;
   char buffer[30] = {'\0'};
   snprintf(&buffer[0], 30, "%d", lookupId);
   std::string msg;
   msg.append(1, REMOVE_CONTENT);
   msg.append(":");
   msg.append(buffer);
   msg.append("\0");
   len = msg.length();
   bytes_sent = send(sockfd, msg.c_str(), len, 0);

   close(sockfd);
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
         removecontent(ip, port, uniqueId);
         break;

      default:
         std::cerr << "Usage: removecontent ip port key" << std::endl;
         exit(-1);

   } //end of input switch

}
