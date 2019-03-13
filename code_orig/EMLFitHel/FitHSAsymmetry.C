//Run with 
//root --hsfit FitHSAsymmetry.C --SKNewPDF.cxx
{
  //Create THSRooFit manager 
  THSRooFit* RF=new THSRooFit("AFit");
  RF->SetOutDir("outMCMC/"); //give output directory
  //RF->SetBinnedFit();

  ///////////////////////////////Load Variables
  RF->LoadVariable("cosThetaKpHel[-1,1]");//going to fit this
  RF->LoadVariable("phiKpHel[-3.14,3.14]");
  RF->LoadVariable("PHI[-3.14,3.14]");
  RF->LoadVariable("pol[0,1]");
  RF->LoadVariable("polState[Polp=1,Polm=-1]");//Load a category

  RF->SetIDBranchName("fgID");

  // RF->GetWorkSpace()->var("Phi")->setBins(36);
  // RF->GetWorkSpace()->var("Pol")->setBins(5);

  //GM
  //RF->GetWorkSpace()->var("fAna_phi")->setBins(36);
  //RF->LoadBinVars("fSpectatorMM",1,0.8,1.1);

  // RF->GetWorkSpace()->var("fAna_beamPolar")->setBins(10);  

  /////////////////////////////Make asymmetry model A and B are fit parameters
  // RF->Factory("SKNewPDF::SigAsym( Phi,Pol,PolState, A[0.1,-1,1],B[0.6,-1,1] )");
  
  // TChain *chainMC=new TChain("T");
  // chainMC->Add("SimASymRes10.root");
  // cout<<"PDF "<<PDF<<endl;
  // // PDF->SetEvTree(chainMC);
  // RF->LoadSpeciesPDF("SigAsym");
  // //  PDF->SetNInt(1E4);
  // //PDF->CheckIntegralParDep(4);
  // //  PDF->SetConstInt();
  // RF->TotalPDF();

  //GM
  // Orig:
  // RF->Factory("SKNewPDF::SigAsym( fAna_phi,fAna_epsilon,fAna_thetaQ,fAna_beamPolar,fAna_targetPolar,fAna_beamPolarDir,fAna_targetPolarDir, A[1,-1,1],B[0.0,-1,1],C[0.0,-1,1],D[0.0,-2,2],E[0.0,-1,1],F[0.0,-1,1],G[0.0,-1,1],H[0.0,-1,1],I[0.0,-1,1],J[0.0,-1,1],K[0.0,-1,1],L[0.0,-1,1],M[0.0,-1,1],N[0.0,-1,1],O[0.0,-1,1],P[0.0,-1,1],Q[0.0,-1,1],R[0.0,-1,1] )");
  // Editted for setting constants:
  RF->Factory("SKNewPDF::SDMEFit(cosThetaKpHel,phiKpHel,PHI,pol,polState,rho000[0.2,-0.5,0.5],Rerho010[0.0,-0.5,0.5],rho01m1[0.0,-0.5,0.5],rho111[0.0,-0.5,0.5],rho100[0.0,-0.5,0.5],Rerho110[0.0,-0.5,0.5],rho11m1[0.0,-0.5,0.5],Imrho210[0.0,-0.5,0.5],Imrho21m1[0.0,-0.5,0.5])");
  // sigma_0
  // A -> D
  //RF->GetWorkSpace()->var("A")->setConstant();
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
  //TChain *chainMC=new TChain("HSParticles");
  //chainMC->Add("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output_MC/file_all.root");
  
  
  TDirectory* saveDir=gDirectory;
  TFile *fileMC=new TFile("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output_MC/file_all.root");
  TTree *chainMC=(TTree*) fileMC->Get("HSParticles");
  saveDir->cd();
  
  
  cout<<"PDF "<<PDF<<endl;
  PDF->SetEvTree(chainMC,RF->GetCut());
  RF->LoadSpeciesPDF("SDMEFit");
  //  PDF->SetNInt(1E4);
  //PDF->CheckIntegralParDep(4);
  //  PDF->SetConstInt();
  RF->TotalPDF();  

  // TChain *chainData=new TChain("T");
  // chainData->Add("DataASymRes10PS02.root");
  // RF->LoadDataSet(chainData);
  // gBenchmark->Start("Binned");
  // PDF->AddProtoData(RF->GetDataSet());
  // //PDF->CheckIntegralParDep(10);
  // // RF->SetStudyPDF("SigAsym"); //study this PDF
  // // RF->SetNStudyTrials(1);  //Perform 20 trials
  // // RF->SetStudyPlot();  //Produce plots for each trial fit
  // RF->FitAndStudy(1);
  // // gBenchmark->Stop("Binned");
  // // gBenchmark->Print("Binned");


  // ********
  // ********  
 //  // Using configured bins:
 //  RF->ConfigureSavedBins("outBins_GM/");

 // //Load weights from Spectator Mmiss fit
 //  RF->LoadWeights("/home/gavin/Documents/analysis/sPlots/HASPECT6/RooFitExamples/testSimple/out_GM_ND3_AnaVarsInTree/WeightsSimple.root","HSsWeights");
 //  RF->SetWeightName("Signal");
 //  //RF->SetDataWeight();  

 //  gBenchmark->Start("Binned");
 //  RF->FitSavedBins(1);
 //  // RF->FitAndStudy(1);    
 //  gBenchmark->Stop("Binned");
 //  gBenchmark->Print("Binned");

  // RF->FitAndStudy(1);  
  // ********  
  // ********  



  // ********
  // ********
  //GM
  // TChain *chainData=new TChain("T");
  TChain *chainData=new TChain("HSParticles");  
  //chainData->Add("/home/gavin/Documents/analysis/sPlots/HASPECT6/RooFitExamples/testProtoData/outBins_GM/fAna_theta0.96_/TreeData.root");
  chainData->Add("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/file_all.root");
  RF->LoadDataSet(chainData);
  gBenchmark->Start("Binned");
  PDF->AddProtoData(RF->GetDataSet());
  //PDF->CheckIntegralParDep(2);
  
  //**RF->SetStudyPDF("SDMEFit"); //study this PDF
  //**RF->SetNStudyTrials(2);  //Perform 20 trials
  //**RF->SetStudyPlot(); //Produce plots for each trial fit
 //Load weights from Spectator Mmiss fit
  //RF->LoadWeights("/home/gavin/Documents/analysis/sPlots/HASPECT6/RooFitExamples/testSimple/out_GM_ND3_AnaVarsInTree/WeightsSimple.root","HSsWeights");
  RF->LoadWeights("/home/louise/g8_phi/sWeights/out/Weights_all.root","HSsWeights");
  RF->SetWeightName("Signal");
  RF->SetDataWeight();

  RF->SetFitMethod(2);
  RF->FitAndStudy(1);
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
