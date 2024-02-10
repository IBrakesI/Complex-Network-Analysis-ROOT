#ifndef GRAPHANALYZER_H
#define GRAPHANALYZER_H

#include <Riostream.h>
#include "Graph.h"



class GraphAnalyzer : public TObject {
///////////////////////////////////////////////
////This class analyzes a graph from Graph/////
///////////////////////////////////////////////
  public : 
  //
  GraphAnalyzer();
  GraphAnalyzer(Graph& g);
  virtual ~GraphAnalyzer();
  
  // Draw the degree histogram and make a fit with the theorical funcions;
  TH1D* DegreeDistributionFit();     
  
  // Draw the plot of n_g/n vs k_mean for a ER graph (here we do not need a graph in the instance of a GraphAnalyzer obj) 
  TH1D* DrawPhaseTransition(unsigned int k_mean_max = 2, double p = 0.0001, unsigned int step = 1000);
  
  // Draw the plot of n_g/n vs f for a graph under an attack ('R' random, 'D' degree)
  TH1D* DrawAttack(unsigned int nPoints = 20,  char typeAttack = 'R',int color = kBlack, int markerStyle = 34);
  
  private :
  //
  Graph* pgraph;  
  
  
  
  
  
  ClassDef(GraphAnalyzer,1)
  


};



#endif
