//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Sep 22 11:56:55 2017 by ROOT version 6.08/06
// from TTree HSParticles/A tree containing reconstructed particles
// found on file: /home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output1/filepKp_all.root
//////////////////////////////////////////////////////////

#ifndef histogrammer_h
#define histogrammer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include "THSOutput.h"
#include "THSWeights.h"
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector


class histogrammer : public TSelector, public THSOutput {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain
   
  //data members for reading weights
   THSWeights* fWeights=0;
   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Double_t> Egamma = {fReader, "Egamma"};
   TTreeReaderValue<Int_t> top = {fReader, "top"};
   TTreeReaderValue<Double_t> MKpKm = {fReader, "MKpKm"};
   TTreeReaderValue<Double_t> MpKm = {fReader, "MpKm"};
   TTreeReaderValue<Double_t> MissM2 = {fReader, "MissM2"};
   TTreeReaderValue<Double_t> MissM = {fReader, "MissM"};
   TTreeReaderValue<Double_t> t = {fReader, "t"};
   
   TTreeReaderValue<Double_t> MpPip = {fReader, "MpPip"};
   TTreeReaderValue<Double_t> MpPim = {fReader, "MpPim"};
   TTreeReaderValue<Double_t> MPiPim = {fReader, "MPipPim"};
   
   TTreeReaderValue<Double_t> phiPhiCm = {fReader, "phiPhiCm"};
   TTreeReaderValue<Double_t> phiProton = {fReader, "phiProton"};
   TTreeReaderValue<Double_t> phiKpCm = {fReader, "phiKpCm"};
   TTreeReaderValue<Double_t> phiKmCm = {fReader, "phiKmCm"};
   TTreeReaderValue<Double_t> phiLamCm = {fReader, "phiLamCm"};
   TTreeReaderValue<Double_t> phiPhimPhiProton = {fReader, "phiPhimPhiProton"};
   TTreeReaderValue<Double_t> cosThetaPhiCm = {fReader, "cosThetaPhiCm"};
   
   TTreeReaderValue<Double_t> phiKpHel = {fReader, "phiKpHel"};
   TTreeReaderValue<Double_t> cosThetaKpHel = {fReader, "cosThetaKpHel"};
   TTreeReaderValue<Double_t> phimPHIKpHel = {fReader, "phimPHIKpHel"};
   TTreeReaderValue<Double_t> phipPHIKpHel = {fReader, "phipPHIKpHel"};
   TTreeReaderValue<Double_t> PHI = {fReader, "PHI"};
   
   TTreeReaderValue<Double_t> phiKpGJ = {fReader, "phiKpGJ"};
   TTreeReaderValue<Double_t> cosThetaKpGJ = {fReader, "cosThetaKpGJ"};
   TTreeReaderValue<Double_t> phimPHIKpGJ = {fReader, "phimPHIKpGJ"};
   TTreeReaderValue<Double_t> phipPHIKpGJ = {fReader, "phipPHIKpGJ"};
   
   TTreeReaderValue<Int_t> polState = {fReader, "polState"};
   TTreeReaderValue<Double_t> pol = {fReader, "pol"};
   TTreeReaderValue<Double_t> fgID = {fReader, "fgID"};


   histogrammer(TTree * /*tree*/ =0) { }
   virtual ~histogrammer() {SafeDelete(fWeights);  }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();
//Add THSHisto functions
   virtual void HistogramList(TString sLabel);
   virtual void FillHistograms(TString sCut);

   ClassDef(histogrammer,0);

};

#endif

#ifdef histogrammer_cxx
void histogrammer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t histogrammer::Notify()
{
   fChain=fReader.GetTree();
   THSOutput::HSNotify(fChain);
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef histogrammer_cxx
