
#include <string.h>
#include <stdio.h>
#include <string>

#include <cmath>
#include "globals.hh"
#include <iostream> 
using namespace std;
#include "CLHEP/Units/PhysicalConstants.h"

#include "LeptsDiffXS.hh"
#include "Randomize.hh"


LeptsDiffXS::LeptsDiffXS(string file, string theParticle) {
  fileName = file;
  //  cout << "DiffXS: " << theParticle << endl;
  
  if(     theParticle == "e-") theMc2 = electron_mass_c2;
  else if(theParticle == "e+") theMc2 = electron_mass_c2;
  else if(theParticle == "proton"  ) theMc2 = proton_mass_c2;
  else if(theParticle == "hydrogen") theMc2 = proton_mass_c2 + electron_mass_c2;
  
  readDXS();
  BuildCDXS();
  //BuildCDXS(1.0, 0.5);
  NormalizeCDXS();
  //InterpolateCDXS();
}



//DXS y KT
void LeptsDiffXS::readDXS( ) {

  std::ifstream fin(fileName.c_str());
  if (fileName == (string)"" || !fin.is_open()) {
    NE = 0;
    bFileFound = false;
    return;
  }
  
  cout << "    Reading\t" << fileName << G4endl;
  bFileFound = true;

  int nee, naa;
  char DXSTypeName[8];

  string line;
  getline(fin, line);
  replace(line.begin(), line.end(), ',', ' '); // csv file
  replace(line.begin(), line.end(), '#', ' '); // comment
  if( sscanf(line.c_str(), "%d %d %s", &naa, &nee, DXSTypeName) == 0) cout << "readerror\n";

  NL = 51;
  NE = nee +1;
  NA = naa +1;

  //  cout << "NE " << NE << " NA " << NA << " " << fileName << endl;

  try {
    f = new double[NE*NA];
    F = new double[NL*NE*NA];
  }
  catch( bad_alloc) {
    cerr << "Error Memory exhausted (f F)\n";
    exit(1);
  }
 
  //float data;
  getline(fin, line);
  replace(line.begin(), line.end(), ',', ' ');
  replace(line.begin(), line.end(), '#', ' '); // comment

  stringstream ss(line);

  for (int e=1; e<NE; e++){
    ss >> Eb[e];
  }

  for(G4int a=0; a<(NA-1); a++){
    getline(fin, line);
    replace(line.begin(), line.end(), ',', ' ');
    stringstream ssl(line);
    for(G4int e=0; e<NE; e++){
      ssl >> f[idx(e,a)];
    }
  }

  fin.close();
}


// CDXS from DXS
void LeptsDiffXS::BuildCDXS() {
  if(NE ==0) return;

  for(int l=0; l<NL; l++)
    for(G4int e=0; e<NE; e++)
      for(G4int a=0; a<(NA-1); a++)
    	F[idx(l,e,a)]=0.0;
	
  // l=10*El/E,  El/E=l/10
  // f(El,E,a) -> f(El,E,a)^(1-El/E)

  for(int l=0; l<NL; l++) {

    for(G4int a=0;a<(NA-1);a++)     // e=0
      F[idx(l,0,a)] = f[idx(0,a)];
    
    for (G4int e=1;e<NE;e++){
      F[idx(l,e,0)] = 0.0;
      double sum=0.0;
      for (G4int a=0; a<(NA-1); a++){
	double hi = f[idx(0,a+1)] -f[idx(0,a)];
	sum += hi * pow(f[idx(e,a)], 1-l/double(NL-1));
	F[idx(l,e,a+1)] = sum;
      }
    }
  }

}



// CDXS & DXS
void LeptsDiffXS::NormalizeCDXS() {
  // Normalize:  1/area
  for(int e=1; e<NE; e++) {
    for (int a=0; a<(NA-1); a++) {
      for(int l=0; l<NL; l++) {
	F[idx(l,e,a)] /= F[idx(l,e,NA-2)];//F[l][e][a] /= F[l][e][na-1];
	f[idx(e,a)] /= F[idx(0,e,NA-2)];
	//if(f[idx(e,a)] < 1.0e-8 ) f[idx(e,a)] = 1.0e-8;
      }
    }
  }

  //Inverse G(eta) -> F(ang)
  for( int l=0; l<NL; l++) {
    for( int e=1; e<NE; e++) {
      for( int i=0; i<=1000; i++) {
	double r = i*0.001;

	int a=0, b=NA-2;
	while( (b-a)>1 ) {
	  int k = (a+b)/2;
	  if( F[idx(l,e,k)] < r) a=k;
	  else                   b=k;
	}
	//linear
	// G[l][e][i] = F[l][0][a] + (F[l][0][b]-F[l][0][a])
	//   / (F[l][e][b] -F[l][e][a]) * (r -F[l][e][a]);
	//rational
	// double dx = F[l][0][a+1] - F[l][0][a];
	// double dF = F[l][e][a+1] - F[l][e][a];
	// double dF2 = F[l][e][a+2] - F[l][e][a+1];
	// double s = (r - F[l][e][a]) / dF;
	// double B = 1 - dF/dx/dx/dF2;
	// double A = 1/dx -B -1;
	// G[l][e][i] = F[l][0][a] + (1 +A +B)*s / (1 +A*s +B*s*s) *dx;
      }
    }
  }
  
  // cout << "tab1F << " << Eb[22] << endl;
  // for (int a=0; a<=(NA-1)-1; a++)
  //   cout << F[0][0][a] << " " << F[0][22][a] << endl;
  // cout << "tabG << " << Eb[22] << endl;
  // for (int i=0; i<=1000; i++)
  //   cout << i*0.001 << " " << G[0][22][i] << endl;


  // int e=29;
  // cout << "tabF " << filename << " " << Eb[e] << endl;
  // for (int a=0; a<=(NA-1); a++) {
  //   cout << f[0][a] << " " << F[0][0][a] << " " << f[e][a];
  //   for(int l=0; l<11; l++)
  //     cout << " " << F[l][e][a];
  //   cout << endl;
  // }
}



