#define Convert_cxx
// The class definition in Convert.h has been generated automatically
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
// Root > T->Process("Convert.C")
// Root > T->Process("Convert.C","some options")
// Root > T->Process("Convert.C+")
//

#include "Convert.h"
#include <TH2.h>
#include <TStyle.h>

void Convert::Begin(TTree * /*tree*/)
{
   THSOutput::HSBegin(fInput,fOutput);
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
  
}

void Convert::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   THSOutput::HSSlaveBegin(fInput,fOutput);
  //Initialise the ouput tree
   Int_t buff=32000;
   Int_t split=0;//note split is important in the TSelector framework, if increased branches in subsequent selectors will be data members of the THSParticle object rather than the whole object (this can lead to name conflicts)
   //Only done once. THSOutput::FinishOutput Resets the tree before new files
   //If you want to save additional branches to a new file, do it here
   //If you are creating a HSParticle tree via fStrParticles then do not create a new tree here, just add the extra branches
   fOutTree=new TTree("HSParticles","A tree containing reconstructed particles"); 

   // Louise
   fOutTree->Branch("Egamma",&eGamma,"Egamma/D");
   fOutTree->Branch("MKpKm",&MKpKm,"MKpKm/D");
   fOutTree->Branch("MpKm",&MpKm,"MpKm/D");
   fOutTree->Branch("MissM",&MissM,"MissM/D");
   fOutTree->Branch("t",&t,"t/D");
   fOutTree->Branch("PhiPhi",&PhiPhi,"PhiPhi/D");
   fOutTree->Branch("CosThetaPhi",&CosThetaPhi,"CosThetaPhi/D");
   fOutTree->Branch("polState",&polState,"polState/I");
   fOutTree->Branch("pol",&pol,"pol/D");
   
   lTarget = new TLorentzVector(1.0,1.0,1.0,1.0);
   lTarget->SetXYZM(0,0,0,0.938272046);

  
   meanPolGPARA=new TH1F("meanPolGPARA","meanPolGPARA",100,1.2,2.2);
   eventsPolGPARA=new TH1F("eventsPolGPARA","eventsPolGPARA",100,1.2,2.2);
   fOutput->Add(meanPolGPARA);
   fOutput->Add(eventsPolGPARA);

	phiPhiPARA = new TH1F("phiPhiPARA","phiPhiPARA", 100, -3.2, 3.2);
	phiPhiPERP = new TH1F("phiPhiPERP","phiPhiPERP", 100, -3.2, 3.2);
	fOutput->Add(phiPhiPARA);
	fOutput->Add(phiPhiPERP);
   
  //e.g. Make some objects to make branch with, these should be declared in the .h file 
 //e.g. fp1=new TLorentzVector();
  //e.g.  fOutTree->Branch("p1",&fp1,buff,split);
  //e.g.  fOutTree->Branch("p1",&fp1,buff,split);
  //if you want kinematic bins you must define fHisbins here
   //fHisbins=new TH*()
   if(fHisbins) fOutput->Add(fHisbins);
   //fHisbins->SetXTitle();//give useful axis name
   //fHisbins->SetYTitle();//give useful axis name
   THSHisto::ChangeNames();
   THSHisto::LoadCut("Cut1");
   THSHisto::LoadHistograms();
}

