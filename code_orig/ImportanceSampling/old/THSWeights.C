/**
	\class THSWeights
	
	Class to control weight structures in HaSpect.
	Consists of 1 tree with event IDs
	And 1 tree with the weights of the different species
	It relies on being able to read the trees into memory
	to sort the ordering, this may cause issues with order >10^7
	events
	
	
*/

#include "THSWeights.h"
#include <TTreeIndex.h>
#include <TSystem.h>
#include <TFile.h>
#include <TMath.h>
#include <TCut.h>
#include <TEntryList.h>
ClassImp(THSWeights);


THSWeights::THSWeights(TString name) :TNamed(name,name){
  fWTree=new TTree(name+"_W","Tree weights for each species");
  fWTree->SetDirectory(0);
  fIDTree=new TTree(name+"_ID","event ids for each entry");
  fIDTree->SetDirectory(0);
  fIDTree->Branch("WID",&fID,"WID/L");
  fCurrEntry=0;
  fIsSorted=kFALSE;
  fN=0;
  fSpecies.clear();
}

THSWeights::~THSWeights(){
  if(fWeightList) delete fWeightList;
  if(fWTree) delete fWTree;
  if(fIDTree) delete fIDTree; 
  if(fFile) {delete fFile;}
}

void THSWeights::SetSpecies(TString name){
  UInt_t NSpecies=fSpecies.size(); 
  fSpecies.insert(pair<TString,Int_t>(name,NSpecies)); //save name in map
  fWVals.ResizeTo(NSpecies+1); //create entry in value array
  //make branch with name and connect to new fWVals entry
  fWVals[NSpecies]=0;
  cout<<"Insert species "<<name <<" "<<NSpecies<<" "<<fWVals[NSpecies]<<endl;
  fWTree->Branch(name,&fWVals[NSpecies],name+TString("/D")); 
}

//////////////////////////////////////////////////////////////
///Move down the tree until you find an event with correct ID.
///This requires the trees are sorted in ID
///and fWTree is a subset of whatever tree requires the weight.
///Fastest if both trees have same events
Bool_t THSWeights::GetEntryFast(Long64_t id){

  if(!fIDv) BuildIndex();
  if(id!=fIDv[fCurrEntry]){//no weight for this id
    return fGotEntry=kFALSE;}
  fWTree->GetEntry(fCurrEntry++); //get entry now we know it exists 
  return fGotEntry=kTRUE;
}

////////////////////////////////////////////////////////////
///for unsorted tree \n
///Move down the tree until find event with correct ID.
///This is faster if trees are sorted in ID and ReStart not required
///(in which case use GetEntryFast)
///and fWTree is a subset of whatever tree requires the weight.
///Fastest if both trees have same events
Bool_t THSWeights::GetEntrySlow(Long64_t id){
  if(!fIDv) BuildIndex();
  Bool_t ReStart=0;
  if(id!=fIDv[fCurrEntry++]){//no weight for this id
    //    fIDTree->GetEntry(fCurrEntry++);
    if(fCurrEntry==fWTree->GetEntries()&&ReStart==0){fCurrEntry=0; ReStart=1;}
    else if(fCurrEntry==fWTree->GetEntries()){
      cout<<"THSWeights::GetEntry entry not found "<<id<<endl; 
      return fGotEntry=kFALSE;}
  }
  fCurrEntry--;
  fWTree->GetEntry(fCurrEntry++); //get entry now we know it exists 
  return fGotEntry=kTRUE;
}

/////////////////////////////////////////////////////////////
///Use a binary search to find the entry for an unsorted tree
Bool_t THSWeights::GetEntryBinarySearch(Long64_t id){

  if(!fIDv) BuildIndex();
  Long64_t entry=TMath::BinarySearch(fN,fIDv,id);
  if(fIDv[entry]!=id) return fGotEntry=kFALSE;
  fWTree->GetEntry(fIDi[entry]);//entry should be OK if these arrays are be ordered...
  return fGotEntry=kTRUE;
}

