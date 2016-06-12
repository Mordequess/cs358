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
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
   switch (argc) {//parse input
      case 4:
         int key = std::atoi(argv[3]);
         int port = std::atoi(argv[2]);
         int ip = inet_aton(atgv[1]);

         //Check if this peer hosts the content
         if (s.lookupContent(key) != NULL)
            // If this peer hosts it,
            s.removecontent(key);
         else
            //Forward the request to the next peer?
            //if nextPeer == initialRequestedPeer, return "Error: no such content"
         break;

      default:
         std::cerr << "Usage: removecontent ip port key" << std::endl;
         exit(-1);

   } //end of input switch

}
