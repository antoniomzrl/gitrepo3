
// clipconti -i conti.txt -o conticlip.txt -r x X y Y z Z

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
using namespace std;

#define FMAX ( std::numeric_limits<double>::max() )
#define FMIN ( std::numeric_limits<double>::min() )

int main(int argc, char *argv[]){

  double x1=FMAX, y1=FMAX, z1=FMAX, x2=FMIN, y2=FMIN, z2=FMIN;
  string fni, fno;

  for(int i=0; i<argc; i++) {
    if( (string)argv[i] == (string)"-r" ) {
      x1 = atof(argv[i+1]);
      x2 = atof(argv[i+2]);
      y1 = atof(argv[i+3]);
      y2 = atof(argv[i+4]);
      z1 = atof(argv[i+5]);
      z2 = atof(argv[i+6]);
      cout << "x  (" << x1 << "," << x2 << ")" << endl
	   << "y  (" << y1 << "," << y2 << ")" << endl
	   << "z  (" << z1 << "," << z2 << ")" << endl;
    }
    if( (string)argv[i] == (string)"-i" )
      fni = argv[i+1];
    if( (string)argv[i] == (string)"-o" )
      fno = argv[i+1];
  }

  ifstream fi(fni.c_str(), ios::in);
  if(! fi.is_open()) {
    cout << "Unable to open file " << fni << endl; 
    exit(-1);
  }

  ofstream fo(fno.c_str(), ios::out);
  if(! fo.is_open()) {
    cout << "Unable to open file " << fno << endl; 
    exit(-1);
  }

  double xm = FMAX, ym = FMAX, zm = FMAX;
  double xM = FMIN, yM = FMIN, zM = FMIN;

  int Nd=0, Ndv=0;

  while( fi.good() ) {
    Nd++;
    string lio, li, b1, b2;
    double x, y, z;
    getline(fi, lio);
    li = lio;

    for(int i=0;i<li.length();i++)
      if( li[i] == ',' )
	li[i] = ' ';

    istringstream iss(li);
    iss >> b1 >> b2 >> x >> y >> z;
    //cout << x << " " << y << " " << z << endl;

    if( x > xM ) xM = x; if( y > yM ) yM = y; if( z > zM ) zM = z;
    if( x < xm ) xm = x; if( y < ym ) ym = y; if( z < zm ) zm = z;

    //double dist = pow(x-cx,2) + pow(y-cy,2) + pow(z-cz,2);
    //double dist = max( fabs(x-cx), fabs(y-cy) );
    //dist = max( dist, fabs(z-cz) );
    //cout << x << " " << y << " " << z << " / " << sqrt(d2) << endl;
    //if(dist < D && b2 == "e-")

    if( x>x1 && x<x2 && y>y1 && y<y2 && z>z1 && z<z2 ) {
      Ndv++;
      fo << lio << endl;
    }
  }

  fo.close();
  fi.close();

  cout << "x: (" << xm << " , " << xM << ")" << endl
       << "y: (" << ym << " , " << yM << ")" << endl
       << "z: (" << zm << " , " << zM << ")" << endl
       << "N/n: " << Nd << " / " << Ndv << endl;

}
