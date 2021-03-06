#define histogrammer_cxx
// The class definition in histogrammer.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("histogrammer.C")
// root> T->Process("histogrammer.C","some options")
// root> T->Process("histogrammer.C+")
//


#include "histogrammer.h"
#include <TH2.h>
#include <TStyle.h>

void histogrammer::Begin(TTree * /*tree*/)
{
   THSOutput::HSBegin(fInput,fOutput);
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
//If you want to split histograms into different kinematic bins, include and configure the lines below
   //fBins=new THSBins("HSBins");
   //fBins->AddAxis("tbins",5,0.0,1.3);
   //fBins->AddAxis("Ebins",3,1.5,2.1);
   if(!fInput) fInput=new TList();
   //fInput->Add(fBins);
}

void histogrammer::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   THSOutput::HSSlaveBegin(fInput,fOutput);
   
   //Get weights from file
   THSHisto::LoadWeights("/home/louise/g8_phi/sWeights/out/Weights_all.root","HSsWeights");
   //THSHisto::LoadWeights("/home/louise/g8_phi/sWeights/out/WeightsKpKm_all.root","HSsWeights");
   //THSHisto::LoadWeights("/home/louise/g8_phi/sWeights/out/WeightspKm_all.root","HSsWeights");
   //THSHisto::LoadWeights("/home/louise/g8_phi/sWeights/out/WeightspKp_all.root","HSsWeights");
   //THSHisto::LoadWeights("/home/louise/g8_phi/sWeights/out/WeightspKpKm_all.root","HSsWeights");
   
  //Initialise and configure histogramming
   THSHisto::InitialiseHisto(fInput);
   //Load histograms for each species that has a weight
   THSHisto::LoadCutsForWeights(); //Loop over weights species and make weighted histograms for each
   THSHisto::LoadCut("All");
   THSHisto::LoadHistograms();

}

Bool_t histogrammer::Process(Long64_t entry)
{
   THSOutput::HSProcessStart(entry);
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetLocalEntry(entry);
 //below you can give vars corresponding to fBins axis
   fWeight=1; //For unweighted histograms
   //if(fBins) fCurrBin=fBins->FindBin(var1,var2,...);//if fBins is defined need to give this meaningful arguments
   if(fBins) fCurrBin=fBins->FindBin(*t, *Egamma);
   FillHistograms("All");
   
   THSHisto::GetWeightEvent(*fgID); //get weights for this event, use fgID as works when permutating also
   THSHisto::FillCutsForWeights(); //Fill weighted hists
   
//	fWeight=fWeights->GetWeight("Signal");
//	if (PARA) FillHistograms("Para");
   
   THSOutput::HSProcessFill();
   return kTRUE;
}

