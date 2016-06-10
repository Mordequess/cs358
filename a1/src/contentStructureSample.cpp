#include "contentStructure.h"

int main() {
   ContentStructure s;
   std::cout << s.addContent("hi") << std::endl;
   std::cout << s.addContent("lo") << std::endl;
   std::cout << s.addContent("fi") << std::endl;
   std::cout << s.addContent("wi") << std::endl;
   std::cout << s.addContent("ai") << std::endl;
   std::cout << s.addContent("di") << std::endl;
   std::cout << s.addContent("fi") << std::endl;

   ContentNode *t;

   t = s.lookupContent(3);
   if (t != NULL) std::cout << t->content << std::endl;
   else std::cerr << "Error: no such content" << std::endl;

   t = s.lookupContent(9);
   if (t != NULL) std::cout << t->content << std::endl;
   else std::cerr << "Error: no such content" << std::endl;

   if (!s.removeContent(3)) std::cerr << "Error: no such content" << std::endl; 
   if (!s.removeContent(9)) std::cerr << "Error: no such content" << std::endl; 
}
