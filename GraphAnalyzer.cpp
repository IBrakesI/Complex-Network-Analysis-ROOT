#include <Riostream.h>
#include <TRandom3.h>
#include <TH1D.h>
#include <TF1.h>
#include "GraphAnalyzer.h"
#include <TMath.h>



ClassImp(GraphAnalyzer)
  //////////////
  //////////////
  //////////////
  
  
//____________________________________________________
GraphAnalyzer::GraphAnalyzer() : TObject(),
pgraph(NULL){
  //default constructor
}


//____________________________________________________
GraphAnalyzer::GraphAnalyzer(Graph& g) : TObject(),
pgraph(&g){
}


//____________________________________________________
GraphAnalyzer::~GraphAnalyzer(){
  //default destructor
  if (pgraph != NULL)  pgraph = NULL;//delete pgraph;
}


//____________________________________________________
TH1D* GraphAnalyzer::DegreeDistributionFit(){
  unsigned int* degreeSequence = pgraph->GetDegreeSequence();
  unsigned int k_max = pgraph->GetMaxDegreeNode().GetDegree();
  unsigned int k_min = pgraph->GetMinDegreeNode().GetDegree();
  cout << "k_max: "<< k_max << endl;
  cout << "k_min: "<< k_min << endl;
  
  TH1D* h1 = new TH1D("degdistr","Degree distribution",100 , k_min-0.5 , k_max+0.5);
  
  unsigned int n = pgraph->GetActiveNodes();
  
  for(unsigned int i = 0; i < n; i++){
    h1->Fill(degreeSequence[i]);
  }
  
  unsigned int type = pgraph->GetType();
  
  if(type == 2){
    cout << "The graph is neither ER nor BA" << endl;
  }
  else {
    if(type == 1){
      TF1 *fitFunction = new TF1("fitFunction", "[0]*x^(-[1])", 0, k_max);
      h1->Fit(fitFunction , "", "", (k_max - k_min)/10.+k_min, k_max+1);
      double chi = fitFunction->GetChisquare();
      //cout << chi<< endl;
      
      double ndf = fitFunction->GetNDF();
      //cout << ndf << endl;

      cout << "p-value: " << TMath::Prob(chi, ndf) << endl;
    }
    else{
      TF1 *fitFunction = new TF1("fitFunction", "[1]*TMath::Poisson(x, [0])", k_min, k_max);
      double p = pgraph->GetProb();
      fitFunction->SetParameters(n*p, n); // you MUST set non-zero initial values for parameters
      h1->Fit(fitFunction , "", "", k_min, k_max);
      double chi = fitFunction->GetChisquare();
      //cout << chi<< endl;
      
      double ndf = fitFunction->GetNDF();
      //cout << ndf << endl;

      cout << "p-value: " << TMath::Prob(chi, ndf) << endl;
    }
    h1->SetMarkerStyle(kFullSquare);
    
    
  } 
  return h1;
}   
   
//____________________________________________________   
TH1D* GraphAnalyzer::DrawPhaseTransition(unsigned int k_mean_max, double p, unsigned int step){
  unsigned int n_max = (unsigned int)(k_mean_max/p);
  
  TH1D* h1 = new TH1D("phasetrans","Phase Transition", n_max-1 , 0. , k_mean_max+0.5);
  
  for(unsigned int n = step; n <= n_max; n+=step){
    //cout << "n: " << n << endl;
    Graph g(n,p);
    unsigned int n_g = g.GiantComponentSize();
    double f = ((double)n_g)/n;
    //cout << "n_g: " << n_g << endl;
    h1->Fill(n*p, f);
    cout << "-";
    
  }
  cout << endl;
  h1->SetMarkerStyle(34);
  //h1->SetLineColor(kWhite);
  h1->SetLineWidth(0);
  h1->GetXaxis()->SetRangeUser(0, k_mean_max+0.1);
  h1->GetYaxis()->SetRangeUser(0, 1); 
  h1->SetStats(0);
  return h1;
}


//____________________________________________________
TH1D* GraphAnalyzer::DrawAttack(unsigned int nPoints,  char typeAttack, int color, int markerStyle){
  if(typeAttack != 'R' && typeAttack != 'D'){
    cout << "typeAttack must be 'R' for random attack or 'D' for degree attack" << endl;
    TH1D* h1 = NULL;
    return h1;
  }
  else {
    unsigned int n = pgraph->GetActiveNodes();
    unsigned int step = ((double)n)/nPoints;
    char pn_hist[10];
    
    sprintf(pn_hist,"attack%c",typeAttack);
    TH1D* h1 = new TH1D("attack","Attack", n , -0.1 , 1.1);   
    
    for(unsigned int i = 0; i < n; i+=step){
      double f = ((double)i)/n;
      if(typeAttack == 'R')
        pgraph->RandomAttack(0.1, step, 'n');
      else 
        pgraph->DegreeAttack(0.1, step, 'n');
        
      unsigned int n_g = pgraph->GiantComponentSize();
               
      //cout << "ng: " << n_g << endl;
      //cout << "______________\n";
      double s = ((double)n_g)/n;
      h1->Fill(f, s);
      
    }
    h1->SetMarkerStyle(markerStyle);
    h1->SetMarkerColor(color);
    h1->SetLineWidth(0);
    h1->GetXaxis()->SetRangeUser(0, 1.1);
    h1->GetYaxis()->SetRangeUser(0, 1.1); 
    h1->SetStats(0);
    return h1;

  }
  


}











