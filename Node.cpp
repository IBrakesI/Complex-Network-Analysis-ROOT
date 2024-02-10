#include <Riostream.h>
#include "Node.h"
#include <vector>
#include <algorithm>


ClassImp(Node)
  //////////////
  //////////////
  //////////////
  
//____________________________________________________
Node::Node() : TObject(),
id(0),
degree(0),
adjVectPtr(NULL) { 
  // default constructor
}  


//____________________________________________________
Node::Node(unsigned int n) : TObject(),
id(n),
degree(0),
adjVectPtr(NULL) {
  // standard constructor
}


//____________________________________________________
Node::Node(const Node &source) : TObject(source),
id(source.id),
degree(source.id) {
  //copy constructor
  if(source.degree > 0) {
    adjVectPtr = new vector<unsigned int>;
    *adjVectPtr = *(source.adjVectPtr);
  }
  else adjVectPtr = NULL;
}


//____________________________________________________
Node::~Node(){
  //default destructor
  if(degree > 0 || adjVectPtr != NULL) delete adjVectPtr;
  //if (adjVectPtr != NULL) delete[] adjVectPtr;

}


//____________________________________________________
void Node::Print(Option_t * option) const{
  cout << "------------------\n";
  cout << "Node Id: " << id
       << "\nDegree : " << degree << endl;
  if (degree > 0) {
    cout << "It is connected to: [";
    for (unsigned int i = 0; i < degree; i++){
      if(i) cout << ", ";
      cout << adjVectPtr->at(i);
    }
    cout << "]" << endl;
  }
}


//____________________________________________________
void Node::Update(const Node& otherNode) {
  if(adjVectPtr == NULL){
    adjVectPtr = new vector<unsigned int>;
  }
  degree++;
  adjVectPtr->push_back(otherNode.id);
}


//____________________________________________________
void Node::Destroy(const Node& otherNode){
  auto it = std::find(adjVectPtr->begin(), adjVectPtr->end(), otherNode.id);
  
  if(it != adjVectPtr->end()){
    adjVectPtr->erase(it);
    degree--;
  }
  else{
    cout << "The node " << otherNode.id << " is not linked to the node " << id << "\n";
  }

}


//____________________________________________________
void Node::SetId(unsigned int n){
  id = n;
}
