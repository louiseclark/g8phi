//************************************* rbtest.C *****************************//
//                      A part of the rootbeer package                        //
//                                                                            //
//                  Ken Livingstn (k.livingston@physics,gla,ac,uk)            //
//                                                                            //
//      Version         Date                    Comment                       //
//      1               21st Feb 2003                                         //
//      1.2d            23st Mar 2004           Simplified                    //
//      2.0             16th Sep 2005           All in root framework         //
//      2.1             20st Feb 2006           Buf fix edition               //
//                                                                            //
// ******************************* rbtest.C **********************************//

// A sample code to access BANKs in root.
// This code can be run as a root macro, or built as an executable:
// See http://nuclear.gla.ac.uk/~kl/rootbeer/manual/html/node5.php
// Or the tutorial in doc/manual.ps

/*******************************************************************************
 * Louise Clark 20 January 2016
 * 
 * Spec for this analysis code
 * ===========================
 * 
 * Uses g8 data already skimmed for 1 or more charged particles
 * Channel is gamma p -> p phi
 * Final state p K+K- (phi -> K+K- 50%)
 * 
 * CUTS
 * Events are included if the following conditions are true
 * 
 * 1. number of particles in event is 2 or 3
 * 2. z vertex is between -40 and 0
 * 3. all combinations of detected final state:
 *    1. p K+ K-
 *    2. missing K-
 *    3. missing K+
 *    4. missing p
 *    0.49 < proton mass squared < 1.44
 *    0.09 < kaon mass squared < 0.49
 * 4. best photon time difference (for proton/K+) is between -1 and 1
 * 5. missing mass
 *    between -0.1 and 0.1 (for all exclusive case)
 *    between 0.4 and 0.6 (for missing kaon)
 *    between 0.84 and 1.04 (for missing proton)
 *    between -0.1 and 0.1 (for all particles detected)
 * 
 * HISTOGRAMS
 * 
 * After cut 1
 * - h0_massp, h0_massm, h0_massn: mass squared from event (+, -, 0)
 * - h0_momp, h0_momm: momentum from event (+, -)
 * - h0_betap, h0_betam: beta from event (+, -)
 * - h0_betamomp, h0_betamomm: beta vs momentum from event (+, -)
 * - h0_yxp, h0_yxm: y vs x from event (+, -)
 * - h0_zp, h0_zm: z position from event (+, -)
 * 
 * After cut 3
 * - h1_multHist: Multiplicity
 * - h1 hists same as h0 above
 * - h1_zProton, h1_zKPlus: z position from event for proton and K+
 * - h1_elossp: proton eloss
 * - h1_elossp_mom: eloss vs mom for proton
 * - h1_elosskp: K+ eloss
 * - h1_elosskp_mom: eloss vs mom for K+
 * - h1_elosskm: K- eloss
 * - h1_elosskm_mom: eloss vs mom for K-
 * - h2_allPhotonTimeDiff - time difference between photon and proton (all photons)
 * - h2_bestPhotonTimeDiff - time difference between photon and proton (best photon)
 * 	 - proton beta taken from 4-vector with PDG mass assigned
 * - h2_photonEnergy - photon energy for all photons
 * - h2_bestPhotonEnergy - photon energy for best photon
 * 
 * After cut 4
 * - h3_bestPhotonTimeDiff - time difference between photon and proton (best photon)
 * - h3_bestPhotonEnergy - photon energy for best photon
 * - h3_missingMass1 - missing mass squared of p, K+ and K- (using best photon for proton) - should be zero
 * - h3_missingMass2 - missing mass of p and K+ (using best photon for proton)
 * - h3_missingMass3 - missing mass of p and K- (using best photon for proton)
 * - h3_missingMass4 - missing mass of K- and K+ (using best photon for K+)
 * - h3_KPlusMissingMass - mass of measured K+ and other particle (i.e. K-) from missing mass
 * - h3_KMinusMissingMass - mass of measured K- and other particle (i.e. K+) from missing mass
 * - h3_MM_pKPlus_pPiPlus - missing mass of proton and kaon like particle with kaon mass assigned vs
 *                          missing mass of proton and kaon like particle with pion mass assigned
 * - h3_MM_pKMinus_pPiMinus - missing mass of proton and kaon like particle with kaon mass assigned vs
 *                            missing mass of proton and kaon like particle with pion mass assigned
 * 
 * After cut 5
 * - h4_multHist: Multiplicity
 * - h4_missingMass1 - missing mass squared of p, K+ and K- (using best photon for proton) - should be zero
 * - h4_missingMass2 - missing mass of p and K+ (using best photon for proton)
 * - h4_missingMass3 - missing mass of p and K- (using best photon for proton)
 * - h4_missingMass4 - missing mass of K- and K+ (using best photon for K+)
 * - h4_KPlusMissingMass - mass of measured K+ and other particle (i.e. K-) from missing mass
 * - h4_KMinusMissingMass - mass of measured K- and other particle (i.e. K+) from missing mass
 * - h4_betamomp - beta vs momentum from event (p and K+)
 * - h4_phi1 - mass of K+ K- from pid 1
 * - h4_phi2 - mass of K+ K- from pid 2
 * - h4_phi3 - mass of K+ K- from pid 3
 * - h4_phi4 - mass of K+ K- from pid 4
 * - h4_phiTotal - mass of K+ K- from all events
 * - h4_MM_pKPlus_pPiPlus - missing mass of proton and kaon like particle with kaon mass assigned vs
 *                          missing mass of proton and kaon like particle with pion mass assigned
 * - h4_MM_pKMinus_pPiMinus - missing mass of proton and kaon like particle with kaon mass assigned vs
 *                            missing mass of proton and kaon like particle with pion mass assigned
 * - h4_KK_pK  - mass of K+ and K- 
 * 			  vs mass of p and K-
 * - h4_KK_pK1/2/3/4 as above for pid 1/2/3/4  
 * - h4_photonEnergy - photon energy for phi production (threshold is 1.57 GeV)
 * 
 ******************************************************************************* 
 */

// ******** required headers ********************************************
//                                                                      *
#include "RootBeerUtil.h"	//gets everything for RootBeer scheme
#include "TFile.h"              // for root files
//                                                                      *
// ******** end of required headers *************************************

// ******** my headers here *********************************************
//                                                                      *
#include "TH1.h"                // eg. for 1D Histogram                 
#include "TH2.h"                // eg. for 2D Histogram
#include "TLorentzVector.h"     //For Lorentz Vectors
#include "TMath.h"
#include "particleDEF.h"        //Particle Definitions
#include "TCanvas.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TROOT.h"
#include "/home/louise/g8_phi/haspect/Events/HaSpect/THSParticle.h"
#include "extra_packages/eloss_rb/eLoss.h"   

#include <iostream>
using namespace std;				// for debugging
//                                                                      *
// ******** end of my headers here **************************************

#define PARA 0                  //defs for polarisation plane
#define PERP 1
#define UNKNOWN 3
	
#define MANUAL 0                //defs for kind of running
#define SWAP 1


//enumerator for the column names in the pol tables
enum {
  E_ID    = 0,
  ENERGY  = 1,
  ENH     = 2,
  ENHERR  = 3,
  ENHFIT  = 4,
  PFIT    = 5,
  PCOR    = 6,
  PCORERR = 7,
  PSMOOTH = 8,
  EDGE    = 9
};

// ********** required global variables *********************************
//                                                                      *
// They are global to get easily filled by parseArgs()
int 	nEventsTot=0;			// ie do all events in all files
char  	InFileName[128];		// in file name parsed from args 
char  	OutFileName[128];		// out file name parsed from args

class TRootBeer *rootbeer;        	// The main class
//                                                                      *
// ********** end of required global variables **************************

// **************** my global variables *********************************
//                                                                      *
// Eg. my hists are global to be easily accessed from the 
// root> prompt after the function is called as a macro

class   TH1D *h0_massp;
class   TH1D *h0_massm;
class   TH1D *h0_massn;
class   TH1D *h0_momp;
class   TH1D *h0_momm;
class   TH1D *h0_zp;
class   TH1D *h0_zm;
class   TH1D *h0_betap;
class   TH1D *h0_betam;
class   TH2D *h0_betamomp;
class   TH2D *h0_betamomm;
class   TH2D *h0_yxp;
class   TH2D *h0_yxm;

class   TH1D *h1_multHist;

class   TH1D *h1_massp;
class   TH1D *h1_massm;
class   TH1D *h1_momp;
class   TH1D *h1_momm;
class   TH1D *h1_zProton;
class   TH1D *h1_zKPlus;
class   TH1D *h1_betap;
class   TH1D *h1_betam;
class   TH2D *h1_betamomp;
class   TH2D *h1_betamomm;
class   TH2D *h1_yxp;
class   TH2D *h1_yxm;

class   TH1F *h1_elossp;
class   TH2F *h1_elossp_mom;
class   TH1F *h1_elosskp;
class   TH2F *h1_elosskp_mom;
class   TH1F *h1_elosskm;
class   TH2F *h1_elosskm_mom;

class 	TH1D *h2_allPhotonTimeDiff;
class 	TH1D *h2_bestPhotonTimeDiff;
class 	TH1D *h2_photonEnergy;
class 	TH1D *h2_bestPhotonEnergy;

class 	TH1D *h3_bestPhotonTimeDiff;
class 	TH1D *h3_bestPhotonEnergy;
class 	TH1D *h3_missingMass1;
class 	TH1D *h3_missingMass2;
class 	TH1D *h3_missingMass3;
class 	TH1D *h3_missingMass4;
class 	TH1D *h3_KPlusMissingMass;
class 	TH1D *h3_KMinusMissingMass;
class 	TH2D *h3_MM_pKPlus_pPiPlus;
class 	TH2D *h3_MM_pKMinus_pPiMinus;

class   TH1D *h4_multHist;
class 	TH1D *h4_missingMass1;
class 	TH1D *h4_missingMass2;
class 	TH1D *h4_missingMass3;
class 	TH1D *h4_missingMass4;
class 	TH1D *h4_KPlusMissingMass;
class 	TH1D *h4_KMinusMissingMass;

//class 	TH1D *h4_KPlusKMinusMass;

class 	TH2D *h4_betamomp;
class 	TH1D *h4_phi1;
class 	TH1D *h4_phi2;
class 	TH1D *h4_phi3;
class 	TH1D *h4_phi4;
class 	TH1D *h4_phiTotal;
class 	TH2D *h4_MM_pKPlus_pPiPlus;
class 	TH2D *h4_MM_pKMinus_pPiMinus;
class 	TH2D *h4_KK_pK1;
class 	TH2D *h4_KK_pK2;
class 	TH2D *h4_KK_pK3;
class 	TH2D *h4_KK_pK4;
class 	TH2D *h4_KK_pK;
class	TH1D *h4_photonEnergy;

TLorentzVector *lPhoton;
TLorentzVector *lTarget;

// detected particles
TLorentzVector *lProtonDet;
TLorentzVector *lKPlusDet;
TLorentzVector *lKMinusDet;

// eloss
TLorentzVector *p_in;  //,*p_out;			// momenta before and after eloss correction
TVector3	vert;			// the vertex

// particles reconstructed from missing mass
TLorentzVector *lProtonMM;
TLorentzVector *lKPlusMM;
TLorentzVector *lKMinusMM;
TLorentzVector *lNoMM;

// p K+ K- with appropriate Det/MM assigned for topology
TLorentzVector *lProton;
TLorentzVector *lKPlus;
TLorentzVector *lKMinus;
TLorentzVector *lPhi;

