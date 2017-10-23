//Make data with
//root 'Model1.C( "Data.root" )'

{

  THSsPlot* RF=new THSsPlot("Simple");
  RF->SetOutDir("out/");
  RF->SetBinnedFit(); //For parameter fits do binned chi2
  ///////////////////////////////Load Variables
  RF->LoadVariable("MissM[-0.003,0.003]");//should be same name as variable in tree  
  RF->LoadAuxVars("Egamma[1.5,4.5]");//should be same name as variable in tree 

  /////////////////////////////Make Model Signal
//  RF->Factory("Gaussian::Signal( MissM, SIMm[0,-0.001,0.001], SIMw[0.0005,5e-5,1e-3] )");
  RF->Factory("CBShape::Signal( MissM, SIMm[0,-0.001,0.001], SIMw[0.0005,5e-5,1e-3], SIMa[0.01,0,2], SIMn[0.1,-1,10] )");
  RF->LoadSpeciesPDF("Signal");


  ////////////////////////////////Additional background
//  RF->Factory("Chebychev::BG(MissM,{a0[-0.1,-1,1],a1[0.1,-1,1]})");
  RF->Factory("Chebychev::BG(MissM,{a0[-0.1,-1,1],a1[0.1,-1,1],a2[0,-1,1]})");
  RF->LoadSpeciesPDF("BG");

   ///////////////////////////Load Data
  TChain chain("HSParticles");
  chain.Add("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/filepKpKm_19.root");
  RF->SetIDBranchName("fgID");//This is not needed if all events from tree go into dataset, i.e. the variable ranges do not cut any events. It is safer to have it...
  //import to RooFit
  RF->LoadDataSet(&chain);
  gBenchmark->Start("Simple");
  RF->FitAndStudy(2); //argument gives number of parameter fits to perform
  //Do some other stuff before saving weights
  RF->DrawTreeVar("MKpKm",100,0.9,2.2);
  RF->DrawTreeVar("MpKm",100,1.4,2.4);
  gBenchmark->Stop("Simple");
  gBenchmark->Print("Simple");
  RF->GetWeights()->Save();

}