//////////////////////////////////////////////////
///Function to merge weights from many root files
Long64_t THSWeights::Merge(TString tempName,TString outName,TString wmName){

  if(outName!=TString("")) {
    //TFile* outFile=new TFile(outName,"recreate");
    SetFile(outName);
  }
  TString dirName=gSystem->DirName(tempName);
  TString prefix=gSystem->BaseName(tempName); //anything after directory in tempname
  if(prefix==TString("")) prefix="Weights";
  void *dir=gSystem->OpenDirectory(dirName);
  if(!dir) cout<<"THSWeights::Merge No directory found : "<<dirName<<endl;
  else cout<<"THSWeights::Merge Merging "<<prefix <<"* in directory "<<dirName<<endl;
  TString fileName;
  while( (fileName=(gSystem->GetDirEntry(dir)))){
    if(fileName==TString("")) break;
    if(fileName==TString("."))continue;
    if(fileName==TString(".."))continue;
    if(!fileName.Contains(prefix))continue;
    if(!fileName.Contains(".root"))continue;
    cout<<"THSWeights::Merge Adding file "<<fileName<<endl;
    //TFile* wfile=new TFile(dirName+"/"+fileName);
    //THSWeights* wm=(THSWeights*)wfile->Get(wmName);
    THSWeights* wm=new THSWeights();
    wm->LoadSaved(dirName+"/"+fileName,wmName);
    fIDName=wm->GetIDName();
    Add(wm);
    delete wm;
    //wfile->Close();
    //delete wfile;
  }
  gSystem->FreeDirectory(dir);
  
  SortWeights();//needs sorted for binary search to work
  PrintWeight();
  cout<<"THSWeights::Merge done "<<endl;
  // if(outName!=TString("")) {
  //   TFile* outFile=new TFile(outName,"recreate");
  //   Write();
  //   outFile->Close();
  //   delete outFile;
  // }
  return Size();
  
}

//////////////////////////////////////
///Find the name for a given index
TString THSWeights::GetSpeciesName(UInt_t isp){
  if(isp>=fSpecies.size()) return TString("");
  //find the name for a given index
  for(StrIntMap_t::iterator it1 = fSpecies.begin(); it1 != fSpecies.end(); ++it1)
    if(it1->second==(Int_t)isp) return it1->first;
  return TString("");
}

/////////////////////////////////////////////////////////////
///Add the tree \n
///Zero weight values in case of missing species in new weights.
///These branches will then just be filled with zero weight for
///the new entries.
void THSWeights::Add(THSWeights* Wts){
  StrIntMap_t *sp0=GetSpeciesp();
  StrIntMap_t *sp1=Wts->GetSpeciesp();
  UInt_t Ns0=sp0->size();
  UInt_t Ns1=sp1->size();
  UInt_t NnewSp=0;
  StrIntMap_t New_sp; //map for additional species from Wts with original index
  cout<<"THSWeights Add "<<Wts->Size()<<" "<<Wts->GetSpecies().size()<<" "<<Wts->GetTree()->GetNbranches()<<" "<<GetTree()->GetNbranches()<<" "<<endl;
  if(fFile) cout<<"FILE "<<fFile->GetName()<<endl;
  //Check for new species
  for(StrIntMap_t::iterator it1 = sp1->begin(); it1 != sp1->end(); ++it1){
    cout<<"THSWeights Add new species :"<<it1->first<<endl;
    if(!(sp0->count(it1->first))){//this species is not in map already
      SetSpecies(it1->first);//add new species branch
      for(Long64_t ie=0;ie<fWTree->GetEntries();ie++)//fill previous entries with 0
	fWTree->GetBranch(it1->first)->Fill();
    }
  }
  //Add the tree
  for(UInt_t isp=0;isp<Ns0;isp++)
    fWVals[isp]=0;
  //if(New_sp==0){//no new species, just merge trees
  TList* tlist=new TList();
  tlist->Add(Wts->GetTree());
  fWTree->Merge(tlist);
  delete tlist;
  TList* wlist=new TList();
  wlist->Add(Wts->GetIDTree());
  fIDTree->Merge(wlist);
  delete wlist;
  //make a list of weights added, this can be used to select contributing entrylists
  if(!fWeightList) {fWeightList=new TList();fWeightList->SetOwner();}
  fWeightList->Add(new TNamed(Wts->GetTitle(),""));//include name of this bin

}

