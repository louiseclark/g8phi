// macro to work out bin limits to have even split of integral
// fileName is file with weighted e.g. t distribution
// histPath directory and name of histogram e.g. "/Signal/t"

void CalculateBinLimits(TString fileName, TString histPath, Int_t numBins, Double_t min, Double_t max) {
	
	TFile* infile=TFile::Open(fileName);
	TH1F* h = (TH1F*)infile->Get(histPath); 
	
	Int_t startBin = h->GetXaxis()->FindBin(min);
	Int_t endBin = h->GetXaxis()->FindBin(max);
	
	Double_t totInt = h->Integral(startBin, endBin);
	Double_t binInt = totInt / numBins;
	
	cout << "totInt " << totInt << endl;
	cout << "binInt " << binInt << endl;
	
	Double_t runInt = 0.0;
	Double_t prevBinLimit = min;
	Int_t prevBin = startBin;
	
	for (int i=startBin; i<endBin; i++) {
	
		runInt = runInt + h->GetBinContent(i);
		
		//cout << "bin "<< i << " runInt " << runInt << " binCenter " <<  h->GetXaxis()->GetBinCenter(i) <<endl;
		//cout << "bin "<< i << " binContent " <<  h->GetBinContent(i) <<endl;
		
		if (runInt > binInt) {
		
			cout << endl;
			cout << "bin limits are " << prevBinLimit << " to " << h->GetXaxis()->GetBinCenter(i) << endl;
			cout << "Integral " << h->Integral(prevBin, i-1) << endl;
			cout << endl;
			
			runInt = h->GetBinContent(i);
			prevBinLimit = h->GetXaxis()->GetBinCenter(i);
			prevBin = i;
		}
		
	}
	
}
