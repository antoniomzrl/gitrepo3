#include "histogram.hh"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

  double a=0, b=1, h=1e-3, datum;

  for(int i=0; i<argc; i++) {
    if( (string)argv[i] == (string)"-a" ) {
      a = atof(argv[i+1]);
      cout << "a:  " << a << endl;
    }
    if( (string)argv[i] == (string)"-b" ) {
      b = atof(argv[i+1]);
      cout << "b:  " << b << endl;
    }
    if( (string)argv[i] == (string)"-h" ) {
      h = atof(argv[i+1]);
      cout << "h:  " << h << endl;
    }
  }

  AmrHistogram * hg = new AmrHistogram(h, a, b, "hg");
  //std::ifstream file;
  //file.open(jmGlobals->MkHg);

  while (! cin.eof() ) {
    cin >> datum;
    hg->AddDatum(datum);
    //cout << "datum " << datum << endl;
  }

  //file.close();
  delete hg;
}
