
#ifndef LeptsElossDistribution_h
#define LeptsElossDistribution_h 1

#include "globals.hh"
#include "Randomize.hh"
#include <string.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <map> 
using namespace std;
class LeptsDistribution;
typedef std::map<G4double, LeptsDistribution*> mddist;
typedef std::map<G4double, mddist > mdmddist;

#define NMAX 20000

class LeptsElossDistribution {

public:
  LeptsElossDistribution( string);    // Constructor
  void ReadFile();          // Read file
  G4double Sample( G4double, G4double);
  G4double SampleMeanEnergyLost( G4double);

  G4bool IsFileFound() const {
    return bFileFound;
  }

private:
  mdmddist theDistributions; // Energy , angle , distribution
  G4int theNDistributions;
  string fileName;
  int    NoBins;
  G4bool bFileFound;

};


#endif
