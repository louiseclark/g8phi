{
  //optional Set input file directory, could be done with --hsin or setenv HSIN
  HSin("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/");
  
  //optional set output directory or file.
  //Directory=>1 out file for every in file
  //file=> 1 total output file
  //HSout("out");
  // N.B. only need all files->directory as files have been merged in the convert_output step
  
  HSout("out/hists_all2.root");
  //HSout("out/hists_costh_E.root");
  //HSout("out/hists_costh_top.root");
  //HSout("out/hists_KpKm.root");
  //HSout("out/hists_pKm.root");
  //HSout("out/hists_pKp.root");
  //HSout("out/hists_pKpKm.root");
  
  //Set up the input chain to be analysed,could remove HSin() if you want
  TChain* tree=new TChain("HSParticles","datachain");
  tree->Add(HSin()+"*.root");
  //tree->Add(HSin()+"file_all.root");
  //tree->Add(HSin()+"file_KpKm.root");
  //tree->Add(HSin()+"file_pKm.root");
  //tree->Add(HSin()+"file_pKp.root");
  //tree->Add(HSin()+"file_pKpKm.root");

  //if using proof
  //Give proof destination file (env variables can't be read directly
  if(gProof) {gProof->AddInput(new TNamed("HSOUT",HSout().Data()));tree->SetProof();}


  tree->Process("histogrammer.C++");
  
  //Not actually sure if this is needed or not...
  if(gProof)gProof->ClearData(TProof::kUnregistered | TProof::kForceClear);
}
