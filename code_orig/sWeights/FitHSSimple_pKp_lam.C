//Make data with
//root 'Model1.C( "Data.root" )'

{

  THSsPlot* RF=new THSsPlot("Simple");
  RF->SetOutDir("out/");
 // RF->SetBinnedFit(); //For parameter fits do binned chi2
  ///////////////////////////////Load Variables
  RF->LoadVariable("MpKm[1.48,1.56]");//should be same name as variable in tree  
  //RF->LoadBinVars("Egamma",1,1.9,2.1);//should be same name as variable in tree 

  /////////////////////////////Make Model Signal
  //RF->Factory("Gaussian::Signal( MissM, SIMm[0.24,0.22,0.26], SIMw[0.01,0.001,0.02] )");
  RF->Factory("Gaussian::Signal( MpKm, SIMm[1.52,1.42,1.62], SIMw[0.01,0.001,0.02] )");
  RF->LoadSpeciesPDF("Signal");


  ////////////////////////////////Additional background
  RF->Factory("Chebychev::BG(MpKm,{a0[-0.1,-10,10],a1[0.1,-10,10],a2[0,-10,10]})");
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
  RF->DrawTreeVar("MissM",100,0.0,1.0);
  RF->DrawTreeVar("MKpKm",100,0.8,1.4);
  gBenchmark->Stop("Simple");
  gBenchmark->Print("Simple");
  RF->GetWeights()->Save();

}
