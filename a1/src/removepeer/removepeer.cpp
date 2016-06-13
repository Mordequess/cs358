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

int main(int argc, char *argv[]) {
   switch (argc) {//parse input
      case 3:
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
