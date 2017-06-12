
#ifndef LeptsDistribution_h
#define LeptsDistribution_h 1

#include "globals.hh"
#include "Randomize.hh"
#include <string.h>
#include <stdio.h>
#include <string>
#include <iostream> 
using namespace std;

#define NMAX 20000

class LeptsDistribution {

public:
  LeptsDistribution();    // Constructor
  void ReadFile( G4String fileName );          // Read file
  G4bool ReadFile( FILE* fp, G4int nData );
  G4double Sample( G4double, G4double);
  G4double SampleMeanEnergyLost( G4double);
  
  G4bool IsFileFound() const {
    return bFileFound;
  }

private:
  int    NoBins;
  double Fnorm;
  G4double E[NMAX], f[NMAX], F[NMAX], eF[NMAX];
  G4bool bFileFound;
};


#endif
