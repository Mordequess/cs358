/*

removepeer
This takes as argument the ip-address and port associated with a peer.
It causes thepeer to be removed from the system.
All the content managed by the peer must be redistributed to theother peers before this peer disappears.
See also the “load-balancing requirement” below.
If this isthe only peer in the system, then all the content vanishes with it.

Example usage and output:

$ removepeer 1.2.3.4 50400

$ removepeer 3.4.5.6 12013

$ removepeer 3.4.5.6 34232

$ removepeer 3.4.5.6 34232
Error: no such peer

*/

#include "removepeer.h"

int main(int argc, char *argv[]) {
   switch (argc) {//parse input
      case 3:
         //port - std::atoi(argv[2])
         //ip
         std::cout << argv[2] << std::endl << argv[1] << std::endl;
         break;

      default:
         std::cerr << "Usage: removepeer ip port " << std::endl;
         exit(-1);

   } //end of input switch

}
