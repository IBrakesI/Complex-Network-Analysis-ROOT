#include <Riostream.h>
#include <TRandom3.h>
#include "Graph.h"

ClassImp(Graph)
  //////////////
  //////////////
  //////////////
  
//____________________________________________________
Graph::Graph() : TObject(),
nNodes(0),
prob(0.),
linksAdded(0),
initialNodes(0),
graphType(0),
degreeTot(0),
NodeList(NULL){
  //default constructor
}


//____________________________________________________
Graph::Graph(unsigned int n) : TObject(),
nNodes(n),
prob(0.),
linksAdded(0),
initialNodes(0),
graphType(2),
degreeTot(0){
  activeNodeIds.reserve(nNodes);
      
  NodeList = new Node[nNodes];
  for(unsigned int i = 0; i < nNodes; i++){        //set the right id of the nodes
    NodeList[i].SetId(i);
    activeNodeIds.push_back(i);                    //set the node as active
  }
}


//____________________________________________________
Graph::Graph(unsigned int n, unsigned int m_0, unsigned int l) : TObject(),
nNodes(n),
prob(0.),
linksAdded(l),
initialNodes(m_0),
degreeTot(0),
graphType(1){ //https://barabasi.com/f/622.pdf
  activeNodeIds.reserve(nNodes);
    
  NodeList = new Node[nNodes];
  for(unsigned int i = 0; i < nNodes; i++){        //set the right id of the nodes
    NodeList[i].SetId(i);
    activeNodeIds.push_back(i);                    //set the node as active
  }
  
  for(unsigned int i = 1; i < m_0; i++){           //first m_0 nodes are fully connected
    for(unsigned int j = 0; j < i; j++){
      AddEgde(NodeList[i],NodeList[j]);
    }
  }
  
  for(unsigned int i = m_0; i < nNodes; i++){      //preferential attachment
    unsigned int linksCreated = 0;                 //for every node it can be created a maximum of linksAdded edges (0<l<=m_0)
    unsigned int degreeSum = degreeTot;            //sum of degree at the step i
    while(linksCreated < linksAdded){              
      for(unsigned int j = 0; j < i-1; j++){
        double probj = ((double)NodeList[j].GetDegree()) / degreeSum;
        if(gRandom->Rndm() < probj){
          AddEgde(NodeList[i],NodeList[j]);
          linksCreated++;
        } 
      }
    }
  } 
}


//____________________________________________________
Graph::Graph(unsigned int n,double p) : TObject(),
nNodes(n),
prob(p),
linksAdded(0),
initialNodes(0),
degreeTot(0),
graphType(0){
  activeNodeIds.reserve(nNodes);
  NodeList = new Node[nNodes];
  for(unsigned int i = 0; i < nNodes; i++){        //set the right id of the nodes
    NodeList[i].SetId(i);
    activeNodeIds.push_back(i);                    //set the node as active
  }
  
  for(unsigned int i = 0; i < nNodes; i++){        //in order not to do the operation twice, j starts from i+1 
    for(unsigned int j = i+1; j < nNodes; j++){
      if(gRandom->Rndm() < prob){                  //an edge is created with a fixed probability = prob
        AddEgde(NodeList[i],NodeList[j]);
      }
    }
  }

}


//____________________________________________________
Graph::~Graph(){
  //default destructor
  if (NodeList != NULL) delete[] NodeList;
}


//____________________________________________________
void Graph::AddEgde(Node& i, Node& j){
  i.Update(j);
  j.Update(i);
  degreeTot += 2;
}


//____________________________________________________
void Graph::RemoveEdge(Node& i, Node& j){
  unsigned int degreei = i.GetDegree();
  unsigned int degreej = j.GetDegree();
  
  i.Destroy(j);
  j.Destroy(i);
  if((degreei != i.GetDegree()) && (degreej != j.GetDegree()))
    degreeTot -= 2;


}


