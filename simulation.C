#include <Riostream.h>
#include <TRandom3.h>
#include <TH1D.h>
#include <TF1.h>
#include "GraphAnalyzer.h"
#include "Graph.h"
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>

//simulation for a ER graph
void simulationER(int seed, unsigned int n, double p);

//simulation for a BA graph
void simulationBA(int seed, unsigned int n, unsigned int m_0, unsigned int l);

//simulation for the study of phase transition
void simulationPhaseTransition(int seed, unsigned int k_mean_max, double p, unsigned int step);

//////////////////////////////
////////implementation////////
//////////////////////////////

void simulationER(int seed = 123, unsigned int n = 10000, double p = 0.002){
  gRandom->SetSeed(seed);
  

  Graph g(n, p);
  GraphAnalyzer ga(g);
  
  TCanvas* cv = new TCanvas;
  TH1D* h = ga.DegreeDistributionFit();
  h->SetXTitle("k");
  h->SetYTitle("frequencies");
  h->Draw("P");
  
  TCanvas* cv2 = new TCanvas;
  TH1D* h2 = ga.DrawAttack(20, 'R', kBlack, 34);
  h2->SetXTitle("f");
  h2->SetYTitle("\\frac{N_{g}}{N}");
  h2->Draw("P0");
  
  Graph g1(n, p);
  GraphAnalyzer ga1(g1);
  TH1D* h3 = ga1.DrawAttack(20, 'D', kRed, 22);
  h3->SetXTitle("f");
  h3->SetYTitle("\\frac{N_{g}}{N}");
  h3->Draw("P0 same");
  
  TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(h2, "Random attack", "p");
  legend->AddEntry(h3, "Degree based attack", "p");

  
  legend->Draw();
 
}


void simulationBA(int seed = 123, unsigned int n = 10000, unsigned int m_0 = 10, unsigned int l = 5){
  gRandom->SetSeed(seed);
  
  Graph g(n, m_0, l);
  GraphAnalyzer ga(g);
  
  TCanvas* cv = new TCanvas;
  TH1D* h = ga.DegreeDistributionFit();
  h->SetXTitle("k");
  h->SetYTitle("frequencies");
  
  cv->SetLogx();
  cv->SetLogy();
  h->Draw("P");
  
  
  TCanvas* cv2 = new TCanvas;
  TH1D* h2 = ga.DrawAttack(20, 'R', kBlack, 34);
  h2->SetXTitle("f");
  h2->SetYTitle("\\frac{N_{g}}{N}");
  h2->Draw("P0");
  
  
  Graph g1(n, m_0, l);
  GraphAnalyzer ga1(g1);
  TH1D* h3 = ga1.DrawAttack(20, 'D', kRed, 22);
  h3->SetXTitle("f");
  h3->SetYTitle("\\frac{N_{g}}{N}");
  h3->Draw("P0 same");
  
  TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(h2, "Random attack", "p");
  legend->AddEntry(h3, "Degree based attack", "p");

  
  legend->Draw();
  
}

void simulationPhaseTransition(int seed = 123, unsigned int k_mean_max = 2, double p = 0.0001, unsigned int step = 1000){
  gRandom->SetSeed(seed);
  
  GraphAnalyzer ga;
  TCanvas* cv1 = new TCanvas;
  TH1D* h1 = ga.DrawPhaseTransition(k_mean_max, p, step);
  h1->SetXTitle("\\langle k \\rangle");
  h1->SetYTitle("\\frac{N_{g}}{N}");
  h1->Draw("P0");
  
}


void simulationPhaseTransition_2(int seed = 123, unsigned int n_sim = 3){
  gRandom->SetSeed(seed);
  
  GraphAnalyzer ga;
  TCanvas* cv1 = new TCanvas;
  TH1D* phist[n_sim];
  
  unsigned int k_mean_max = 2;
  double p_min = 0.0001, p_max = 0.001; 
  
  for(unsigned i = 0; i < n_sim; i++){
    double p = p_min + (p_max - p_min)*((double)i)/n_sim;
    unsigned int step = (unsigned int)(k_mean_max/(p*20.));
    
    phist[i] = ga.DrawPhaseTransition(k_mean_max, p, step);
    phist[i]->SetMarkerStyle(20+i);
    phist[i]->SetMarkerColor(1+i);
    phist[i]->SetXTitle("\\langle k \\rangle");
    phist[i]->SetYTitle("\\frac{N_{g}}{N}");
    phist[i]->Draw("P0 same");
  }
  
  
  
}
