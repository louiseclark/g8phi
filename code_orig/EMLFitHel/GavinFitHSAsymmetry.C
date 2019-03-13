//Run with 
//root --hsfit FitHSAsymmetry.C --SKNewPDF.cxx
{
  //Create THSRooFit manager 
  THSRooFit* RF=new THSRooFit("AFit");
  RF->SetOutDir("out_69/"); //give output directory
  RF->SetBinnedFit();

  ///////////////////////////////Load Variables
  // RF->LoadVariable("Phi[-180,180]");//going to fit this
  // RF->LoadVariable("Pol[0,1]");
  // RF->LoadVariable("PolState[Polp=1,Polm=-1]");//Load a category

  //GM
  // RF->LoadVariable("fAna_phi[-3.141,3.141]");//going to fit this
  // RF->LoadVariable("fAna_beamPolar[0,100]");
  // RF->LoadVariable("fAna_targetPolar[0,100]");
  // RF->LoadVariable("fAna_beamPolarDir[beamPolarDirP=1,beamPolarDirM=-1]");//Load a category
  // RF->LoadVariable("fAna_targetPolarDir[targetPolarDirP=1,targetPolarDirM=-1]");
  // RF->LoadVariable("fAna_thetaQ[0,1]");
  // RF->LoadVariable("fAna_epsilon[0,1]");
  RF->LoadVariable("fAna_phi[]");//going to fit this
  RF->LoadVariable("fAna_beamPolar[]");
  RF->LoadVariable("fAna_targetPolar[]");
  RF->LoadVariable("fAna_beamPolarDir[beamPolarDirP=1,beamPolarDirM=-1]");//Load a category
  RF->LoadVariable("fAna_targetPolarDir[targetPolarDirP=1,targetPolarDirM=-1]");
  RF->LoadVariable("fAna_thetaQ[]");
  RF->LoadVariable("fAna_epsilon[]");  

  RF->SetIDBranchName("fuID");

  // RF->GetWorkSpace()->var("Phi")->setBins(36);
  // RF->GetWorkSpace()->var("Pol")->setBins(5);

  //GM
  RF->GetWorkSpace()->var("fAna_phi")->setBins(36);
  RF->LoadBinVars("fSpectatorMM",1,0.8,1.1);

  // RF->GetWorkSpace()->var("fAna_beamPolar")->setBins(10);  

  /////////////////////////////Make asymmetry model A and B are fit parameters
  // RF->Factory("SKNewPDF::SigAsym( Phi,Pol,PolState, A[0.1,-1,1],B[0.6,-1,1] )");
  // RooHSEventsPDF* PDF=dynamic_cast<RooHSEventsPDF*>(RF->GetWorkSpace()->pdf("SigAsym"));
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
  RF->Factory("SKNewPDF::SigAsym( fAna_phi,fAna_epsilon,fAna_thetaQ,fAna_beamPolar,fAna_targetPolar,fAna_beamPolarDir,fAna_targetPolarDir, A[1,-1,1],B[0.0,-1,1],C[0.011,-1,1],D[-1.4372,-2,2],E[0.0,-1,1],F[0.0,-1,1],G[0.0,-1,1],H[0.0,-1,1],I[0.0,-1,1],J[0.0,-1,1],K[0.0,-1,1],L[0.0,-1,1],M[0.0,-1,1],N[0.0,-1,1],O[0.0,-1,1],P[0.0,-1,1],Q[0.0,-1,1],R[0.0,-1,1] )");

  // sigma_0
  // A -> D
  RF->GetWorkSpace()->var("A")->setConstant();
  RF->GetWorkSpace()->var("B")->setConstant();
  RF->GetWorkSpace()->var("C")->setConstant();
  RF->GetWorkSpace()->var("D")->setConstant();

  // A_LU = sigma_LU
  // R
  // RF->GetWorkSpace()->var("R")->setConstant();

  // A_UL = sigma_z / sigma_0
  // E -> L
  RF->GetWorkSpace()->var("E")->setConstant();
  RF->GetWorkSpace()->var("F")->setConstant();
  RF->GetWorkSpace()->var("G")->setConstant();
  RF->GetWorkSpace()->var("H")->setConstant();
  RF->GetWorkSpace()->var("I")->setConstant();
  RF->GetWorkSpace()->var("J")->setConstant();
  RF->GetWorkSpace()->var("K")->setConstant();
  RF->GetWorkSpace()->var("L")->setConstant();  

  // A_LL = (-1)*sigma_ez / sigma_0
  // M, N, O, P, Q
  RF->GetWorkSpace()->var("M")->setConstant();
  RF->GetWorkSpace()->var("N")->setConstant();
  RF->GetWorkSpace()->var("O")->setConstant();
  RF->GetWorkSpace()->var("P")->setConstant();
  RF->GetWorkSpace()->var("Q")->setConstant();

 
  RooHSEventsPDF* PDF=dynamic_cast<RooHSEventsPDF*>(RF->GetWorkSpace()->pdf("SigAsym"));
  // TChain *chainMC=new TChain("T");
  // chainMC->Add("/w/work14/gavin/analysis/eg1dvcs/RootOutputFiles/pass1_v5_pi_minus/RootOutputGMSelector_Strict1_NH3_AnaVarsInTree_9576Bfiles.root");
  // cout<<"PDF "<<PDF<<endl;
  // PDF->SetEvTree(chainMC);
  RF->LoadSpeciesPDF("SigAsym");
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
  TChain *chainData=new TChain("BinnedTree");  
  //chainData->Add("/home/gavin/Documents/analysis/sPlots/HASPECT6/RooFitExamples/testProtoData/outBins_GM/fAna_theta0.96_/TreeData.root");
  chainData->Add("/scratch/TreeData.root");
  RF->LoadDataSet(chainData);
  gBenchmark->Start("Binned");
  // PDF->AddProtoData(RF->GetDataSet());
 //Load weights from Spectator Mmiss fit
  //RF->LoadWeights("/home/gavin/Documents/analysis/sPlots/HASPECT6/RooFitExamples/testSimple/out_GM_ND3_AnaVarsInTree/WeightsSimple.root","HSsWeights");
  RF->LoadWeights("/scratch/WeightsSimple.root","HSsWeights");
  RF->SetWeightName("Signal");
  RF->SetDataWeight();

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