// check missing masses
TLorentzVector *lKPlusMissingMass;		// addition of K+ and Missing K-
TLorentzVector *lKMinusMissingMass;		// addition of K- and Missing K+

TLorentzVector *lPiPlus;				// K+ like particle 4v with pion mass assigned (misidentified pion)
TLorentzVector *lPiMinus;				// K- like particle 4v with pion mass assigned (misidentified pion)
TLorentzVector *lMMProtonPiPlus;		// missing mass of proton and pion (lPiPlus)
TLorentzVector *lMMProtonPiMinus;		// missing mass of proton and pion (lPiMinus)

// Polarization

//default values for command line args
Int_t  EgSetting=1900;
Double_t polLeft=0.0; //Ken2 may alread be set like this - not sure
Double_t polRight=0.0; //Ken2
Double_t ELeft=200.0;
Double_t ERight=0.0;
Int_t Column=PSMOOTH;

//To implement Mike Duggers further correction ot the poalrization
Double_t ldPar[6][2][2]={
  //For 1.3 Manual: (PARA0,PARA1, PERP0,PERP1)
  1.004337,  -.4986358E-02, 1.009058,  -.5687501E-02,
  //For 1.5 Manual:
  1.012704,  -.3850369E-02, 1.002215,  -.2256782E-02,
  //For 1.7 Manual:
  0.9888860, -.2256782E-02, 0.9924032, -.3019766E-02,
  //For 2.1 Manual:
  1.004469,  -.2188895E-01, 1.067167,  -.1876592E-01,
  //For 1.7 Auto:
  0.9844326, 0.7438295E-03, 1.032343,  -.1255454E-01,
  //For 1.9 Auto:
  1.008734,  -.1557868E-02, 0.9865551,  0.2070320E-02,
};
const Double_t diffFix = 0.135;
const Double_t width = 0.2;

Int_t	currentPlane=UNKNOWN;
Int_t   polPlane=0;
Double_t  currentEdge=0.0;
Double_t		polDegree;
Double_t		polDegreeOrig;
Int_t   currentMode=MANUAL;
char *paraPolTable;
char *perpPolTable;

// edge tables
int run;                 //run no, extracted from current file name
char edgeTable[200];
Int_t edgeEventLow[5000];            //hold the current table of edge positions for event ranges
Int_t edgeEventHigh[5000];
Double_t edgeEventEdge[5000];
Int_t edgeEventPlane[5000];
Int_t edgeEventN;
Int_t edgeIndex=0;
Int_t lastEdgeEvent=0;
Double_t lastCohEdge=0.0;
Int_t lastCohPlane=-1;
  
//variables to handle the photon polarization using full pol tables
Double_t 	polTable[2][500][385][10];  //where its [plane][edge][E_id][field]
Int_t 		polTableN[2]={0,0};            //No of entries for para and perp
Char_t 		polFirstLines[2][500][250];   //to keep the 1st lines if needed (info from files)

class TH1F *meanPol[2], *meanPolG[2],*eventsPol[2],*eventsPolG[2];
class TH1F *meanPolOrig[2], *eventsPolOrig[2], *meanPolGOrig[2], *eventsPolGOrig[2], *meanPolPhi[2], *eventsPolPhi[2], *meanPolGPhi[2], *eventsPolGPhi[2];
Double_t gBinsArray[385]; 

//                                                                      *
// **********************************************************************


// ********* required functions defined in this code ******************** 
//                                                                      *
void printUsage();			// print usage when run as exe       
int  parseArgs(int,char **);		// parse args when run as exe
//                                                                      *
// ********* end of required functions defined in this code ************* 

// ********* my functions defined in this code ************************** 
//                                                                      *
void lcg8PhiAna(int, char *,char *); // main func. (same name as this macro)

// Polarization
int LoadPolTable(int plane, Char_t *PolTableList);
Double_t GetPol(Int_t plane, Double_t edge, Int_t eid, Int_t poltype, Double_t lowThresh, Double_t highThresh);
Double_t GetPol(Int_t plane, Double_t edge, Double_t eg, Int_t poltype, Double_t lowThresh, Double_t highThresh);
int LoadEdgeTable(Char_t *EdgeTable);
Int_t GetEdge(Int_t event, Double_t *edge, Int_t *plane);
Double_t mikeTest(Double_t pol, Double_t egamma, Double_t cohEdge, Int_t Plane, Int_t setting); //Mikes pol correction
Double_t polFix0(Double_t xE, Int_t plane, Int_t parSet); //Mikes pol correction 


//                                                                      *
// ********* end of my functions defined in this code ******************* 


// ---------- FUNCTION DEFINITIONS BEGIN HERE ---------------------

// ---------- Required main() for making an executable -------------
#ifdef ROOTEXE
int main(int argc,char **argv){		
  if((parseArgs(argc,argv))!=0) return(-1);	//parse input args
  lcg8PhiAna(nEventsTot,InFileName,OutFileName);	//call user function
  return(0);
}
#endif
// ---------- end of required main() for making an executable ------

// ---------- user main function  -----------------------------------------------
// This gets called by main() when running as an executable,
// or interactively from the root> prompt.
// should have the same name as this file (minus the .C suffix)
// customize the "non-required" sections
 
