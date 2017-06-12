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

//root -b -p -q .x plothgs.cc\(\"MYFILE.root\"\)

void plothgs(const char* filename, bool logy = 0, char* type2d = 0) 
{
  TCanvas * theCanvas = new TCanvas("dd");
  theCanvas->cd();
  gStyle->SetOptStat(1111111);
  gStyle->SetOptTitle(0);
  if( logy ) theCanvas->SetLogy(1);

  TFile File( filename );

  TIter ListOfHistograms( File.GetListOfKeys() );
  TKey *HistoKey = 0;

  TH1F *histo1;
  TH2F *histo2;
  
  int nh = 0;
  string head, fn;

  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    nh++;
    string hisname = HistoKey->GetName();
    TH1* histo = (TH1*) File.Get( HistoKey->GetName() );
    if( histo->GetDimension() == 1 ) {
      histo1 = (TH1F*) File.Get( HistoKey->GetName() );
      histo1->Draw("E");
      head = string(filename) + string("_") + histo->GetName();
      fn = head + string(".gif");
      cout << "fn: " << fn << endl;
      cout << "  MEAN =     " << histo->GetMean() << endl;
      cout << "  RMS  =     " << histo->GetRMS() << endl;
      cout << "  NENTRIES = " << histo->GetEntries() << endl;
      TAxis * xAxis = histo1->GetXaxis();
      // AMR tab:

      for (int i=0; i <= xAxis->GetNbins()+2; i++) {
	cout << setw(8) << xAxis->GetBinCenter(i)
	     << setw(16) << histo1->GetBinContent(i)
	     << "\t" << xAxis->GetBinLabel(i) << endl;
      }

      for(int i=0; i<xAxis->GetNbins()+2; i++) {
        //cout << xAxis->GetBinCenter(ii) << " " << histo1->GetBinContent(ii) << endl;
	cout << setw(16) << xAxis->GetBinCenter(i)
	     << setw(16) << histo1->GetBinContent(i)
	     << "\t" << xAxis->GetBinLabel(i)
	     << endl;
      }
    } else if( histo->GetDimension() == 2 ) {
      histo2 = (TH2F*) File.Get( HistoKey->GetName() );
      histo2->Draw(type2d);
      fn = string(filename) + string("_") + histo2->GetName() + string(".gif");
    }

    theCanvas->Update();

    theCanvas->Print(fn.c_str());
    
  }
  
}