void THSWeights::PrintWeight(){
  cout<<"Weights "<<GetName()<<" contains "<<Size() <<" events associated file is ";
  if(fFile) cout<<fFile->GetName()<< " "<<fFile->GetTitle()<<endl;
  else cout<<endl;
  cout<<"ID branch name : "<<fIDName<<endl;
  cout<<"Species are : "<<endl;
  for(map<TString,Int_t>::iterator it=fSpecies.begin();it!=fSpecies.end();++it)
    cout<<it->first<<endl;
  Int_t Nit=0;
  cout<<"The first ten entries are :"<<endl;
  Int_t Nw=10;
  if(Size()<10) Nw=Size();
  for(Int_t i=0;i<Nw;i++){
  // for(Int_t i=3900;i<4100;i++){
    GetEntry(i);
    cout<<fID<<" "<<fWVals[0]<<" ";
    for(UInt_t iss=1;iss<fSpecies.size();iss++)
      cout<<fWVals[iss]<< " ";
    cout<<endl;
  }
  if(fWeightList){
    cout<<"These weights are combined from :"<<endl;
    for(Int_t i=0;i<fWeightList->GetEntries();i++)
      cout<<fWeightList->At(i)->GetName()<<endl;
  }
}

void THSWeights::BuildIndex(){
  cout<<"THSWeights::BuildIndex "<<fIDTree->BuildIndex(TString("(Long64_t)WID"))<<endl;
  TTreeIndex *index = (TTreeIndex*)fIDTree->GetTreeIndex();
  fIDi=index->GetIndex();//entry numbers
  fIDv=index->GetIndexValues();//id values
  fN=fWTree->GetEntries();
  cout<<"Done"<<endl;
}

