/*

addpeer
This causes a new peer to be created and added to your system.
The optional command-line arguments are the ip-address and port associated with one other peer in your system.
If these command-line arguments are not provided, you assume that this is the first peer in the system.
You should output the non-loopback ip address and port at which the peer is created.
The peer is always created on this host.
If the host has more than one non-loopback ip address, you pick any of those addresses to print.

Example usage and output:
$ addpeer
1.2.3.4 50400

$ addpeer 1.2.3.4 50400
3.4.5.6 12013

$ addpeer 1.2.3.4 50400
3.4.5.6 34232


Load-balancing requirement
At any moment, you should ensure that each peer maintains an equally fairshare of the content items.
That is, if at a particular moment we have P peers and C pieces of content, then each peer must host between floor(c/p) and ceiling(c/p) pieces of content.
For us to query and check this, you havethe following additional requirement:

We will write a tcp client that will connect() to the ip-address and port of a peer.
Once the peer accepts the connection request, we will send() the string “allkeys”.
We will then recv().
The peer must send back the keys of all the pieces of content it hosts as a string, separated by ',', and terminated by '0'.

E.g., the string the peer responds with may be “23,4,12,0.”
Your peer may shutdown() the connection as soon as it sends this.
(We will shutdown() from our end anyway.)

*/

#include <iostream>
#include <cstdlib>
#include <strings.h>
#include <arpa/inet.h>
#include "../mybind.c"
#include "addpeer.h"


int main(int argc, char *argv[]) {
   int sockfd;
   switch (argc) {//parse input
      case 3:
         //port - std::atoi(argv[2])
         //ip
         std::cout << argv[2] << std::endl << argv[1] << std::endl;
         break;

      case 1:
         // Initialize the network "addpeer"
         sockaddr_in serv_addr; //= new sockaddr_in(AF_INET, 0, addr, 0);
         /* First call to socket() function */
         sockfd = socket(AF_INET, SOCK_STREAM, 0);
         if (sockfd < 0) {
            perror("ERROR opening socket");
            exit(1);
         }
         bzero((char *) &serv_addr, sizeof(serv_addr));
         serv_addr.sin_family = AF_INET;
         inet_aton("0.0.0.0", &serv_addr.sin_addr);
         if (mybind(sockfd, &serv_addr) < 0) {
            perror("ERROR on binding");
            exit(1);
         }

         break;

      default:
         std::cerr << "Usage: addpeer [ ip port ]" << std::endl;
         exit(-1);
   } //end of input switch

   return 0;
}
