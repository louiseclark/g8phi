//Make data with
//root 'Model1.C( "Data.root" )'

{

  THSsPlot* RF=new THSsPlot("Simple");
  RF->SetOutDir("out_phi/");
 // RF->SetBinnedFit(); //For parameter fits do binned chi2
  ///////////////////////////////Load Variables
  RF->LoadVariable("MKpKm[0.98,1.06]");//should be same name as variable in tree  
  //RF->LoadBinVars("Egamma",1,1.9,2.1);//should be same name as variable in tree 
  RF->LoadAuxVars("Egamma[1.5,4.5]");//should be same name as variable in tree 

  /////////////////////////////Make Model Signal
  //RF->Factory("Gaussian::Signal( MissM, SIMm[0.24,0.22,0.26], SIMw[0.01,0.001,0.02] )");
  RF->Factory("Gaussian::Signal( MKpKm, SIMm[1.02,0.92,1.12], SIMw[0.01,0.001,0.02] )");
  RF->LoadSpeciesPDF("Signal");


  ////////////////////////////////Additional background
  //RF->Factory("Chebychev::BG(MKpKm,{a0[-0,-10,10],a1[0,-10,10]})");
  RF->Factory("Chebychev::BG(MKpKm,{a0[-0.1,-10,10],a1[0.1,-10,10],a2[0,-10,10]})");
  RF->LoadSpeciesPDF("BG");

   ///////////////////////////Load Data
  TChain chain("HSParticles");
  chain.Add("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/filepKp_all.root");
  RF->SetIDBranchName("fgID");//This is not needed if all events from tree go into dataset, i.e. the variable ranges do not cut any events. It is safer to have it...
  //import to RooFit
  RF->LoadDataSet(&chain);
  gBenchmark->Start("Simple");
  RF->FitAndStudy(1); //argument gives number of parameter fits to perform
  //Do some other stuff before saving weights
  RF->DrawTreeVar("MissM",100,0.3,0.7);
  RF->DrawTreeVar("MpKm",100,1.4,2.4);
  gBenchmark->Stop("Simple");
  gBenchmark->Print("Simple");
  RF->GetWeights()->Save();
  
  
  
  
  THSsPlot* RF2=new THSsPlot("Simple");
  RF2->SetOutDir("out_lam/");
 // RF->SetBinnedFit(); //For parameter fits do binned chi2
  ///////////////////////////////Load Variables
  RF2->LoadVariable("MpKm[1.4,1.64]");//should be same name as variable in tree  
  //RF->LoadBinVars("Egamma",1,1.9,2.1);//should be same name as variable in tree 
  RF2->LoadAuxVars("Egamma[1.5,4.5]");//should be same name as variable in tree 

  /////////////////////////////Make Model Signal
  //RF->Factory("Gaussian::Signal( MissM, SIMm[0.24,0.22,0.26], SIMw[0.01,0.001,0.02] )");
  RF2->Factory("Gaussian::Signal( MpKm, SIMm[1.52,0.92,1.12], SIMw[0.01,0.001,0.02] )");
  RF2->LoadSpeciesPDF("Signal");


  ////////////////////////////////Additional background
  //RF->Factory("Chebychev::BG(MKpKm,{a0[-0,-10,10],a1[0,-10,10]})");
  RF2->Factory("Chebychev::BG(MpKm,{a0[-0.1,-10,10],a1[0.1,-10,10],a2[0,-10,10]})");
  RF2->LoadSpeciesPDF("BG");

   ///////////////////////////Load Data
  TChain chain2("HSParticles");
  chain2.Add("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/filepKp_all.root");
  RF2->SetIDBranchName("fgID");//This is not needed if all events from tree go into dataset, i.e. the variable ranges do not cut any events. It is safer to have it...
  //import to RooFit
  RF2->LoadDataSet(&chain2);
  gBenchmark->Start("Simple");
  RF2->FitAndStudy(1); //argument gives number of parameter fits to perform
  //Do some other stuff before saving weights
  RF2->DrawTreeVar("MissM",100,0.3,0.7);
  RF2->DrawTreeVar("MKpKm",100,0.8,1.2);
  gBenchmark->Stop("Simple");
  gBenchmark->Print("Simple");
  RF2->GetWeights()->Save();
  

}
