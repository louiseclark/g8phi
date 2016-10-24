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
 * - h2_allPhotonTimeDiff - time difference between photon and proton (all photons)
 * - h2_bestPhotonTimeDiff - time difference between photon and proton (best photon)
 * 	 - proton beta taken from 4-vector with PDG mass assigned
 * - h2_photonEnergy - photon energy for all photons
 * - h2_bestPhotonEnergy - photon energy for best photon
 * 
 * After cut 4
 * - h3_bestPhotonTimeDiff - time difference between photon and proton (best photon)
 * - h3_bestPhotonEnergy - photon energy for best photon
 * - h3_missingMass1 - missing mass of p, K+ and K- (using best photon for proton) - should be zero
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
 * - h4_missingMass1 - missing mass of p, K+ and K- (using best photon for proton) - should be zero
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
#include "extra_packages/eloss_rb/eLoss.h"      

#include <iostream>				// for debugging
//                                                                      *
// ******** end of my headers here **************************************



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
class 	TH2D *h4_KK_pK;
class	TH1D *h4_photonEnergy;

TLorentzVector *lPhoton;
TLorentzVector *lTarget;

// detected particles
TLorentzVector *lProtonDet;
TLorentzVector *lKPlusDet;
TLorentzVector *lKMinusDet;

// particles reconstructed from missing mass
TLorentzVector *lProtonMM;
TLorentzVector *lKPlusMM;
TLorentzVector *lKMinusMM;
TLorentzVector *lNoMM;