void lcg8PhiAna(int nEvents, char *file, char *outFileName){   // main user function

  // *****************variables which are required***********************
  //                                                                    *
  int	event=0;             	// Event counter for current file
  int	eventTot=0;	       	// Total event count
  class	TFile *outFile;         // the file to put the results
  char 	inFile[128];	       	// holds the current file name
  int 	fileNo=0;	       	// current file no if sorting though a list
  //                                                                    *
  // ********* end of variables which are required **********************


  // ******************* my variables ***********************************
  //                                                                    *
  int 	sect_no=0;			// hold the number of a bank sector
  char 	name[100];			// hold temp names for hists 
  //                                                                    *
  // ******************* end of my variables ****************************
  
  // THSParticle trees
  Double_t  fgID=0;
  
  // Macro loading moved to RootBeerSetup.cxx
  //TString HSANA=gSystem->Getenv("HSANA");
  //gInterpreter->AddIncludePath(HSANA);
  //gROOT->LoadMacro(HSANA+"/THSParticle.C++");
  
  THSParticle* hsProton=new THSParticle(2212);
  THSParticle* hsKp=new THSParticle(321);
  THSParticle* hsKm=new THSParticle(-321);
  THSParticle* hsBeam=new THSParticle(22);
 
  Int_t buff=32000;
  Int_t split=0;
  
  // p K+ K-
  TFile* filepKpKm=new TFile("filepKpKm.root","recreate");
  TTree* treepKpKm=new TTree("HSParticles","output tree",0);
  treepKpKm->Branch("fgID",&fgID,"fgID/D");
  treepKpKm->Branch("pol",&polDegree,"pol/D");
  treepKpKm->Branch("polState",&polPlane,"polState/I");
  treepKpKm->Branch("proton",&hsProton,buff,split);
  treepKpKm->Branch("Kp",&hsKp,buff,split);
  treepKpKm->Branch("Km",&hsKm,buff,split);
  treepKpKm->Branch("beam",&hsBeam,buff,split);

  // p K+
  TFile* filepKp=new TFile("filepKp.root","recreate");
  TTree* treepKp=new TTree("HSParticles","output tree",0);
  treepKp->Branch("fgID",&fgID,"fgID/D");
  treepKp->Branch("pol",&polDegree,"pol/D");
  treepKp->Branch("polState",&polPlane,"polState/I");
  treepKp->Branch("proton",&hsProton,buff,split);
  treepKp->Branch("Kp",&hsKp,buff,split);
  treepKp->Branch("beam",&hsBeam,buff,split);
  
  // p K-
  TFile* filepKm=new TFile("filepKm.root","recreate");
  TTree* treepKm=new TTree("HSParticles","output tree",0);
  treepKm->Branch("fgID",&fgID,"fgID/D");
  treepKm->Branch("pol",&polDegree,"pol/D");
  treepKm->Branch("polState",&polPlane,"polState/I");
  treepKm->Branch("proton",&hsProton,buff,split);
  treepKm->Branch("Km",&hsKm,buff,split);
  treepKm->Branch("beam",&hsBeam,buff,split);  
  
  // K+ K-
  TFile* fileKpKm=new TFile("fileKpKm.root","recreate");
  TTree* treeKpKm=new TTree("HSParticles","output tree",0);
  treeKpKm->Branch("fgID",&fgID,"fgID/D");
  treeKpKm->Branch("pol",&polDegree,"pol/D");
  treeKpKm->Branch("polState",&polPlane,"polState/I");
  treeKpKm->Branch("Kp",&hsKp,buff,split);
  treeKpKm->Branch("Km",&hsKm,buff,split);
  treeKpKm->Branch("beam",&hsBeam,buff,split);  
  
 
  // ************************* required init stuff **********************
  //                                                                    *
#ifdef ROOTEXE					//if running as an executable
  outFile=new TFile(outFileName,"recreate");	//open output file 
#endif
  eventTot=0;			 		// init the total event counter
  //
  //                                                                    *
  // ****************** end of required init stuff **********************
  
  // ******* User initialisation ****************************************
  //                                                                    *
  //
  // ** DONT FORGET TO DEFINE THE BANKS YOU WANT RIGHT HERE
  // This line can be generated by running:
  // >getbanks -Nevents <BosFile| rootDSTFile>
 
  const  char *mybanks[]={"EVNT","TAGR","SCPB","null"};	// make a list of the banks you want to use
  //const  char *mybanks[]={"SCR ","TAGR","null"};	// make a list of the banks you want to use
  // ** DONT FORGET TO DEFINE THE BANKS YOU WANT RIGHT HERE
  //
  //Set up my histograms (variables already defined as globals above)
 
  // Some sample hists with raw data
  h0_massp = new TH1D("h0_massp", "Event mass +", 1000, 0.0, 1.2);
  h0_massm = new TH1D("h0_massm", "Event mass -", 1000, 0.0, 1.2);
  h0_massn = new TH1D("h0_massn", "Event mass 0", 1000, 0.0, 1.2);
  
  h0_momp = new TH1D("h0_momp", "Event p +", 1000, 0.0, 4.0);
  h0_momm = new TH1D("h0_momm", "Event p -", 1000, 0.0, 4.0);

  h0_zp = new TH1D("h0_zp", "Event z +", 1000, -100.0, 100.0);
  h0_zm = new TH1D("h0_zm", "Event z -", 1000, -100.0, 100.0);
  
  h0_betap = new TH1D("h0_betap", "Event beta +", 1000, 0.0, 1.5);
  h0_betam = new TH1D("h0_betam", "Event beta -", 1000, 0.0, 1.5);  
  
  h0_betamomp = new TH2D("h0_betamomp", "Event beta vs p +", 500, 0.0, 4.0, 500, 0.0, 1.5);
  h0_betamomp->GetXaxis()->SetTitle("p");
  h0_betamomp->GetYaxis()->SetTitle("beta");
  h0_betamomm = new TH2D("h0_betamomm", "Event beta vs p -", 500, 0.0, 4.0, 500, 0.0, 1.5);
  h0_betamomm->GetXaxis()->SetTitle("p");
  h0_betamomm->GetYaxis()->SetTitle("beta");
  
  h0_yxp = new TH2D("h0_yxp", "Event y vs x +", 500, -100.0, 100.0, 500, -100.0, 100.0);
  h0_yxp->GetXaxis()->SetTitle("x");
  h0_yxp->GetYaxis()->SetTitle("y");
  h0_yxm = new TH2D("h0_yxm", "Event y vs x -", 500, -100.0, 100.0, 500, -100.0, 100.0);
  h0_yxm->GetXaxis()->SetTitle("x");
  h0_yxm->GetYaxis()->SetTitle("y");  
  		     
  // After particle ID
  h1_multHist = new TH1D("h1_multiplicity", "Multiplicity - initial PID", 5, 0, 5);  	
  
  h1_massp = new TH1D("h1_massp", "Event mass +", 1000, 0.0, 1.2);
  h1_massm = new TH1D("h1_massm", "Event mass -", 1000, 0.0, 1.2);

  h1_momp = new TH1D("h1_momp", "Event p +", 1000, 0.0, 4.0);
  h1_momm = new TH1D("h1_momm", "Event p -", 1000, 0.0, 4.0);

  h1_zProton = new TH1D("h1_zProton", "Event z proton", 1000, -100.0, 100.0);
  h1_zKPlus = new TH1D("h1_zKPlus", "Event z K+", 1000, -100.0, 100.0);
  
  h1_betap = new TH1D("h1_betap", "Event beta +", 1000, 0.0, 1.5);
  h1_betam = new TH1D("h1_betam", "Event beta -", 1000, 0.0, 1.5);  
  
  h1_betamomp = new TH2D("h1_betamomp", "Event beta vs p +", 500, 0.0, 4.0, 500, 0.0, 1.5);
  h1_betamomp->GetXaxis()->SetTitle("p");
  h1_betamomp->GetYaxis()->SetTitle("beta");
  h1_betamomm = new TH2D("h1_betamomm", "Event beta vs p -", 500, 0.0, 4.0, 500, 0.0, 1.5);
  h1_betamomm->GetXaxis()->SetTitle("p");
  h1_betamomm->GetYaxis()->SetTitle("beta");
  
  h1_yxp = new TH2D("h1_yxp", "Event y vs x +", 500, -100.0, 100.0, 500, -100.0, 100.0);
  h1_yxp->GetXaxis()->SetTitle("x");
  h1_yxp->GetYaxis()->SetTitle("y");
  h1_yxm = new TH2D("h1_yxm", "Event y vs x -", 500, -100.0, 100.0, 500, -100.0, 100.0);
  h1_yxm->GetXaxis()->SetTitle("x");
  h1_yxm->GetYaxis()->SetTitle("y");  

  h1_elossp = new TH1F("h1_elossp","Proton energy loss in target and start counter",500,0,50); 
  h1_elossp ->GetXaxis()->SetTitle("Energy loss (MeV)");
  h1_elossp_mom = new TH2F("h1_elossp_mom","Proton energy loss in target and start counter",100,0,3,500,0,50);
  h1_elossp_mom->GetXaxis()->SetTitle("Proton momentum (GeV)");
  h1_elossp_mom->GetYaxis()->SetTitle("Energy loss (MeV)");
  
  h1_elosskp = new TH1F("h1_elosskp","K+ energy loss in target and start counter",500,0,50); 
  h1_elosskp ->GetXaxis()->SetTitle("Energy loss (MeV)");
  h1_elosskp_mom = new TH2F("h1_elosskp_mom","K+ energy loss in target and start counter",100,0,3,500,0,50);
  h1_elosskp_mom->GetXaxis()->SetTitle("K+ momentum (GeV)");
  h1_elosskp_mom->GetYaxis()->SetTitle("Energy loss (MeV)");
  
  h1_elosskm = new TH1F("h1_elosskm","K- energy loss in target and start counter",500,0,50); 
  h1_elosskm ->GetXaxis()->SetTitle("Energy loss (MeV)");
  h1_elosskm_mom = new TH2F("h1_elosskm_mom","K- energy loss in target and start counter",100,0,3,500,0,50);
  h1_elosskm_mom->GetXaxis()->SetTitle("K- momentum (GeV)");
  h1_elosskm_mom->GetYaxis()->SetTitle("Energy loss (MeV)");
  
  //initialise eloss targ_X, targ_y, targ_z, st_offset
  //targ_X, targ_y, targ_z, st_offset
  initELoss(0.0,0.0,-20.0,-24.06);
  
  h2_allPhotonTimeDiff = new TH1D("h2_allPhotonTimeDiff", "Proton - photon time diff - all photons", 1000, -50.0, 50.0);  
  h2_bestPhotonTimeDiff = new TH1D("h2_bestPhotonTimeDiff", "Proton - photon time diff - best photons", 1000, -50.0, 50.0);  		     
  
  h2_photonEnergy = new TH1D("h2_photonEnergy", "Photon Energy", 1000, 0.0, 3.0);
  h2_bestPhotonEnergy = new TH1D("h2_bestPhotonEnergy", "Best Photon Energy", 1000, 0.0, 3.0);
  
  h3_bestPhotonTimeDiff = new TH1D("h3_bestPhotonTimeDiff", "Proton - photon time diff - best photons", 1000, -50.0, 50.0);  		     
  h3_bestPhotonEnergy = new TH1D("h3_bestPhotonEnergy", "Best Photon Energy", 1000, 0.0, 3.0);
  
  h3_missingMass1 = new TH1D("h3_missingMass1", "Missing Mass^2 (pK+K-)", 1000, -1.5, 1.5);
  h3_missingMass2 = new TH1D("h3_missingMass2", "Missing Mass (pK+)", 1000, 0.0, 1.6);
  h3_missingMass3 = new TH1D("h3_missingMass3", "Missing Mass (pK-)", 1000, 0.0, 1.6);
  h3_missingMass4 = new TH1D("h3_missingMass4", "Missing Mass (K+K-)", 1000, 0.0, 1.6);
  h3_KPlusMissingMass = new TH1D("h3_KPlusMissingMass", "Mass K+ and Missing K-", 1000, 0.0, 2.0);
  h3_KMinusMissingMass = new TH1D("h3_KMinusMissingMass", "Mass K- and Missing K+", 1000, 0.0, 2.0);
  
  h3_MM_pKPlus_pPiPlus = new TH2D("h3_MM_pKPlus_pPiPlus", "MM(pK+) vs MM(pPi+)", 200, 0.0, 1.2, 200, 0.0, 1.2);
  h3_MM_pKPlus_pPiPlus->GetXaxis()->SetTitle("MM(pPi+)");
  h3_MM_pKPlus_pPiPlus->GetYaxis()->SetTitle("MM(pK+)");  
  h3_MM_pKMinus_pPiMinus = new TH2D("h3_MM_pKMinus_pPiMinus", "MM(pK-) vs MM(pPi-)", 200, 0.0, 1.2, 200, 0.0, 1.2);
  h3_MM_pKMinus_pPiMinus->GetXaxis()->SetTitle("MM(pPi-)");
  h3_MM_pKMinus_pPiMinus->GetYaxis()->SetTitle("MM(pK-)");  
  
  h4_multHist = new TH1D("h4_multiplicity", "Multiplicity - after cuts", 5, 0, 5);  	  
  h4_missingMass1 = new TH1D("h4_missingMass1", "Missing Mass^2 (pK+K-)", 1000, -1.5, 1.5);
  h4_missingMass2 = new TH1D("h4_missingMass2", "Missing Mass (pK+)", 1000, 0.0, 1.6);
  h4_missingMass3 = new TH1D("h4_missingMass3", "Missing Mass (pK-)", 1000, 0.0, 1.6);
  h4_missingMass4 = new TH1D("h4_missingMass4", "Missing Mass (K+K-)", 1000, 0.0, 1.6);
  h4_KPlusMissingMass = new TH1D("h4_KPlusMissingMass", "Mass K+ and Missing K-", 1000, 0.0, 2.0);
  h4_KMinusMissingMass = new TH1D("h4_KMinusMissingMass", "Mass K- and Missing K+", 1000, 0.0, 2.0);
  h4_phi1 = new TH1D("h4_phi1", "Mass K+ K- pid 1", 1000, 0.8, 1.4);
  h4_phi2 = new TH1D("h4_phi2", "Mass K+ K- pid 2", 1000, 0.8, 1.4);
  h4_phi3 = new TH1D("h4_phi3", "Mass K+ K- pid 3", 1000, 0.8, 1.4);
  h4_phi4 = new TH1D("h4_phi4", "Mass K+ K- pid 4", 1000, 0.8, 1.4);
  h4_phiTotal = new TH1D("h4_phiTotal", "Mass K+ K- All events", 1000, 0.8, 2.0);  

  h4_betamomp = new TH2D("h4_betamomp", "Beta vs p +", 500, 0.0, 4.0, 500, 0.0, 1.5);
  h4_betamomp->GetXaxis()->SetTitle("p");
  h4_betamomp->GetYaxis()->SetTitle("beta");

  h4_MM_pKPlus_pPiPlus = new TH2D("h4_MM_pKPlus_pPiPlus", "MM(pK+) vs MM(pPi+)", 200, 0.0, 1.2, 200, 0.0, 1.2);
  h4_MM_pKPlus_pPiPlus->GetXaxis()->SetTitle("MM(pPi+)");
  h4_MM_pKPlus_pPiPlus->GetYaxis()->SetTitle("MM(pK+)");
  h4_MM_pKMinus_pPiMinus = new TH2D("h4_MM_pKMinus_pPiPlus", "MM(pK-) vs MM(pPi-)", 200, 0.0, 1.2, 200, 0.0, 1.2);
  h4_MM_pKMinus_pPiMinus->GetXaxis()->SetTitle("MM(pPi-)");
  h4_MM_pKMinus_pPiMinus->GetYaxis()->SetTitle("MM(pK-)");

  h4_KK_pK1 = new TH2D("h4_KK_pK", "Mass (K+ and K-) vs Mass (p and K-) pid 1", 200, 1.2, 2.1, 200, 0.7, 1.8);
  h4_KK_pK1->GetXaxis()->SetTitle("Mass (p and K-)");
  h4_KK_pK1->GetYaxis()->SetTitle("Mass (K+ and K-)");

  h4_KK_pK2 = new TH2D("h4_KK_pK", "Mass (K+ and K-) vs Mass (p and K-) pid 2", 200, 1.2, 2.1, 200, 0.7, 1.8);
  h4_KK_pK2->GetXaxis()->SetTitle("Mass (p and K-)");
  h4_KK_pK2->GetYaxis()->SetTitle("Mass (K+ and K-)");

  h4_KK_pK3 = new TH2D("h4_KK_pK", "Mass (K+ and K-) vs Mass (p and K-) pid 3", 200, 1.2, 2.1, 200, 0.7, 1.8);
  h4_KK_pK3->GetXaxis()->SetTitle("Mass (p and K-)");
  h4_KK_pK3->GetYaxis()->SetTitle("Mass (K+ and K-)");

  h4_KK_pK4 = new TH2D("h4_KK_pK", "Mass (K+ and K-) vs Mass (p and K-) pid 4", 200, 1.2, 2.1, 200, 0.7, 1.8);
  h4_KK_pK4->GetXaxis()->SetTitle("Mass (p and K-)");
  h4_KK_pK4->GetYaxis()->SetTitle("Mass (K+ and K-)");

  h4_KK_pK = new TH2D("h4_KK_pK", "Mass (K+ and K-) vs Mass (p and K-) All events", 200, 1.2, 2.1, 200, 0.7, 1.8);
  h4_KK_pK->GetXaxis()->SetTitle("Mass (p and K-)");
  h4_KK_pK->GetYaxis()->SetTitle("Mass (K+ and K-)");


  h4_photonEnergy = new TH1D("h4_photonEnergy", "Photon Energy", 1000, 0.0, 3.0);

  // 4 vectors
  lPhoton = new TLorentzVector(1.0,1.0,1.0,1.0);
  lTarget = new TLorentzVector(1.0,1.0,1.0,1.0);
  lTarget->SetXYZM(0,0,0,PROTON_MASS);

  // detected particles
  lProtonDet = new TLorentzVector(1.0,1.0,1.0,1.0);
  lKPlusDet = new TLorentzVector(1.0,1.0,1.0,1.0);
  lKMinusDet = new TLorentzVector(1.0,1.0,1.0,1.0);
  
  // eloss
  p_in = new TLorentzVector(1.0,1.0,1.0,1.0);

  // p K+ K- with appropriate Det/MM assigned for channel
  lProton = new TLorentzVector(1.0,1.0,1.0,1.0);
  lKPlus = new TLorentzVector(1.0,1.0,1.0,1.0);
  lKMinus = new TLorentzVector(1.0,1.0,1.0,1.0);
  lPhi = new TLorentzVector(1.0,1.0,1.0,1.0);
  
  // particles reconstructed from missing mass
  lProtonMM = new TLorentzVector(1.0,1.0,1.0,1.0);
  lKPlusMM = new TLorentzVector(1.0,1.0,1.0,1.0);
  lKMinusMM = new TLorentzVector(1.0,1.0,1.0,1.0);
  lNoMM = new TLorentzVector(1.0,1.0,1.0,1.0);
 
  // check missing masses
  lKPlusMissingMass = new TLorentzVector(1.0,1.0,1.0,1.0);  
  lKMinusMissingMass = new TLorentzVector(1.0,1.0,1.0,1.0);  
  
  lPiPlus = new TLorentzVector(1.0,1.0,1.0,1.0);
  lPiMinus = new TLorentzVector(1.0,1.0,1.0,1.0);
  lMMProtonPiPlus = new TLorentzVector(1.0,1.0,1.0,1.0);
  lMMProtonPiMinus = new TLorentzVector(1.0,1.0,1.0,1.0);
  
  const float vl = 29.9792458;                     //Speed of light in cm/ns

  // Polarization
  // Settings based on commandline argument (EgSetting) for energy setting...
  //These need to be changed for each coherent peak position
  //Eg low should be coh peak - 0.4
  //Eg High should be coh peak + 0.05
  //	add links to updated polarization tables here...
  switch(EgSetting){
  case 1300:
      //EgLow = 0.9;
      //EgHigh = 1.35;
      //meanPolFileName="/home/clasg8/rootbeer2.1/poltabs/manual_1.3_runs_meanpol.dat";
      paraPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_1.3_Manual_PARA.dat";
      perpPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_1.3_Manual_PERP.dat";
      
      break;
  case 1500:
      //EgLow = 1.1;
      //EgHigh = 1.55;
      //meanPolFileName="/home/clasg8/rootbeer2.1/poltabs/total_1.5_runs_meanpol.dat";
      paraPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_1.5_Manual_PARA.dat";
      perpPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_1.5_Manual_PERP.dat";
      break;
  case 1700: //manual
      //EgLow = 1.3;
      //EgHigh = 1.75;
      //meanPolFileName="/home/clasg8/rootbeer2.1/poltabs/total_1.7_runs_meanpol.dat";
      paraPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_1.7_Manual_PARA.dat";
      perpPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_1.7_Manual_PERP.dat";
      break;
  case 1701: //auto
      //EgLow = 1.3;
      //EgHigh = 1.75;
      //meanPolFileName="/home/clasg8/rootbeer2.1/poltabs/total_1.7_runs_meanpol.dat";
      paraPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_1.7_Auto_PARA.dat";
      perpPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_1.7_Auto_PERP.dat";
      currentMode=SWAP;
      break;
  case 1900:
      //EgLow = 1.5;
      //EgHigh = 1.95;
      //meanPolFileName="/home/clasg8/rootbeer2.1/poltabs/auto_1.9_runs_meanpol.dat";
      paraPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_1.9_Auto_PARA.dat";
      perpPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_1.9_Auto_PERP.dat";
      currentMode=SWAP;
      break;
  case 2100:
      //EgLow = 1.7;
      //EgHigh = 2.15;
      //meanPolFileName="/home/clasg8/rootbeer2.1/poltabs/manual_2.1_runs_meanpol.dat";
      paraPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_2.1_Manual_PARA.dat";
      perpPolTable="/home/clasg8/rootbeer2.1/poltabs/PolTables_2.1_Manual_PERP.dat";
      break;
  default:
      //EgLow = 0.0;
      //EgHigh = 0.0;
      cerr << "Fatal Error: Unknown value on energy_setting " <<  EgSetting << endl;

      break;
  }  

  // Load the polarization tables
  if((LoadPolTable(PARA,paraPolTable ))!=0) return;
  if((LoadPolTable(PERP,perpPolTable ))!=0) return;

  //hists for working out the mean pol.
  
  //make an array of bin edges for the tagger based on the ecounter centres.
  for(int b=1;b<384;b++){
    gBinsArray[b]=0.0005*(polTable[PARA][0][385-b][ENERGY]+polTable[PARA][0][384-b][ENERGY]); //mean of centred conv to GeV
  }
  gBinsArray[0]=gBinsArray[1]-0.5*(gBinsArray[2]-gBinsArray[1]);
  gBinsArray[384]=gBinsArray[383]+0.5*(gBinsArray[383]-gBinsArray[382]);
  
  meanPol[PARA]=new TH1F("meanPolPARA","meanPolPARA",384,0,384);
  meanPol[PERP]=new TH1F("meanPolPERP","meanPolPERP",384,0,384);
  meanPolOrig[PARA]=new TH1F("meanPolOrigPARA","meanPolOrigPARA",384,0,384);
  meanPolOrig[PERP]=new TH1F("meanPolOrigPERP","meanPolOrigPERP",384,0,384);
  eventsPol[PARA]=new TH1F("eventsPolPARA","eventsPolPARA",384,0,384);
  eventsPol[PERP]=new TH1F("eventsPolPERP","eventsPolPERP",384,0,384);
  eventsPolOrig[PARA]=new TH1F("eventsPolOrigPARA","eventsPolOrigPARA",384,0,384);
  eventsPolOrig[PERP]=new TH1F("eventsPolOrigPERP","eventsPolOrigPERP",384,0,384);
  meanPolG[PARA]=new TH1F("meanPolGPARA","meanPolGPARA",384,gBinsArray);
  meanPolG[PERP]=new TH1F("meanPolGPERP","meanPolGPERP",384,gBinsArray);
  meanPolGOrig[PARA]=new TH1F("meanPolGOrigPARA","meanPolGOrigPARA",384,gBinsArray);
  meanPolGOrig[PERP]=new TH1F("meanPolGOrigPERP","meanPolGOrigPERP",384,gBinsArray);
  eventsPolG[PARA]=new TH1F("eventsPolGPARA","eventsPolGPARA",384,gBinsArray);
  eventsPolG[PERP]=new TH1F("eventsPolGPERP","eventsPolGPERP",384,gBinsArray);
  eventsPolGOrig[PARA]=new TH1F("eventsPolGOrigPARA","eventsPolGOrigPARA",384,gBinsArray);
  eventsPolGOrig[PERP]=new TH1F("eventsPolGOrigPERP","eventsPolGOrigPERP",384,gBinsArray);
  meanPol[PARA]->SetStats(kFALSE);
  meanPol[PARA]->GetXaxis()->SetTitle("E_ID");
  meanPolOrig[PARA]->SetStats(kFALSE);
  meanPolOrig[PARA]->GetXaxis()->SetTitle("E_ID");
  meanPolG[PARA]->SetStats(kFALSE);
  meanPolG[PARA]->GetXaxis()->SetTitle("E_{#gamma} (GeV)");
  meanPolGOrig[PARA]->SetStats(kFALSE);
  meanPolGOrig[PARA]->GetXaxis()->SetTitle("E_{#gamma} (GeV)");

  meanPolPhi[PARA]=new TH1F("meanPolPhiPARA","meanPolPhiPARA",384,0,384);
  meanPolPhi[PERP]=new TH1F("meanPolPhiPERP","meanPolPhiPERP",384,0,384);
  eventsPolPhi[PARA]=new TH1F("eventsPolPhiPARA","eventsPolPhiPARA",384,0,384);
  eventsPolPhi[PERP]=new TH1F("eventsPolPhiPERP","eventsPolPhiPERP",384,0,384);
  meanPolGPhi[PARA]=new TH1F("meanPolGPhiPARA","meanPolGPhiPARA",384,gBinsArray);
  meanPolGPhi[PERP]=new TH1F("meanPolGPhiPERP","meanPolGPhiPERP",384,gBinsArray);
  eventsPolGPhi[PARA]=new TH1F("eventsPolGPhiPARA","eventsPolGPhiPARA",384,gBinsArray);
  eventsPolGPhi[PERP]=new TH1F("eventsPolGPhiPERP","eventsPolGPhiPERP",384,gBinsArray);

		     
  //                                                                    *
  // ******* End of User initialisation  ********************************


  // ********************** main file loop **********************************************************
  //                                                                                                *
  while((fileNo=getNextFile(inFile,file))!=-1){  // loop while files are still avialable 

    // Start of user stuff to do before sorting each file  ---------------
    fprintf(stderr,"Sorting file - %s\n",inFile);
    
    // get the table of coherent edges and polarization planes per event number range for this run
    sscanf(strstr(inFile,"flist_0")+7,"%d",&run);
    sprintf(edgeTable,"/home/clasg8/rootbeer2.1/dat/pol/run_%d_pol_edges.dat",run);
	LoadEdgeTable(edgeTable);
    
    // End of user stuff to do before sorting each file  ---------------


    if((rootbeer=createBeerObject(inFile))==NULL) return; 	// create rootbeer object
    rootbeer->SetBankStatus(mybanks,ON);			// switch on the banks required
    rootbeer->StartServer();                  			// start the server running
    rootbeer->ListServedBanks();			       	// list the banks which will be served

    event=0;		//zero the event counter for the file

    // ********************** main event loop ******************************************************
    //                                                                                             *

    while(event>=0){                                           	// do all events
      event = rootbeer->GetEvent();                            	// get next event
	  
	  
      // Start of user customized section ----------------------------

	  // CUT 1
	  if ((EVNT_NH != 2) && (EVNT_NH != 3)) continue;
	  
	  // Count the number of particles of each type
	  // Need 1 proton, 1 K+, and optional K-
	  int numProton = 0;
	  int numKPlus = 0;
	  int numKMinus = 0;
	  int rowProton = -1;
	  int rowKPlus = -1;
	  int rowKMinus = -1;	  
	  
      for(int row=0;row<EVNT_NH;row++){ 		      	// loop over rows in EVNT bank
		
		// initial hists with raw data
		if((EVNT[row].Charge == 1) && (EVNT[row].Mass > 0.0)){
			
			h0_massp->Fill(EVNT[row].Mass);       
			h0_momp->Fill(EVNT[row].Pmom);         
			h0_zp->Fill(EVNT[row].Z);         
			h0_betap->Fill(EVNT[row].Beta); 
			h0_betamomp->Fill(EVNT[row].Pmom, EVNT[row].Beta);
			h0_yxp->Fill(EVNT[row].X, EVNT[row].Y);
		}
		if((EVNT[row].Charge == -1) && (EVNT[row].Mass > 0.0)){
			
			h0_massm->Fill(EVNT[row].Mass);       
			h0_momm->Fill(EVNT[row].Pmom);         
			h0_zm->Fill(EVNT[row].Z);         
			h0_betam->Fill(EVNT[row].Beta); 
			h0_betamomm->Fill(EVNT[row].Pmom, EVNT[row].Beta);
			h0_yxm->Fill(EVNT[row].X, EVNT[row].Y);         
		}      
		if(EVNT[row].Charge == 0){
			
			h0_massn->Fill(EVNT[row].Mass);     
		}
		
		// CUT 2
		// remove anything outside the z of the target
		if ((EVNT[row].Z < -40.0) || (EVNT[row].Z >0.0)) continue;
		
		// Proton
		if ((EVNT[row].Charge == 1) && 
		    (EVNT[row].Mass > 0.49) && (EVNT[row].Mass < 1.44)) {
		
			numProton++;
			rowProton = row;
		}		
		
		// K+
		if ((EVNT[row].Charge == 1) && 
		    (EVNT[row].Mass > 0.09) && (EVNT[row].Mass < 0.49)) {
		
			numKPlus++;
			rowKPlus = row;
		}

		// K-
		if ((EVNT[row].Charge == -1) && 
		    (EVNT[row].Mass > 0.09) && (EVNT[row].Mass < 0.49)) {
		
			numKMinus++;
			rowKMinus = row;
		}
		
		
	  } // end of single event loop

	  // CUT 3
	  // Check if correct particles in event

	  bool includePID = false;
	  int pid = 0;
	  if (EVNT_NH == 3 && numProton == 1 && numKPlus ==1 && numKMinus == 1) {
	     includePID = true;
	     pid = 1;
      }
      else if (EVNT_NH == 2 && numProton == 1 && numKPlus ==1) {
	     includePID = true;
	     pid = 2;		  
	  } 
      else if (EVNT_NH == 2 && numProton == 1 && numKMinus ==1) {
	     includePID = true;
	     pid = 3;		  
	  } 
      else if (EVNT_NH == 2 && numKPlus == 1 && numKMinus ==1) {
	     includePID = true;
	     pid = 4;		  
	  } 
	  
	  if (!(includePID)) continue;

	  h1_multHist->Fill(pid);	  
	  
	  // Create the same histograms as before, but with just these events

	  if (numProton != 0) {
		  h1_massp->Fill(EVNT[rowProton].Mass);       
		  h1_momp->Fill(EVNT[rowProton].Pmom);         
		  h1_zProton->Fill(EVNT[rowProton].Z);         
		  h1_betap->Fill(EVNT[rowProton].Beta); 
		  h1_betamomp->Fill(EVNT[rowProton].Pmom, EVNT[rowProton].Beta);
		  h1_yxp->Fill(EVNT[rowProton].X, EVNT[rowProton].Y);
	  }
	  if (numKPlus != 0) {
		  h1_massp->Fill(EVNT[rowKPlus].Mass);       
		  h1_momp->Fill(EVNT[rowKPlus].Pmom);         
		  h1_zKPlus->Fill(EVNT[rowKPlus].Z);         
		  h1_betap->Fill(EVNT[rowKPlus].Beta); 
		  h1_betamomp->Fill(EVNT[rowKPlus].Pmom, EVNT[rowKPlus].Beta);
		  h1_yxp->Fill(EVNT[rowKPlus].X, EVNT[rowKPlus].Y);
	  }
	  
	  if (numKMinus != 0) {
		  h1_massm->Fill(EVNT[rowKMinus].Mass);       
		  h1_momm->Fill(EVNT[rowKMinus].Pmom);    
		  h1_betam->Fill(EVNT[rowKMinus].Beta); 
		  h1_betamomm->Fill(EVNT[rowKMinus].Pmom, EVNT[rowKMinus].Beta);
		  h1_yxm->Fill(EVNT[rowKMinus].X, EVNT[rowKMinus].Y);
	  }
	  
      // Create 4-vectors for each of the detected particles
      // Since we've now ID'd these particles, give them an exact mass
      // We trust the momentum from the EVNT but we don't trust the mass

	  if (numProton != 0) {

		  lProtonDet->SetXYZM(EVNT[rowProton].Cx*EVNT[rowProton].Pmom,EVNT[rowProton].Cy*EVNT[rowProton].Pmom,EVNT[rowProton].Cz*EVNT[rowProton].Pmom,PROTON_MASS);
		  
		  // apply eloss to proton
		  // make 3vector for vertex
		  vert.SetXYZ(EVNT[rowProton].X,EVNT[rowProton].Y,EVNT[rowProton].Z);
		  // call eloss
		  p_in=eLoss(lProtonDet,PROTON_MASS,vert,ELOSS_TARG_LH2,ELOSS_CELL_g8b);
		  // histogram the eloss in MeV
		  h1_elossp->Fill(1000.0*(p_in->P()-lProtonDet->P()));
		  h1_elossp_mom->Fill(lProtonDet->P(),1000.0*(p_in->P()-lProtonDet->P()));
		  //Reset 4-Vector to eloss values
		  lProtonDet->SetXYZM(p_in->X(),p_in->Y(),p_in->Z(),p_in->M());
		  
	  }
		  
	  if (numKPlus != 0) {
		  
		  lKPlusDet->SetXYZM(EVNT[rowKPlus].Cx*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cy*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cz*EVNT[rowKPlus].Pmom,KAON_CHARGED_MASS);

		  // apply eloss to K+
		  // make 3vector for vertex
		  vert.SetXYZ(EVNT[rowKPlus].X,EVNT[rowKPlus].Y,EVNT[rowKPlus].Z);
		  // call eloss
		  p_in=eLoss(lKPlusDet,KAON_CHARGED_MASS,vert,ELOSS_TARG_LH2,ELOSS_CELL_g8b);
		  // histogram the eloss in MeV
		  h1_elosskp->Fill(1000.0*(p_in->P()-lKPlusDet->P()));
		  h1_elosskp_mom->Fill(lKPlusDet->P(),1000.0*(p_in->P()-lKPlusDet->P()));
		  //Reset 4-Vector to eloss values
		  lKPlusDet->SetXYZM(p_in->X(),p_in->Y(),p_in->Z(),p_in->M());
		  
		  // Maybe the K+ is actually a Pi+
		  lPiPlus->SetXYZM(EVNT[rowKPlus].Cx*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cy*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cz*EVNT[rowKPlus].Pmom,PI_CHARGED_MASS);  
	  }
	  
	  if (numKMinus != 0) {
		  
		  lKMinusDet->SetXYZM(EVNT[rowKMinus].Cx*EVNT[rowKMinus].Pmom,EVNT[rowKMinus].Cy*EVNT[rowKMinus].Pmom,EVNT[rowKMinus].Cz*EVNT[rowKMinus].Pmom,KAON_CHARGED_MASS);
		  
		  // apply eloss to K+
		  // make 3vector for vertex
		  vert.SetXYZ(EVNT[rowKMinus].X,EVNT[rowKMinus].Y,EVNT[rowKMinus].Z);
		  // call eloss
		  p_in=eLoss(lKMinusDet,KAON_CHARGED_MASS,vert,ELOSS_TARG_LH2,ELOSS_CELL_g8b);
		  // histogram the eloss in MeV
		  h1_elosskm->Fill(1000.0*(p_in->P()-lKMinusDet->P()));
		  h1_elosskm_mom->Fill(lKMinusDet->P(),1000.0*(p_in->P()-lKMinusDet->P()));
		  //Reset 4-Vector to eloss values
		  lKMinusDet->SetXYZM(p_in->X(),p_in->Y(),p_in->Z(),p_in->M());		  
		  
		  // Maybe the K- is actually a Pi-
		  lPiMinus->SetXYZM(EVNT[rowKMinus].Cx*EVNT[rowKMinus].Pmom,EVNT[rowKMinus].Cy*EVNT[rowKMinus].Pmom,EVNT[rowKMinus].Cz*EVNT[rowKMinus].Pmom,PI_CHARGED_MASS);  
	  }


	  // Get properties of the particle to be used for photon selection
	  // used proton if detected, otherwise use K+
	  // Calculate the beta
	  float betaTimingParticle = 0.0;
	  int rowTimingParticle = -1;
	  if (pid==4) {
          betaTimingParticle=(lKPlusDet->Rho())/sqrt((lKPlusDet->Rho()*lKPlusDet->Rho())+(KAON_CHARGED_MASS*KAON_CHARGED_MASS));
          rowTimingParticle = rowKPlus;
      }
      else {
          betaTimingParticle=(lProtonDet->Rho())/sqrt((lProtonDet->Rho()*lProtonDet->Rho())+(PROTON_MASS*PROTON_MASS));
          rowTimingParticle = rowProton;
      }
	  // Why not take from 4-vector?
	  //betaProton = lProton->Beta();

  	  // Find the best photon based on the proton timing
	  int bestPhotonRow = -1;
	  float bestTimeDiff = 1000.0;
	  float timeDiff = 0.0;
		  
	  for (int row=0; row<TAGR_NH; row++) {
			
		if((TAGR[row].STAT!=15)&&(TAGR[row].STAT!=7)) continue; //check good photon 

			//Calculate the time difference
			            // time on start counter         -  time to travel from vertex position in target = vertex time for proton
			timeDiff=(((SCPB[EVNT[rowTimingParticle].SCstat-1].Time)-((SCPB[EVNT[rowTimingParticle].SCstat-1].Path)/(vl*betaTimingParticle)))
					  -((TAGR[row].TPHO)+((EVNT[rowTimingParticle].Z)/vl))-0.667);
                        // photon time from tagger   + time to travel to z position in target - offset since target centred at -20cm = vertex for photon
			            //                                                                      0.667 = 20cm / vl
			h2_allPhotonTimeDiff->Fill(timeDiff);
			h2_photonEnergy->Fill(TAGR[row].ERG);

			// Check if this is the minimum time difference so far
			if (fabs(timeDiff) < fabs(bestTimeDiff)) {
			
				bestPhotonRow = row;
				bestTimeDiff = timeDiff;
			}
			
	  }
	
	  // Plot the time difference for the best photons
	  h2_bestPhotonTimeDiff->Fill(bestTimeDiff);	
	  h2_bestPhotonEnergy->Fill(TAGR[bestPhotonRow].ERG);
	  
	  // CUT 4
	  // drop anything with time difference > 1 i.e. best photon is best of a bad lot
	  if (fabs(bestTimeDiff)>1.0) continue; 

	  // Plot the time difference after the cut
	  h3_bestPhotonTimeDiff->Fill(bestTimeDiff);	
	  h3_bestPhotonEnergy->Fill(TAGR[bestPhotonRow].ERG);
	  
	  // get the polarization
	  
	  //if(currentRadiator!=AMORPHOUS){ // do I need to check radiator?
		GetEdge(HEAD[0].NEVENT,&currentEdge,&currentPlane); //get the current edge position
      //}

	  //Extract the photon polarization information
      //Nothing in the photon pol stops the event going into the tree for likelihood, or other analysis.
      //However, if the photon pol does not meet the following criteria it is flagged as 99.9.
      //To fill histograms like phi distribs, where they will be fitted later, and mean polarizations used, 
    
      if(((currentPlane==PARA)||(currentPlane==PERP))&&
		((currentEdge>0.0)&&(1000.0*TAGR[bestPhotonRow].ERG >currentEdge-ELeft)&&(1000.0*TAGR[bestPhotonRow].ERG<currentEdge+ERight))){       
		//	polDegree=GetPol(currentPlane,currentEdge,(int)((TAGR[bestPhoton].E_id+0.5)/2.0),PSMOOTH,0.2,0.3);
		polDegree=GetPol(currentPlane,currentEdge,(int)((TAGR[bestPhotonRow].E_id+1.0)/2.0),Column,polLeft,polRight);
		polDegreeOrig=GetPol(currentPlane,currentEdge,(int)((TAGR[bestPhotonRow].E_id+1.0)/2.0),Column,polLeft,polRight);      
      
		//Now use Mikes correction (but not for 2.1)
		if (EgSetting!=2100) {
			polDegree=mikeTest(polDegree,1000.0*TAGR[bestPhotonRow].ERG,currentEdge,currentPlane,EgSetting);  //Ken2
		}
      
		//cout << "Event:" << HEAD[0].NEVENT << "  Edge:" << currentEdge <<  " E_ID:" << (int)((TAGR[bestPhotonRow].E_id+0.5)/2.0) 
		//     << " Eg:" << TAGR[bestPhotonRow].ERG << " Pol:" << polDegree << endl;
		if((polDegree>0.0) && (polDegree<1.0)){
			meanPol[currentPlane]->Fill((int)((TAGR[bestPhotonRow].E_id+1.0)/2.0),polDegree);
			eventsPol[currentPlane]->Fill((int)((TAGR[bestPhotonRow].E_id+1.0)/2.0));
			meanPolG[currentPlane]->Fill(TAGR[bestPhotonRow].ERG,polDegree); 
			eventsPolG[currentPlane]->Fill(TAGR[bestPhotonRow].ERG);
		}
		
		if((polDegreeOrig>0.0) && (polDegreeOrig<1.0)){
			meanPolOrig[currentPlane]->Fill((int)((TAGR[bestPhotonRow].E_id+1.0)/2.0),polDegreeOrig);
			eventsPolOrig[currentPlane]->Fill((int)((TAGR[bestPhotonRow].E_id+1.0)/2.0));
			meanPolGOrig[currentPlane]->Fill(TAGR[bestPhotonRow].ERG,polDegreeOrig); 
			eventsPolGOrig[currentPlane]->Fill(TAGR[bestPhotonRow].ERG);
		}
		
	  }
	  else {
		continue;
		//polDegree=99.9;
	  }
	  
	  if (currentPlane==PARA) {
		  polPlane = 1;
	  }
	  else if (currentPlane==PERP) {
		  polPlane = -1;
	  }
	  
	  // Now assign the 4 vectors to use depending on the channel
	  lPhoton->SetPxPyPzE(0,0,TAGR[bestPhotonRow].ERG,TAGR[bestPhotonRow].ERG);
	  if (pid==1) {
		  // p K+ K- detected
		  *lProton = *lProtonDet;
		  *lKPlus = *lKPlusDet;
		  *lKMinus = *lKMinusDet;
		  *lNoMM = (*lTarget + *lPhoton) - (*lProton + *lKPlus + *lKMinus);
		  *lPhi = *lKPlus + *lKMinus;
		  h3_missingMass1->Fill(lNoMM->M2());

		  // CUT 5
		  // Only keep events where missing mass is close to mass of expected particle (zero in this case)
	      if ((lNoMM->M() < -0.1) || (lNoMM->M() > 0.1)) continue;

		  h4_missingMass1->Fill(lNoMM->M2());
		  
	      // Plot the phi for this pid
	      h4_phi1->Fill(lPhi->M());
	      h4_KK_pK1->Fill((*lProton + *lKMinus).M(), (*lKPlus + *lKMinus).M());
	      
	      //Fill HSParticles
	      hsProton->SetP4(*lProton);
	      hsKp->SetP4(*lKPlus);
	      hsKm->SetP4(*lKMinus);
	      hsBeam->SetP4(*lPhoton);
	      treepKpKm->Fill();
		  
	  }
	  else if (pid==2) {
		  // missing K-
		  *lProton = *lProtonDet;
		  *lKPlus = *lKPlusDet;
		  *lKMinusMM = (*lTarget + *lPhoton) - (*lProton + *lKPlus);
		  *lKMinus = *lKMinusMM;
		  lKMinus->SetPxPyPzE(lKMinus->Px(),lKMinus->Py(),lKMinus->Pz(),sqrt((lKMinus->P()*lKMinus->P())+(KAON_CHARGED_MASS*KAON_CHARGED_MASS))); 
  		  *lPhi = *lKPlus + *lKMinus;
		  h3_missingMass2->Fill(lKMinusMM->M());
		  
		  // Plot the mass of the K+ and missing K-
	      *lKPlusMissingMass = *lKPlus + *lKMinusMM;
	      h3_KPlusMissingMass->Fill(lKPlusMissingMass->M());
	      
	      // Compare missing mass of pK+ with pPi+
		  *lMMProtonPiPlus = (*lTarget + *lPhoton) - (*lProton + *lPiPlus);
		  h3_MM_pKPlus_pPiPlus->Fill(lMMProtonPiPlus->M(), lKMinusMM->M());	
		  
		  // CUT 5
		  // Only keep events where missing mass is close to mass of expected particle
	      if ((lKMinusMM->M() < 0.4) || (lKMinusMM->M() > 0.6)) continue;
	      
	      // Replot the missing masses
		  h4_missingMass2->Fill(lKMinusMM->M());
	      h4_KPlusMissingMass->Fill(lKPlusMissingMass->M());
	      
     	  // Compare missing mass of pK+ with pPi+ after the K- cut
 	      h4_MM_pKPlus_pPiPlus->Fill(lMMProtonPiPlus->M(), lKMinusMM->M());
	      
	      // Plot the phi for this pid
	      h4_phi2->Fill(lPhi->M());
		  h4_KK_pK2->Fill((*lProton + *lKMinus).M(), (*lKPlus + *lKMinus).M());
	      
	      //Fill HSParticles
	      hsProton->SetP4(*lProton);
	      hsKp->SetP4(*lKPlus);
	      hsBeam->SetP4(*lPhoton);
	      treepKp->Fill();	      

	  }
	  else if (pid==3) {
		  // missing K+
		  *lProton = *lProtonDet;
		  *lKMinus = *lKMinusDet;
		  *lKPlusMM = (*lTarget + *lPhoton) - (*lProton + *lKMinus);
		  *lKPlus = *lKPlusMM;
		  lKPlus->SetPxPyPzE(lKPlus->Px(),lKPlus->Py(),lKPlus->Pz(),sqrt((lKPlus->P()*lKPlus->P())+(KAON_CHARGED_MASS*KAON_CHARGED_MASS))); 
		  *lPhi = *lKPlus + *lKMinus;
		  h3_missingMass3->Fill(lKPlusMM->M());
		  
		  // Plot the mass of the K- and missing K+
	      *lKMinusMissingMass = *lKMinus + *lKPlusMM;
	      h3_KMinusMissingMass->Fill(lKMinusMissingMass->M());
	      
		  // Compare missing mass of pK- with pPi-
		  *lMMProtonPiMinus = (*lTarget + *lPhoton) - (*lProton + *lPiMinus);
		  h3_MM_pKMinus_pPiMinus->Fill(lMMProtonPiMinus->M(), lKPlusMM->M());	

		  // CUT 5
		  // Only keep events where missing mass is close to mass of expected particle
	      if ((lKPlusMM->M() < 0.4) || (lKPlusMM->M() > 0.6)) continue;
	      
	      // Replot the missing masses
		  h4_missingMass3->Fill(lKPlusMM->M());
	      h4_KMinusMissingMass->Fill(lKMinusMissingMass->M());	      

		  // Compare missing mass of pK- with pPi- after the K+ cut
		  *lMMProtonPiMinus = (*lTarget + *lPhoton) - (*lProton + *lPiMinus);
		  h4_MM_pKMinus_pPiMinus->Fill(lMMProtonPiMinus->M(), lKPlusMM->M());	
	      
	      // Plot the phi for this pid
	      h4_phi3->Fill(lPhi->M());	     
		  h4_KK_pK3->Fill((*lProton + *lKMinus).M(), (*lKPlus + *lKMinus).M());
	      
  	      //Fill HSParticles
	      hsProton->SetP4(*lProton);
	      hsKm->SetP4(*lKMinus);
	      hsBeam->SetP4(*lPhoton);
	      treepKm->Fill();

	  }
	  else if (pid==4) {
		  // missing p
		  *lKPlus = *lKPlusDet;
		  *lKMinus = *lKMinusDet;
		  *lProtonMM = (*lTarget + *lPhoton) - (*lKPlus + *lKMinus);
		  *lProton = *lProtonMM;
		  lProton->SetPxPyPzE(lProton->Px(),lProton->Py(),lProton->Pz(),sqrt((lProton->P()*lProton->P())+(PROTON_MASS*PROTON_MASS))); 
		  *lPhi = *lKPlus + *lKMinus;
		  h3_missingMass4->Fill(lProtonMM->M());
		  
		  // CUT 5
		  // Only keep events where missing mass is close to mass of expected particle
	      if ((lProtonMM->M() < 0.84) || (lProtonMM->M() > 1.04)) continue;
	      
	      // Replot the missing masses
		  h4_missingMass4->Fill(lProtonMM->M());
		  
	      // Plot the phi for this pid
	      h4_phi4->Fill(lPhi->M());
		  h4_KK_pK4->Fill((*lProton + *lKMinus).M(), (*lKPlus + *lKMinus).M());
	      
	      //Fill HSParticles
	      hsKp->SetP4(*lKPlus);
	      hsKm->SetP4(*lKMinus);
	      hsBeam->SetP4(*lPhoton);
	      treeKpKm->Fill();	      
		  
	  }
	  
	  if (pid==1 || pid==2 || pid==3 || pid==4) {
	   
	    if((polDegree>0.0) && (polDegree<1.0)){
	      meanPolPhi[currentPlane]->Fill((int)((TAGR[bestPhotonRow].E_id+1.0)/2.0),polDegree);
	      eventsPolPhi[currentPlane]->Fill((int)((TAGR[bestPhotonRow].E_id+1.0)/2.0));
	      meanPolGPhi[currentPlane]->Fill(TAGR[bestPhotonRow].ERG,polDegree); 
	      eventsPolGPhi[currentPlane]->Fill(TAGR[bestPhotonRow].ERG);
	    }
	    
	  }
	  
	  h4_multHist->Fill(pid);
	  
	  // Replot beta vs p for proton and K+
   	  h4_betamomp->Fill(EVNT[rowProton].Pmom, EVNT[rowProton].Beta);
 	  h4_betamomp->Fill(EVNT[rowKPlus].Pmom, EVNT[rowKPlus].Beta);
	  
	  // Plot the mass of K+K- total
	  h4_phiTotal->Fill(lPhi->M());
 	   	  
 	  // Mass of K+ and K- vs Mass of p and K- 
 	  // Should see hyperons?
 	  h4_KK_pK->Fill((*lProton + *lKMinus).M(), (*lKPlus + *lKMinus).M());
 	  
 	  // Check photon energy now
 	  // Should only be photons > 1.57 GeV
 	  h4_photonEnergy->Fill(TAGR[bestPhotonRow].ERG);


      //eg of handling of multiple sector bank SCR
//      for(int sector=0; sector<SCR_NS;sector++){              	// for all sectors
//	sect_no=SCR_S[sector];                                 	// get the number of the sector 
//	for(int row=0;row<SCR_NH[sector];row++){            	// for all hits in that sector
//	  scr_hist[sect_no]->Fill(SCR[sector][row].id);       	// fill  hist
//	} 
 //     }   
      /**********************************************************************
       *  End of user customized section ----------------------------
	  */
      eventTot++;
      fgID++;
      if(eventTot%1000 == 0) fprintf(stdout,"done %d\n",eventTot);
      if((nEvents>0)&&(eventTot >=nEvents)) break;		//break if nEvents done
    }
    //                                                                                             *
    // ********************** end of main event loop ***********************************************

    // Start of user stuff to do after sorting each file  ---------------
    fprintf(stdout,"sorted %d events from file: %s\n",abs(event),inFile);
    // End of user stuff to do after sorting each file  ---------------




    delete rootbeer;                              		//Call the class destructor
    if((nEvents>0)&&(eventTot >=nEvents)) break;		//break if nEvents done
  }
  //                                                                                                *
  // ********************** end of main file loop ***************************************************



  // ******** Do any final stuff here ***********************************
  //                                                                    *
  
  //Divide the polarization totals by the no of events to get the mean
  meanPol[PARA]->Divide(eventsPol[PARA]);
  meanPol[PERP]->Divide(eventsPol[PERP]);
  meanPolG[PARA]->Divide(eventsPolG[PARA]);
  meanPolG[PERP]->Divide(eventsPolG[PERP]);
  meanPolOrig[PARA]->Divide(eventsPolOrig[PARA]);
  meanPolOrig[PERP]->Divide(eventsPolOrig[PERP]);
  meanPolGOrig[PARA]->Divide(eventsPolGOrig[PARA]);
  meanPolGOrig[PERP]->Divide(eventsPolGOrig[PERP]);
  meanPolPhi[PARA]->Divide(eventsPolPhi[PARA]);
  meanPolPhi[PERP]->Divide(eventsPolPhi[PERP]);
  meanPolGPhi[PARA]->Divide(eventsPolGPhi[PARA]);
  meanPolGPhi[PERP]->Divide(eventsPolGPhi[PERP]);

  
  // write out the trees for sWeights
  filepKpKm->cd();
  treepKpKm->Write();
  filepKpKm->Close();
  delete filepKpKm;
  
  filepKp->cd();
  treepKp->Write();
  filepKp->Close();
  delete filepKp;
  
  filepKm->cd();
  treepKm->Write();
  filepKm->Close();
  delete filepKm;
  
  fileKpKm->cd();
  treeKpKm->Write();
  fileKpKm->Close();
  delete fileKpKm;
  
// Write out the plots
TCanvas *c1=new TCanvas("c1","c1",800,600);

h0_massp->Draw();
c1->SaveAs("h0_massp.gif");
c1->Clear();

h0_massm->Draw();
c1->SaveAs("h0_massm.gif");
c1->Clear();

h0_massn->Draw();
c1->SaveAs("h0_massn.gif");
c1->Clear();

h0_momp->Draw();
c1->SaveAs("h0_momp.gif");
c1->Clear();

h0_momm->Draw();
c1->SaveAs("h0_momm.gif");
c1->Clear();

h0_zp->Draw();
c1->SaveAs("h0_zp.gif");
c1->Clear();

h0_zm->Draw();
c1->SaveAs("h0_zm.gif");
c1->Clear();

h0_betap->Draw();
c1->SaveAs("h0_betap.gif");
c1->Clear();

h0_betam->Draw();
c1->SaveAs("h0_betam.gif");
c1->Clear();

gStyle->SetPalette(1);
h0_betamomp->Draw("COLZ");
c1->SaveAs("h0_betamomp.gif");
c1->Clear();

gStyle->SetPalette(53);
h0_betamomp->Draw("COLZ");
c1->SaveAs("h0_betamomp2.gif");
c1->Clear();

gStyle->SetPalette(1);
h0_betamomm->Draw("COLZ");
c1->SaveAs("h0_betamomm.gif");
c1->Clear();

gStyle->SetPalette(53);
h0_betamomm->Draw("COLZ");
c1->SaveAs("h0_betamomm2.gif");
c1->Clear();

h0_yxp->Draw();
c1->SaveAs("h0_yxp.gif");
c1->Clear();

h0_yxm->Draw();
c1->SaveAs("h0_yxm.gif");
c1->Clear();

h1_multHist->Draw();
c1->SaveAs("h1_multHist.gif");
c1->Clear();

h1_massp->Draw();
c1->SaveAs("h1_massp.gif");
c1->Clear();

h1_massm->Draw();
c1->SaveAs("h1_massm.gif");
c1->Clear();

h1_momp->Draw();
c1->SaveAs("h1_momp.gif");
c1->Clear();

h1_momm->Draw();
c1->SaveAs("h1_momm.gif");
c1->Clear();

h1_zProton->Draw();
c1->SaveAs("h1_zProton.gif");
c1->Clear();

h1_zKPlus->Draw();
c1->SaveAs("h1_zKPlus.gif");
c1->Clear();

h1_betap->Draw();
c1->SaveAs("h1_betap.gif");
c1->Clear();

h1_betam->Draw();
c1->SaveAs("h1_betam.gif");
c1->Clear();

gStyle->SetPalette(1);
h1_betamomp->Draw("COLZ");
c1->SaveAs("h1_betamomp.gif");
c1->Clear();

gStyle->SetPalette(1);
h1_betamomm->Draw("COLZ");
c1->SaveAs("h1_betamomm.gif");
c1->Clear();

h1_yxp->Draw();
c1->SaveAs("h1_yxp.gif");
c1->Clear();

h1_yxm->Draw();
c1->SaveAs("h1_yxm.gif");
c1->Clear();

h1_elossp->Draw();
c1->SaveAs("h1_elossp.gif");
c1->Clear();

gStyle->SetPalette(1);
h1_elossp_mom->Draw("COLZ");
c1->SaveAs("h1_elossp_mom.gif");
c1->Clear();

h1_elosskp->Draw();
c1->SaveAs("h1_elosskp.gif");
c1->Clear();

gStyle->SetPalette(1);
h1_elosskp_mom->Draw("COLZ");
c1->SaveAs("h1_elosskp_mom.gif");
c1->Clear();

h1_elosskm->Draw();
c1->SaveAs("h1_elosskm.gif");
c1->Clear();

gStyle->SetPalette(1);
h1_elosskm_mom->Draw("COLZ");
c1->SaveAs("h1_elosskm_mom.gif");
c1->Clear();

h2_allPhotonTimeDiff->Draw();
c1->SaveAs("h2_allPhotonTimeDiff.gif");
c1->Clear();

h2_bestPhotonTimeDiff->Draw();
c1->SaveAs("h2_bestPhotonTimeDiff.gif");
c1->Clear();

h2_photonEnergy->Draw();
c1->SaveAs("h2_photonEnergy.gif");
c1->Clear();

h2_bestPhotonEnergy->Draw();
c1->SaveAs("h2_bestPhotonEnergy.gif");
c1->Clear();

h3_bestPhotonTimeDiff->Draw();
c1->SaveAs("h3_bestPhotonTimeDiff.gif");
c1->Clear();

h3_bestPhotonEnergy->Draw();
c1->SaveAs("h3_bestPhotonEnergy.gif");
c1->Clear();

h3_missingMass1->Draw();
c1->SaveAs("h3_missingMass1.gif");
c1->Clear();

h3_missingMass2->Draw();
c1->SaveAs("h3_missingMass2.gif");
c1->Clear();

h3_missingMass3->Draw();
c1->SaveAs("h3_missingMass3.gif");
c1->Clear();

h3_missingMass4->Draw();
c1->SaveAs("h3_missingMass4.gif");
c1->Clear();

h3_KPlusMissingMass->Draw();
c1->SaveAs("h3_KPlusMissingMass.gif");
c1->Clear();

h3_KMinusMissingMass->Draw();
c1->SaveAs("h3_KMinusMissingMass.gif");
c1->Clear();

gStyle->SetPalette(1);
h3_MM_pKPlus_pPiPlus->Draw("COLZ");
c1->SaveAs("h3_MM_pKPlus_pPiPlus.gif");
c1->Clear();

h3_MM_pKMinus_pPiMinus->Draw("COLZ");
c1->SaveAs("h3_MM_pKMinus_pPiMinus.gif");
c1->Clear();

h4_multHist->Draw();
c1->SaveAs("h4_multHist.gif");
c1->Clear();

h4_missingMass1->Draw();
c1->SaveAs("h4_missingMass1.gif");
c1->Clear();

h4_missingMass2->Draw();
c1->SaveAs("h4_missingMass2.gif");
c1->Clear();

h4_missingMass3->Draw();
c1->SaveAs("h4_missingMass3.gif");
c1->Clear();

h4_missingMass4->Draw();
c1->SaveAs("h4_missingMass4.gif");
c1->Clear();

h4_KPlusMissingMass->Draw();
c1->SaveAs("h4_KPlusMissingMass.gif");
c1->Clear();

h4_KMinusMissingMass->Draw();
c1->SaveAs("h4_KMinusMissingMass.gif");
c1->Clear();

gStyle->SetPalette(1);
h4_betamomp->Draw("COLZ");
c1->SaveAs("h4_betamomp.gif");
c1->Clear();

h4_phi1->Draw();
c1->SaveAs("h4_phi1.gif");
c1->Clear();

h4_phi2->Draw();
c1->SaveAs("h4_phi2.gif");
c1->Clear();

h4_phi3->Draw();
c1->SaveAs("h4_phi3.gif");
c1->Clear();

h4_phi4->Draw();
c1->SaveAs("h4_phi4.gif");
c1->Clear();

h4_phiTotal->Draw();
c1->SaveAs("h4_phiTotal.gif");
c1->Clear();

gStyle->SetOptStat(0);
gStyle->SetPalette(1);

h4_MM_pKPlus_pPiPlus->Draw("COLZ");
c1->SetGrid();
c1->SaveAs("h4_MM_pKPlus_pPiPlus.gif");
c1->Clear();   

h4_MM_pKMinus_pPiMinus->Draw("COLZ");
c1->SetGrid();
c1->SaveAs("h4_MM_pKMinus_pPiMinus.gif");
c1->Clear();      

h4_KK_pK1->Draw("COLZ");
c1->SetGrid();
c1->SaveAs("h4_KK_pK1.gif");
c1->Clear();     

h4_KK_pK2->Draw("COLZ");
c1->SetGrid();
c1->SaveAs("h4_KK_pK2.gif");
c1->Clear();  

h4_KK_pK3->Draw("COLZ");
c1->SetGrid();
c1->SaveAs("h4_KK_pK3.gif");
c1->Clear();

h4_KK_pK4->Draw("COLZ");
c1->SetGrid();
c1->SaveAs("h4_KK_pK4.gif");
c1->Clear();    

h4_KK_pK->Draw("COLZ");
c1->SetGrid();
c1->SaveAs("h4_KK_pK.gif");
c1->Clear();  

h4_photonEnergy->Draw();
c1->SaveAs("h4_photonEnergy.gif");
c1->Clear();

meanPolG[PARA]->Draw();
c1->SaveAs("meanPolGPara.gif");
c1->Clear();

meanPolG[PERP]->Draw();
c1->SaveAs("meanPolGPerp.gif");
c1->Clear();

meanPolGOrig[PARA]->Draw();
c1->SaveAs("meanPolGOrigPara.gif");
c1->Clear();

meanPolGOrig[PERP]->Draw();
c1->SaveAs("meanPolGOrigPerp.gif");
c1->Clear();

meanPolGPhi[PARA]->Draw();
c1->SaveAs("meanPolGPhiPara.gif");
c1->Clear();

meanPolGPhi[PERP]->Draw();
c1->SaveAs("meanPolGPhiPerp.gif");
c1->Clear();
  
  
  // Start of user stuff to do at the end  ------------------------
  fprintf(stdout,"Sorted Total of  %d events\n",eventTot);
  // End of user stuff to do at the end  --------------------------

#ifdef ROOTEXE
  //if it's an executable, write to the output file
  outFile->Write();
  outFile->Close();
#endif
  //                                                                    *
  // ********************************************************************
}
// ---------- end of user main function  -----------------------------------------------



