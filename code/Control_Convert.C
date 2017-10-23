{
  //Set up the input chain to be analysed
   TChain* tree=new TChain("HSParticles","datachain");
   
   
   // All files
   tree->Add("/home/louise/rootbeer2.2/analysis/g8phi/rootbeer_output/all_trees/*");
   
   // One file per topology
   // KpKm
   //tree->Add("/home/louise/rootbeer2.2/analysis/g8phi/rootbeer_output/all_trees/fileKpKm*");
   // pKm
   //tree->Add("/home/louise/rootbeer2.2/analysis/g8phi/rootbeer_output/all_trees/filepKm*");
   // pKp
   //tree->Add("/home/louise/rootbeer2.2/analysis/g8phi/rootbeer_output/all_trees/filepKp_*");
   // pKpKm
   //tree->Add("/home/louise/rootbeer2.2/analysis/g8phi/rootbeer_output/all_trees/filepKpKm*");
   
   // One file per edge setting
   //tree->Add("/home/louise/rootbeer2.2/analysis/g8phi/rootbeer_output/all_trees/file*17.root");
   //tree->Add("/home/louise/rootbeer2.2/analysis/g8phi/rootbeer_output/all_trees/file*19.root");
   //tree->Add("/home/louise/rootbeer2.2/analysis/g8phi/rootbeer_output/all_trees/file*21.root");
   
   //Set the ouput directory or file (if full file name is given just 1 file is created, while if a directory it will be created and filled with files of the same name as the input 
   gSystem->Setenv("HSOUT",
// All files in one file   
		   "/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/file_all.root");
// All files in individual files		   
//		   "/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output");

// One file per topology
// KpKm   
//		   "/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/file_KpKm.root");
// pKm   
//		   "/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/file_pKm.root");
// pKp   
//		   "/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/file_pKp.root");
// pKpKm   
//		   "/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/file_pKpKm.root");

// One file per edge setting
//		   "/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/file_17.root");
//		   "/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/file_19.root");
//		   "/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/file_21.root");


   
   //Configure the analysis (i.e PROOF, source compilation...)
   TString HSANA=gSystem->Getenv("HSANA");
   gROOT->LoadMacro(HSANA+"/HSControl.C");
   TProof* prf=HSControl(kFALSE, kFALSE); //where the proof stuff is set, kTRUE means use PROOF
   
   //Use an event list from a previous analysis 
   //If only 1 file was output the event list will be in its top directory, so just give the file name
   //If many files were ouput the event list will be in the same directory with the name ParentEventList.root
   //HSEntryList("/PATH TO PERVIOUS EVENTLIST/ParentEventList.root");

   tree->Process("Convert.C++");

   //Not actually sure if this is needed or not...
   if(prf)prf->ClearData(TProof::kUnregistered | TProof::kForceClear);
 }