//____________________________________________________
void Graph::RemoveNode(Node& node){
  if(node.GetVector() != NULL){
    unsigned int end = node.GetVector()->size();
    for(unsigned int i = 0; i < end; i++){
      unsigned int otherNodeId = node.GetVector()->at(0); //I remove always the first node in order not to get an out of range error
      RemoveEdge(node, NodeList[otherNodeId]);
    }
  }
  auto it = std::find(activeNodeIds.begin(), activeNodeIds.end(), node.GetId());
  if (it != activeNodeIds.end())
    activeNodeIds.erase(it);
  
}


//____________________________________________________
void Graph::Print(Option_t * option) const{
  cout << "This is a ";
  if(graphType == 1) cout << "Barabasi-Albert ";
  else if(graphType == 0) cout << "Erdos-Renyi ";
  cout << "graph." << endl
       << "It contains " << activeNodeIds.size() << " nodes with " << degreeTot/2 << " edges." << endl
       << "The node list is:\n";
  for(unsigned int i : activeNodeIds) NodeList[i].Print();
  //for(unsigned int i = 0; i < nNodes; i++) NodeList[i].Print();
}


//____________________________________________________
void Graph::PrintIdSequence() const{
  unsigned int* idSequence = GetIdSequence();
  cout << "[";
  for(unsigned int i = 0; i < activeNodeIds.size(); i++){
    if(i) cout << ", ";
    cout << idSequence[i];
  } 
  cout << "] \n";
}


//____________________________________________________
void Graph::PrintDegreeSequence() const{
  unsigned int* degreeSequence = GetDegreeSequence();
  cout << "[";
  for(unsigned int i = 0; i < activeNodeIds.size(); i++){
    if(i) cout << ", ";
    cout << degreeSequence[i];
  } 
  cout << "] \n";

}


//____________________________________________________
unsigned int* Graph::GetDegreeSequence() const{
  unsigned int* degreeSequence = new unsigned int[activeNodeIds.size()];
  unsigned int j = 0;
  for(unsigned int i : activeNodeIds){
    degreeSequence[j] = NodeList[i].GetDegree();
    j++;
  }
  return degreeSequence;
}


//____________________________________________________
unsigned int* Graph::GetIdSequence() const{

  unsigned int* idSequence = new unsigned int[activeNodeIds.size()];
  unsigned int j = 0;
  for(unsigned int i : activeNodeIds){
    idSequence[j] = i;
    j++;
  }
  return idSequence;
}


//____________________________________________________
Node& Graph::GetNode(unsigned int i) const{
  return NodeList[i];
//in future you can check if the id i is in NodeList
}


//____________________________________________________
Node& Graph::GetMaxDegreeNode() const{
  unsigned int* degreeSequence = GetDegreeSequence();
  unsigned int maxDegreeNodeIndex = 0,
               maxDegree = 0;
  for(unsigned int i = 0; i < activeNodeIds.size(); i++){
    if (degreeSequence[i] > maxDegree){
      maxDegree = degreeSequence[i];
      maxDegreeNodeIndex = i;
    }
  }
  return NodeList[activeNodeIds[maxDegreeNodeIndex]];
}


//____________________________________________________
Node& Graph::GetMinDegreeNode() const{
  unsigned int* degreeSequence = GetDegreeSequence();
  unsigned int minDegreeNodeIndex = 0,
               minDegree = activeNodeIds.size();
  for(unsigned int i = 0; i < activeNodeIds.size(); i++){
    if (degreeSequence[i] < minDegree){
      minDegree = degreeSequence[i];
      minDegreeNodeIndex = i;
    }
  }
  return NodeList[activeNodeIds[minDegreeNodeIndex]];
}


