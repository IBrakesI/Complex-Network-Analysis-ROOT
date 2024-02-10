#ifndef NODE_H
#define NODE_H

#include <Riostream.h>
#include <vector>

class Node : public TObject {
///////////////////////////////////////////////
/////This class instances nodes of a graph/////
///////////////////////////////////////////////
  public : 
  //
  Node();
  Node(unsigned int n);
  Node(const Node &source);
  virtual ~Node();
  
  unsigned int GetId() const {return id;}
  unsigned int GetDegree() const {return degree;}
  vector<unsigned int>* GetVector() const {return adjVectPtr;}
  
  void Update(const Node& otherNode);
  void SetId(unsigned int n);
  void Destroy(const Node& otherNode);
  
  virtual void Print(Option_t * option = "") const;
  
  private :
  //
  unsigned int id;                    // node's id
  vector<unsigned int> *adjVectPtr;   // adjacency vector of the node
  unsigned int degree;
  
  
  
  ClassDef(Node,1)
  


};



#endif
