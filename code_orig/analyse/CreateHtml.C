void CreateHtml() {
	
	ofstream outHtml;
	outHtml.open("outhtml/summary.html");
	TCanvas *can = new TCanvas("can","can",800,600);
	gStyle->SetOptFit();
	
	outHtml << "<HTML>" << endl;
	outHtml << "<head>" << endl;
	outHtml << "<style>" << endl;
	outHtml << "body {" << endl;
	outHtml << "    background-color: white;" << endl;
	outHtml << "}" << endl;
	outHtml << "" << endl;
	outHtml << "h1 {" << endl;
	outHtml << "    color: black;" << endl;
	outHtml << "    margin-left: 40px;" << endl;
	outHtml << "    background-color: lightgrey;" << endl;
	outHtml << "}" << endl;
	outHtml << "td {" << endl;
	outHtml << "    border: 1px solid black;" << endl;
	outHtml << "    text-align: center;" << endl;
	outHtml << "}" << endl;
	outHtml << "</style>" << endl;
	outHtml << "</head> " << endl;
	outHtml << "<body>" << endl;
	outHtml << "<TABLE>" << endl;
	
	bool doBins = true;	
	
	if (doBins) {
		
		TFile *infile = TFile::Open("analyse_Ecosth.root");
		string dirs[3] = {"Signal", "BG", "All"};
		vector<TString> histNames;
		bool storeHistNames = true;
		
		// all histograms for each bin
		for (int i=0; i<3; i++) {
					
			TDirectory *weightsDir = infile->GetDirectory(dirs[i].c_str());	
			TIter	nextTbinDir(weightsDir->GetListOfKeys());
			TKey 	*tbinKey;
			
			while ((tbinKey=(TKey*)nextTbinDir())) {
			
				TDirectory *tbinDir = weightsDir->GetDirectory(tbinKey->GetName());
				TIter	nextEbinDir(tbinDir->GetListOfKeys());
				TKey 	*ebinKey;
				
				while ((ebinKey=(TKey*)nextEbinDir())) {
					
					TDirectory *ebinDir = tbinDir->GetDirectory(ebinKey->GetName());
					TIter	nextHist(ebinDir->GetListOfKeys());
					TKey	*histKey;
					TString heading = dirs[i]+" "+(TString)tbinDir->GetName()+" "+(TString)ebinDir->GetName();
					outHtml << "<TR><TD><h1>"+heading+"</h1>" << endl;
					outHtml << "<TR><TD>" << endl;
					outHtml << "<TABLE>" << endl;
					
					int histNum = 0;
					while ((histKey=(TKey*)nextHist())) {
									
						if (histNum%4==0) outHtml << "<TR>"; // new row every 4 hists		
						histNum++;
						
						TH1F* hist = (TH1F*) histKey->ReadObj();
						hist->Draw();
						TString imFileName = dirs[i]+(TString)tbinDir->GetName()+(TString)ebinDir->GetName()+histKey->GetName()+".gif";
						can->SaveAs("outhtml/"+imFileName);
						can->Clear();	
						outHtml << "<td><image src=\""+imFileName+"\" width=\"80%\"></image>" << endl;
						
						if (storeHistNames) histNames.push_back(histKey->GetName());

					}					
					outHtml << "</TABLE>" << endl;
					storeHistNames = false; // only need to do this first time round
				}
			}
		}
		
		// all bins for each histogram
		for (int histNum=0; histNum < histNames.size(); histNum++) {

			for (int i=0; i<3; i++) {

				outHtml << "<h1>"+histNames[histNum]+"</h1>";
				outHtml << "<TABLE>" << endl;
				outHtml << "<TR><TD><h1>"+dirs[i]+"</h1>" << endl;
				TDirectory *weightsDir = infile->GetDirectory(dirs[i].c_str());	
				TIter	nextTbinDir(weightsDir->GetListOfKeys());
				TKey 	*tbinKey;
				bool writeHead = true;
				
				while ((tbinKey=(TKey*)nextTbinDir())) {

					TDirectory *tbinDir = weightsDir->GetDirectory(tbinKey->GetName());
					TIter	nextEbinDir(tbinDir->GetListOfKeys());
					TKey 	*ebinKey;
					
					// write the headings
					if (writeHead) {
						while ((ebinKey=(TKey*)nextEbinDir())) {
							outHtml << "<TD><h1>"+(TString)ebinKey->GetName()+"</h1>";
						}
						outHtml << endl;
						writeHead = false;
					}
					// write the rows
					nextEbinDir = tbinDir->GetListOfKeys();
					outHtml << "<TR><TD><h1>"+(TString)tbinKey->GetName()+"</h1>";
					while ((ebinKey=(TKey*)nextEbinDir())) {
						TString imFileName = dirs[i]+(TString)tbinKey->GetName()+(TString)ebinKey->GetName()+histNames[histNum]+".gif";
						outHtml << "<td><image src=\""+imFileName+"\" width=\"80%\"></image>" << endl;
					}
				}
				outHtml << "</TABLE>" << endl;
			}
			
		}		
		
	}
	else {
		
		TFile *infile = TFile::Open("/home/louise/g8_phi/sWeights/hist/out/hists_all.root");
		TFile* outfile = new TFile("out/analyse_nobins.root","recreate");	
		string dirs[3] = {"Signal", "BG", "All"};
		
		for (int i=0; i<3; i++) {
			
			TDirectory *inDir = infile->GetDirectory(dirs[i].c_str());
			cout << dirs[i].c_str() << endl;
			outfile->mkdir(dirs[i].c_str());
			TDirectory *outDir = outfile->GetDirectory(dirs[i].c_str());
			//DoFits(inDir, outDir);
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

	outHtml << "</TABLE>" << endl;
	outHtml << "</body>" << endl;
	outHtml << "</HTML>" << endl;
	outHtml.close();
}
