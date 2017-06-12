// csv2csv < fin > fout 

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;


int main(int argc, char *argv[]){
  string fni, fno;

  for(int i=0; i<argc; i++) {
    if( (string)argv[i] == (string)"-i" ) fni = argv[i+1];
    if( (string)argv[i] == (string)"-o" ) fno = argv[i+1];
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

  int Nl=0;

  while( fi.good() ) {
    string li;
    getline(fi, li);
    Nl++;
    for(int i=0;i<li.length();i++)
      if( li[i] == ',' || li[i] == '#')
	li[i] = ' ';

Sin acabar!!!
    
    istringstream sli(li);
    sli >> b1 >> b2 >> x >> y >> z;

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
