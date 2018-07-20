
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

void tabhgs2(const char* filename, bool logy = 0)  {

  TFile File( filename );

  TIter ListOfHistograms( File.GetListOfKeys() );
  TKey *HistoKey = 0;

  TH1F *histo1;
  
  
  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    string hisname = HistoKey->GetName();
    TH1* histo = (TH1*) File.Get( HistoKey->GetName() );
    
    histo1 = (TH1F*) File.Get( HistoKey->GetName() );
    string fn = string(filename) + string("_") + histo->GetName() + ".csv";
    ofstream fo;
    fo.open( fn.c_str() );
    fo << histo->GetEntries() << " , " << fn << endl;
	
    TAxis * xAxis = histo1->GetXaxis();

    for (int i=0; i <= xAxis->GetNbins()+1; i++) {
      fo << xAxis->GetBinLabel(i)      << " , "
	 << histo1->GetBinContent(i) << endl;
    }
    fo.close();
  }
  
}