// ---------- required parseArgs() function  -------------------------------------------
// parses the input args when running as an executable
// set up the no of events, and input and output file names in global variables
// or prints usage
int parseArgs(int argc,char **argv){
  switch(argc){
  case(1):				// 1 or 2 args not enough
  case(2):
    printUsage();
    return(-1);
    break;
  case(3):				// 3 args rbtest <infile> <outfile>
    if((strstr(argv[1],"-N"))!=NULL){
      printUsage();
      return(-1);
    }
    else{
      strcpy(InFileName,argv[1]);
      strcpy(OutFileName,argv[2]);
    }
    break;
  case(4):				// 4 args must be "rbtest -N# <infile> <outfile>" 
    if(strncmp(argv[1],"-N",2)!=0){  	//print usage then exit */
      printUsage();
      return(-1);
    }
    else{
      sscanf(argv[1]+2,"%d",&nEventsTot);	//get the required event no
      strcpy(InFileName,argv[2]);
      strcpy(OutFileName,argv[3]);
    }      
    break;
  default:
    printUsage();
    return(-1);
    break;
  }
  return(0);
}
// ---------- end of required parseArgs() function  -------------------------------------

// ---------- required printUsage() function  -------------------------------------------
// When running as executable, gets called if no args, or silly args
void printUsage(){
  fprintf(stderr,"\nusage:\n\n");
  fprintf(stderr,"lcg8PhiAna -h   Print this message\n\n");
  fprintf(stderr,"lcg8PhiAna [-Nevents] <infile> <outfile.root> \n");
  fprintf(stderr,"lcg8PhiAna [-Nevents] <-Lfilelist> <outfile.root> \n");
}
// ---------- end of required printUsage() function  -------------------------------------------




