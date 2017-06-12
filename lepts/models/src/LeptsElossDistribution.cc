
#include "LeptsElossDistribution.hh"
#include "LeptsDistribution.hh"


LeptsElossDistribution::LeptsElossDistribution(string file) 
{
  fileName = file;

  ReadFile();
}


void LeptsElossDistribution::ReadFile() 
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
      LeptsDistribution* dist = new LeptsDistribution();
      theNDistributions ++;
      mddist angleDist;
      angleDist[angleSep] = dist;
      theDistributions[energySep] = angleDist;
      
      if( fscanf(fp,"%s %i", comment, &nData)==0) cout << "readerror5\n";
      if( dist->ReadFile( fp, nData ) ) {
	G4Exception("LeptsElossDistribution",
		    "",
		    FatalException,
		    ("End of file found while reading file"+ fileName).c_str());	
      }
    }
  }
  
  fclose(fp);

}



G4double LeptsElossDistribution::Sample( G4double eMin, G4double eMax) {
// Sample Energy from Cumulative distr. G4interval [eMin, eMax]

  if( eMin > eMax) return 0.0;

  //Get the distribution to do the sampling
  LeptsDistribution* distr  = 0;
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

  //  G4cout << " LEPTSElossDistribution::Sample(  " << distr << " NDIST " << theNDistributions << G4endl; //GDEB
  return distr->Sample(eMin, eMax);
}


G4double LeptsElossDistribution::SampleMeanEnergyLost( G4double e) {

  LeptsDistribution* distr  = 0;
  if( theNDistributions == 1 ){
    distr = (*( (*(theDistributions.begin())).second ).begin()).second;
  }

  return distr->SampleMeanEnergyLost(e);
}
