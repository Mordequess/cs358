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

int addContent(int IP, int port, std::string content) {
   int sockfd = socket(PF_INET, SOCK_STREAM, 0); // do some error checking!

   dest_addr.sin_family = AF_INET;          // host byte order
   dest_addr.sin_port = htons(port);   // short, network byte order
   dest_addr.sin_addr.s_addr = inet_addr(IP);
   //memset(&(dest_addr.sin_zero), '\0', 8);  // zero the rest of the struct
   // don't forget to error check the connect()!

   if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
       exit(-1);
   } 

   int len, bytes_sent;
   std::string msg = "2:"+content;
   len = msg.length();
   bytes_sent = send(sockfd, (char *)msg, len, 0);

   close(sockfd);
}






int main(int argc, char *argv[]) {
   switch (argc) {//parse input
      case 4:
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







         int newSocket;
         sockaddr remoteAddress;
         socklen_t remoteAddressLength;
         int count = 0 , byte_count;
         char buffer[512];

         newSocket = accept(sockfd, &remoteAddress, &remoteAddressLength); //this blocks: how to pass control?




         //forward the request to the input ip/port.
         //if this peer matches those params
            //if more than a single peer
               //if s.size()+1 > ceil(c/p)
                  //pass to the next peer in line for load balancing
            //std::cout << s.addContent(argv[3])) << std::endl;
         break;

      default:
         std::cerr << "Usage: addcontent ip port content" << std::endl;
         exit(-1);

   } //end of input switch

}
