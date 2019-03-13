 TList * Graphs=new TList();
 

void GraphParameters2Bins(TString DirName,TString Var,int systemNum, int binType){

  //gROOT->LoadMacro("$HSANA/THSBins.C+");



  TFile* file=new TFile(DirName+"DataEntries.root");
  THSBins* DataBins=(THSBins*)file->Get("HSDataBins");
  TString BinName=DataBins->GetBinName(0);
  cout<<BinName<<endl;

  Int_t va= DataBins->GetAxisi(Var);
  TString AxisName=Var;

  // TList * Graphs=new TList();
  Graphs->SetName("AllGraphs");
  for(Int_t ib=0;ib<DataBins->GetN();ib++){
    TString redName=DataBins->GetBinName(ib);
    //Strip the variable we are plotting from the name
    //so we can find the relevant graph
    TString axisBin=redName(redName.Index(AxisName),TString(redName(redName.Index(AxisName)+AxisName.Sizeof()-1,redName.Sizeof())).First("_")+AxisName.Sizeof());
    redName.Replace(redName.Index(AxisName),TString(redName(redName.Index(AxisName)+AxisName.Sizeof()-1,redName.Sizeof())).First("_")+AxisName.Sizeof(),"");
    
 
    Int_t iP=DataBins->GetParti(va,axisBin);//index for this bin on this axis
    cout<<redName<<" "<<axisBin<<" "<<iP<<" "<<va<<endl;
    //Open the file with the results
    //TFile* fileR=new TFile(DirName+TString("Plots")+DataBins->GetBinName(ib)+".root");
    TFile* fileR=new TFile(DirName+TString("Results")+DataBins->GetBinName(ib)+".root");
   if(!fileR->IsOpen()) continue;
    RooFitResult* result=(RooFitResult*)gDirectory->Get("HSFitResult");
    file->cd();
    RooArgList Pars= result->floatParsFinal();
    //Loop over parameters getting values and error for graph
    TGraphErrors* graph=0;
    for(Int_t ipar=0;ipar<Pars.getSize();ipar++){
      //if graph doesn't exist yet create new one
      if(!(graph= dynamic_cast<TGraphErrors*>(Graphs->FindObject(redName+Pars[ipar].GetName())))){
		graph=new TGraphErrors(0);
		graph->SetNameTitle(redName+Pars[ipar].GetName(),redName+Pars[ipar].GetName());
		Graphs->Add(graph);
      }
      Int_t Npoint=graph->GetN();
      cout<<redName<<" "<<DataBins->GetAxis(va).GetBinCenter(iP+1)<<endl;
      graph->SetPoint(Npoint,DataBins->GetAxis(va).GetBinCenter(iP+1),((RooRealVar*)(&Pars[ipar]))->getVal());
      //graph->SetPointError(Npoint,DataBins->GetAxis(va).GetBinWidth(iP+1)/2,((RooRealVar*)(&Pars[ipar]))->getError());
      graph->SetPointError(Npoint,0.0,((RooRealVar*)(&Pars[ipar]))->getError());
	  graph->SetMarkerStyle(24);
	  graph->SetMarkerColor(4);
	  graph->SetMarkerSize(0.8);
    }
    
    delete result;
    fileR->Close();
    delete fileR;
    //TGraphErrors* graph=Graphs->Get(redName);
  }

  TFile* fileG=new TFile(DirName+"ParGraphs"+Var+".root","recreate");
  Graphs->Write();
  fileG->Close();
  delete fileG;

  file->Close();
  delete file;

  TString graphTitle = "";
  if (systemNum==0) graphTitle = "SDMEs in Helicity system";
  if (systemNum==1) graphTitle = "SDMEs in Gottfried-Jackson system";
  if (systemNum==2) graphTitle = "SDMEs in Adair system";

  TString axisTitle = "";
  if (binType==0) axisTitle = "-t GeV^{2}/c^{2}";
  if (binType==1) axisTitle = "cos #theta_{#phi,cm}";
  if (binType==2) axisTitle = "E_{#gamma}";
  
  // values for comparison
  float lepsHelValues[] = {0.1,-0.05,0.05,
					    0.0,0.05,0.02,
					    0.2,0.0,-0.25};
  float lepsGJValues[] = {0.2,0.1,0.1,
					    0.05,-0.1,-0.1,
					    0.12,0.15,-0.2};
  float lepsAdValues[] = {0.1,0.05,0.05,
					    0.0,0.0,-0.05,
					    0.2,0.1,-0.25};
  float deyHelValue = 0.2;
  float deyAdValues1[] = {0.55,0.5,0.25,0.1};
  float deyAdValues2[] = {-0.2,0.0,0.05,0.1};
  float deyAdValues3[] = {-0.1,0.0,0.1,0.0};
  float deyGJValues1[] = {0.35,0.35,0.3,0.22};
  float deyGJValues2[] = {-0.12,-0.1,0.0,0.15};
  float deyGJValues3[] = {-0.1,0.0,0.15,0.05};
  

  float ctBins[] = {-0.75,-0.25,0.25,0.75};
					    
  // 3x3 plots comparing to LEPS and Dey					    
  TCanvas *c1=new TCanvas("c1","c1",800,600);
  c1->Divide(3,3,0,0);
  c1->SetTitle(graphTitle);

  for (int i=0; i<10; i++) {
	
	TGraph* g = (TGraph*) Graphs->At(i);
	const char *title = "";
	TLine *line = new TLine(g->GetXaxis()->GetBinCenter(g->GetXaxis()->GetFirst()),0.0,g->GetXaxis()->GetBinCenter(g->GetXaxis()->GetLast()),0.0);
	line->SetLineStyle(2);
	if (i>6) {
		c1->GetPad(i)->SetBottomMargin(0.2);
	}
	int pad = 0;
	if (i==0) {
		title = "Im#rho^{2}_{1 0}";
		pad = 8;
		g->GetXaxis()->SetLabelSize(0.065);
		g->GetXaxis()->SetTitle(axisTitle);
		g->GetXaxis()->SetTitleSize(0.065);
	}
	else if (i==1) {
		title = "Im#rho^{2}_{1 -1}";
		pad = 9;
		g->GetXaxis()->SetLabelSize(0.065);
		g->GetXaxis()->SetTitle(axisTitle);
		g->GetXaxis()->SetTitleSize(0.065);
	}
	else if (i==2) {
		title = "Re#rho^{0}_{1 0}";
		pad = 2;
	}
	else if (i==3) {
		title = "Re#rho^{1}_{1 0}";
		pad = 6;
   
	}
	else if (i==4) {
		continue;
	}
	else if (i==5) {
		title = "#rho^{0}_{0 0}";
		pad = 1;
		g->GetYaxis()->SetLabelSize(0.065);
		
	}
	else if (i==6) {
		title = "#rho^{0}_{1 -1}";
		pad = 3;
	}
	else if (i==7) {
		title = "#rho^{1}_{0 0}";
		pad = 5;
	}
	else if (i==8) {
		title = "#rho^{1}_{1 1}";
		pad = 4;
		g->GetYaxis()->SetLabelSize(0.065);
	}
	else if (i==9) {
		title = "#rho^{1}_{1 -1}";
		pad = 7;
		g->GetXaxis()->SetLabelSize(0.065);
		g->GetXaxis()->SetTitle(axisTitle);
		g->GetXaxis()->SetTitleSize(0.065);
		g->GetYaxis()->SetLabelSize(0.065);
	}

	c1->cd(pad);
	cout << g->GetTitle() << " " << title << endl;
	g->SetTitle(title);
	g->SetName("g");
    g->GetHistogram()->SetMinimum(-0.7);
    g->GetHistogram()->SetMaximum(0.7);
	g->Draw("AP");
	
	TGraph *lg = new TGraph();
	lg->SetMarkerStyle(28);
	lg->SetMarkerColor(6);
	lg->SetMarkerSize(0.8);
	lg->SetName("lg");
	
	TGraph *dg = new TGraph();
	dg->SetMarkerStyle(32);
	dg->SetMarkerColor(8);
	dg->SetMarkerSize(0.8);
	lg->SetName("dg");

	if (systemNum==0 && binType==0) {
		lg->SetPoint(lg->GetN(),0.2,lepsHelValues[pad-1]);
		lg->Draw("P");
	}
	if (systemNum==1 && binType==0) {
		lg->SetPoint(lg->GetN(),0.2,lepsGJValues[pad-1]);
		lg->Draw("P");
	}
	if (systemNum==2 && binType==0) {
		lg->SetPoint(lg->GetN(),0.2,lepsAdValues[pad-1]);
		lg->Draw("P");
	}
	if (systemNum==0 && binType==1) {
		if (pad==1) {
			dg->SetPoint(dg->GetN(),0.7,deyHelValue);
			dg->Draw("P");
		}
	}
	if (systemNum==2 && binType==1) {
		
		for (int j=0; j<4; j++) {
			if (pad==1) {
				dg->SetPoint(dg->GetN(),ctBins[j],deyAdValues1[j]);
				dg->Draw("P");
			}
			else if (pad==2) {
				dg->SetPoint(dg->GetN(),ctBins[j],deyAdValues2[j]);
				dg->Draw("P");
			}
			else if (pad==3) {
				dg->SetPoint(dg->GetN(),ctBins[j],deyAdValues3[j]);
				dg->Draw("P");
			}
		}
	}
	if (systemNum==1 && binType==1) {
		
		for (int j=0; j<4; j++) {
			if (pad==1) {
				dg->SetPoint(dg->GetN(),ctBins[j],deyGJValues1[j]);
				dg->Draw("P");
			}
			else if (pad==2) {
				dg->SetPoint(dg->GetN(),ctBins[j],deyGJValues2[j]);
				dg->Draw("P");
			}
			else if (pad==3) {
				dg->SetPoint(dg->GetN(),ctBins[j],deyGJValues3[j]);
				dg->Draw("P");
			}
		}
	}

	line->Draw();

  }
  c1->SaveAs("results/"+graphTitle+" "+Var+".png");
  
  if (binType==2) { // energy bins
	  
	  // Values for comparison with Julien's thesis
	  // rho000, rho01m1, 1/2(rho11m1-Imrho21m1), 1/2(rho11m1+Imrho21m1), 2rho111+rho100
	  float julHelValues21Para[] = {0.0448,-0.1350,0.1916,0.0492,0.0142};
	  float julHelValues21ParaUnc[] = {0.0246,0.0259,0.0392,0.0402,0.0371};
	  float julHelValues19Para[] = {0.0393,-0.1570,0.1019,-0.0201,0.0332};
	  float julHelValues19ParaUnc[] = {0.0106,-0.0290,0.0443,0.0438,0.0503};
	  
	  float julHelValues21Perp[] = {0.0528,-0.1112,0.1986,0.0694,0.0142};
	  float julHelValues21PerpUnc[] = {0.0247,0.0260,0.0373,0.0387,0.0371};
	  float julHelValues19Perp[] = {0.0140,-0.0960,0.1350,0.0119,0.0332};
	  float julHelValues19PerpUnc[] = {0.0219,0.0245,0.0356,0.0356,0.0503};

	  float julGJValues21Para[] = {0.4015,-0.0174,0.1235,0.0987,0.0142};
	  float julGJValues21ParaUnc[] = {0.0304,0.0277,0.0399,0.0404,0.0371};
	  float julGJValues19Para[] = {0.4375,0.0531,0.0945,-0.0213,0.0332};
	  float julGJValues19ParaUnc[] = {0.0342,0.0297,0.0447,0.0431,0.0503};
	  
	  float julGJValues21Perp[] = {0.3560,0.0460,0.1454,0.0417,0.0142};
	  float julGJValues21PerpUnc[] = {0.0289,0.0273,0.0371,0.0381,0.0371};
	  float julGJValues19Perp[] = {0.3764,0.1058,0.1079,0.0011,0.0332};
	  float julGJValues19PerpUnc[] = {0.0293,0.0255,0.0356,0.0360,0.0503}; 
	  
	  float julValues21Para[] = {0.0,0.0,0.0,0.0,0.0};
	  float julValues21ParaUnc[] = {0.0,0.0,0.0,0.0,0.0};
	  float julValues19Para[] = {0.0,0.0,0.0,0.0,0.0};
	  float julValues19ParaUnc[] = {0.0,0.0,0.0,0.0,0.0};
	  
	  float julValues21Perp[] = {0.0,0.0,0.0,0.0,0.0};
	  float julValues21PerpUnc[] = {0.0,0.0,0.0,0.0,0.0};
	  float julValues19Perp[] = {0.0,0.0,0.0,0.0,0.0};
	  float julValues19PerpUnc[] = {0.0,0.0,0.0,0.0,0.0};
	  
	  if (systemNum==0) {
		  for (int i=0; i<5; i++) {
			  julValues21Para[i] = julHelValues21Para[i];
			  julValues21ParaUnc[i] = julHelValues21ParaUnc[i];
			  julValues19Para[i] = julHelValues19Para[i];
			  julValues19ParaUnc[i] = julHelValues19ParaUnc[i];
			  
			  julValues21Perp[i] = julHelValues21Perp[i];
			  julValues21PerpUnc[i] = julHelValues21PerpUnc[i];
			  julValues19Perp[i] = julHelValues19Perp[i];
			  julValues19PerpUnc[i] = julHelValues19PerpUnc[i];
		  }
		  
	  }
	  else if (systemNum==1) {
		  for (int i=0; i<5; i++) {
			  julValues21Para[i] = julGJValues21Para[i];
			  julValues21ParaUnc[i] = julGJValues21ParaUnc[i];
			  julValues19Para[i] = julGJValues19Para[i];
			  julValues19ParaUnc[i] = julGJValues19ParaUnc[i];
			  
			  julValues21Perp[i] = julGJValues21Perp[i];
			  julValues21PerpUnc[i] = julGJValues21PerpUnc[i];
			  julValues19Perp[i] = julGJValues19Perp[i];
			  julValues19PerpUnc[i] = julGJValues19PerpUnc[i];
		  }
		  
	  }
	  	  
	  // Plots comparing to Julien
	  TCanvas *c2=new TCanvas("c2","c2",800,600);
	  c2->Divide(2,3,0,0);
	  c2->SetTitle(graphTitle);
	  TString titles[] = {"#rho^{0}_{0 0}","#rho^{0}_{1 -1}",
						  "#frac{1}{2}(#rho^{1}_{1 -1} - Im#rho^{2}_{1 -1})", "#frac{1}{2}(#rho^{1}_{1 -1} + Im#rho^{2}_{1 -1})",
						  "2#rho^{1}_{1 1}+#rho^{1}_{0 0}",""};
	  leg = new TLegend(0.1,0.7,0.48,0.9);						  
	 
	  for (int i=0; i<5; i++) {
		  
		// My values
		TGraphErrors* g3=new TGraphErrors(0);
		g3->SetName("g3");
		g3->SetMarkerStyle(24);
	    g3->SetMarkerColor(7);
	    g3->SetMarkerSize(0.8);
	    g3->SetFillColor(0);
	    
	    if (i==0) {
			TGraphErrors* myG = (TGraphErrors*) Graphs->At(5);
			for (int j=0; j<2; j++) {
				Double_t x = 0.0;
				Double_t y = 0.0;
				myG->GetPoint(j,x,y);
				g3->SetPoint(j,x,y);
				g3->SetPointError(j,0.0,myG->GetErrorY(j));
			}
		}

	    if (i==1) {
			TGraphErrors* myG = (TGraphErrors*) Graphs->At(6);
			for (int j=0; j<2; j++) {
				Double_t x = 0.0;
				Double_t y = 0.0;
				myG->GetPoint(j,x,y);
				g3->SetPoint(j,x,y);
				g3->SetPointError(j,0.0,myG->GetErrorY(j));
			}
		}
	    if (i==2) {
			TGraphErrors* myG1 = (TGraphErrors*) Graphs->At(9);
			TGraphErrors* myG2 = (TGraphErrors*) Graphs->At(1);
			for (int j=0; j<2; j++) {
				Double_t x1 = 0.0;
				Double_t y1 = 0.0;
				Double_t x2 = 0.0;
				Double_t y2 = 0.0;
				myG1->GetPoint(j,x1,y1);
				myG2->GetPoint(j,x2,y2);				
				if (x1==x2) {
					g3->SetPoint(j,x1,0.5*(y1-y2));
					g3->SetPointError(j,0.0,sqrt(myG1->GetErrorY(j)*myG1->GetErrorY(j) + myG2->GetErrorY(j)*myG2->GetErrorY(j)));
				}
			}
		}		

	    if (i==3) {
			TGraphErrors* myG1 = (TGraphErrors*) Graphs->At(9);
			TGraphErrors* myG2 = (TGraphErrors*) Graphs->At(1);
			for (int j=0; j<2; j++) {
				Double_t x1 = 0.0;
				Double_t y1 = 0.0;
				Double_t x2 = 0.0;
				Double_t y2 = 0.0;
				myG1->GetPoint(j,x1,y1);
				myG2->GetPoint(j,x2,y2);				
				if (x1==x2) {
					g3->SetPoint(j,x1,0.5*(y1+y2));
					g3->SetPointError(j,0.0,sqrt(myG1->GetErrorY(j)*myG1->GetErrorY(j) + myG2->GetErrorY(j)*myG2->GetErrorY(j)));
				}
			}
		}	
		
	    if (i==4) {
			TGraphErrors* myG1 = (TGraphErrors*) Graphs->At(8);
			TGraphErrors* myG2 = (TGraphErrors*) Graphs->At(7);
			for (int j=0; j<2; j++) {
				Double_t x1 = 0.0;
				Double_t y1 = 0.0;
				Double_t x2 = 0.0;
				Double_t y2 = 0.0;
				myG1->GetPoint(j,x1,y1);
				myG2->GetPoint(j,x2,y2);				
				if (x1==x2) {
					g3->SetPoint(j,x1,2*y1 +y2);
					g3->SetPointError(j,0.0,sqrt(myG1->GetErrorY(j)*myG1->GetErrorY(j) + myG2->GetErrorY(j)*myG2->GetErrorY(j)));
				}
			}
		}			

		
		// Julien Para
		TGraphErrors* g=new TGraphErrors(0);
		
		g->SetTitle(titles[i]);
		g->SetName("g");
		g->SetMarkerStyle(25);
	    g->SetMarkerColor(4);
	    g->SetMarkerSize(0.8);
	    g->SetFillColor(0);

		g->SetPoint(0,1.8,julValues19Para[i]);
		g->SetPoint(1,2.0,julValues21Para[i]);
		g->SetPointError(0,0.0,julValues19ParaUnc[i]);
		g->SetPointError(1,0.0,julValues21ParaUnc[i]);
		
		// Julien Perp
		TGraphErrors* g2=new TGraphErrors(0);
		
		g2->SetName("g2");
		g2->SetMarkerStyle(26);
	    g2->SetMarkerColor(2);
	    g2->SetMarkerSize(0.8);
	    g2->SetFillColor(0);

		g2->SetPoint(0,1.8,julValues19Perp[i]);
		g2->SetPoint(1,2.0,julValues21Perp[i]);
		g2->SetPointError(0,0.0,julValues19PerpUnc[i]);
		g2->SetPointError(1,0.0,julValues21PerpUnc[i]);


		c2->cd(i+1);
		g->GetHistogram()->SetMinimum(-0.7);
        g->GetHistogram()->SetMaximum(0.7);
		g->Draw("AP");
		g2->Draw("P");
		g3->Draw("P");
		g->GetXaxis()->SetRangeUser(1.5,2.3);
		if (i==4) {
			g->GetXaxis()->SetLabelSize(0.065);
			g->GetXaxis()->SetTitle(axisTitle);
			g->GetXaxis()->SetTitleSize(0.065);
			c2->GetPad(i+1)->SetBottomMargin(0.2);
		}
		if (i==0 || i==2 || i==4) {
			g->GetYaxis()->SetLabelSize(0.065);
		}	
		
		TLine *line = new TLine(g->GetXaxis()->GetBinCenter(g->GetXaxis()->GetFirst()),0.0,g->GetXaxis()->GetBinCenter(g->GetXaxis()->GetLast()),0.0);
		line->SetLineStyle(3);
		line->Draw();
		c2->Update();
		if (i==0) {
			leg->AddEntry("g","Salamanca (PARA)");
			leg->AddEntry("g2","Salamanca (PERP)");
			leg->AddEntry("g3","This work");
		}
	  }
	 
	 c2->cd(6); 
     leg->Draw();
	  
	 c2->SaveAs("results/"+graphTitle+"5vars_"+Var+".png");  
    
  }
  
  return;

}
