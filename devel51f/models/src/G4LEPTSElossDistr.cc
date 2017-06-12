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
#include "G4LEPTSElossDistr.hh"
#include "G4LEPTSDistribution.hh"


G4LEPTSElossDistr::G4LEPTSElossDistr(string file) 
{
  fileName = file;

  ReadFile();
}


void G4LEPTSElossDistr::ReadFile() 
{
  theNDistributions = 0;

  FILE * fp;

  if ((fp=fopen(fileName.c_str(), "r"))==NULL){
    //G4cout << "Error reading " << fileName << G4endl;
    NoBins = 0;
    bFileFound = false;
    return;
  } 

  G4cout << " Reading\t" << fileName;

  bFileFound = true;
  //  G4cout << "Read Eloss Distro (" << fileName << ") " << G4endl;
  G4int nEnergies, nAngles, nData;
  char comment[32];
  
  if( fscanf(fp,"%s %i", comment, &nEnergies)==0) cout << "readerror1\n";
  for( G4int ie = 0; ie < nEnergies; ie++ ){
    float energySep; 
    if( fscanf(fp,"%s %f", comment, &energySep)==0) cout << "readerror2\n";
    if( fscanf(fp,"%s %i", comment, &nAngles)==0) cout << "readerror3\n";
    for( G4int ia = 0; ia < nAngles; ia++ ){
      float angleSep; 
      if( fscanf(fp,"%s %f", comment, &angleSep)==0) cout << "readerror4\n";
      G4LEPTSDistribution* dist = new G4LEPTSDistribution();
      theNDistributions ++;
      mddist angleDist;
      angleDist[angleSep] = dist;
      theDistributions[energySep] = angleDist;
      
      if( fscanf(fp,"%s %i", comment, &nData)==0) cout << "readerror5\n";
      if( dist->ReadFile( fp, nData ) ) {
	G4Exception("G4LEPTSElossDistr",
		    "",
		    FatalException,
		    ("End of file found while reading file"+ fileName).c_str());	
      }
    }
  }
  
  fclose(fp);

}



G4double G4LEPTSElossDistr::Sample( G4double eMin, G4double eMax) 
{
// Sample Energy from Cumulative distr. G4interval [eMin, eMax]

  if( eMin > eMax) return 0.0;

  //Get the distribution to do the sampling
  G4LEPTSDistribution* distr  = 0;
  if( theNDistributions == 1 ){
    distr = (*( (*(theDistributions.begin())).second ).begin()).second;
  } else{
    mdmddist::const_iterator itedd;
    for( itedd = theDistributions.begin(); itedd != theDistributions.end(); itedd++ ){
      G4double energySep = (*itedd).first;
      if( eMax < energySep ) {
	//tt	if( eMax <= energySep ) { 
	mddist dist1 = (*itedd).second;
	mddist::const_iterator ited;
	for( ited = dist1.begin(); ited != dist1.end(); ited++ ){
	  G4double angleSep = (*ited).first;
	  if( 1 < angleSep ) {
	    distr = (*ited).second;
	    break;
	  }
	}
	break;
      }
    }
  }

  //  G4cout << " LEPTSElossDistr::Sample(  " << distr << " NDIST " << theNDistributions << G4endl; //GDEB
  return distr->Sample(eMin, eMax);
}