// Polarization

int LoadPolTable(int plane, Char_t *PolTableList){
  FILE *fplist,*fpfile;              //file pointers for filelist and each file therein
  Char_t lline[250];                 //for reading in lines from filelist
  Char_t fline[250];                 //for reading in lines from file
  Char_t filename[250];              //file
  Int_t  fcount=0;                   //counter for no of files read in
  Int_t  chancount=0;                //counter for no of channels read in
  Int_t  eid=0;
  Double_t edge=0.0;                 
  
  if((fplist=fopen(PolTableList,"r"))==NULL){ //open filelist
    cerr << "Error Couldn't open file: " << PolTableList << endl;
    return -1;
  }

  fcount=0; 
  //for each file in the list
  while(fgets(lline,240,fplist) != NULL){
    if((lline[0] == '*')||(lline[0] == '#')) continue; //skip comments
    sscanf(lline,"%s",filename);                       //read in filename
     
    cout << "opening " << filename << "   " << endl;
    if((fpfile=fopen(filename,"r"))==NULL){              //open file
      cerr << "Error Couldn't open file: " << filename << endl;
      return -1;
    }
        
    fgets(polFirstLines[plane][polTableN[plane]],240,fpfile ); //save the 1st line

    //scan the bit after the last "_" in the filename to get the edge energy
    sscanf(strrchr(filename,'_')+1,"%lg",&polTable[plane][fcount][0][EDGE]);

    chancount=0;                                             //starting array at 1 for consistency with E_ID
    while((fgets(fline,240,fpfile)) != NULL){
      if((fline[0] == '*')||(fline[0] == '#')) continue;     //skip comments    
      sscanf(fline,"%d",&eid);                               //first get the E_ID
      sscanf(fline,"%*d%lg%lg%lg%lg%lg%lg%lg%lg",
	     &polTable[plane][fcount][eid][ENERGY],
	     &polTable[plane][fcount][eid][ENH],
	     &polTable[plane][fcount][eid][ENHERR],
	     &polTable[plane][fcount][eid][ENHFIT],
	     &polTable[plane][fcount][eid][PFIT],
	     &polTable[plane][fcount][eid][PCOR],
	     &polTable[plane][fcount][eid][PCORERR],
	     &polTable[plane][fcount][eid][PSMOOTH]);
      chancount++; 
    }
    fclose(fpfile); //close the file
    if(chancount!=384){
      cerr << "Should be 384 lines in " << filename << " - only got " << chancount << endl;
      return -1;
    }
    polTableN[plane]++;
    
    fcount++;
  }

  fclose(fplist);

  return(0);
}

