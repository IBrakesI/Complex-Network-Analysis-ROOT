#ifndef GRAPH_H
#define GRAPH_H

#include <Riostream.h>
#include "Node.h"



class Graph : public TObject {
///////////////////////////////////////////////
////This class instances a undirected graph////
////http://networksciencebook.com/ main ref////
///////////////////////////////////////////////
  public : 
  //
  Graph();
  Graph(unsigned int n);                                    //initializing a Graph with n nodes and no edges
  Graph(unsigned int n, unsigned int m_0, unsigned int l);  //constructor for a Barabasi Albert graph
  Graph(unsigned int n, double p);                          //constructor for an Erdos Renyi graph
  virtual ~Graph();
  
  void AddEgde(Node& i, Node& j);                           //add an undirected edge between two nodes 
  void RemoveEdge(Node& i, Node& j);                        //remove an undirected edge between two nodes
  virtual void Print(Option_t * option = "") const;
  void RemoveNode(Node& node);                              //remove a nove from the graph (at the end it will have degree = 0)
  
  //https://www.geeksforgeeks.org/connected-components-in-an-undirected-graph/
  unsigned int GiantComponentSize();                        //returns the size of the giant component
  void ConnectedComponents();                               //returns every connected component of the graph
  void DFSUtil(int v, bool visited[], bool print = true);   //this is for implementing giant component size 
  
  void RandomAttack(double f, unsigned int n, char by = 'f');  //Random attack. if by = 'f' I remove a fraction f of node,
                                                               //otherwise, if by = 'n' I remove n node frome the graph
  void DegreeAttack(double f, unsigned int n, char by = 'f');  //Degree attack. if by = 'f' I remove a fraction f of node,
                                                               //otherwise, if by = 'n' I remove n node frome the graph
  
  
  
  unsigned int* GetIdSequence() const;
  unsigned int* GetDegreeSequence() const;
  void PrintIdSequence() const;
  void PrintDegreeSequence() const;
  
  Node& GetMaxDegreeNode() const;
  Node& GetMinDegreeNode() const;
  
  unsigned int GetType() const {return graphType;}
  
  Node& GetNode(unsigned int i) const;
  
  unsigned int GetInitialNodes() const {return nNodes;}
  unsigned int GetActiveNodes() const {return activeNodeIds.size();}
  double GetProb() const {return prob;}
  
  
  
  private :
  //
   
  unsigned int nNodes;       //number of nodes
  double       prob;         //prob in Erdos Renyi
  unsigned int linksAdded;   //links that are added every step in Barabasi Albert
  unsigned int initialNodes; //number of initial nodes in Barabasi Albert
  unsigned int graphType;    //type of the graph (0 for ER, 1 for BA, 2 for others)
  
  
  vector<unsigned int> activeNodeIds; //ids' vector of nodes currently active (not removed from graph)
  unsigned int         degreeTot;     //sum of the degree of the nodes
  Node                 *NodeList;     //list of the all nodes
  
  
  
  ClassDef(Graph,1)
  


};



#endif
