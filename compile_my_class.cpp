#include <Riostream.h>
#include <TString.h>

void compile_my_class(TString myopt = "fast"){
  TString opt;
  if(opt.Contains("force")){
   opt = "kfg";
  }
  else opt = "kg";
  gSystem->CompileMacro("Node.cpp",opt.Data());
  gSystem->CompileMacro("Graph.cpp",opt.Data()); 
  //gROOT->LoadMacro("Graph.cpp");
  gSystem->CompileMacro("GraphAnalyzer.cpp",opt.Data());
}