////////////////////////////////////////////////////////////////////////////
///GetEntryFast only works properly on trees where the ID is in order \n
///This is not guaranteed particualrly if weights are merged from different bins,
///reorder here
void THSWeights::SortWeights(){
  TTree* idtree=fIDTree->CloneTree(0); //create empty tree with branch adresses set //Clone before create index so do not have to save index
  idtree->SetDirectory(fFile); //set file to save memory
  BuildIndex();
  TTree* Mwtree=fWTree->CloneTree(); //create clone tree in memory or very slow!
  Mwtree->SetDirectory(0);
  TTree* wtree=fWTree->CloneTree(0); //create empty tree with branch adresses set
  wtree->SetDirectory(fFile);//set file to save memory
   
  cout<<"THSWeights::SortWeights() reordering trees "<<fWTree->GetDirectory()<<endl; 
  for( Long64_t i =  0; i < fN ; i++ ) {
    fID=fIDv[i];
    idtree->Fill(); //fill as ordered by the build index
    //wtree is synched with id tree
    Mwtree->GetEntry(fIDi[i]);
    wtree->Fill(); //fill as ordered by the build index
  }
  cout<<"THSWeights::SortWeights() reordering trees"<<endl;
  //swap sorted trees to datamembers
  delete fIDTree;fIDTree=nullptr;
  delete fWTree;fWTree=nullptr;
  delete Mwtree;Mwtree=nullptr;
  fIDTree=idtree;
  fWTree=wtree;
  cout<<"THSWeights::SortWeights() entries "<<fIDTree->GetEntries()<<" "<<fWTree->GetEntries()<<endl;
  //resetbranch addresses
  fIDTree->SetBranchAddress("WID",&fID);
  for(UInt_t iss=0;iss<fSpecies.size();iss++)
  for(StrIntMap_t::iterator it1 = fSpecies.begin(); it1 != fSpecies.end(); ++it1)
    fWTree->SetBranchAddress(it1->first,&fWVals[it1->second]);
  
  cout<<"THSWeights::SortWeights Print new ordering"<<endl;
  PrintWeight();
  //reset index
  fIDv=0;//these have been deleted with orig fIDTree
  fIDi=0;
  //  BuildIndex();//build for new tree!
  fIsSorted=kTRUE;
}
/////////////////////////////////////////////////////////////////
///Set file for keeping weights on disk
//Should be done before sort etc to save memory
void THSWeights::SetFile(TString filename){
  TDirectory *saveDir=gDirectory;
  cout<<"THSWeights::SetFile "<<filename<<endl;
  fFile=new TFile(filename,"recreate");
  if(fIDTree)fIDTree->SetDirectory(fFile);
  if(fWTree)fWTree->SetDirectory(fFile);
  saveDir->cd();
}
////////////////////////////////////////////////////////////////
///Finally save weights to disk
void THSWeights::Save(){
  cout<<"void THSWeights::Save() "<<fFile<<endl;
  cout<<fIDTree<<" "<<fWTree<<endl;
  if(!fFile) {cout<<"THSWeights::Save() no file associated with "<<GetName()<<" so not saved"<<endl;return;}

  cout<<" THSWeights::Save() "<<fFile->GetName()<<" "<<fFile->GetTitle()<<endl;
  fWTree->Print();
  fIDTree->Print();
  //  fWTree->SetDirectory(fFile);
  fFile->cd();
  fWTree->Write();//Note can't just save whole object
  fIDTree->Write(); //As 1GB limit on object buffers in TFile
  Write();//save the rest (not trees) of the weights class
  fFile->Close();
  delete fFile;fFile=0;fWTree=0;fIDTree=0;
  cout<<"THSWeights::Save() Saved weights to file"<<endl;

}
///////////////////////////////////////////////////////////////
///Give file name and name (in .root file) of weights object to load weights
///into and empty weights object
///e.g. THSWeights* wts=new THSWeights();
///     wts->LoadSaved("path_to_/Weight_File.root","HSWeight");
void THSWeights::LoadSaved(TString fname,TString wname){
  TDirectory* savedir=gDirectory;
  TFile* wfile=new TFile(fname);
  wfile->ls();
  THSWeights* file_wts=(THSWeights*)wfile->Get(wname);//read into memory
  fName=file_wts->GetName();
  fTitle=file_wts->GetTitle();
  cout<<fName<<" "<<fTitle<<endl;
  savedir->cd();
  TTree* tempTree=0;
  tempTree=(TTree*)wfile->Get(wname+"_W");
  fWTree=tempTree->CloneTree();
  delete tempTree;
  fWTree->SetDirectory(0);
  fSpecies=file_wts->GetSpecies();
  fWVals.ResizeTo(fSpecies.size());
  for(UInt_t i=0;i<fSpecies.size();i++)
    fWTree->SetBranchAddress(GetSpeciesName(i),&fWVals[i]); 

  fIDName=file_wts->GetIDName();
  tempTree=(TTree*)wfile->Get(wname+"_ID");
  fIDTree=tempTree->CloneTree();
  // fIDTree=(TTree*)file_wts->GetIDTree()->Clone();
  delete tempTree;
  fIDTree->SetDirectory(0);
  fIDTree->SetBranchAddress("WID",&fID);
 
  fCurrEntry=0;
  fIsSorted=kFALSE;
  fN=fWTree->GetEntries();
  delete file_wts;file_wts=nullptr;  
  wfile->Close();
  delete wfile;wfile=nullptr;
}

////////////////////////////////////////////////////////////
///Given a tree selection weight events that pass with wgt
///and enter the weight into this object
void THSWeights::WeightBySelection(TTree* tree,TCut cut,Double_t wgt){
  TDirectory *saveDir=gDirectory;
  if(fFile) fFile->cd();
  //Find events which pass cut
  tree->Draw(">>wlist",cut,"entrylist");
  TEntryList *elist =nullptr; 
  elist=dynamic_cast<TEntryList*>(gDirectory->Get("wlist"));
  elist->Print();
  Long64_t listEntries = elist->GetN();
  //Now only want ID branch
  tree->SetBranchStatus("*",0);
  tree->SetBranchStatus(fIDName,1);
  Double_t wID=0;
  tree->SetBranchAddress(fIDName,&wID);
  
  tree->SetEntryList(elist,"");
  //loop over events which passed cut
  for (Long64_t el = 0; el < listEntries; el++) {
    Long64_t entryNumber = tree->GetEntryNumber(el);
    tree->GetEntry(entryNumber);
    FillWeight(wID,wgt);
  }
  delete elist;elist=nullptr;
  tree->SetEntryList(nullptr);
  
  //turn all branches back on
  tree->ResetBranchAddresses();
  tree->SetBranchStatus("*",1);
  cout<<"THSWeights::WeightBySelection Added "<<listEntries<<" events with a weight of "<<wgt<<" from selection " <<cut<<endl;
  saveDir->cd();
}