//____________________________________________________
unsigned int Graph::GiantComponentSize(){
  bool* visited = new bool[nNodes];
  for (unsigned int v = 0; v < nNodes; v++)
    visited[v] = false;
    
  unsigned int sum = 0,
               nNodesVisited = 0,
               componentSize = 0,
               max = 0;
                 
  for (unsigned int v = 0; v < nNodes; v++) {
    if (visited[v] == false) {
      DFSUtil(v, visited, false);    
    }
      
    sum = 0;                                    //sum is the number of nodes visited
    for (unsigned int i = 0; i < nNodes; i++)          
      if(visited[i])
        sum +=1;
        
    componentSize = sum - nNodesVisited;        //componentSize is the size of the component to which v belongs
    nNodesVisited = sum;                        //nNodesVisited ide the number of nodes visited at the step i-1
    if (componentSize > max) 
      max = componentSize;                      //max contains the giant component size
          
    if (nNodesVisited == nNodes) break;         //if all nodes are visited end the cycle
  }
  return max;  
}


//____________________________________________________
void Graph::ConnectedComponents(){
  bool* visited = new bool[nNodes];             //mark all the vertices as not visited
  for (unsigned int v = 0; v < nNodes; v++)
    visited[v] = false;
 
  for (unsigned int v = 0; v < nNodes; v++) {
    if (visited[v] == false) {
      DFSUtil(v, visited);                      //print all reachable vertices from v
      cout << "\n\n";
    }
  }
  delete[] visited;
}


//____________________________________________________
void Graph::DFSUtil(int v, bool visited[], bool print){
  visited[v] = true;                            //mark the current node as visited and print it
  if(print)
    cout << v << " ";
    
  if (NodeList[v].GetDegree()){                 //if degree of v is 0 do not continue
    vector<unsigned int>::iterator i;
    for (i = NodeList[v].GetVector()->begin(); i != NodeList[v].GetVector()->end(); ++i) 
      if (!visited[*i])                         //recur for all the vertices adjacent to this vertex
        DFSUtil(*i, visited, print);
  }
}


//____________________________________________________
void Graph::RandomAttack(double f, unsigned int n, char by){
  if ((0. < f) && (f <= 1.) && (n > 0) && (n <= activeNodeIds.size())){
    if (by == 'f'){
      unsigned int nActiveNodes = activeNodeIds.size(),
                   i = 0;
      
      while( (double)(i)/nActiveNodes < f){
        unsigned int it = gRandom->Integer(activeNodeIds.size());      //it is the index of actedNodeIds that must be removed
        RemoveNode(NodeList[activeNodeIds[it]]);
        i++;
      }
    }
    else if (by == 'n'){
      for(unsigned int i = 0; i < n; i++){
      
        unsigned int it = gRandom->Integer(activeNodeIds.size());      //it is the index of actedNodeIds that must be removed
        RemoveNode(NodeList[activeNodeIds[it]]);
      }
    }
    else {
      cout << "Error: by must be 'f' if you want to remove a certain fraction of nodes ";
      cout << "or 'n' if you want to remove a number n of nodes\n";
    }
  }
  else cout << "Error: f must be in ]0,1] and n must be in ]0,nActiveNodes]\n";
}


//____________________________________________________
void Graph::DegreeAttack(double f, unsigned int n, char by){
  if ((0. < f) && (f <= 1.) && (n > 0) && (n <= activeNodeIds.size())){
    if (by == 'f'){
      unsigned int nActiveNodes = activeNodeIds.size(),
                   i = 0;
      
      while( (double)(i)/nActiveNodes < f){
        RemoveNode(GetMaxDegreeNode());
        i++;
      }
    }
    else if (by == 'n'){
      for(unsigned int i = 0; i < n; i++)
        RemoveNode(GetMaxDegreeNode());
    }
    else {
      cout << "Error: by must be 'f' if you want to remove a certain fraction of nodes ";
      cout << "or 'n' if you want to remove a number n of nodes\n";
    }
  }
  else cout << "Error: f must be in ]0,1] and n must be in ]0,nActiveNodes]\n";
}


