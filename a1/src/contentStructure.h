#ifndef CONTENTSTRUCTURE
#define CONTENTSTRUCTURE

#include <iostream>
#include <vector>

class ContentNode {
public:
   std::string content;
   int unique_id;

   ContentNode(std::string c, int u) {
      content = c;
      unique_id = u;
   }
};

class ContentStructure {
public:
   static int contentCount;
   std::vector<ContentNode*> s;

   ContentStructure() {
   }

   //adds the content to this structure
   //returns the unique_id
   int addContent(std::string c) {
      ContentNode* newNode = new ContentNode(c, ++contentCount);
      s.push_back(newNode);
      return contentCount;
   }

   //pass in unique_id of content to be removed
   //returns true for successful removal
   //returns false if content does not exist in this structure
   bool removeContent(int id) {
      if (s.size() < 1) return false;

      int iter = 0; 
      while (s[iter]->unique_id != id) {
         iter++;
         if (iter == s.size()) return false;
      }
      delete s[iter];
      s.erase(s.begin() + iter);
      return true;
   }

   //pass in unique_id of content you are looking for
   //returns Node* of content if it exists
   //returns NULL if content does not exist in this structure
   ContentNode* lookupContent(int id) {
      int iter = 0; 
      while (s[iter]->unique_id != id) {
         iter++;
         if (iter == s.size()) return NULL;
      }
      return s[iter];
   }
};
int ContentStructure::contentCount = -1;

#endif
