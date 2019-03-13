void CreateImpWeights2() {

	// Get the data and sim trees
	TFile *fileData=new TFile("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output/file_all.root");
	TTree *treeData= (TTree*) fileData->Get("HSParticles");
	
	TFile *fileMC=new TFile("/home/louise/g8_phi/haspect/Events/Projects/g8phi/convert_output_MC10M/file_all.root");
	TTree *treeMC= (TTree*) fileMC->Get("HSParticles");

	// Create the importance weights
	THSWeights* impWeights = new THSWeights("impWeights");
	impWeights->SetSpecies("LikeData");
	impWeights->SetIDName("fgID");
	
	// t and Egamma
	impWeights->SetFile("out/impWeights_tE.root");
		
		
	// Egamma
	TH2* weightHist = new TH2F("impWeights","impWeights",100,0.0,2.0, 100,1.5,2.1);
	impWeights->ImportanceSampling2(treeMC, treeData, weightHist, "t:Egamma");
	impWeights->Draw1DWithWeights(treeMC,weightHist,"Egamma");
	impWeights->Draw1DWithWeights(treeMC,weightHist,"t");

	impWeights->PrintWeight();
	
	weightHist->SetLineColor(2);
	weightHist->Draw("hist");
	
	// t
	treeData->Draw("t","","same");
	
	// Egamma
	treeData->Draw("Egamma","","same");
	
	impWeights->Save();
}
