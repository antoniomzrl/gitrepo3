
#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TPad.h"
#include "TCanvas.h"
#include <iostream>
#include <TStyle.h>
#include "TMarker.h"
#include "TLine.h"
#include "TText.h"

#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

void myhadd(string files)  {

  istringstream iss(files);
  string fileout, filelist, file;

  iss >> fileout >> filelist;
  cout << "fileout:  " << fileout << endl;
  cout << "filelist: " << filelist << endl;

  TH1 * hs  = new TH1D("Suma",       "Number of ints", 3, 0, 3);
  TH1 * hsc = new TH1D("SumaCua",    "Number of ints", 3, 0, 3);
  TH1 * hm  = new TH1D("PerPrimary", "Number of ints", 3, 0, 3);
  TH1 * he  = new TH1D("RelError",   "Number of ints", 3, 0, 3);
  int Nf = 0;

  ifstream infile(filelist.c_str() );

  while (infile >> file) {
    cout << "processing file: " << file << endl; 
    TFile File( file.c_str() );
    TIter ListOfHistograms( File.GetListOfKeys() );
    TKey * HistoKey = 0;
    Nf++;
    
    while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
      TH1   * hi = (TH1F*) File.Get( HistoKey->GetName() );
      TAxis * xAxis = hi->GetXaxis();
      string hisname = hi->GetName();

      int ipri = -1; double npri = -1;
      //cout << "xxxAxisBins: " << xAxis->GetNbins()+1 << endl;
      for( int i=0; i <= xAxis->GetNbins()+1; i++) {
	string label = xAxis->GetBinLabel(i);
	if( label.find("Primaries") != std::string::npos) {
	  ipri = i;
	  npri = hi->GetBinContent(i);
	}
      }

      if( ipri == -1) continue;
      //cout << Nf << " " << " ipri " << ipri << endl;
      
      for (int i=0; i <= xAxis->GetNbins()+1; i++) {
	string label = hisname + "_" + xAxis->GetBinLabel(i) + "/pri";
	double v = hi->GetBinContent(i)/npri;
	if(i == ipri || label.find("Volume") != std::string::npos || label.find("Mass") != std::string::npos ) {
	  v = hi->GetBinContent(i);
	  label = hisname + "_" + xAxis->GetBinLabel(i);
	}

	hs->Fill(  label.c_str(), v   );
	hsc->Fill( label.c_str(), v*v );
      }
    }
  }

  hs->LabelsOption("a", "X");
  hsc->LabelsOption("a", "X");

  TAxis * xAxis = hs->GetXaxis();
  for (int i=0; i <= xAxis->GetNbins()+1; i++) {
    string label = xAxis->GetBinLabel(i);
    double vm  = hs->GetBinContent(i)/Nf;
    double vcm = hsc->GetBinContent(i)/Nf;
    if(label.find("Primaries") != std::string::npos) {
      vm  = hs->GetBinContent(i);
      vcm = hsc->GetBinContent(i);
    }
    double sd = sqrt(vcm -vm*vm);
    double sdm = sd/sqrt(Nf);
    hm->Fill( label.c_str(), vm );
    he->Fill( label.c_str(), sdm/vm);
  }
   
  hm->LabelsOption("a", "X");
  he->LabelsOption("a", "X");

  TFile * Fo = new TFile(fileout.c_str(), "NEW");
  hm->Write();
  he->Write();
  Fo->Close();
}
