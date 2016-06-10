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

*/

#include "addpeer.h"

int main(int argc, char *argv[]) {
   switch (argc) {//parse input
      case 3:
         //port - std::atoi(argv[2])
         //ip
         std::cout << argv[2] << std::endl << argv[1] << std::endl;

      case 1:
         //if not first, error
         std::cout << "howdy" << std::endl;
         break;

      default:
         std::cerr << "Usage: addpeer [ ip port ]" << std::endl;
         exit(-1);

   } //end of input switch

}
