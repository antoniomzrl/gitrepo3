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
#include "G4VLEPTSModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VLEPTSModel::G4VLEPTSModel(const G4String& modelName) : G4VEmModel(modelName),isInitialised(false) 
{
  theMeanFreePathTable=NULL;

  verboseLevel = 0;
  theXSType = XSEnergy; // dummy initialization
  theType = (G4String)"Energy";
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VLEPTSModel::~G4VLEPTSModel() 
{

  if(theMeanFreePathTable) {
    theMeanFreePathTable->clearAndDestroy();
    delete theMeanFreePathTable;
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4VLEPTSModel::Init() 
{
  theLowestEnergyLimit = 1e-26*eV;
  theHighestEnergyLimit = 1.0*MeV;
  //t    theHighestEnergyLimit = 15.0*MeV;
  SetLowEnergyLimit(theLowestEnergyLimit);
  SetHighEnergyLimit(theHighestEnergyLimit);
  theNumbBinTable = 1000;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double G4VLEPTSModel::GetMeanFreePath(const G4Material* aMaterial,
			       const G4ParticleDefinition* ,
			       G4double kineticEnergy )
{

  G4double MeanFreePath;
  G4bool isOutRange ;

  if( verboseLevel >= 3 ) G4cout << aMaterial->GetIndex() << " G4VLEPTSModel::GetMeanFreePath " << kineticEnergy << " > " << theHighestEnergyLimit << " < " << theLowestEnergyLimit << G4endl;
  if (kineticEnergy > theHighestEnergyLimit || kineticEnergy < theLowestEnergyLimit)
    MeanFreePath = DBL_MAX;
  else
    MeanFreePath = (*theMeanFreePathTable)(aMaterial->GetIndex())->
                                       GetValue(kineticEnergy, isOutRange);

  if( verboseLevel >= 3 ) G4cout << aMaterial->GetIndex() << GetName() << " G4VLEPTSModel::GetMeanFreePath MFP= " << MeanFreePath << G4endl;
  return MeanFreePath;
}

G4String XSName[15] = { "XSEnergy", 
	     "XSTotal", "XSElastic", "XSInelastic",
	     "XSIonisation", "XSExcitation", "XSDissociation",
	     "XSVibration", "XSAttach-Pstr-CT", "XSRotation"
			
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4VLEPTSModel::BuildPhysicsTable(const G4ParticleDefinition& aParticleType) 
{
  //CHECK IF PATH VARIABLE IS DEFINED
  char* path = getenv("G4LEDATA");
  if( !path ) {
    G4Exception("G4VLEPTSModel",
		"",
		FatalException,
		"variable G4LEDATA not defined");
  }

  // Build microscopic cross section table and mean free path table
   
  G4String aParticleName = aParticleType.GetParticleName();

  if (theMeanFreePathTable) {
    theMeanFreePathTable->clearAndDestroy();
    delete theMeanFreePathTable;
  }
  
  theMeanFreePathTable = new G4PhysicsTable(G4Material::GetNumberOfMaterials());

  //LOOP TO MATERIALS IN GEOMETRY 
  const G4MaterialTable * materialTable = G4Material::GetMaterialTable() ;
  std::vector<G4Material*>::const_iterator matite;
  for( matite = materialTable->begin(); matite != materialTable->end(); matite++ ) {
    const G4Material * aMaterial = (*matite);
    G4String mateName = aMaterial->GetName();

    //READ PARAMETERS FOR THIS MATERIAL
    string dirName = string(path) + "/lepts/";

    // backport
    char * bPath = getenv("LEPTSXSD");
    if( bPath) dirName = string(bPath) + "/";

    string FnParam  = dirName + mateName + ".txt";
    string FnMaster = dirName + "MasterDB.txt";
    string fnDXS, fnDXSROT, fnDXSSEC, fnIXS, fnELD, fnELDV, fnRMT;

    //G4bool bData = ReadParam(FnParam, aParticleName, aMaterial, fnDXS, fnIXS, fnELD, fnRMT);
    G4bool bData = ReadMasterFile(FnMaster, aParticleName, aMaterial,
				  fnDXS, fnDXSROT, fnDXSSEC,
				  fnIXS, fnELD, fnELDV, fnRMT);

    if( !fnIXS.empty() )    fnIXS  = dirName + fnIXS;
    if( !fnRMT.empty() )    fnRMT  = dirName + fnRMT;
    if( !fnELD.empty() )    fnELD  = dirName + fnELD;
    if( !fnELDV.empty() )   fnELDV = dirName + fnELDV;
    if( !fnDXS.empty() )    fnDXS  = dirName + fnDXS;
    if( !fnDXSROT.empty() ) fnDXSROT = dirName + fnDXSROT;
    if( !fnDXSSEC.empty() ) fnDXSSEC = dirName + fnDXSSEC;

    //AMR
    // if( !bData ) {      // AMR Try without prefix "LEPTS_"
    //   size_t sp = FnParam.find("LEPTS_");
    //   if (sp != std::string::npos) {
    // 	FnParam.erase(sp, 6);
    // 	bData = ReadParam( FnParam, aParticleName, aMaterial, fnDXS, fnIXS, fnELD, fnRMT);
    //   }
    // }


    if( !bData ) {
      G4PhysicsLogVector* ptrVector = new G4PhysicsLogVector(theLowestEnergyLimit, theHighestEnergyLimit, 2);
      ptrVector->PutValue(0, DBL_MAX);
      ptrVector->PutValue(1, DBL_MAX);

      unsigned int matIdx = aMaterial->GetIndex();
      theMeanFreePathTable->insertAt( matIdx , ptrVector ) ;
      continue; // MATERIAL NOT EXISTING, DO NOT READ OTHER FILES
    }

    //READ INTEGRAL CROSS SECTION FOR THIS MATERIAL
    
    std::map< G4int, std::vector<G4double> > integralXS = ReadIXS(fnIXS, aMaterial, theType);
    if( verboseLevel >= 2 ) G4cout << GetName() << " : " << theXSType << " " << mateName << " INTEGRALXS " << integralXS.size() << G4endl;

    if( integralXS.size() == 0 ) {
      //G4cerr << "Integral XS set to 0 for " << mateName << "\n\n";
      G4PhysicsLogVector* ptrVector = new G4PhysicsLogVector(theLowestEnergyLimit, theHighestEnergyLimit, 2);
      ptrVector->PutValue(0, DBL_MAX);
      ptrVector->PutValue(1, DBL_MAX);

      unsigned int matIdx = aMaterial->GetIndex();
      theMeanFreePathTable->insertAt( matIdx , ptrVector ) ;

    } else {
    
      if( verboseLevel >= 2 ) {
	std::map< G4int, std::vector<G4double> >::const_iterator itei;
	for( itei = integralXS.begin(); itei != integralXS.end(); itei++ ){
	  G4cout << GetName() << " : " << (*itei).first << " INTEGRALXS NDATA " << (*itei).second.size() << G4endl;
	}
      }
      
      BuildMeanFreePathTable( aMaterial, integralXS );
  
      theDiffXS[aMaterial] = new G4LEPTSDiffXS(fnDXS, aParticleName);
      if( !theDiffXS[aMaterial]->IsFileFound() ) {
	G4Exception("G4VLEPTSModel::BuildPhysicsTable", "", FatalException,
		    (G4String("File not found :" + fnDXS).c_str()));
      }

      theDiffXSROT[aMaterial] = new G4LEPTSDiffXS(fnDXSROT, aParticleName);
      if( !theDiffXS[aMaterial]->IsFileFound() ) {
	G4Exception("G4VLEPTSModel::BuildPhysicsTable", "", FatalException,
		    (G4String("File not found :" + fnDXSROT).c_str()));
      }
      
      theDiffXSSEC[aMaterial] = new G4LEPTSDiffXS(fnDXSSEC, aParticleName);
      if( !theDiffXS[aMaterial]->IsFileFound() ) {
	G4Exception("G4VLEPTSModel::BuildPhysicsTable", "", FatalException,
		    (G4String("File not found :" + fnDXSSEC).c_str()));
      }

      theRMTDistr[aMaterial] = new G4LEPTSDistribution();
      theRMTDistr[aMaterial]->ReadFile(fnRMT);

      if( fnELD.empty() ) return;
      theElostDistr[aMaterial] = new G4LEPTSElossDistr(fnELD);
      if( !theElostDistr[aMaterial]->IsFileFound() ) {
	G4Exception("G4VLEPTSModel::BuildPhysicsTable", "", FatalException,
		    (G4String("File not found :" + fnELD).c_str()));
      }

      if( fnELDV.empty() ) return;
      theElostDistrVib[aMaterial] = new G4LEPTSElossDistr(fnELDV);
      if( !theElostDistrVib[aMaterial]->IsFileFound() ) {
	G4Exception("G4VLEPTSModel::BuildPhysicsTable", "", FatalException,
		    (G4String("File not found :" + fnELDV).c_str()));
      }

    }

  }
  
}

void G4VLEPTSModel::BuildMeanFreePathTable( const G4Material* aMaterial, std::map< G4int, std::vector<G4double> >& integralXS )
{
  int theColumn = theXSType;
  if( theNXSsub[aMaterial] == 1) theColumn =1;

  G4double LowEdgeEnergy, fValue, crossSection=0, eVEnergy;

  //BUILD MEAN FREE PATH TABLE FROM INTEGRAL CROSS SECTION
  unsigned int matIdx = aMaterial->GetIndex();
  G4PhysicsLogVector* ptrVector = new G4PhysicsLogVector(theLowestEnergyLimit, theHighestEnergyLimit, theNumbBinTable);

    
  for (G4int ii=0; ii < theNumbBinTable; ii++) {
    LowEdgeEnergy = ptrVector->GetLowEdgeEnergy(ii);
    if( verboseLevel >= 2 )
      G4cout << GetName() << " " << ii << " LowEdgeEnergy " << LowEdgeEnergy << " > "
	     << theLowestEnergyLimit << " < " << theHighestEnergyLimit << G4endl;
    //-      fValue = ComputeMFP(LowEdgeEnergy, material, aParticleName);
    fValue = 0.;
    if( LowEdgeEnergy >= theLowestEnergyLimit && 
	LowEdgeEnergy <= theHighestEnergyLimit) {
      G4double NbOfMoleculesPerVolume = aMaterial->GetDensity()/theMolecularMass[aMaterial]*Avogadro; 
      
      G4double SIGMA = 0. ;
      //-      for ( size_t elm=0 ; elm < aMaterial->GetNumberOfElements() ; elm++ ) {
      crossSection = 0.;
      eVEnergy = LowEdgeEnergy/eV;
	
      if( verboseLevel >= 2 )
	G4cout << " eVEnergy " << eVEnergy << " LowEdgeE " << LowEdgeEnergy
	       << " " << integralXS[theColumn][1] << G4endl;
	
      if(eVEnergy < integralXS[0][1] ) {
	crossSection = 0.;
      }
      else {
	G4int Bin = 0; // locate bin                                                                           
	G4double aa, bb;
	for( G4int jj=1; jj<theNXSdat[aMaterial]; jj++) {  // Extrapolate for E > Emax !!!
	  if( verboseLevel >= 3 )
	    G4cout << " GET BIN " << jj << " "<< eVEnergy << " > " << integralXS[0][jj] << G4endl; 
	  if( eVEnergy > integralXS[0][jj]) {
	    Bin = jj;
	  } else {
	    break;
	  }
	}
	aa = integralXS[0][Bin];
	bb = integralXS[0][Bin+1];
	crossSection = (integralXS[theColumn][Bin] + (integralXS[theColumn][Bin+1]-integralXS[theColumn][Bin])
			/ (bb-aa)*(eVEnergy-aa) ) * 1.e-16*cm2;
	  
	if( verboseLevel >= 3 )
	  G4cout << GetName() << " crossSection " <<  crossSection << " " <<integralXS[theColumn][Bin]
		 << " + " << (integralXS[theColumn][Bin+1]-integralXS[theColumn][Bin]) << " / " << (bb-aa)
		 << " *" << (eVEnergy-aa) << " * " << 1.e-16*cm2 << G4endl;;
	  
	//	  SIGMA += NbOfAtomsPerVolume[elm] * crossSection;
	SIGMA = NbOfMoleculesPerVolume * crossSection;
	if( verboseLevel >= 2 )
	  G4cout << GetName() << " ADDING SIGMA " << SIGMA << " NAtoms " << NbOfMoleculesPerVolume
		 << " Bin " << Bin << " TOTAL " << aa << " " << bb 
		 << " XS " << integralXS[theColumn][Bin]  << " " << integralXS[theColumn][Bin+1] << G4endl;
      }
      
      fValue = SIGMA > DBL_MIN ? 1./SIGMA : DBL_MAX;
    }
    
    ptrVector->PutValue(ii, fValue);
    if( verboseLevel >= 2 )
      G4cout << GetName() << " ptrvector(" << ii << ") = " << eVEnergy << "\t" << fValue
	     << "\t" << crossSection/1e-16/cm2 << G4endl;
  }
  
  theMeanFreePathTable->insertAt( matIdx , ptrVector ) ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// G4double G4VLEPTSModel::SampleAngle(const G4Material* aMaterial, G4double e, G4double el) 
// {
//   G4double x;

//   if( e < 10001 ) {
//     G4double xe = theDiffXS[aMaterial]->SampleAngle(e, 0.0);
//     G4double xi = theDiffXS[aMaterial]->SampleAngleMT(e, el);
//     x = xe + xi;
//     if(x > pi) {
//       cout << "yyyyyyy " << x << " " << twopi-x << endl;
//       x = twopi -x;
//     }
//   }
//   else {
//     G4double Ei = e;                                       //incidente
//     G4double Ed = e -el;                                   //dispersado
      
//     G4double Pi = sqrt( pow( (Ei/27.2/137),2) +2*Ei/27.2); //incidente
//     G4double Pd = sqrt( pow( (Ed/27.2/137),2) +2*Ed/27.2); //dispersado

//     G4double Kmin = Pi - Pd;
//     G4double Kmax = Pi + Pd;

//     G4double KR = theRMTDistr[aMaterial]->Sample(Kmin, Kmax);          //sorteo mom. transf.
      
//     G4double co = (Pi*Pi + Pd*Pd - KR*KR) / (2*Pi*Pd);    //cos ang. disp.
//     if( co > 1. ) co = 1.;
//     x = std::acos(co); //*360/twopi;                         //ang. dispers.
//   }
//   return(x);
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// G4ThreeVector G4VLEPTSModel::SampleNewDirection(const G4Material* aMaterial,
// 						G4ThreeVector P0Dir, G4double e, G4double el) {
//   G4double x = SampleAngle(aMaterial, e, el);

//   G4double cosTeta = std::cos(x); //*twopi/360.0);
//   G4double sinTeta = std::sqrt(1.0-cosTeta*cosTeta);
//   G4double Phi     = twopi * G4UniformRand() ;
//   G4double dirx    = sinTeta*std::cos(Phi) , diry = sinTeta*std::sin(Phi) , dirz = cosTeta ;

//   G4ThreeVector P1Dir(dirx, diry, dirz);

//   return(P1Dir);
// }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ThreeVector G4VLEPTSModel::SampleNewDirection(G4ThreeVector P0Dir, G4double x) 
{
  G4double cosTeta = std::cos(x); //*twopi/360.0);
  G4double sinTeta = std::sqrt(1.0-cosTeta*cosTeta);
  G4double Phi     = twopi * G4UniformRand() ;
  G4double dirx    = sinTeta*std::cos(Phi) , diry = sinTeta*std::sin(Phi) , dirz = cosTeta ;

  G4ThreeVector P1Dir( dirx,diry,dirz );
  P1Dir.rotateUz(P0Dir);

  return(P1Dir);
}


// Inversa de ElasticEnergyTransfer
double G4VLEPTSModel::ElasticAngle(double E, double W, double MT, double MP) {
  double co = (E * (E+2*MP) - W * (E+MP+MT)) /
    std::sqrt( E * (E+2*MP) * (E-W) * (E-W+2*MP) );
  return( acos(co));
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double G4VLEPTSModel::ElasticEnergyTransfer(G4double E, G4double ang,
					      G4double MT, G4double MP) 
{
  G4double co = std::cos(ang);
  G4double si = std::sin(ang);

  G4double W = ( (E+MP)*si*si + MT - co*std::sqrt(MT*MT-MP*MP*si*si) ) * E*(E+2*MP)
    / ( (E+MP+MT)*(E+MP+MT) - E*(E+2*MP)*co*co );

  //G4double W2 = 2*MP/MT*(1-co)*E;
  //G4cout << "WWWWWWWWW: " << W/E << " " << E/W << " " << W2/W << G4endl;
  //G4cout << "Mm " << MT/MeV << " " << MP/MeV << G4endl;
  //if( W > E*0.9 )
  //cout << "EnergyTransfer: " << E/eV << " " << W/eV << " " << ang << endl;
  return W;
}


// FractionElasticEnergyTransferNonRelativistic (W/E)
G4double G4VLEPTSModel::FractionET(G4double ang, G4double MT, G4double MP) 
{
  G4double co = std::cos(ang);
  G4double si = std::sin(ang);

  G4double F = ( MP*si*si + MT - co*std::sqrt(MT*MT-MP*MP*si*si) ) *2*MP / (MP+MT)/(MP+MT);
  return F;
}

// Ya no sirve pq la receta es invertible
double G4VLEPTSModel::AdjustAngle(double angE, double W, double E, double MT, double MP) {
  double a=angE, b=pi/2, eps= 0.1*twopi/360.0;
  if(a>b) { a=pi/2; b=angE; }
  
  double fa = ElasticEnergyTransfer(E, a, MT, MP) -W;
  double fb = ElasticEnergyTransfer(E, b, MT, MP) -W;
  if( (fa*fb)>0) {
    cout << "AdjustAngle Error" << endl;
    return(angE);
  }
  
  while( (b-a)> eps) {
    double x = (b+a)/2.0;         // punto medio
    //double x = (a*fb-b*fa)/(fb-fa); // cuerda
    double fx = ElasticEnergyTransfer(E, x, MT, MP) -W;
    //cout << "(" << a/twopi*360 << " , " << b/twopi*360 << ")\t" << fa << " / " << fb << endl;
    if( (fa*fx)>0 ) {
      a = x; fa = fx;
    } else {
      b = x; fb = fx;
    }
  }

  double ang = (a+b)/2.0;
  return(ang);  
}

//AMR 2015-11
bool G4VLEPTSModel::GetValues(string FnMaster, string pName, string theMaterial,
			      string& FnDXS, string& FnDXSROT, string& FnDXSSEC,
			      string& FnIXS, string& FnELD, string& FnELDV, string& FnRMT,
			      double& IonisPot, double& IonisPotInt, double& IonisPotInt2) {

  std::ifstream fin(FnMaster.c_str() );
  if (!fin.is_open()) {
    G4cout << "\nNo File " << FnMaster << " " << theMaterial << endl;
    return false;
  }
  else {
    G4cout << "    (" << pName << " / " << theMaterial << " / " << XSName[theXSType] << ")\n";
  }

  IonisPot=0;
  IonisPotInt=1e9*eV;
  IonisPotInt2=1e9*eV;
  
  string str, w1, w2, w3;
  G4bool InSection = false, MaterialFound = false;
  string theMat = " " + theMaterial + " ";
  
  while( getline(fin, str) ) {
    str = str + " ";
    istringstream iss(str);
    iss >> w1 >> w2 >> w3;

    if( w1 == "begin" || w1 == "BEGIN") {
      size_t found = str.find(theMat);
      if (found != std::string::npos) {
	InSection = true;
	MaterialFound = true;
	cout << "Masterfile section: " << str << endl;
      }
    }
    else if( w1 == "end" || w1 == "END")
      InSection = false;

    if( InSection == true) {
      if(     w1 == "IP"                    ) IonisPot = atof(w2.c_str());
      else if(w1 == "IIP"                   ) IonisPotInt = atof(w2.c_str());
      else if(w1 == "IIP2"                  ) IonisPotInt2 = atof(w2.c_str());
      else if(w1 == pName && w2 == "DXS"    ) FnDXS = w3;
      else if(w1 == pName && w2 == "DXSROT" ) FnDXSROT = w3;
      else if(w1 == pName && w2 == "DXSSEC" ) FnDXSSEC = w3;
      else if(w1 == pName && w2 == "IXS"    ) FnIXS = w3;
      else if(w1 == pName && w2 == "RMT"    ) FnRMT = w3;
      else if(w1 == pName && w2 == "ELD"    ) FnELD = w3;
      else if(w1 == pName && w2 == "ELDVIB" ) FnELDV = w3;
      else if(w1 == pName && w2 == "ELDION" && theXSType == XSIonisation  ) FnELD = w3;
      else if(w1 == pName && w2 == "ELDEXC" && theXSType == XSExcitation  ) FnELD = w3;
      else if(w1 == pName && w2 == "ELDDIS" && theXSType == XSDissociation) FnELD = w3;
      else if(w1 == "inherit") {
	string pMaterial = w2;
	cout << theMaterial << " derived from  " << pMaterial << endl;
	GetValues(FnMaster, pName, pMaterial,
		  FnDXS, FnDXSROT, FnDXSSEC, FnIXS, FnELD, FnELDV, FnRMT,
		  IonisPot, IonisPotInt, IonisPotInt2);
      }
    }
  }

  if( MaterialFound == false) {
    cout << "\nCouple not found (" << pName << " / " << theMaterial << ") ";
    return false;
  }
  else
    return true;
}

//AMR 2015-05
G4bool G4VLEPTSModel::ReadMasterFile(G4String FnMaster, G4String pName, const G4Material* aMaterial,
				     string& FnDXS, string& FnDXSROT, string& FnDXSSEC,
				     string& FnIXS, string& FnELD, string& FnELDV, string& FnRMT) {

  string theMaterial = aMaterial->GetName();
  double IonisPot, IonisPotInt, IonisPotInt2;
  
  GetValues(FnMaster, pName, theMaterial,
   	    FnDXS, FnDXSROT, FnDXSSEC, FnIXS, FnELD, FnELDV, FnRMT,
	    IonisPot, IonisPotInt, IonisPotInt2);

  if( verboseLevel >= 1 )
    G4cout << "ReadMaster" << FnMaster << G4endl
	   << "IP:      " << IonisPot << " IIP: "
	   << IonisPotInt << " IIP2: "  << IonisPotInt2 << G4endl
	   << "FnIXS    " << FnIXS << G4endl
      	   << "FnDXS    " << FnDXS << G4endl
	   << "FnDXSROT " << FnDXSROT << G4endl
	   << "FnDXSSEC " << FnDXSSEC << G4endl
	   << "FnRMT    " << FnRMT << G4endl
	   << "FnELD    " << FnELD << G4endl
  	   << "FnELDV   " << FnELDV << G4endl;

  theIonisPot[aMaterial] = IonisPot * eV;
  theIonisPotInt[aMaterial] = IonisPotInt * eV;
  theIonisPotInt2[aMaterial] = IonisPotInt2 * eV;

  G4double MolecularMass = 0;
  size_t nelem = aMaterial->GetNumberOfElements();
  const G4int*  atomsV = aMaterial->GetAtomsVector();
  for( size_t ii = 0; ii < nelem; ii++ ) {
    if( atomsV ) {
      MolecularMass += aMaterial->GetElement(ii)->GetA()*atomsV[ii]/g;
    } else {
      MolecularMass += aMaterial->GetElement(ii)->GetA()*1./g;
    }
    //    G4cout << " MMASS1 " << mmass/g << " " << aMaterial->GetElement(ii)->GetName() << " " << aMaterial->GetElement(ii)->GetA()/g << G4endl;
  }
  //  G4cout << " MMASS " << MolecularMass << " " << MolecularMass*g <<  " ME " << mmass << " " << mmass/g << G4endl; 
  theMolecularMass[aMaterial] = MolecularMass* g/mole;
  //  theMolecularMass[aMaterial] = aMaterial->GetMassOfMolecule()*Avogadro;  // Material mixtures do not calculate molecular mass

  InitMassArrays(pName);

  if( verboseLevel >= 1 )
    {
    double MP = theMassProjectile[aMaterial];
    double MT = theMassTarget[aMaterial];
    cout << "EET " << MT/MeV << " " << MP/MeV << " MeV" << endl;
    for(double E=10*eV; E < 200*MeV; E*=100) {
      for(double a=0; a<181; a+=1) {
	double ar = a*twopi/360.0;
	double W = ElasticEnergyTransfer(E, ar, MT, MP);
	cout << "wewe " << a << "\t" << W/E << "\t"  << E/eV << "\t\t" << 2.0/(1 + MT/MP)<< endl;
      }
      cout << endl;
    }
    cout << "EETend" << endl;
  }

  return true;
}


void G4VLEPTSModel::InitMassArrays(const G4String theParticle) {
 
  G4double theMc2 = 0.0;
  if(     theParticle == "e-") theMc2 = electron_mass_c2;
  else if(theParticle == "e+") theMc2 = electron_mass_c2;
  else if(theParticle == "proton"  ) theMc2 = proton_mass_c2;
  else if(theParticle == "hydrogen") theMc2 = proton_mass_c2 + electron_mass_c2;

    
  const G4MaterialTable * materialTable = G4Material::GetMaterialTable() ;
  std::vector<G4Material*>::const_iterator matite;
  for( matite = materialTable->begin(); matite != materialTable->end(); matite++ ) {
    const G4Material * aMaterial = (*matite);
    theMassTarget[aMaterial]  = theMolecularMass[aMaterial] /(6.02214179e+23/mole)*c_light*c_light;
    theMassProjectile[aMaterial] = theMc2;

    G4cout << "    " << aMaterial->GetName() << " " << theMolecularMass[aMaterial]/(g/mole)
	   << " g/mole " << theMassTarget[aMaterial]/MeV << " MeV "
	   << theParticle << " " << theMassProjectile[aMaterial]/MeV << " MeV "
	   << aMaterial->GetDensity()/(g/cm3) << " g/cm3 "
	   << G4endl;
  }
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// old version
std::map< G4int, std::vector<G4double> > G4VLEPTSModel::ReadIXS(G4String fnIXS, const G4Material* aMaterial ) 
{
  std::map< G4int, std::vector<G4double> > integralXS; // process type - energy
  //G4cout << "fnIXS (" << fnIXS << ")" << G4endl;

  std::ifstream fin(fnIXS);
  if (!fin.is_open()) {
    //G4Exception("G4VLEPTSModel::ReadIXS", "", JustWarning,
    //(G4String("File not found: ")+ fnIXS).c_str());
    return integralXS;
  }

  G4cout << "    Reading\t" << fnIXS << G4endl;
  
  G4int nXSdat, nXSp;
  fin >> nXSdat >> nXSp;
  if( verboseLevel >= 1 )
    G4cout << "Read IXS (" << fnIXS << ")\t nXSdat: " << nXSdat << " nXSp: "  << nXSp << G4endl;
  theNXSdat[aMaterial] = nXSdat;
  theNXSsub[aMaterial] = nXSp;

  G4double xsdat;
  for (G4int ip=0; ip<=nXSp; ip++) {   
    integralXS[ip].push_back(0.);
  }
  for (G4int ie=1; ie<=nXSdat; ie++) {
    for (G4int ip=0; ip<=nXSp; ip++) {   
      fin >> xsdat;
      integralXS[ip].push_back(xsdat);

      if( verboseLevel >= 3 )
	G4cout << GetName() << " IXS(" << ie << " " << ip << ")= " << integralXS[ip][ie] << " " << xsdat
	       << " / " << nXSdat << " " << nXSp << G4endl; 
      // xsdat 1e-16*cm2
    }
  }
  fin.close();

  return integralXS;
}


// new version
std::map< G4int, std::vector<G4double> >
G4VLEPTSModel::ReadIXS(G4String fnIXS, const G4Material* aMaterial, G4String xsType)
{
  std::map< G4int, std::vector<G4double> > integralXS; // process type - energy
  //G4cout << "fnIXS (" << fnIXS << ")" << G4endl;

  std::ifstream fin(fnIXS);
  if (!fin.is_open()) {
    //G4Exception("G4VLEPTSModel::ReadIXS", "", JustWarning,
    //		(G4String("File not found: ")+ fnIXS).c_str());
    return integralXS;
  }

  string line;
  getline(fin, line);
  if(line[0] != '#') {
    fin.close();
    integralXS = ReadIXS(fnIXS, aMaterial); //old version
    return integralXS;
  }
  else
    line[0] = ' ';
  
  istringstream stm(line);
  string word;
  vector <string> vline;
  int nXSdat=0, theColumn=0;
  
  while( stm >> word ) {
    if(word != (string)",") {
      vline.push_back(word);
    }
  }
  
  for( unsigned int i=0; i < vline.size(); i++) {
    //cout << "vline (" << vline[i] << ") (" << xsType << ")" << endl;
    //if( vline[i] == xsType) theColumn = i+1;
    size_t found = vline[i].find(xsType);
    if (found != std::string::npos) theColumn = i+1;
  }

  cout << "    Reading\t" << fnIXS << "\t(" << xsType << ", column "
       << theColumn << "/" << vline.size() << ")" << endl;
  
  double theE, theX;
  
  integralXS[0].push_back(0.);
  integralXS[1].push_back(0.);

  while( getline(fin, line) ) {
    replace( line.begin(), line.end(), ',', ' ');
    istringstream strm(line);
    strm >> theE;
    for(int i=1; i<theColumn; i++) strm >> theX;
    nXSdat++;
    integralXS[0].push_back(theE);
    integralXS[1].push_back(theX);

    if( verboseLevel >= 3 )
      cout << GetName() << " IXS= " << theE << " " << theX << endl; 
  }
  
  fin.close();

  theNXSsub[aMaterial] = 1;
  theNXSdat[aMaterial] = nXSdat;
  
  return integralXS;
}