void histogrammer::SlaveTerminate()
{
   THSOutput::HSSlaveTerminate();
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void histogrammer::Terminate()
{
   THSOutput::HSTerminate();
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}
 
void histogrammer::HistogramList(TString sLabel){
  TDirectory::AddDirectory(kFALSE); //do not add to current directory
  
    //now define all histograms and add to Output

	fOutput->Add(MapHist(new TH1F("massKpKm","massKpKm", 1000, 0.8, 1.8)));
	fOutput->Add(MapHist(new TH1F("masspKm","masspKm", 1000, 0.8, 1.8)));
	fOutput->Add(MapHist(new TH1F("missMass2","missMass2", 1000, -0.3, 1.2)));
	fOutput->Add(MapHist(new TH1F("missMass","missMass", 1000, -0.5, 1.2)));
	fOutput->Add(MapHist(new TH2F("mass_pKm_KpKm","mass_pKm_KpKm", 200, 1.2, 2.1, 200, 0.7, 1.8)));
	fOutput->Add(MapHist(new TH1F("t","t", 100, 0.0, 2.5)));

	fOutput->Add(MapHist(new TH1F("phiPhiCmPARA","phiPhiCmPARA", 100, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phiPhiCmPERP","phiPhiCmPERP", 100, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phiProtonPARA","phiProtonPARA", 100, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phiProtonPERP","phiProtonPERP", 100, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phiPhimPhiProton","phiPhimPhiProton", 100, 1.6, 4.8)));

	fOutput->Add(MapHist(new TH1F("cosThetaPhiCmPARA","cosThetaPhiCmPARA", 100, -1.0, 1.0)));
	fOutput->Add(MapHist(new TH1F("cosThetaPhiCmPERP","cosThetaPhiCmPERP", 100, -1.0, 1.0)));

	fOutput->Add(MapHist(new TH1F("phiKpHelPARA","phiKpHelPARA", 100, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phiKpHelPERP","phiKpHelPERP", 100, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("cosThetaKpHelPARA","cosThetaKpHelPARA", 100, -1.0, 1.0)));
	fOutput->Add(MapHist(new TH1F("cosThetaKpHelPERP","cosThetaKpHelPERP", 100, -1.0, 1.0)));

	fOutput->Add(MapHist(new TH1F("phiKpGJPARA","phiKpGJPARA", 100, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phiKpGJPERP","phiKpGJPERP", 100, -3.2, 3.2)));  
	fOutput->Add(MapHist(new TH1F("cosThetaKpGJPARA","cosThetaKpGJPARA", 100, -1.0, 1.0)));
	fOutput->Add(MapHist(new TH1F("cosThetaKpGJPERP","cosThetaKpGJPERP", 100, -1.0, 1.0)));  

	fOutput->Add(MapHist(new TH1F("phimPHIKpHelPARA","phimPHIKpHelPARA", 200, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phimPHIKpHelPERP","phimPHIKpHelPERP", 200, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phipPHIKpHelPARA","phipPHIKpHelPARA", 200, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phipPHIKpHelPERP","phipPHIKpHelPERP", 200, -3.2, 3.2)));

	fOutput->Add(MapHist(new TH2F("phiVPHIKpHelPARA","phiVPHIKpHelPARA", 100, -3.2, 3.2, 100, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH2F("phiVPHIKpHelPERP","phiVPHIKpHelPERP", 100, -3.2, 3.2, 100, -3.2, 3.2)));

	fOutput->Add(MapHist(new TH1F("phimPHIKpGJPARA","phimPHIKpGJPARA", 200, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phimPHIKpGJPERP","phimPHIKpGJPERP", 200, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phipPHIKpGJPARA","phipPHIKpGJPARA", 200, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH1F("phipPHIKpGJPERP","phipPHIKpGJPERP", 200, -3.2, 3.2)));

	fOutput->Add(MapHist(new TH2F("phiVPHIKpGJPARA","phiVPHIKpGJPARA", 100, -3.2, 3.2, 100, -3.2, 3.2)));
	fOutput->Add(MapHist(new TH2F("phiVPHIKpGJPERP","phiVPHIKpGJPERP", 100, -3.2, 3.2, 100, -3.2, 3.2)));

	// Phi mass vs Angular distributions to check correlation
	fOutput->Add(MapHist(new TH2F("MPhiVsPhiKpHelPARA","MPhiVsPhiKpHelPARA", 100, -3.2, 3.2, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsPhiKpHelPERP","MPhiVsPhiKpHelPERP", 100, -3.2, 3.2, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsCosThetaKpHelPARA","MPhiVsCosThetaKpHelPARA", 100, -1.0, 1.0, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsCosThetaKpHelPERP","MPhiVsCosThetaKpHelPERP", 100, -1.0, 1.0, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsPhiKpGJPARA","MPhiVsPhiKpGJPARA", 100, -3.2, 3.2, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsPhiKpGJPERP","MPhiVsPhiKpGJPERP", 100, -3.2, 3.2, 100, 0.98, 1.06)));  
	fOutput->Add(MapHist(new TH2F("MPhiVsCosThetaKpGJPARA","MPhiVsCosThetaKpGJPARA", 100, -1.0, 1.0, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsCosThetaKpGJPERP","MPhiVsCosThetaKpGJPERP", 100, -1.0, 1.0, 100, 0.98, 1.06)));  
	fOutput->Add(MapHist(new TH2F("MPhiVsPhimPHIKpHelPARA","MPhiVsPhimPHIKpHelPARA", 200, -3.2, 3.2, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsPhimPHIKpHelPERP","MPhiVsPhimPHIKpHelPERP", 200, -3.2, 3.2, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsPhipPHIKpHelPARA","MPhiVsPhipPHIKpHelPARA", 200, -3.2, 3.2, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsPhipPHIKpHelPERP","MPhiVsPhipPHIKpHelPERP", 200, -3.2, 3.2, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsPhimPHIKpGJPARA","MPhiVsPhimPHIKpGJPARA", 200, -3.2, 3.2, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsPhimPHIKpGJPERP","MPhiVsPhimPHIKpGJPERP", 200, -3.2, 3.2, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsPhipPHIKpGJPARA","MPhiVsPhipPHIKpGJPARA", 200, -3.2, 3.2, 100, 0.98, 1.06)));
	fOutput->Add(MapHist(new TH2F("MPhiVsPhipPHIKpGJPERP","MPhiVsPhipPHIKpGJPERP", 200, -3.2, 3.2, 100, 0.98, 1.06)));


  //label includes kinematic bin and additional cut name
  // e.g fOutput->Add(MapHist(new TH1F("Mp1","M_{p1}",100,0,2)));
  //end of histogram list
  TDirectory::AddDirectory(kTRUE); //back to normal
}
void histogrammer::FillHistograms(TString sCut){ 
  if(fBins&&fCurrBin==-1) return; //Outwith bin limits so no hists
  fCurrCut=sCut;
  //Get histogram from list
  //Fill histogram
  FindHist("massKpKm")->Fill(*MKpKm,fWeight);
  FindHist("masspKm")->Fill(*MpKm,fWeight);
  FindHist("missMass2")->Fill(*MissM2,fWeight);
  FindHist("missMass")->Fill(*MissM,fWeight);
  ((TH2F*)FindHist("mass_pKm_KpKm"))->Fill(*MpKm, *MKpKm, fWeight);
  FindHist("t")->Fill(*t,fWeight);
  FindHist("phiPhimPhiProton")->Fill(*phiPhimPhiProton,fWeight);
  
  if (*polState==1) {
	  FindHist("phiPhiCmPARA")->Fill(*phiPhiCm,fWeight);
	  FindHist("phiProtonPARA")->Fill(*phiProton,fWeight);
	  FindHist("cosThetaPhiCmPARA")->Fill(*cosThetaPhiCm,fWeight);
	  
	  FindHist("phiKpHelPARA")->Fill(*phiKpHel,fWeight);
	  FindHist("cosThetaKpHelPARA")->Fill(*cosThetaKpHel,fWeight);
	  
	  FindHist("phiKpGJPARA")->Fill(*phiKpGJ,fWeight);
	  FindHist("cosThetaKpGJPARA")->Fill(*cosThetaKpGJ,fWeight);
	  
	  FindHist("phimPHIKpHelPARA")->Fill(*phimPHIKpHel,fWeight);
	  FindHist("phipPHIKpHelPARA")->Fill(*phipPHIKpHel,fWeight);
	  ((TH2F*)FindHist("phiVPHIKpHelPARA"))->Fill(*PHI, *phiKpHel, fWeight);

	  FindHist("phimPHIKpGJPARA")->Fill(*phimPHIKpGJ,fWeight);
	  FindHist("phipPHIKpGJPARA")->Fill(*phipPHIKpGJ,fWeight);
	  ((TH2F*)FindHist("phiVPHIKpGJPARA"))->Fill(*PHI, *phiKpGJ, fWeight);
	  
	  // Phi mass vs Angular distributions to check correlation
	  ((TH2F*)FindHist("MPhiVsPhiKpHelPARA"))->Fill(*phiKpHel, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsCosThetaKpHelPARA"))->Fill(*cosThetaKpHel, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsPhiKpGJPARA"))->Fill(*phiKpGJ, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsCosThetaKpGJPARA"))->Fill(*cosThetaKpGJ, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsPhimPHIKpHelPARA"))->Fill(*phimPHIKpHel, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsPhipPHIKpHelPARA"))->Fill(*phipPHIKpHel, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsPhimPHIKpGJPARA"))->Fill(*phimPHIKpGJ, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsPhipPHIKpGJPARA"))->Fill(*phipPHIKpGJ, *MKpKm, fWeight);  
	  
  }
  
  if (*polState==-1) {
	  FindHist("phiPhiCmPERP")->Fill(*phiPhiCm,fWeight);
	  FindHist("phiProtonPERP")->Fill(*phiProton,fWeight);
	  FindHist("cosThetaPhiCmPERP")->Fill(*cosThetaPhiCm,fWeight);
	  
	  FindHist("phiKpHelPERP")->Fill(*phiKpHel,fWeight);
	  FindHist("cosThetaKpHelPERP")->Fill(*cosThetaKpHel,fWeight);
	  
	  FindHist("phiKpGJPERP")->Fill(*phiKpGJ,fWeight);
	  FindHist("cosThetaKpGJPERP")->Fill(*cosThetaKpGJ,fWeight);
	  
	  FindHist("phimPHIKpHelPERP")->Fill(*phimPHIKpHel,fWeight);
	  FindHist("phipPHIKpHelPERP")->Fill(*phipPHIKpHel,fWeight);
	  ((TH2F*)FindHist("phiVPHIKpHelPERP"))->Fill(*PHI, *phiKpHel, fWeight);

	  FindHist("phimPHIKpGJPERP")->Fill(*phimPHIKpGJ,fWeight);
	  FindHist("phipPHIKpGJPERP")->Fill(*phipPHIKpGJ,fWeight);
	  ((TH2F*)FindHist("phiVPHIKpGJPERP"))->Fill(*PHI, *phiKpGJ, fWeight);
	  
	  // Phi mass vs Angular distributions to check correlation
	  ((TH2F*)FindHist("MPhiVsPhiKpHelPERP"))->Fill(*phiKpHel, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsCosThetaKpHelPERP"))->Fill(*cosThetaKpHel, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsPhiKpGJPERP"))->Fill(*phiKpGJ, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsCosThetaKpGJPERP"))->Fill(*cosThetaKpGJ, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsPhimPHIKpHelPERP"))->Fill(*phimPHIKpHel, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsPhipPHIKpHelPERP"))->Fill(*phipPHIKpHel, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsPhimPHIKpGJPERP"))->Fill(*phimPHIKpGJ, *MKpKm, fWeight);
	  ((TH2F*)FindHist("MPhiVsPhipPHIKpGJPERP"))->Fill(*phipPHIKpGJ, *MKpKm, fWeight);  
	  
  }  
  
  //Find and fill weighted hist below here
  //e.g. FindHist("Mp1")->Fill(fp1->M(),fWeight);
  //e.g. ((TH2F*)FindHist("Mp1VMmiss"))->Fill(fp1->M(),Mmiss,fWeight);
}