// p K+ K- with appropriate Det/MM assigned for channel
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

  
  h2_allPhotonTimeDiff = new TH1D("h2_allPhotonTimeDiff", "Proton - photon time diff - all photons", 1000, -50.0, 50.0);  
  h2_bestPhotonTimeDiff = new TH1D("h2_bestPhotonTimeDiff", "Proton - photon time diff - best photons", 1000, -50.0, 50.0);  		     
  
  h2_photonEnergy = new TH1D("h2_photonEnergy", "Photon Energy", 1000, 0.0, 3.0);
  h2_bestPhotonEnergy = new TH1D("h2_bestPhotonEnergy", "Best Photon Energy", 1000, 0.0, 3.0);
  
  h3_bestPhotonTimeDiff = new TH1D("h3_bestPhotonTimeDiff", "Proton - photon time diff - best photons", 1000, -50.0, 50.0);  		     
  h3_bestPhotonEnergy = new TH1D("h3_bestPhotonEnergy", "Best Photon Energy", 1000, 0.0, 3.0);
  h3_missingMass1 = new TH1D("h3_missingMass1", "Missing Mass (pK+K-)", 1000, -2.0, 2.0);
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
  h4_missingMass1 = new TH1D("h4_missingMass1", "Missing Mass (pK+K-)", 1000, -2.0, 2.0);
  h4_missingMass2 = new TH1D("h4_missingMass2", "Missing Mass (pK+)", 1000, 0.0, 1.6);
  h4_missingMass3 = new TH1D("h4_missingMass3", "Missing Mass (pK-)", 1000, 0.0, 1.6);
  h4_missingMass4 = new TH1D("h4_missingMass4", "Missing Mass (K+K-)", 1000, 0.0, 1.6);
  h4_KPlusMissingMass = new TH1D("h4_KPlusMissingMass", "Mass K+ and Missing K-", 1000, 0.0, 2.0);
  h4_KMinusMissingMass = new TH1D("h4_KMinusMissingMass", "Mass K- and Missing K+", 1000, 0.0, 2.0);
  h4_phi1 = new TH1D("h4_phi1", "Mass K+ K- pid 1", 1000, 0.0, 2.0);
  h4_phi2 = new TH1D("h4_phi2", "Mass K+ K- pid 2", 1000, 0.0, 2.0);
  h4_phi3 = new TH1D("h4_phi3", "Mass K+ K- pid 3", 1000, 0.0, 2.0);
  h4_phi4 = new TH1D("h4_phi4", "Mass K+ K- pid 4", 1000, 0.0, 2.0);
  h4_phiTotal = new TH1D("h4_phiTotal", "Mass K+ K- All events", 1000, 0.0, 2.0);  

  h4_betamomp = new TH2D("h4_betamomp", "Beta vs p +", 500, 0.0, 4.0, 500, 0.0, 1.5);
  h4_betamomp->GetXaxis()->SetTitle("p");
  h4_betamomp->GetYaxis()->SetTitle("beta");

  h4_MM_pKPlus_pPiPlus = new TH2D("h4_MM_pKPlus_pPiPlus", "MM(pK+) vs MM(pPi+)", 200, 0.0, 1.2, 200, 0.0, 1.2);
  h4_MM_pKPlus_pPiPlus->GetXaxis()->SetTitle("MM(pPi+)");
  h4_MM_pKPlus_pPiPlus->GetYaxis()->SetTitle("MM(pK+)");
  h4_MM_pKMinus_pPiMinus = new TH2D("h4_MM_pKMinus_pPiPlus", "MM(pK-) vs MM(pPi-)", 200, 0.0, 1.2, 200, 0.0, 1.2);
  h4_MM_pKMinus_pPiMinus->GetXaxis()->SetTitle("MM(pPi-)");
  h4_MM_pKMinus_pPiMinus->GetYaxis()->SetTitle("MM(pK-)");

  //h4_KK_pMM = new TH2D("h4_KK_pMM", "Mass (K+ and Missing Mass) vs Mass (p and Missing Mass)", 200, 1.2, 2.5, 200, 0.7, 2.5);
  //h4_KK_pMM->GetXaxis()->SetTitle("Mass (p and K-)");
  //h4_KK_pMM->GetYaxis()->SetTitle("Mass (K+ and Missing Mass)");
  //h4_KK_pMM->GetXaxis()->SetNdivisions(14);
  //h4_KK_pMM->GetYaxis()->SetNdivisions(18);

  h4_KK_pK = new TH2D("h4_KK_pK", "Mass (K+ and K-) vs Mass (p and K-)", 200, 1.2, 2.1, 200, 0.7, 1.8);
  h4_KK_pK->GetXaxis()->SetTitle("Mass (p and K-)");
  h4_KK_pK->GetYaxis()->SetTitle("Mass (K+ and K-)");
  //h4_KK_pK->GetXaxis()->SetNdivisions(14);
  //h4_KK_pK->GetYaxis()->SetNdivisions(18);
  h4_photonEnergy = new TH1D("h4_photonEnergy", "Photon Energy", 1000, 0.0, 3.0);

  // 4 vectors
  lPhoton = new TLorentzVector(1.0,1.0,1.0,1.0);
  lTarget = new TLorentzVector(1.0,1.0,1.0,1.0);
  lTarget->SetXYZM(0,0,0,PROTON_MASS);

  // detected particles
  lProtonDet = new TLorentzVector(1.0,1.0,1.0,1.0);
  lKPlusDet = new TLorentzVector(1.0,1.0,1.0,1.0);
  lKMinusDet = new TLorentzVector(1.0,1.0,1.0,1.0);

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

		     
  //                                                                    *
  // ******* End of User initialisation  ********************************


  // ********************** main file loop **********************************************************
  //                                                                                                *
  while((fileNo=getNextFile(inFile,file))!=-1){  // loop while files are still avialable 

    // Start of user stuff to do before sorting each file  ---------------
    fprintf(stderr,"Sorting file - %s\n",inFile);
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
      if (pid==1) {
		  // p K+ K- detected
		  lProtonDet->SetXYZM(EVNT[rowProton].Cx*EVNT[rowProton].Pmom,EVNT[rowProton].Cy*EVNT[rowProton].Pmom,EVNT[rowProton].Cz*EVNT[rowProton].Pmom,PROTON_MASS);
		  lKPlusDet->SetXYZM(EVNT[rowKPlus].Cx*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cy*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cz*EVNT[rowKPlus].Pmom,KAON_CHARGED_MASS);
		  lKMinusDet->SetXYZM(EVNT[rowKMinus].Cx*EVNT[rowKMinus].Pmom,EVNT[rowKMinus].Cy*EVNT[rowKMinus].Pmom,EVNT[rowKMinus].Cz*EVNT[rowKMinus].Pmom,KAON_CHARGED_MASS);
	  }
	  else if (pid==2) {
		  // missing K-
		  lProtonDet->SetXYZM(EVNT[rowProton].Cx*EVNT[rowProton].Pmom,EVNT[rowProton].Cy*EVNT[rowProton].Pmom,EVNT[rowProton].Cz*EVNT[rowProton].Pmom,PROTON_MASS);
		  lKPlusDet->SetXYZM(EVNT[rowKPlus].Cx*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cy*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cz*EVNT[rowKPlus].Pmom,KAON_CHARGED_MASS);
	  }
	  else if (pid==3) {
		  // missing K+
		  lProtonDet->SetXYZM(EVNT[rowProton].Cx*EVNT[rowProton].Pmom,EVNT[rowProton].Cy*EVNT[rowProton].Pmom,EVNT[rowProton].Cz*EVNT[rowProton].Pmom,PROTON_MASS);
		  lKMinusDet->SetXYZM(EVNT[rowKMinus].Cx*EVNT[rowKMinus].Pmom,EVNT[rowKMinus].Cy*EVNT[rowKMinus].Pmom,EVNT[rowKMinus].Cz*EVNT[rowKMinus].Pmom,KAON_CHARGED_MASS);
	  }
	  else if (pid==4) {
		  // missing p
		  lKPlusDet->SetXYZM(EVNT[rowKPlus].Cx*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cy*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cz*EVNT[rowKPlus].Pmom,KAON_CHARGED_MASS);
		  lKMinusDet->SetXYZM(EVNT[rowKMinus].Cx*EVNT[rowKMinus].Pmom,EVNT[rowKMinus].Cy*EVNT[rowKMinus].Pmom,EVNT[rowKMinus].Cz*EVNT[rowKMinus].Pmom,KAON_CHARGED_MASS);
	  }

	  if (numKPlus != 0) {
		  // Maybe the K+ is actually a Pi+
		  lPiPlus->SetXYZM(EVNT[rowKPlus].Cx*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cy*EVNT[rowKPlus].Pmom,EVNT[rowKPlus].Cz*EVNT[rowKPlus].Pmom,PI_CHARGED_MASS);  
	  }
	  
	  if (numKMinus != 0) {
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
	  
	  // Now assign the 4 vectors to use depending on the channel
	  lPhoton->SetPxPyPzE(0,0,TAGR[bestPhotonRow].ERG,TAGR[bestPhotonRow].ERG);
	  if (pid==1) {
		  // p K+ K- detected
		  *lProton = *lProtonDet;
		  *lKPlus = *lKPlusDet;
		  *lKMinus = *lKMinusDet;
		  *lNoMM = (*lTarget + *lPhoton) - (*lProton + *lKPlus + *lKMinus);
		  *lPhi = *lKPlus + *lKMinus;
		  h3_missingMass1->Fill(lNoMM->M());

		  // CUT 5
		  // Only keep events where missing mass is close to mass of expected particle (zero in this case)
	      if ((lNoMM->M() < -0.1) || (lNoMM->M() > 0.1)) continue;

		  h4_missingMass1->Fill(lNoMM->M());
		  
	      // Plot the phi for this pid
	      h4_phi1->Fill(lPhi->M());
		  
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

h4_KK_pK->Draw("COLZ");
c1->SetGrid();
c1->SaveAs("h4_KK_pK.gif");
c1->Clear();     

h4_photonEnergy->Draw();
c1->SaveAs("h4_photonEnergy.gif");
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