Double_t GetPol(Int_t plane, Double_t edge, Int_t eid, Int_t poltype = PSMOOTH, Double_t lowThresh=0.2, Double_t highThresh=0.3){
  //get polarization based on eid and edge position

  Int_t eIndex=0;
  Double_t pol=-1.0;
  
  //Check edge in in range of tables
  if((edge<polTable[plane][1][0][EDGE])||(edge>polTable[plane][polTableN[plane]-1][0][EDGE])) return -1.0;
  
  //  cout << "In range" << endl;

  //find index
  for(eIndex=0;eIndex<polTableN[plane];eIndex++){
    if(polTable[plane][eIndex][0][EDGE]>=edge) break;
  }
  //  cout << "Index = " << eIndex << endl;

  pol=polTable[plane][eIndex][eid][poltype];
  if((polTable[plane][eIndex][0][ENERGY]<edge)&&(pol<lowThresh)) pol = -1.0;
  if((polTable[plane][eIndex][0][ENERGY]>edge)&&(pol<highThresh)) pol = -1.0;

  return pol;
}



Double_t GetPol(Int_t plane, Double_t edge, Double_t eg, Int_t poltype = PSMOOTH, Double_t lowThresh=0.2, Double_t highThresh=0.3){
  //get polarization based on ephoton energy and edge position

  Int_t eIndex=0;
  Double_t pol=-1.0;
  Int_t eid=0;
  
  //Check edge in range of tables
  if((edge<polTable[plane][1][0][EDGE])||(edge>polTable[plane][polTableN[plane]-1][0][EDGE])) return -1.0;


  //find index
  for(int eIndex=0;eIndex<polTableN[plane];eIndex++){
    if(polTable[plane][eIndex][0][EDGE]>=edge) break;
  }
  cout << "Index = " << eIndex << endl;

  //find eid
  for(eid=1;eid<=384;eid++){
    if(polTable[plane][eIndex][eid][ENERGY]<=eg) break;
  }
  cout << "eid = " << eid <<endl;

  pol=polTable[plane][eIndex][eid][poltype];
  if((polTable[plane][eIndex][0][ENERGY]<edge)&&(pol<lowThresh)) pol = -1.0;
  if((polTable[plane][eIndex][0][ENERGY]>edge)&&(pol<highThresh)) pol = -1.0;

  return pol;
}

