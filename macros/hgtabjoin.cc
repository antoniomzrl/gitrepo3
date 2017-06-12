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

void hgtabjoin(const char* filename)  {

  TFile File( filename );

  TIter ListOfHistograms( File.GetListOfKeys() );
  TKey *HistoKey = 0;
  ofstream fi;
  
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

  //write tab csv:
  string fo = string(filename) + string(".csv");
  fi.open( fo.c_str() );
  fi << "#Table:" << endl << setw(16) << "#x(mm) ,";
  
  for(int j=0; j<nhi; j++)
    fi << setw(16) << hi[j]->GetName() << " ,";
  fi << endl;
    
  for (int i=0; i <= xAxis->GetNbins()+1; i++) {
    fi << setw(16) << xAxis->GetBinCenter(i) << " ,";
    for(int j=0; j<nhi; j++)
      fi << setw(16) << hi[j]->GetBinContent(i) << " ,";
    fi << endl;
  }
  fi.close();
}
