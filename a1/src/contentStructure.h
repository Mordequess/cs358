#ifndef CONTENTSTRUCTURE
#define CONTENTSTRUCTURE

#include <iostream>
#include <vector>



class ContentStructure {
private:
   class ContentNode {
   public:
      std::string content;
      int unique_id;

      ContentNode(std::string c, int u) {
         content = c;
         unique_id = u;
      }
   };

   std::vector<ContentNode*> s;

public:

   ContentStructure() {
   }

   //adds content to this structure
   void addContent(std::string c, int id) {
      ContentNode *newNode = new ContentNode(c, id);
      if (s.size() < 1) {
         s.push_back(newNode);
         return;
      }
      for (int i = s.size()-1; i >= 0; --i) {
         if (newNode->unique_id > s[i]->unique_id) {
            s.insert(s.begin()+i+1, newNode);
            break;
         }
      }
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
   //returns content string if it exists here
   //returns empty string if content does not exist in this structure
   std::string lookupContent(int id) {
      int iter = 0; 
      while (s.at(iter)->unique_id != id) {
         iter++;
         if (iter == s.size()) return "";
      }
      return s.at(iter)->content;
   }
};

#endif
