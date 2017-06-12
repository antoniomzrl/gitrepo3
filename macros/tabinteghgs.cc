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

//root -b -p -q .x tabhgs.cc\(\"MYFILE.root\"\)

void tabinteghgs(const char* filename, bool logy = 0)  {

  TFile File( filename );

  TIter ListOfHistograms( File.GetListOfKeys() );
  TKey *HistoKey = 0;

  TH1F *histo1;
  
  string head, fn;

  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    string hisname = HistoKey->GetName();
    TH1* histo = (TH1*) File.Get( HistoKey->GetName() );

      histo1 = (TH1F*) File.Get( HistoKey->GetName() );
      fn = string(filename) + string("_") + histo->GetName();
      cout << "fn: " << fn 
	   << " INT      " << histo->GetEntries()*histo->GetMean()
	   << " NENTRIES " << histo->GetEntries()
	   << " MEAN     " << histo->GetMean()
	   << " RMS      " << histo->GetRMS()
	   << endl;
	
      TAxis * xAxis = histo1->GetXaxis();

      double Integ=0;
	
	for (int i=xAxis->GetNbins()+1; i>=0; i--) {
	  Integ += histo1->GetBinContent(i);
	  cout << xAxis->GetBinCenter(i)*1e6 << " ,"
	       << "\t" << histo1->GetBinContent(i) << " ,"
	       << "\t" << Integ*1e-6
	       << " " << xAxis->GetBinLabel(i)
	       << endl;
	}
  }
  
}
