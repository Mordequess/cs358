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
#include "../peer.h"

#include <stdlib.h>

int main(int argc, char *argv[]) {
   int key, port, ip;
   switch (argc) {//parse input
      case 4:
         key = atoi(argv[3]);
         port = atoi(argv[2]);
         //ip = inet_aton(atgv[1]);
         //Pass this request to the designated peer
         //if that peer might be me:
            //ContentNode* node = s.lookupcontent(key);
            // if(node != NULL)
            //    std::cout << node.content << std::endl;
            // else
               //if nextPeer == initialRequestedPeer, return "Error: no such content"
               //pass to the next peer
         break;

      default:
         std::cerr << "Usage: lookupcontent ip port key" << std::endl;
         exit(-1);

   } //end of input switch

}
