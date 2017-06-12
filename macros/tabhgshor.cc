
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

//root -b -p -q .x tabhgshor.cc\(\"MYFILE.root\"\)
//horizontal tab:

void tabhgshor(const char* filename, bool logy = 0)  {

  TFile File( filename );

  TIter ListOfHistograms( File.GetListOfKeys() );
  TKey *HistoKey = 0;

  TH1F *histo1;

  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    string hisname = HistoKey->GetName();
    TH1* histo = (TH1*) File.Get( HistoKey->GetName() );
    
    histo1 = (TH1F*) File.Get( HistoKey->GetName() );
    string fn = string(filename) + string("_horiz_") + histo->GetName() + ".csv";
    ofstream fo;
    fo.open( fn.c_str() );

	
    TAxis * xAxis = histo1->GetXaxis();

    fo << "# " << fn;
    for (int i=0; i <= xAxis->GetNbins()+1; i++) {
      //if( xAxis->GetBinLabel(i) != string("conv") )
	fo << xAxis->GetBinLabel(i) << " , ";
    }
    fo << endl;
    for (int i=0; i <= xAxis->GetNbins()+1; i++) {
      //if( xAxis->GetBinLabel(i) != string("conv") )
	fo << histo1->GetBinContent(i) << " , ";
    }
    fo << endl;

    fo.close();
  }
}
