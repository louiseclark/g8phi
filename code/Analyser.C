{
	
//	DIR *pdir = NULL; 
//	pdir = opendir ("/home/louise/g8_phi/sWeights/hist/out"); 
//	struct dirent *pent = NULL;
//	while (pent = readdir (pdir)) {
//		cout << pent->d_name << endl;
//	}
//	closedir (pdir);

	boolean	binsMethod = false;
	
	if (binsMethod) {
	
		TFile *infile = TFile::Open("/home/louise/g8_phi/sWeights/hist/out/file_all.root");
		TFile* outfile = new TFile("out/analyseBins_all.root","recreate");	
		
		TIter 	nextWeightsDir(infile->GetListOfKeys());
		TKey	*weightsKey;
		
		while ((weightsKey=(TKey*)nextWeightsDir())) {
		
			TDirectory *weightsDir = infile->GetDirectory(weightsKey->GetName());		
			TIter	nextTbinDir(weightsDir->GetListOfKeys());
			TKey 	*tbinKey;
			outfile->mkdir(weightsKey->GetName());
			TDirectory *weightsDirOut = outfile->GetDirectory(weightsKey->GetName());
			
			while ((tbinKey=(TKey*)nextTbinDir())) {
			
				TDirectory *tbinDir = weightsDir->GetDirectory(tbinKey->GetName());
				TIter	nextEbinDir(tbinDir->GetListOfKeys());
				TKey 	*ebinKey;
				weightsDirOut->mkdir(tbinKey->GetName());
				TDirectory *tbinDirOut = weightsDirOut->GetDirectory(tbinKey->GetName());
				
				while ((ebinKey=(TKey*)nextEbinDir())) {
					
					TDirectory *ebinDir = tbinDir->GetDirectory(ebinKey->GetName());
					tbinDirOut->mkdir(ebinKey->GetName());
					TDirectory *ebinDirOut = tbinDirOut->GetDirectory(ebinKey->GetName());
									
					TH1F* phiPhiCmPARA = (TH1F*)ebinDir->Get("phiPhiCmPARA");
					TH1F* phiPhiCmPERP = (TH1F*)ebinDir->Get("phiPhiCmPERP"); 
					
					phiPhiCmPARA->Rebin(2);
					phiPhiCmPERP->Rebin(2);
					
					TH1 *phiPhiAsym = phiPhiCmPARA->GetAsymmetry(phiPhiCmPERP);
					phiPhiAsym->SetTitle("phiPhiAsym");
					phiPhiAsym->SetName("phiPhiAsym");
					
					ebinDirOut->cd();
					phiPhiAsym->Write();
				}
			}
		}
		outfile->Close();
		
	}	
	
	else {
		TFile *infile = TFile::Open("/home/louise/g8_phi/sWeights/hist/out/hists_all.root");
		TFile* outfile = new TFile("out/analyse_all.root","recreate");	
		string dirs[3] = {"Signal", "BG", "All"};
		
		for (int i=0; i<3; i++) {
		
			infile->cd(dirs[i].c_str());
			
			TH1F* phiPhiCmPARA = (TH1F*)gDirectory->Get("phiPhiCmPARA");
			TH1F* phiPhiCmPERP = (TH1F*)gDirectory->Get("phiPhiCmPERP"); 
			
			phiPhiCmPARA->Rebin(2);
			phiPhiCmPERP->Rebin(2);
			
			TH1 *phiPhiAsym = phiPhiCmPARA->GetAsymmetry(phiPhiCmPERP);
			phiPhiAsym->SetTitle("phiPhiAsym");
			phiPhiAsym->SetName("phiPhiAsym");

			TH1F* phiProtonPARA = (TH1F*)gDirectory->Get("phiProtonPARA");
			TH1F* phiProtonPERP = (TH1F*)gDirectory->Get("phiProtonPERP"); 
			
			phiProtonPARA->Rebin(2);
			phiProtonPERP->Rebin(2);
			
			TH1 *phiProtonAsym = phiProtonPARA->GetAsymmetry(phiProtonPERP);
			phiProtonAsym->SetTitle("phiProtonAsym");
			phiProtonAsym->SetName("phiProtonAsym");
			
			TF1* cos2phiPhi=new TF1("cos2phiPhi","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
			phiPhiAsym->Fit("cos2phiPhi");
			TF1* cos2phiProton=new TF1("cos2phiProton","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
			phiProtonAsym->Fit("cos2phiProton");
			
			TH1F* phiKpHelPARA = (TH1F*)gDirectory->Get("phiKpHelPARA");
			TF1* wPhiPARA = new TF1("wPhiPARA","[0]+[1]*(1-[2]*cos(2*x+[3]))", -3.2,3.2);
			phiKpHelPARA->Fit("wPhiPARA");

			TH1F* phiKpHelPERP = (TH1F*)gDirectory->Get("phiKpHelPERP");
			TF1* wPhiPERP = new TF1("wPhiPERP","[0]+[1]*(1-[2]*cos(2*x+[3]))", -3.2,3.2);
			phiKpHelPERP->Fit("wPhiPERP");

			
			outfile->mkdir(dirs[i].c_str());
			outfile->cd(dirs[i].c_str());

			phiPhiAsym->Write();
			phiProtonAsym->Write();
			phiKpHelPARA->Write();
			phiKpHelPERP->Write();
			outfile->cd("..");
		}
		outfile->Close();
	//}

}
