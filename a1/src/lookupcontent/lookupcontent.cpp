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

int main(int argc, char *argv[]) {
   switch (argc) {//parse input
      case 4:
         //key
         //port - std::atoi(argv[2])
         //ip
         std::cout << argv[2] << std::endl << argv[1] << std::endl;
         break;

      default:
         std::cerr << "Usage: lookupcontent ip port key" << std::endl;
         exit(-1);

   } //end of input switch

}