Bool_t Convert::Process(Long64_t entry)
{
   THSOutput::HSProcessStart(entry);
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either Convert::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
   GetEntry(entry); //lazy and slow, you can speed the code up by getting the branches you need to use instead
   //Ready to do some analysis here, before the Fill
   
   // Louise
   MM = new TLorentzVector(1.0,1.0,1.0,1.0);
   lPhi = new TLorentzVector(1.0,1.0,1.0,1.0);
   lProton = new TLorentzVector(1.0,1.0,1.0,1.0);
   lKp = new TLorentzVector(1.0,1.0,1.0,1.0);
   lKm = new TLorentzVector(1.0,1.0,1.0,1.0);
   
   
   if (b_Km==0) {
	  // missing K-
	  *lKm = (*lTarget + beam->P4()) - (proton->P4() + Kp->P4());
	  *lProton = proton->P4();
	  *lKp = Kp->P4();
	  *MM = (*lTarget + beam->P4()) - (proton->P4() + Kp->P4());
   }
   if (b_Kp==0) {
	  // missing K+
      *lKp = (*lTarget + beam->P4()) - (proton->P4() + Km->P4());
      *lProton = proton->P4();
      *lKm = Km->P4();
      *MM = (*lTarget + beam->P4()) - (proton->P4() + Km->P4());
   }
   if (b_proton==0) {
	  // missing proton
	  *lProton = (*lTarget + beam->P4()) - (Kp->P4() + Km->P4());
	  *lKp = Kp->P4();
	  *lKm = Km->P4();
	  *MM = (*lTarget + beam->P4()) - (Kp->P4() + Km->P4());
   }
   if ((b_Km != 0) && (b_Kp != 0) && (b_proton != 0)) {
	  *lProton = proton->P4();
	  *lKp = Kp->P4();
	  *lKm = Km->P4();
      *MM = (*lTarget + beam->P4()) - (proton->P4() + Kp->P4() + Km->P4());
   }

   *lPhi = *lKp + *lKm; //Kp->P4() + Km->P4();

   eGamma = beam->P4().E();
   MKpKm = lPhi->M();
   MpKm = (*lProton + *lKm).M();
   //MpKm = (proton->P4() + Km->P4()).M();
   MissM = MM->M2();

   // 4-momentum transfer
   lt = new TLorentzVector(1.0,1.0,1.0,1.0);
   *lt = beam->P4() - *lPhi;
   t = -lt->M2();  
 
	TVector3 boostV = (beam->P4() + *lTarget).BoostVector();
	TLorentzVector comPhi = *lPhi;
	comPhi.Boost(-boostV);
    PhiPhi = comPhi.Phi();
   
   CosThetaPhi = comPhi.CosTheta();
   
   if (polState==1 && pol>0.0 && pol<1.0) {
	 // << "Filling " << eGamma << " " << pol << " " << endl;
	 meanPolGPARA->Fill(eGamma, pol);
	 eventsPolGPARA->Fill(eGamma);
	 phiPhiPARA->Fill(PhiPhi);
   }
   if (polState==-1 && pol>0.0 && pol<1.0) {
	 phiPhiPERP->Fill(PhiPhi);
   }
   
   Int_t kinBin=0;//default to bins defined
   //Int_t kinBin=GetKinBin();//if fHisbins is defined need to give this meaningful arguments
   //EnterKinBinList(kinBin,entry);//save evente in kinematic bins entry lists
   FillHistograms("Cut1",kinBin);
   THSOutput::HSProcessFill(); 


   return kTRUE;
}

void Convert::SlaveTerminate()
{

   THSOutput::HSSlaveTerminate();
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void Convert::Terminate()
{
   cout << "Dividing " << endl;
   cout << "eventsPolGPARA entries " << eventsPolGPARA->GetEntries();
   meanPolGPARA->Divide(eventsPolGPARA);

   THSOutput::HSTerminate();
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}
void Convert::HistogramList(TString sLabel){
  TDirectory::AddDirectory(kFALSE); //do not add to current directory
  //now define all histograms and add to Output
  //label includes kinematic bin and additional cut name
  // e.g fOutput->Add(MapHist(new TH1F("Mp1"+sLabel,"M_{p1}"+sLabel,100,0,2)));
  //end of histogram list
  TDirectory::AddDirectory(kTRUE); //back to normal
}
void Convert::FillHistograms(TString sCut,Int_t bin){
  fCurrCut=sCut;
  fCurrBin=bin;
  //Get histogram from list
  //Fill histogram
  // e.g. FindHist("Mp1")->Fill(fp1->M());
}