int LoadEdgeTable(Char_t *EdgeTable){
  FILE *fpfile;                      //file pointers for filelist and each file therein
  Char_t fline[250];                 //for reading in lines from file

  cout << "opening " << EdgeTable << "   " << endl;
  if((fpfile=fopen(EdgeTable,"r"))==NULL){              //open file
    cerr << "Error Couldn't open file: " << EdgeTable << endl;
    return -1;
  }


  edgeEventN=0;                      //initialize the counter
  edgeIndex=0;                       //and index for current table
  lastEdgeEvent=0;                   //etc
  lastCohEdge=0.0;
  lastCohPlane=-1;
  
  while((fgets(fline,240,fpfile)) != NULL){
    if((fline[0] == '*')||(fline[0] == '#')) continue;     //skip comments
    sscanf(fline,"%d%d%lg%d",        //scan in the tables
	   &edgeEventLow[edgeEventN],
	   &edgeEventHigh[edgeEventN],
	   &edgeEventEdge[edgeEventN],
	   &edgeEventPlane[edgeEventN]);
    edgeEventN++;                    //inc the counter fo the no of lines in the table
  }
  fclose(fpfile);
  return 0;
}


Int_t GetEdge(Int_t event, Double_t *edge, Int_t *plane){

  //check event no >= previous event
  
  if(event<lastEdgeEvent){
    cerr << "Warning: event (= " << event << ") is earlier than previous event (= " <<  lastEdgeEvent << ") - ignoring" << endl;
    return -1;
  }
  lastEdgeEvent=event;

  //keep going until we're in range or don't find a range 
  while((!((event >= edgeEventLow[edgeIndex])&&(event <= edgeEventHigh[edgeIndex])))&&(edgeIndex<edgeEventN)) edgeIndex++;
  if(edgeIndex>=edgeEventN){
    cerr << "Error: event (= " << event << ") is not in the range of this table" << endl;    
    edgeIndex=0; //reset to zero
    return -1;
  }
  *edge = edgeEventEdge[edgeIndex];
  *plane = edgeEventPlane[edgeIndex];
  return 0;
}

