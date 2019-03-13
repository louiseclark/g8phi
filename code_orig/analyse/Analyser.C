TCanvas *c2=new TCanvas("c2","c2",800,600);
bool doHist = true;

void DoFits(TDirectory *inDir, TDirectory *mcDir, TDirectory *outDir){	
	
	int rebin_n = 4;
	
	// acceptance correct 
	TH1F* phiKpHelPARA2 = (TH1F*)inDir->Get("phiKpHelPARA");
	TH1F* mcphiKpHelPARA = (TH1F*)mcDir->Get("phiKpHelPARA");
	
	int scaleFactor = 1/(mcphiKpHelPARA->GetBinContent(mcphiKpHelPARA->GetMaximumBin()));
	TH1F *scaledphiKpHelPARA = mcphiKpHelPARA;
	scaledphiKpHelPARA->Scale(scaleFactor);
	TH1F *corrphiKpHelPARA = phiKpHelPARA2;
	corrphiKpHelPARA->Divide(scaledphiKpHelPARA);
	corrphiKpHelPARA->SetTitle("corrphiKpHelPARA");
	corrphiKpHelPARA->SetName("corrphiKpHelPARA");
	
	
	// beam asymmetry all events
	
	// phi
	TH1F* phiPhiCmPARA = (TH1F*)inDir->Get("phiPhiCmPARA");
	TH1F* phiPhiCmPERP = (TH1F*)inDir->Get("phiPhiCmPERP"); 
	
	phiPhiCmPARA->Sumw2();
	phiPhiCmPERP->Sumw2();
	phiPhiCmPARA->Rebin(rebin_n);
	phiPhiCmPERP->Rebin(rebin_n);
	
	TH1 *phiPhiAsym = phiPhiCmPARA->GetAsymmetry(phiPhiCmPERP);
	phiPhiAsym->SetTitle("phiPhiAsym");
	phiPhiAsym->SetName("phiPhiAsym");
	
	TF1* cos2phiPhi=new TF1("cos2phiPhi","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	phiPhiAsym->Fit("cos2phiPhi");

	// proton
	TH1F* phiProtonPARA = (TH1F*)inDir->Get("phiProtonPARA");
	TH1F* phiProtonPERP = (TH1F*)inDir->Get("phiProtonPERP"); 
	
	phiProtonPARA->Sumw2();
	phiProtonPERP->Sumw2();
	phiProtonPARA->Rebin(rebin_n);
	phiProtonPERP->Rebin(rebin_n);
	
	TH1 *phiProtonAsym = phiProtonPARA->GetAsymmetry(phiProtonPERP);
	phiProtonAsym->SetTitle("phiProtonAsym");
	phiProtonAsym->SetName("phiProtonAsym");
	
	TF1* cos2phiProton=new TF1("cos2phiProton","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	phiProtonAsym->Fit("cos2phiProton");

	// K+
	TH1F* phiKpCmPARA = (TH1F*)inDir->Get("phiKpCmPARA");
	TH1F* phiKpCmPERP = (TH1F*)inDir->Get("phiKpCmPERP"); 
	
	phiKpCmPARA->Sumw2();
	phiKpCmPERP->Sumw2();
	phiKpCmPARA->Rebin(rebin_n);
	phiKpCmPERP->Rebin(rebin_n);
	
	TH1 *phiKpAsym = phiKpCmPARA->GetAsymmetry(phiKpCmPERP);
	phiKpAsym->SetTitle("phiKpAsym");
	phiKpAsym->SetName("phiKpAsym");
	
	TF1* cos2phiKp=new TF1("cos2phiKp","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	phiKpAsym->Fit("cos2phiKp");
	
	// K-
	TH1F* phiKmCmPARA = (TH1F*)inDir->Get("phiKmCmPARA");
	TH1F* phiKmCmPERP = (TH1F*)inDir->Get("phiKmCmPERP"); 
	
	phiKmCmPARA->Sumw2();
	phiKmCmPERP->Sumw2();
	phiKmCmPARA->Rebin(rebin_n);
	phiKmCmPERP->Rebin(rebin_n);
	
	TH1 *phiKmAsym = phiKmCmPARA->GetAsymmetry(phiKmCmPERP);
	phiKmAsym->SetTitle("phiKmAsym");
	phiKmAsym->SetName("phiKmAsym");
	
	TF1* cos2phiKm=new TF1("cos2phiKm","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	phiKmAsym->Fit("cos2phiKm");	
	
	// Lambda
	TH1F* phiLamCmPARA = (TH1F*)inDir->Get("phiLamCmPARA");
	TH1F* phiLamCmPERP = (TH1F*)inDir->Get("phiLamCmPERP"); 
	
	phiLamCmPARA->Sumw2();
	phiLamCmPERP->Sumw2();
	phiLamCmPARA->Rebin(rebin_n);
	phiLamCmPERP->Rebin(rebin_n);
	
	TH1 *phiLamAsym = phiLamCmPARA->GetAsymmetry(phiLamCmPERP);
	phiLamAsym->SetTitle("phiLamAsym");
	phiLamAsym->SetName("phiLamAsym");
	
	TF1* cos2phiLam=new TF1("cos2phiLam","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	phiLamAsym->Fit("cos2phiLam");

	// beam asymmetry (lambda events)
	
	// phi
	TH1F* LphiPhiCmPARA = (TH1F*)inDir->Get("LphiPhiCmPARA");
	TH1F* LphiPhiCmPERP = (TH1F*)inDir->Get("LphiPhiCmPERP"); 
	
	LphiPhiCmPARA->Sumw2();
	LphiPhiCmPERP->Sumw2();
	LphiPhiCmPARA->Rebin(rebin_n);
	LphiPhiCmPERP->Rebin(rebin_n);
	
	TH1 *LphiPhiAsym = LphiPhiCmPARA->GetAsymmetry(LphiPhiCmPERP);
	LphiPhiAsym->SetTitle("LphiPhiAsym");
	LphiPhiAsym->SetName("LphiPhiAsym");
	
	TF1* Lcos2phiPhi=new TF1("Lcos2phiPhi","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	LphiPhiAsym->Fit("Lcos2phiPhi");
	
	// K+
	TH1F* LphiKpCmPARA = (TH1F*)inDir->Get("LphiKpCmPARA");
	TH1F* LphiKpCmPERP = (TH1F*)inDir->Get("LphiKpCmPERP"); 
	
	LphiKpCmPARA->Sumw2();
	LphiKpCmPERP->Sumw2();
	LphiKpCmPARA->Rebin(rebin_n);
	LphiKpCmPERP->Rebin(rebin_n);
	
	TH1 *LphiKpAsym = LphiKpCmPARA->GetAsymmetry(LphiKpCmPERP);
	LphiKpAsym->SetTitle("LphiKpAsym");
	LphiKpAsym->SetName("LphiKpAsym");
	
	TF1* Lcos2phiKp=new TF1("Lcos2phiKp","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	LphiKpAsym->Fit("Lcos2phiKp");
	
	// K-
	TH1F* LphiKmCmPARA = (TH1F*)inDir->Get("LphiKmCmPARA");
	TH1F* LphiKmCmPERP = (TH1F*)inDir->Get("LphiKmCmPERP"); 
	
	LphiKmCmPARA->Sumw2();
	LphiKmCmPERP->Sumw2();
	LphiKmCmPARA->Rebin(rebin_n);
	LphiKmCmPERP->Rebin(rebin_n);
	
	TH1 *LphiKmAsym = LphiKmCmPARA->GetAsymmetry(LphiKmCmPERP);
	LphiKmAsym->SetTitle("LphiKmAsym");
	LphiKmAsym->SetName("LphiKmAsym");
	
	TF1* Lcos2phiKm=new TF1("Lcos2phiKm","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	LphiKmAsym->Fit("Lcos2phiKm");	
	
	// Lambda
	TH1F* LphiLamCmPARA = (TH1F*)inDir->Get("LphiLamCmPARA");
	TH1F* LphiLamCmPERP = (TH1F*)inDir->Get("LphiLamCmPERP"); 
	
	LphiLamCmPARA->Sumw2();
	LphiLamCmPERP->Sumw2();
	LphiLamCmPARA->Rebin(rebin_n);
	LphiLamCmPERP->Rebin(rebin_n);
	
	TH1 *LphiLamAsym = LphiLamCmPARA->GetAsymmetry(LphiLamCmPERP);
	LphiLamAsym->SetTitle("LphiLamAsym");
	LphiLamAsym->SetName("LphiLamAsym");
	
	TF1* Lcos2phiLam=new TF1("Lcos2phiLam","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	LphiLamAsym->Fit("Lcos2phiLam");	
	
	// beam asymmetry (phi events)
	
	// phi
	TH1F* PphiPhiCmPARA = (TH1F*)inDir->Get("PphiPhiCmPARA");
	TH1F* PphiPhiCmPERP = (TH1F*)inDir->Get("PphiPhiCmPERP"); 
	
	PphiPhiCmPARA->Sumw2();
	PphiPhiCmPERP->Sumw2();
	PphiPhiCmPARA->Rebin(rebin_n);
	PphiPhiCmPERP->Rebin(rebin_n);
	
	TH1 *PphiPhiAsym = PphiPhiCmPARA->GetAsymmetry(PphiPhiCmPERP);
	PphiPhiAsym->SetTitle("PphiPhiAsym");
	PphiPhiAsym->SetName("PphiPhiAsym");
	
	TF1* Pcos2phiPhi=new TF1("Pcos2phiPhi","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	PphiPhiAsym->Fit("Pcos2phiPhi");
	
	// K+
	TH1F* PphiKpCmPARA = (TH1F*)inDir->Get("PphiKpCmPARA");
	TH1F* PphiKpCmPERP = (TH1F*)inDir->Get("PphiKpCmPERP"); 
	
	PphiKpCmPARA->Sumw2();
	PphiKpCmPERP->Sumw2();
	PphiKpCmPARA->Rebin(rebin_n);
	PphiKpCmPERP->Rebin(rebin_n);
	
	TH1 *PphiKpAsym = PphiKpCmPARA->GetAsymmetry(PphiKpCmPERP);
	PphiKpAsym->SetTitle("PphiKpAsym");
	PphiKpAsym->SetName("PphiKpAsym");
	
	TF1* Pcos2phiKp=new TF1("Pcos2phiKp","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	PphiKpAsym->Fit("Pcos2phiKp");
	
	// K-
	TH1F* PphiKmCmPARA = (TH1F*)inDir->Get("PphiKmCmPARA");
	TH1F* PphiKmCmPERP = (TH1F*)inDir->Get("PphiKmCmPERP"); 
	
	PphiKmCmPARA->Sumw2();
	PphiKmCmPERP->Sumw2();
	PphiKmCmPARA->Rebin(rebin_n);
	PphiKmCmPERP->Rebin(rebin_n);
	
	TH1 *PphiKmAsym = PphiKmCmPARA->GetAsymmetry(PphiKmCmPERP);
	PphiKmAsym->SetTitle("PphiKmAsym");
	PphiKmAsym->SetName("PphiKmAsym");
	
	TF1* Pcos2phiKm=new TF1("Pcos2phiKm","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	PphiKmAsym->Fit("Pcos2phiKm");	
	
	// Lambda
	TH1F* PphiLamCmPARA = (TH1F*)inDir->Get("PphiLamCmPARA");
	TH1F* PphiLamCmPERP = (TH1F*)inDir->Get("PphiLamCmPERP"); 
	
	PphiLamCmPARA->Sumw2();
	PphiLamCmPERP->Sumw2();
	PphiLamCmPARA->Rebin(rebin_n);
	PphiLamCmPERP->Rebin(rebin_n);
	
	TH1 *PphiLamAsym = PphiLamCmPARA->GetAsymmetry(PphiLamCmPERP);
	PphiLamAsym->SetTitle("PphiLamAsym");
	PphiLamAsym->SetName("PphiLamAsym");
	
	TF1* Pcos2phiLam=new TF1("Pcos2phiLam","[0]+[1]*cos((2*x+[2]))",-3.2,3.2); 
	PphiLamAsym->Fit("Pcos2phiLam");	
	
	// phi distribution of K+ in helicity frame
	TH1F* phiKpHelPARA = (TH1F*)inDir->Get("phiKpHelPARA");
	//TF1* wPhiPARA = new TF1("wPhiPARA","[0]+[1]*(1-[2]*cos(2*x+[3]))", -3.2,3.2);
	TF1* wPhiPARA = new TF1("wPhiPARA","[0]*(1-[1]*cos(2*x+[2]))", -3.2,3.2);
	phiKpHelPARA->Fit("wPhiPARA");

	TH1F* phiKpHelPERP = (TH1F*)inDir->Get("phiKpHelPERP");
	//TF1* wPhiPERP = new TF1("wPhiPERP","[0]+[1]*(1-[2]*cos(2*x+[3]))", -3.2,3.2);
	TF1* wPhiPERP = new TF1("wPhiPERP","[0]*(1-[1]*cos(2*x+[2]))", -3.2,3.2);
	phiKpHelPERP->Fit("wPhiPERP");
	
	// cos theta distribution of K+ in GJ frame
	TH1F* cosThetaKpGJPARA = (TH1F*)inDir->Get("cosThetaKpGJPARA");
	TF1* wCosThetaPARA = new TF1("wCosThetaPARA","[0]*(0.5*(1-[1])*(1-x*x)+[1]*x*x)", -3.2,3.2);
	cosThetaKpGJPARA->Fit("wCosThetaPARA");

	TH1F* cosThetaKpGJPERP = (TH1F*)inDir->Get("cosThetaKpGJPERP");
	TF1* wCosThetaPERP = new TF1("wCosThetaPERP","[0]*(0.5*(1-[1])*(1-x*x)+[1]*x*x)", -3.2,3.2);
	cosThetaKpGJPERP->Fit("wCosThetaPERP");
	
	outDir->cd();
	cout << "Writing to outDir " << outDir->GetName() << endl;
	corrphiKpHelPARA->Write();
	phiPhiAsym->Write();
	phiProtonAsym->Write();
	phiKpAsym->Write();
	phiKmAsym->Write();
	phiLamAsym->Write();
	LphiPhiAsym->Write();
	LphiKpAsym->Write();
	LphiKmAsym->Write();
	LphiLamAsym->Write();
	PphiPhiAsym->Write();
	PphiKpAsym->Write();
	PphiKmAsym->Write();
	PphiLamAsym->Write();
	phiKpHelPARA->Write();
	phiKpHelPERP->Write();
	cosThetaKpGJPARA->Write();
	cosThetaKpGJPERP->Write();
	
	TCanvas *can = new TCanvas("can","can",800,600);
	phiPhiAsym->Draw();
	can->SaveAs((TString)outDir->GetName()+"phiPhiAsym.gif");
	can->Clear();
	
	if (doHist) {
		gStyle->SetOptFit();
		c2->Clear();
		c2->Divide(4,3);
		c2->cd(1);
		phiPhiAsym->Draw();
		c2->cd(2);
		phiProtonAsym->Draw();
		c2->cd(3);
		c2->SaveAs("sigHists.png");
		doHist = false;
	}
	
}
void Analyser() {
	
//	DIR *pdir = NULL; 
//	pdir = opendir ("/home/louise/g8_phi/sWeights/hist/out"); 
//	struct dirent *pent = NULL;
//	while (pent = readdir (pdir)) {
//		cout << pent->d_name << endl;
//	}
//	closedir (pdir);

	bool doBins = false;	
	
	if (doBins) {
		
		TFile *infile = TFile::Open("/home/louise/g8_phi/sWeights/hist/out/hists_costh_top.root");
		TFile* outfile = new TFile("out/analyse_costh_top.root","recreate");	
		
		//TIter 	nextWeightsDir(infile->GetListOfKeys());
		//TKey	*weightsKey;
		string dirs[3] = {"Signal", "BG", "All"};
		
		//while ((weightsKey=(TKey*)nextWeightsDir())) {
		for (int i=0; i<3; i++) {
					
			//TDirectory *weightsDir = infile->GetDirectory(weightsKey->GetName());	
			TDirectory *weightsDir = infile->GetDirectory(dirs[i].c_str());	
			TIter	nextTbinDir(weightsDir->GetListOfKeys());
			TKey 	*tbinKey;
			//outfile->mkdir(weightsKey->GetName());
			//TDirectory *weightsDirOut = outfile->GetDirectory(weightsKey->GetName());
			outfile->mkdir(dirs[i].c_str());
			TDirectory *weightsDirOut = outfile->GetDirectory(dirs[i].c_str());
			
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
					
					//DoFits(ebinDir, ebinDirOut);					
									
				}
			}
		}
		
		// sum the t bins, i.e. binned only in E
		//TDirectory *signalDir = infile->GetDirectory("Signal");
		//TIter	nextTbinDir(signalDir->GetListOfKeys());
		//TKey 	*tbinKey;
		//outfile->cd("Signal");
		//outfile->GetDirectory("Signal")->mkdir("Summed_tBins");
		//TDirectory *sumtBinsDir = outfile->GetDirectory("Signal")->GetDirectory("Summed_tBins");
		
		//while ((tbinKey=(TKey*)nextTbinDir())) {
			
			//TDirectory *tbinDir = signalDir->GetDirectory(tbinKey->GetName());
			//TIter	nextEbinDir(tbinDir->GetListOfKeys());
			//TKey 	*ebinKey;
			
			//// Get the first hist to start adding to
			//ebinKey=(TKey*)nextEbinDir();
			//TDirectory *ebinDir = tbinDir->GetDirectory(ebinKey->GetName());
			//TH1F* phiPhiCmPARA = (TH1F*)ebinDir->Get("phiPhiCmPARA");
			
			//while ((ebinKey=(TKey*)nextEbinDir())) {
			
				//ebinDir = tbinDir->GetDirectory(ebinKey->GetName());
				//phiPhiCmPARA->Add((TH1F*)ebinDir->Get("phiPhiCmPARA"));
			//}
						
			//sumtBinsDir->cd();
			//phiPhiCmPARA->Write();

		//}
		
		outfile->Close();
	}
	else {
		
		TFile *infile = TFile::Open("/home/louise/g8_phi/sWeights/hist/out/hists_all.root");
		TFile *mcfile = TFile::Open("/home/louise/g8_phi/sWeights/hist/outMC/hists_all.root");
		TFile* outfile = new TFile("out2/analyse_nobins.root","recreate");	
		string dirs[3] = {"Signal", "BG", "All"};
		
		for (int i=0; i<3; i++) {
			
			TDirectory *inDir = infile->GetDirectory(dirs[i].c_str());
			TDirectory *mcDir = mcfile->GetDirectory(dirs[i].c_str());
			cout << dirs[i].c_str() << endl;
			outfile->mkdir(dirs[i].c_str());
			TDirectory *outDir = outfile->GetDirectory(dirs[i].c_str());
			DoFits(inDir, mcDir, outDir);
			//outfile->cd("..");
			
			//if (i==0) {
				//TCanvas *c2=new TCanvas("c2","c2",800,600);
				//c2->Divide(4,3);
				//c2->cd(1);
				//TH1F* sig1 = (TH1F*)outfile->GetDirectory(dirs[i].c_str())->Get("phiPhiAsym");
				//sig1->Draw();
				//c2->cd(2);
				//TH1F* sig2 = (TH1F*)outfile->GetDirectory(dirs[i].c_str())->Get("phiProtonAsym");
				//sig2->Draw();
				//c2->cd(3);
				//c2->SaveAs("sigHists.gif");
				////c1->Clear();
			//}
			
		}
		outfile->Close();
		
	}
	//c2->SaveAs("sigHists.gif");
}

