void CreateHtmlJS() {
	
	ofstream outHtml;
	outHtml.open("/home/louise/public_html/temp/summary.html");
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
	outHtml << "    text-align: left;" << endl;
	outHtml << "}" << endl;
	outHtml << "</style>" << endl;
	outHtml << "</head> " << endl;
	outHtml << "<body>" << endl;
	
   outHtml << "<SCRIPT LANGUAGE=\"JavaScript\" TYPE=\"text/javascript\">"  << endl;
   outHtml << "<!-- Hide JavaScript from older browsers " << endl;
   outHtml << "function xtoggleMe(e){" << endl;
   outHtml << "  if(!e)return true;" << endl;
   outHtml << "  if(e.style.display==\"none\"){" << endl;
   outHtml << "    e.style.display=\"\"" << endl;
   outHtml << "  } else {" << endl;
   outHtml << "    e.style.display=\"none\"" << endl;
   outHtml << "  }" << endl;
   outHtml << "  return true;" << endl;
   outHtml << "}" << endl;
   outHtml << "function showHideMe(e, chk){" << endl;
   outHtml << "  if(!e)return true;" << endl;
   outHtml << "  e.style.display = chk.checked ? \"\" : \"none\";" << endl;
   outHtml << "  return true;" << endl;
   outHtml << "}" << endl;
   outHtml << "function showHideAll(a, chk){" << endl;
   outHtml << "    " << endl;
   outHtml << "  var list = document.getElementsByClassName(a);" << endl;
   outHtml << "  for(i = 0;i < list.length; i++)" << endl;
   outHtml << "  {" << endl;
   outHtml << "    showHideMe(list[i], chk);" << endl;
   outHtml << "  }" << endl;
   outHtml << "  return true;" << endl;
   outHtml << "}" << endl;
   outHtml << "function updateDisplay() {" << endl;
   outHtml << "  // first turn everything on" << endl;
   outHtml << "  var list1 = document.getElementsByName('level1');" << endl;
   outHtml << "  for(i=0;i<list1.length; i++) {" << endl;
   outHtml << "    var elist = document.getElementsByClassName(list1[i].value);" << endl;
   outHtml << "    for (j=0; j<elist.length; j++) {" << endl;
   outHtml << "        elist[j].style.display = \"\";" << endl;
   outHtml << "    }" << endl;
   outHtml << "  }" << endl;
   outHtml << "  " << endl;
   outHtml << "  // now hide if not selected (level 1) " << endl;
   outHtml << "  for(i=0;i<list1.length; i++) {" << endl;
   outHtml << "    var elist = document.getElementsByClassName(list1[i].value);" << endl;
   outHtml << "    for (j=0; j<elist.length; j++) {" << endl;
   outHtml << "        if (!list1[i].checked) {" << endl;
   outHtml << "            elist[j].style.display = \"none\";" << endl;
   outHtml << "        }" << endl;
   outHtml << "    }" << endl;
   outHtml << "  }" << endl;
   outHtml << "  // now hide if not selected (level 2 - 1st level bins) " << endl;
   outHtml << "  var list2 = document.getElementsByName('level2');" << endl;
   outHtml << "  for(i=0;i<list2.length; i++) {" << endl;
   outHtml << "    var elist = document.getElementsByClassName(list2[i].value);" << endl;
   outHtml << "    for (j=0; j<elist.length; j++) {" << endl;
   outHtml << "        if (!list2[i].checked) {" << endl;
   outHtml << "            elist[j].style.display = \"none\";" << endl;
   outHtml << "        }" << endl;
   outHtml << "    }" << endl;
   outHtml << "  }" << endl;
   outHtml << "  // now hide if not selected (level 3 - 2nd level bins) " << endl;
   outHtml << "  var list3 = document.getElementsByName('level3');" << endl;
   outHtml << "  for(i=0;i<list3.length; i++) {" << endl;
   outHtml << "    var elist = document.getElementsByClassName(list3[i].value);" << endl;
   outHtml << "    for (j=0; j<elist.length; j++) {" << endl;
   outHtml << "        if (!list3[i].checked) {" << endl;
   outHtml << "            elist[j].style.display = \"none\";" << endl;
   outHtml << "        }" << endl;
   outHtml << "    }" << endl;
   outHtml << "  }  " << endl;
   outHtml << "  // now hide if not selected (level 4 - histograms) " << endl;
   outHtml << "  var list4 = document.getElementsByName('level4');" << endl;
   outHtml << "  for(i=0;i<list4.length; i++) {" << endl;
   outHtml << "    var elist = document.getElementsByClassName(list4[i].value);" << endl;
   outHtml << "    for (j=0; j<elist.length; j++) {" << endl;
   outHtml << "        if (!list4[i].checked) {" << endl;
   outHtml << "            elist[j].style.display = \"none\";" << endl;
   outHtml << "        }" << endl;
   outHtml << "    }" << endl;
   outHtml << "  }" << endl;
   outHtml << "  return true;" << endl;
   outHtml << "}" << endl;
   outHtml << "" << endl;
   outHtml << "//  End script hiding -->" << endl;
   outHtml << "</SCRIPT>    " << endl;
   outHtml << "    " << endl;
	
	TFile *infile = TFile::Open("/home/louise/g8K0Sig/code/sWeights/hist/K0pi0_weights/out_Ecosthbins/hists_all.root");
	string dirs[3] = {"Signal", "BG", "All"};
	vector<TString> histNames;
	bool storeHistNames = true;


	// checkboxes
    outHtml << "<form>	" << endl;
	outHtml << "<TABLE><tr><td>" << endl;
	// Level 1
	for (int i=0; i<3; i++) {
				
		TDirectory *weightsDir = infile->GetDirectory(dirs[i].c_str());	
		outHtml << "<input type=\"checkbox\" name=\"level1\" value=\"" + dirs[i] + "\" checked onclick=\"return updateDisplay()\">" + dirs[i] + "<BR>" << endl;

	}
	// Level 2 - 1st level bins
	outHtml << "<TD>" << endl;
	TDirectory *weightsDir = infile->GetDirectory(dirs[0].c_str());	
	TIter	nextTbinDir(weightsDir->GetListOfKeys());
	TKey 	*tbinKey;
	while ((tbinKey=(TKey*)nextTbinDir())) {
		TDirectory *tbinDir = weightsDir->GetDirectory(tbinKey->GetName());
		outHtml << "<input type=\"checkbox\" name=\"level2\" value=\"" + (TString)tbinDir->GetName() + "\" checked onclick=\"return updateDisplay()\">" + (TString)tbinDir->GetName() + "<BR>" << endl;
	}
	
	// Level 3 - 2nd level bins
	outHtml << "<TD>" << endl;
	nextTbinDir = weightsDir->GetListOfKeys();
	tbinKey=(TKey*)nextTbinDir();
	TDirectory *tbinDir = weightsDir->GetDirectory(tbinKey->GetName());
	TIter	nextEbinDir(tbinDir->GetListOfKeys());
	TKey 	*ebinKey;
	
	while ((ebinKey=(TKey*)nextEbinDir())) {
		TDirectory *ebinDir = tbinDir->GetDirectory(ebinKey->GetName());
		outHtml << "<input type=\"checkbox\" name=\"level3\" value=\"" + (TString)ebinDir->GetName() + "\" checked onclick=\"return updateDisplay()\">" + (TString)ebinDir->GetName() + "<BR>" << endl;
	}
	
	// Level 4 - histograms
	outHtml << "<TD>" << endl;
	nextEbinDir = tbinDir->GetListOfKeys();
	ebinKey=(TKey*)nextEbinDir();
	TDirectory *ebinDir = tbinDir->GetDirectory(ebinKey->GetName());
	TIter	nextHist(ebinDir->GetListOfKeys());
	TKey	*histKey;
	
	while ((histKey=(TKey*)nextHist())) {
		outHtml << "<input type=\"checkbox\" name=\"level4\" value=\"" + (TString)histKey->GetName() + "\" checked onclick=\"return updateDisplay()\">" + histKey->GetName() + "<BR>" << endl;
	}	
	outHtml << "</TABLE>" << endl;	

	
	// all histograms for each bin
	outHtml << "<TABLE>" << endl;
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
				outHtml << "<TR><TD class=\""+heading+"\"><h1>"+heading+"</h1>" << endl;
				outHtml << "<TR><TD>" << endl;
				outHtml << "<TABLE>" << endl;
				
				int histNum = 0;
				while ((histKey=(TKey*)nextHist())) {
								
					if (histNum%4==0) outHtml << "<TR>"; // new row every 4 hists		
					histNum++;
					
					TH1F* hist = (TH1F*) histKey->ReadObj();
					hist->Draw();
					TString histName = histKey->GetName();
					TString imFileName = dirs[i]+(TString)tbinDir->GetName()+(TString)ebinDir->GetName()+histName+".gif";
					can->SaveAs("/home/louise/public_html/temp/"+imFileName);
					can->Clear();	
					outHtml << "<td class=\""+heading+" "+histName+"\"><image src=\""+imFileName+"\" width=\"80%\"></image>" << endl;
					
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

	outHtml << "</TABLE>" << endl;
	outHtml << "</body>" << endl;
	outHtml << "</HTML>" << endl;
	outHtml.close();
}
