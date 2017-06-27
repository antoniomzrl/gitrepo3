
#include "visu.hh"

extern vector <string> tpi, ntpi;
extern DataObject * d;

void CompressTpi() {

  for(unsigned int i=0; i<tpi.size(); i++) {
    if( d->Hist[i] ) {
      cout << right << setw(4) << i << " "
	   << left  << setw(32) << tpi[i]
	   << left  << setw(32) << ntpi[i]
	   << right << setw(12) << d->Hist[i]
	   << endl;
    }
  }

  vector <int> idx;

  for(unsigned int i=0; i<tpi.size(); i++) 
    if( d->Hist[i] )
      idx.push_back(i);

  for(unsigned int i=0; i<idx.size(); i++)
    cout << i << " " << idx[i] << endl;

  for(int i=0; i<(int)idx.size(); i++) {
    if( i < idx[i] ) {
      tpi[i] = tpi[idx[i]];
      ntpi[i] = ntpi[idx[i]];
    }
  }

  tpi.resize(idx.size());
  ntpi.resize(idx.size());

  
  
  for(unsigned int i=0; i<(d->r).size(); i++)
    for(unsigned int j=0; j<idx.size(); j++)
      if( d->r[i].f[1] == idx[j]) 
	d->r[i].f[1] =j;

  ComputeFrame(d);
  ComputeHistogram(d);
      
  for(unsigned int i=0; i<tpi.size(); i++) {
    cout << right << setw(4) << i << " "
	 << left  << setw(32) << tpi[i]
	 << left  << setw(32) << ntpi[i]
	 << right << setw(12) << d->Hist[i]
	 << endl;
  }
  
}
