
void GraphSDMEs() {
	
	TFile *resFile = new TFile("outCosTh/Resultst0.20_.root");
	TTree *resTree = (TTree*) resFile->Get("ResultsTree");
	TTreeReader resReader("ResultsTree", resFile);
	TTreeReaderValue<Double_t> myrho000(resReader, "rho000");
	
	while (resReader.Next()) {
		cout << "rho000 " << *myrho000 << endl;
	}
	
}
