//Run with 
//root --hsfit FitHSAsymmetry.C --VectorSDME.cxx
{
  //Create THSRooFit manager 
  THSRooFit* RF=new THSRooFit("AFit");
  RF->SetOutDir("sep18/out_costh_E_oldhas/"); //give output directory
  // ran with haspect directory /home/louise/HASPECT6_jul18/HaSpect/
  //RF->SetBinnedFit();

  ///////////////////////////////Load Variables
  RF->LoadVariable("cosThetaKpHel[-1,1]");//going to fit this
  RF->LoadVariable("phiKpHel[-3.14,3.14]");
  RF->LoadVariable("PHI[-3.14,3.14]");
  RF->LoadVariable("pol[0,1]");
  RF->LoadVariable("polState[Polp=1,Polm=-1]");//Load a category

  RF->SetIDBranchName("fgID");
  
  //RF->LoadBinVars("t",4,0,1.6);
  //Double_t binLimits[] = {0.1, 0.46, 0.58, 0.8, 1.6};
  //Double_t binLimits[] = {0.1, 0.4125, 0.4875, 0.5625, 0.6625, 0.8125, 1.6};
  //RF->LoadBinVars("t",4,binLimits);
  RF->LoadBinVars("cosThetaPhiCm",4,-1.0,1.0);
  //RF->LoadBinVars("Egamma",2,1.5,2.1);
  Double_t ebinLimits[] = {1.5, 1.94, 2.1};
  RF->LoadBinVars("Egamma",2,ebinLimits);

 
  // TChain *chainData=new TChain("T");
  TChain *chainData=new TChain("HSParticles");  
  chainData->Add("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/file_all.root");
  //////////////////////////////Split data into bins and load them
  RF->MakeBinnedTrees(chainData,"Data");
  
  //TDirectory* saveDir=gDirectory;
 // TFile *fileMC=new TFile("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output_MC/file_all.root");
  //TTree *chainMC=(TTree*) fileMC->Get("HSParticles");
  //saveDir->cd();
  TChain *chainMC=new TChain("HSParticles");  
  chainMC->Add("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output_MC10M/file_all.root");
 
  RF->MakeBinnedTrees(chainMC,"SDMEFit");
  
  
  RF->ConfigureSavedBins(RF->GetOutDir());

  
  // Default starting values 
  //RF->Factory("VectorSDME::SDMEFit(cosThetaKpHel,phiKpHel,PHI,pol,polState,rho000[0.1,-0.5,0.5],Rerho010[0.0,-0.5,0.5],rho01m1[0.0,-0.5,0.5],rho111[0.0,-0.5,0.5],rho100[0.0,-0.5,0.5],Rerho110[0.0,-0.5,0.5],rho11m1[0.0,-0.5,0.5],Imrho210[0.0,-0.5,0.5],Imrho21m1[0.0,-0.5,0.5])");
  RF->Factory("SKNewPDF::SDMEFit(cosThetaKpHel,phiKpHel,PHI,pol,polState,rho000[0.1,-0.5,0.5],Rerho010[0.0,-0.5,0.5],rho01m1[0.0,-0.5,0.5],rho111[0.0,-0.5,0.5],rho100[0.0,-0.5,0.5],Rerho110[0.0,-0.5,0.5],rho11m1[0.0,-0.5,0.5],Imrho210[0.0,-0.5,0.5],Imrho21m1[0.0,-0.5,0.5])");
  
  // Feed back in the values I get after fitting with default starting values (1.0 bin)
  //RF->Factory("VectorSDME::SDMEFit(cosThetaKpHel,phiKpHel,PHI,pol,polState,rho000[0.267,-0.5,0.5],Rerho010[0.014,-0.5,0.5],rho01m1[-0.015,-0.5,0.5],rho111[-0.005,-0.5,0.5],rho100[-0.077,-0.5,0.5],Rerho110[0.053,-0.5,0.5],rho11m1[0.0,-0.5,0.5],Imrho210[0.062,-0.5,0.5],Imrho21m1[0.092,-0.5,0.5])");
  
  //RF->GetWorkSpace()->var("rho000")->setConstant();
  //RF->GetWorkSpace()->var("Rerho010")->setConstant();
  //RF->GetWorkSpace()->var("rho01m1")->setConstant();
  //RF->GetWorkSpace()->var("rho111")->setConstant();
  //RF->GetWorkSpace()->var("rho100")->setConstant();
  //RF->GetWorkSpace()->var("Rerho110")->setConstant();
  //RF->GetWorkSpace()->var("rho11m1")->setConstant();
  //RF->GetWorkSpace()->var("Imrho210")->setConstant();
  //RF->GetWorkSpace()->var("Imrho21m1")->setConstant();
  
 
  RooHSEventsPDF* PDF=dynamic_cast<RooHSEventsPDF*>(RF->GetWorkSpace()->pdf("SDMEFit"));
  
  
  cout<<"PDF "<<PDF<<endl;
  RF->LoadSpeciesPDF("SDMEFit");
  RF->TotalPDF();  
  
  // load weights for the simulation (importance sampling to make simulation like data)
  //PDF->LoadWeights("LikeData","/home/louise/g8_phi/ImportanceSampling/impWeights.root","impWeights"); 

  // load weights for the data (signal weights)
  RF->LoadWeights("/home/louise/g8_phi/sWeights/out/Weights_all.root","HSsWeights");
  RF->SetWeightName("Signal");
 
  RF->SetFitMethod(1);
  RF->FitSavedBins(1);
  // ********
  // ********

  // Old:
  //PDF->CheckIntegralParDep(10);
  // RF->SetStudyPDF("SigAsym"); //study this PDF
  // RF->SetNStudyTrials(1);  //Perform 20 trials
  // RF->SetStudyPlot();  //Produce plots for each trial fit

  // gBenchmark->Stop("Binned");
 // gBenchmark->Print("Binned");
}