G4double LeptsDiffXS::RelativisticMomentum( G4double T) {
  //G4double p  = sqrt(pow( (T/27.2/137),2) +2*T/27.2); // p
  G4double cp = sqrt( (T + 2.0*theMc2*T)*T );
  return( cp ); // c*p
}

// F Interpolator
inline double LeptsDiffXS::FF(int l, int e, int a, double E) {
  double x1, x2, y1, y2, x, y;

  x1 = Eb[e];
  x2 = Eb[e+1];
  y1 = F[idx(l,e,  a)];
  y2 = F[idx(l,e+1,a)];
  x  = E;

  //y = y2;                               // step
  //y = (y1*(x2-x) + y2*(x-x1))/(x2-x1);  // linear
  double eps = 1e-12;                     // log-log
  y = exp( (log(y1+eps)*log(x2/x)+log(y2+eps)*log(x/x1))/log(x2/x1) );

  return(y);
}


// f Interpolator
inline double LeptsDiffXS::ff(int e, int a, double E) {
  double x1, x2, y1, y2, x, y;

  x1 = Eb[e];
  x2 = Eb[e+1];
  y1 = f[idx(e,  a)];
  y2 = f[idx(e+1,a)];
  x  = E;

  //y = y2;                               // step
  //y = (y1*(x2-x) + y2*(x-x1))/(x2-x1);  // linear
  double eps = 1e-12;                     // log-log
  y = exp( (log(y1+eps)*log(x2/x)+log(y2+eps)*log(x/x1))/log(x2/x1) );

  return(y);
}



G4double LeptsDiffXS::SampleAngle(G4double Energy, G4double El) {
  //  cout << "sampleang " << NE << " " << NL << " " << NA<< " " <<Energy << " " << El << endl;
  
  // loc Energy lost bin:
  int l=(NL-1)*El/Energy;

  // locate Energy bin
  int a=1, b=NE-1;
  while( (b-a)>1 ) {
    int k = (a+b)/2;
    if( Eb[k] < Energy) a=k;
    else                b=k;
  }
  int e = a;

  // for(a=0; a<NA-2; a++)
  //   cout << "leaEF " << l << " " << e << " " << a << " " << Energy << " "
  // 	 << F[idx(l,e,  a)] << " " << FF(l,e,a,Energy) << endl;
  

  // sample angle
  G4double r = G4UniformRand();

  a=0, b=NA-2;
  while( (b-a)>1 ) {
    int k = (a+b)/2;
    if( FF(l,e,k,Energy) < r) a=k;
    else                      b=k;
  }

  double x;
  
  double dx = F[idx(l,0,a+1)] - F[idx(l,0,a)];
  double dF = FF(l,e,a+1,Energy) - FF(l,e,a,Energy);
  double s = (r - FF(l,e,a,Energy)) / dF;
  
  // (1) linear F, step f
  x = F[idx(l,0,a)] + s*dx;


  // (2) linear f, sqr F
  // double fa = ff(e,a,Energy);
  // double df = ff(e,a+1,Energy) - ff(e,a,Energy);
  // double sf = ff(e,a+1,Energy) + ff(e,a,Energy);
  // double dx2 = F[idx(l,0,a+2)] - F[idx(l,0,a+1)];
  // double dF2 = FF(l,e,a+2,Energy) - FF(l,e,a+1,Energy);
  // double fa = dF/dx;
  // double fb = dF2/dx2;
  // double df = fb - fa;
  // double sf = fb + fa;
  // if( df == 0) x = F[idx(l,0,a)] + s*dx;
  // else         x = F[idx(l,0,a)] + ( sqrt(fa*fa + sf*df*s) -fa) *dx/df;
  

  // (3) cubic F, quad f
  // double A = 1/dx/dF;
  // double B = 3 -2*A -dF/dx/dF2;
  // double x = F[l][0][a] +(A +B*s +(1-A-B)*s*s)*s *dx;

  // (4) rational F, f
  // double B = 1 - dF/dx/dx/dF2;
  // double A = 1/dx -B -1;
  // double x = F[l][0][a] + (1 +A +B)*s / (1 +A*s +B*s*s) *dx;


  // (5) direct sample from inverse-cumulative
  // int i = lrint(1000*r);
  // double x = G[l][e][i];

  //  cout << "sampleang " << NE << " " << NL << " " << NA<< " " <<Energy << " " << El
  //<< " " << e << " " << l << " " << x << endl << endl;
  return( x);
}


void LeptsDiffXS::PrintDXS(G4int Ne) {
  // Debug
  //#include <string>
  //using namespace std;
  cout << "NE " << Ne << endl;
}