////////////////////////////////////////////////////////////
///Given a tree selection weight events that pass with wgt
///and enter the weight into this object. wgt is a branch in the tree
void THSWeights::WeightBySelection(TTree* tree,TCut cut,TString wgt){
  TDirectory *saveDir=gDirectory;
  if(fFile) fFile->cd();
  //Find events which pass cut
  tree->Draw(">>wlist",cut,"entrylist");
  TEntryList *elist =nullptr; 
  elist=dynamic_cast<TEntryList*>(gDirectory->Get("wlist"));
  elist->Print();
  Long64_t listEntries = elist->GetN();
  //Now only want ID branch
  tree->SetBranchStatus("*",0);
  tree->SetBranchStatus(fIDName,1);
  tree->SetBranchStatus(wgt,1);
  Double_t wID=0;
  Double_t eventWeight;
  tree->SetBranchAddress(fIDName,&wID);
  tree->SetBranchAddress(wgt,&eventWeight);
  
  tree->SetEntryList(elist,"");
  //loop over events which passed cut
  for (Long64_t el = 0; el < listEntries; el++) {
    Long64_t entryNumber = tree->GetEntryNumber(el);
    tree->GetEntry(entryNumber);
    FillWeight(wID,eventWeight);
  }
  delete elist;elist=nullptr;
  tree->SetEntryList(nullptr);
  
  //turn all branches back on
  tree->ResetBranchAddresses();
  tree->SetBranchStatus("*",1);
  cout<<"THSWeights::WeightBySelection Added "<<listEntries<<" events with a weight taken from branch " << wgt << " from tree " << tree->GetName() << " according to selection " <<cut<<endl;
  saveDir->cd();
}

////////////////////////////////////////////////////////////
/// LC Jul 2018
/// Importance Sampling
/// Given a data tree and a simulation tree, output the weights required to adjust the simulated distribution 
/// to match the data distribution for the given variable
void THSWeights::ImportanceSampling(TTree* MCTree, TTree* dataTree, TH1* weightHist, TString var) {

  // create sim and data hists based on weightHist (empty hist with appropriate ax	is and binning)
  TH1* mcHist = (TH1*) weightHist->Clone();
  mcHist->SetName("mcHist");
  TH1* dataHist = (TH1*) weightHist->Clone();
  dataHist->SetName("dataHist");
  
  // fill the histograms from the trees with the quantity specified by var parameter
  MCTree->Draw(var+">>mcHist","","goff");
  dataTree->Draw(var+">>dataHist","","goff");
  
  // create hist with ratio of data to MC
  TH1* ratioHist = (TH1*) dataHist->Clone();
  ratioHist->Divide(mcHist);
  
  // loop around the MC tree filling weights
  Double_t  wID=0;
  Double_t val=0;
  MCTree->SetBranchAddress(fIDName,&wID);
  MCTree->SetBranchAddress(var,&val);
  Int_t nentries = MCTree->GetEntries();
  for (int i=0; i<nentries; i++) {
    MCTree->GetEntry(i);
    FillWeight(wID, 
	       ratioHist->GetBinContent( ratioHist->GetXaxis()->FindBin(val)));	
    //  cout << var << " value is " << val << " weight is " << ratioHist->GetBinContent(ratioHist->GetXaxis()->FindBin(val)) << endl;
    
  }
//  cout << "nentries is " << nentries << endl;
	
}
void  THSWeights::Draw1DWithWeights(TTree* tree,TH1* his,TString var,TString species){
  TLeaf *leafVar=tree->GetLeaf(var);
  TLeaf *leafID=tree->GetLeaf(fIDName);
  Int_t ispecies=0;
  if(!(species==TString("")))
    ispecies=fSpecies[species];

  for(Int_t i=0;i<tree->GetEntries();i++){
    tree->GetEntry(i);
    
    if(GetEntryBinarySearch(leafID->GetValue())){//find the weight for this event
      his->Fill(leafVar->GetValue(),GetWeight(ispecies));
    }
  }
}
