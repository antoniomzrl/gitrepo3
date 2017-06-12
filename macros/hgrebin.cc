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

void hgrebin(const char* filename, int Nrb = 10)  {

  cout << filename << " Nrb: " << Nrb << endl;
  
  TFile File( filename );

  TIter ListOfHistograms( File.GetListOfKeys() );
  TKey *HistoKey = 0;

  TH1F * hi[50];
  TAxis * xAxis;
  int nhi=0;
  
  while ( (HistoKey = (TKey*)ListOfHistograms()) ) {
    hi[nhi] = (TH1F*) File.Get( HistoKey->GetName() );
    string name = string(filename) + string(" ") + hi[nhi]->GetName();
    xAxis = hi[nhi]->GetXaxis();

    if( xAxis->GetNbins() < 100)
      cout << "\tignored: " << xAxis->GetNbins() << " " << name << endl;
    else {
      cout << nhi << " " << xAxis->GetNbins()
	   << " histo: (" << name << ") NENTRIES " << hi[nhi]->GetEntries()
	   << " MEAN     " << hi[nhi]->GetMean() << " RMS      " << hi[nhi]->GetRMS()
	   << endl;
      nhi++;
    }
  }

  string fo = string("rb_") + string(filename);
  TFile * tfo = new TFile(fo.c_str(), "RECREATE");

  //Rebin & write
  for(int j=0; j<nhi; j++) {
    hi[j]->Rebin(Nrb);
    hi[j]->Write();
  }

  tfo->Close();
}