Double_t mikeTest(Double_t pol, Double_t egamma, Double_t cohEdge, Int_t Plane, Int_t setting){
  //where pol is from lookup tables
  //egamma = photon energy in MeV
  //cohEdge = current coherent edge pos
  //plane = PARA (0), PERP (1)
  //setting = 1300,1500,1700,1701,1900,2100  (1701 = 1700 auto)

  Int_t paramSet=-1;
  Double_t eDiff=0.0;
  Double_t pF=1.0;
  Double_t norm=0.0;

  eDiff=(cohEdge-egamma)/1000.0;
  if((eDiff>width)||(eDiff<0.0)) return -1; //return if outwith width

  //get the param set from the setting

  switch(setting){
  case 1300:
    paramSet=0;
    break;
  case 1500:
    paramSet=1;
    break;
  case 1700:
    paramSet=2;
    break;
  case 2100:
    paramSet=3;
    break;
  case 1701:
    paramSet=4;
    break;
  case 1900:
    paramSet=5;
    break;
  default:
    cerr << "Fatal Error: Unknown value on energy_setting " <<  setting << endl;
    return -1.0;
    break;
  }
  
  
  pF=polFix0(eDiff,Plane,paramSet);
  norm=polFix0(diffFix,Plane,paramSet);
  pF /= norm;
  if (pF>0.0) pF=pol/pF;
  else pF=pol;
  
  return pF;
}

Double_t polFix0(Double_t xE, Int_t plane, Int_t parSet){
 
  Double_t yVal=0.0;
  Double_t tmpFun=1.0;
  Int_t region=-1;
  Double_t pF=-1.0;
  
  region=(int)(8.0*(xE)/width); //region 0-8
  
  yVal = (1.0/0.025)*xE-0.5;
  
  tmpFun = 1.0;
  if(region>=1){
    for(int r=1;r<=region;r++){
      tmpFun = tmpFun*(ldPar[parSet][plane][0] + ldPar[parSet][plane][1]*yVal);
      yVal = yVal - 1.0;
    }
  }
  pF = tmpFun;
  if (tmpFun==0.0) pF = 1.0;
  return pF;
}
