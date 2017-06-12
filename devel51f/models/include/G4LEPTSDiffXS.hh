//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#ifndef G4LEPTSDiffXS_h
#define G4LEPTSDiffXS_h 1


#include <string>
using namespace std;
#include <CLHEP/Units/SystemOfUnits.h>
using namespace CLHEP;

class G4LEPTSDiffXS {

public:

  G4LEPTSDiffXS( string, string);   // Constructor

  void readDXS();    // Read file
  void BuildCDXS();
  void BuildCDXS(G4double, G4double);
  void NormalizeCDXS();
  void PrintDXS(int);
  
  G4double RelativisticMomentum( G4double);
  G4double SampleAngle(G4double, G4double);
  //  G4double SampleAngleMT(G4double, G4double);
  //G4double SampleAngleInelastic(G4double, G4double);
  
  G4double SampleAngleEthylene(G4double, G4double);
  G4bool IsFileFound() const {
    return bFileFound;
  }

private:
  string fileName;
  G4double theMc2;
  G4double Eb[100];
  //  G4double DXS[100][190], CDXS[100][190], IDXS[100][19000], ICDXS[100][19000];
  //G4double DXS[100][290], CDXS[100][290], ICDXS[100][19000];
  //  G4double KT[100][190],  CKT[100][190],  IKT[100][19000];
  //G4double KT[100][290],  IKT[100][19000];


  //New dxs=f, cdxs=F[El/E*10][Ebin][aBin], Finv=Fs(direct sample)
  //G4double f[40][300], F[11][40][300]; //, G[11][40][1001];

  int NL, NE, NA;

  double *f, *F;
  inline int idx(int e, int a) { return NA*e + a; };
  inline int idx(int l, int e, int a) { return NA*(NE*l+e) +a; }; //7.47
  //inline int idx(int l, int e, int a) { return NA*(NL*e+l) +a; }; //7.27
  //inline int idx(int l, int e, int a) { return NE*(NL*a+l) +e; }; //7.26

  inline double FF(int l, int e, int a, double E);
  inline double ff(int e, int a, double E);  
  G4bool bFileFound;
};

#endif
