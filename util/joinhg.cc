#include "histogram.hh"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
using namespace std;

void AddHistogram(AmrHistogram *, char *);

double reflex = 0;

int main(int argc, char *argv[]){
  double a=0, b=1, h=1e-3;
  int ifil = 1;
  char of[64] = "jhg";

  for(int i=0; i<argc; i++) {
    if( (string)argv[i] == (string)"-a" ) {
      a = atof(argv[i+1]);
    }
    if( (string)argv[i] == (string)"-b" ) {
      b = atof(argv[i+1]);
    }
    if( (string)argv[i] == (string)"-h" ) {
      h = atof(argv[i+1]);
    }
    if( (string)argv[i] == (string)"-o" ) {
      strcpy(of, argv[i+1]);
    }
    if( (string)argv[i] == (string)"-rflx" ) {
      reflex = atof(argv[i+1]);
    }
    if( (string)argv[i] == (string)"-i" ) { //last arg
      ifil = i+1;
    }
  }

  AmrHistogram * jhg = new AmrHistogram(h, a, b, of);

  for(int i=ifil; i<argc; i++) {
    AddHistogram(jhg, argv[i]);
  }

  delete jhg;

  exit(0);
}


void AddHistogram(AmrHistogram * hg, char * fn) {
  cout << "AddHistogram (" << fn << ") ";

  ifstream fi(fn, ios::in);
  if(! fi.is_open()) {
    cout << "Unable to open file " << fn << endl; 
    exit(-1);
  }

  string line, c1;
  double c2, c4, c5, c6, c7;
  long int c3;

  while( fi.good() ) {
    getline(fi, line);
    //cout << "(" << line << ")" << endl;
    istringstream iss(line);
    iss >> c1;

    if( c1 != (string)"#" && c1 != (string)"(1)" ) {
      iss >> c2 >> c3 >> c4 >> c5 >> c6 >> c7;
      cout << c1 <<" " << c2 << " " << c3 << " " << c4<< endl;

      if(reflex > 0) c2 = reflex -c2;

      hg->AddDatum(c2, c7);
      //for( long int i=0; i<c3; i++) hg->AddDatum(c2, c4/c3);
    }

  }

}
