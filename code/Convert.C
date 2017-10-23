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
   //If you are creating a HSParticle tree via fStrParticles then do not create a new tree here, just add the extra brasnches
   fOutTree=new TTree("HSParticles","A tree containing reconstructed particles");

   // Louise
   fOutTree->Branch("Egamma",&eGamma,"Egamma/D");
   fOutTree->Branch("MKpKm",&MKpKm,"MKpKm/D");
   fOutTree->Branch("MpKm",&MpKm,"MpKm/D");
   fOutTree->Branch("MissM2",&MissM,"MissM2/D");
   fOutTree->Branch("MissM",&MissM,"MissM/D");
   fOutTree->Branch("t",&t,"t/D");
   
   fOutTree->Branch("phiPhiCm",&phiPhiCm,"phiPhiCm/D");
   fOutTree->Branch("phiProton",&phiProton,"phiProton/D");
   fOutTree->Branch("phiPhimPhiProton",&phiPhimPhiProton,"phiPhimPhiProton/D");
   fOutTree->Branch("cosThetaPhiCm",&cosThetaPhiCm,"cosThetaPhiCm/D");
   
   fOutTree->Branch("phiKpHel",&phiKpHel,"phiKpHel/D");
   fOutTree->Branch("cosThetaKpHel",&cosThetaKpHel,"cosThetaKpHel/D");
   fOutTree->Branch("phimPHIKpHel",&phimPHIKpHel,"phimPHIKpHel/D");
   fOutTree->Branch("phipPHIKpHel",&phipPHIKpHel,"phipPHIKpHel/D");
   fOutTree->Branch("PHI",&PHI,"PHI/D");
   
   fOutTree->Branch("phiKpGJ",&phiKpGJ,"phiKpGJ/D");
   fOutTree->Branch("cosThetaKpGJ",&cosThetaKpGJ,"cosThetaKpGJ/D");
   fOutTree->Branch("phimPHIKpGJ",&phimPHIKpGJ,"phimPHIKpGJ/D");
   fOutTree->Branch("phipPHIKpGJ",&phipPHIKpGJ,"phipPHIKpGJ/D");
   
   fOutTree->Branch("polState",&polState,"polState/I");
   fOutTree->Branch("pol",&pol,"pol/D");

   lTarget = new TLorentzVector(1.0,1.0,1.0,1.0);
   lTarget->SetXYZM(0,0,0,0.938272046);


   meanPolGPARA=new TH1F("meanPolGPARA","meanPolGPARA",100,1.2,2.2);
   eventsPolGPARA=new TH1F("eventsPolGPARA","eventsPolGPARA",100,1.2,2.2);
   fOutput->Add(meanPolGPARA);
   fOutput->Add(eventsPolGPARA);

    phiPhiCmPARA = new TH1F("phiPhiCmPARA","phiPhiCmPARA", 100, -3.2, 3.2);
    phiPhiCmPERP = new TH1F("phiPhiCmPERP","phiPhiCmPERP", 100, -3.2, 3.2);
    fOutput->Add(phiPhiCmPARA);
    fOutput->Add(phiPhiCmPERP);
    
    cosThetaPhiCmPARA = new TH1F("cosThetaPhiCmPARA","cosThetaPhiCmPARA", 100, -1.0, 1.0);
    cosThetaPhiCmPERP = new TH1F("cosThetaPhiCmPERP","cosThetaPhiCmPERP", 100, -1.0, 1.0);
    fOutput->Add(cosThetaPhiCmPARA);
    fOutput->Add(cosThetaPhiCmPERP);
    
    phiKpHelPARA = new TH1F("phiKpHelPARA","phiKpHelPARA", 100, -3.2, 3.2);
    phiKpHelPERP = new TH1F("phiKpHelPERP","phiKpHelPERP", 100, -3.2, 3.2);
    fOutput->Add(phiKpHelPARA);
    fOutput->Add(phiKpHelPERP);    
    
    cosThetaKpHelPARA = new TH1F("cosThetaKpHelPARA","cosThetaKpHelPARA", 100, -1.0, 1.0);
    cosThetaKpHelPERP = new TH1F("cosThetaKpHelPERP","cosThetaKpHelPERP", 100, -1.0, 1.0);
    fOutput->Add(cosThetaKpHelPARA);
    fOutput->Add(cosThetaKpHelPERP);       

    phiKpGJPARA = new TH1F("phiKpGJPARA","phiKpGJPARA", 100, -3.2, 3.2);
    phiKpGJPERP = new TH1F("phiKpGJPERP","phiKpGJPERP", 100, -3.2, 3.2);
    fOutput->Add(phiKpGJPARA);
    fOutput->Add(phiKpGJPERP);    
    
    cosThetaKpGJPARA = new TH1F("cosThetaKpGJPARA","cosThetaKpGJPARA", 100, -1.0, 1.0);
    cosThetaKpGJPERP = new TH1F("cosThetaKpGJPERP","cosThetaKpGJPERP", 100, -1.0, 1.0);
    fOutput->Add(cosThetaKpGJPARA);
    fOutput->Add(cosThetaKpGJPERP);       

    
    massKpKm = new TH1F("massKpKm","massKpKm", 1000, 0.8, 1.8);
    fOutput->Add(massKpKm);
    masspKm = new TH1F("masspKm","masspKm", 1000, 0.8, 1.8);
    fOutput->Add(masspKm);
    missMass2 = new TH1F("missMass2","missMass2", 1000, 0.0, 1.2);
    fOutput->Add(missMass2);
    missMass = new TH1F("missMass","missMass", 1000, -0.5, 1.2);
    fOutput->Add(missMass);
    mass_pKm_KpKm = new TH2F("mass_pKm_KpKm","mass_pKm_KpKm", 200, 1.2, 2.1, 200, 0.7, 1.8);
    fOutput->Add(mass_pKm_KpKm);
    

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
   MissM2 = MM->M2();
   MissM = MM->M();
   phiProton = lProton->Phi();
   phiPhimPhiProton = TVector2::Phi_0_2pi(lPhi->Phi() - lProton->Phi());
   
   if (courseCut) {
      if ((MpKm>1.5 && MpKm<1.54) || MKpKm<1.0 || MKpKm>1.04) return false;
   }
   
   // cut events with polarisation not in (0,1)
   if (!(pol > 0.0 && pol <= 1.0)) return false;
   
   // fill mass hists
   massKpKm->Fill(MKpKm);
   masspKm->Fill(MpKm);
   missMass2->Fill(MissM2);
   missMass->Fill(MissM);
   mass_pKm_KpKm->Fill(MpKm,MKpKm);

   // 4-momentum transfer
   lt = new TLorentzVector(1.0,1.0,1.0,1.0);
   *lt = beam->P4() - *lPhi;
   t = -lt->M2();

    // Boost to CM 
    TVector3 cmBV = (beam->P4() + *lTarget).BoostVector();
    TLorentzVector lPhiCM = *lPhi;
    lPhiCM.Boost(-cmBV);
    phiPhiCm = lPhiCM.Phi();
    cosThetaPhiCm = lPhiCM.CosTheta();
    
    // Boost to helicity frame = rest frame of phi with z axis opposite to proton, y axis normal to production plane (p_gamma x p_phi(or p_proton? same thing?))
    TVector3 kkBV = (*lPhi).BoostVector();
    TLorentzRotation kkRot(-kkBV);
    TLorentzVector kkBeam = kkRot*(beam->P4());    
    TLorentzVector kkProt = kkRot*(*lProton); 
    TLorentzVector kkKp   = kkRot*(*lKp);
    
    //now calculate GJ angles =angle of Kp in GJ frame, as helicity but with z axis with beam
    TVector3 zGJ = kkBeam.Vect().Unit();  // should this be +kkBeam?
    TVector3 yGJ = kkProt.Vect().Cross(zGJ).Unit();
    TVector3 xGJ = yGJ.Cross(zGJ);
    TVector3 anglesGJ( (kkKp.Vect()).Dot(xGJ), (kkKp.Vect()).Dot(yGJ), (kkKp.Vect()).Dot(zGJ) ); 
    
	phiKpGJ = anglesGJ.Phi();
    cosThetaKpGJ = anglesGJ.CosTheta();
	
	//now calculate helicity angles
	TVector3 zHel = -kkProt.Vect().Unit();
	TVector3 yHel = kkBeam.Vect().Cross(zHel).Unit();
	TVector3 xHel = yHel.Cross(zHel);
 
	TVector3 anglesHel( (kkKp.Vect()).Dot(xHel), (kkKp.Vect()).Dot(yHel), (kkKp.Vect()).Dot(zHel) );

	phiKpHel=anglesHel.Phi();
	cosThetaKpHel=anglesHel.CosTheta();	
	
	// PHI - phi, PHI + phi
	// PHI = angle between polarization plane and production plane
	// phi = Kp phi angle in helicity / GJ system
	PHI = 0.0;
	if (polState==1) {
		PHI = phiPhiCm;
	}
	if (polState==-1) {
		PHI = TVector2::Phi_mpi_pi(phiPhiCm - TMath::Pi()/2);
	}
	phimPHIKpHel = TVector2::Phi_mpi_pi(phiKpHel - PHI);
	phimPHIKpGJ = TVector2::Phi_mpi_pi(phiKpGJ - PHI);
	phipPHIKpHel = TVector2::Phi_mpi_pi(phiKpHel + PHI);
	phipPHIKpGJ = TVector2::Phi_mpi_pi(phiKpGJ + PHI);
	

   if (polState==1 && pol>0.0 && pol<1.0) {
         meanPolGPARA->Fill(eGamma, pol);
         eventsPolGPARA->Fill(eGamma);
         phiPhiCmPARA->Fill(phiPhiCm);
         cosThetaPhiCmPARA->Fill(cosThetaPhiCm);

         phiKpHelPARA->Fill(phiKpHel);
         cosThetaKpHelPARA->Fill(cosThetaKpHel);
         phiKpGJPARA->Fill(phiKpGJ);
         cosThetaKpGJPARA->Fill(cosThetaKpGJ);

   }
   if (polState==-1 && pol>0.0 && pol<1.0) {
         phiPhiCmPERP->Fill(phiPhiCm);
         cosThetaPhiCmPERP->Fill(cosThetaPhiCm);

         phiKpHelPERP->Fill(phiKpHel);
         cosThetaKpHelPERP->Fill(cosThetaKpHel);
         phiKpGJPERP->Fill(phiKpGJ);
         cosThetaKpGJPERP->Fill(cosThetaKpGJ);

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
   cout << "Dividing " << endl;
   meanPolGPARA->Divide(eventsPolGPARA);
   
   THSOutput::HSSlaveTerminate();
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void Convert::Terminate()
		{
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
