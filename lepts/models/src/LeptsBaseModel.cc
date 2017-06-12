
#include "LeptsBaseModel.hh"


LeptsBaseModel::LeptsBaseModel(const G4String& modelName) : G4VEmModel(modelName) {
  theMeanFreePathTable=NULL;

  verboseLevel = 0;
  theXSType = XSEnergy; // dummy initialization
  theType = (G4String)"Energy";

  cout << "(Constructor LeptsBaseModel " << modelName << ")" << endl;
  
  bIXS    = true;
  bDXS    = false;
  bDXSMT  = false;
  bDXSSEC = false;
  bELD    = false;
}


LeptsBaseModel::~LeptsBaseModel() {
  if(theMeanFreePathTable) {
    theMeanFreePathTable->clearAndDestroy();
    delete theMeanFreePathTable;
  }
}



void LeptsBaseModel::Initialise(const G4ParticleDefinition* aParticle, const G4DataVector&) {
  theLowestEnergyLimit = 1e-26*eV;
  theHighestEnergyLimit = 10.0*MeV;
  //t    theHighestEnergyLimit = 15.0*MeV;
  SetLowEnergyLimit(theLowestEnergyLimit);
  SetHighEnergyLimit(theHighestEnergyLimit);
  theNumbBinTable = 1000;

  BuildPhysicsTable( *aParticle );

  fParticleChange = GetParticleChangeForGamma();

  LowestElectronicExcitationEnergy = 0;
  LowestNeutralDisociationEnergy = 0;
}



G4double LeptsBaseModel::CrossSectionPerVolume(const G4Material* mate,
					       const G4ParticleDefinition* aParticle,
					       G4double kineticEnergy, G4double, G4double) {
  if( kineticEnergy < theLowestEnergyLimit )
    return DBL_MAX;
  else
    return 1.0/GetMeanFreePath( mate, aParticle, kineticEnergy);
}



G4double LeptsBaseModel::GetMeanFreePath(const G4Material* aMaterial,
					 const G4ParticleDefinition* ,
					 G4double kineticEnergy ) {

  G4double MeanFreePath;
  G4bool isOutRange ;

  if( verboseLevel >= 3 )
    cout << aMaterial->GetIndex() << " LeptsBaseModel::GetMeanFreePath " << kineticEnergy
	 << " > " << theHighestEnergyLimit << " < " << theLowestEnergyLimit << endl;
  if (kineticEnergy > theHighestEnergyLimit || kineticEnergy < theLowestEnergyLimit)
    MeanFreePath = DBL_MAX;
  else
    MeanFreePath = (*theMeanFreePathTable)(aMaterial->GetIndex())->
      GetValue(kineticEnergy, isOutRange);

  if( verboseLevel >= 3 )
    cout << aMaterial->GetIndex() << GetName() << " LeptsBaseModel::GetMeanFreePath MFP= "
	 << MeanFreePath << endl;
  return MeanFreePath;
}


G4String XSName[15] = { "XSEnergy", 
	     "XSTotal", "XSElastic", "XSInelastic",
	     "XSIonisation", "XSExcitation", "XSDissociation",
			"XSVibration", "XSAttach-Pstr-CT", "XSRotation",
			"XSUserProc"
			
};


void LeptsBaseModel::BuildPhysicsTable(const G4ParticleDefinition& aParticleType) {
  //CHECK IF PATH VARIABLE IS DEFINED
  char* path = getenv("G4LEDATA");
  if( !path ) {
    G4Exception("LeptsBaseModel",
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
    string fnDXS, fnDXSMT, fnDXSSEC, fnIXS, fnELD;

    G4bool bData = ReadMasterFile(FnMaster, aParticleName, aMaterial,
				  fnDXS, fnDXSMT, fnDXSSEC,
				  fnIXS, fnELD);

    if( !fnIXS.empty()   )  fnIXS  = dirName + fnIXS;
    if( !fnELD.empty()   )  fnELD  = dirName + fnELD;
    if( !fnDXS.empty()   )  fnDXS  = dirName + fnDXS;
    if( !fnDXSMT.empty()  ) fnDXSMT  = dirName + fnDXSMT;
    if( !fnDXSSEC.empty() ) fnDXSSEC = dirName + fnDXSSEC;


    if( !bData ) {
      G4PhysicsLogVector* ptrVector = new G4PhysicsLogVector(theLowestEnergyLimit,
							     theHighestEnergyLimit, 2);
      ptrVector->PutValue(0, DBL_MAX);
      ptrVector->PutValue(1, DBL_MAX);

      unsigned int matIdx = aMaterial->GetIndex();
      theMeanFreePathTable->insertAt( matIdx , ptrVector ) ;
      continue; // MATERIAL NOT EXISTING, DO NOT READ OTHER FILES
    }

    //READ INTEGRAL CROSS SECTION FOR THIS MATERIAL
    
    std::map< G4int, std::vector<G4double> > integralXS = ReadIXS(fnIXS, aMaterial, theType);
    cout << GetName() << " : " << theXSType << " " << mateName << " INTEGRALXS "
	 << integralXS.size() << endl;

    if( integralXS.size() == 0 ) {
      //G4cerr << "Integral XS set to 0 for " << mateName << "\n\n";
      G4PhysicsLogVector* ptrVector = new G4PhysicsLogVector(theLowestEnergyLimit,
							     theHighestEnergyLimit, 2);
      ptrVector->PutValue(0, DBL_MAX);
      ptrVector->PutValue(1, DBL_MAX);

      unsigned int matIdx = aMaterial->GetIndex();
      theMeanFreePathTable->insertAt( matIdx , ptrVector ) ;

    }
    else {
      if( verboseLevel >= 2 ) {
	std::map< G4int, std::vector<G4double> >::const_iterator itei;
	for( itei = integralXS.begin(); itei != integralXS.end(); itei++ ){
	  cout << GetName() << " : " << (*itei).first << " INTEGRALXS NDATA "
	       << (*itei).second.size() << endl;
	}
      }
      
      BuildMeanFreePathTable( aMaterial, integralXS );
  
      if( bDXS)    theDiffXS[aMaterial]    = new LeptsDiffXS(fnDXS,    aParticleName);
      if( bDXSMT)  theDiffXSMT[aMaterial]  = new LeptsDiffXS(fnDXSMT,  aParticleName);
      if( bDXSSEC) theDiffXSSEC[aMaterial] = new LeptsDiffXS(fnDXSSEC, aParticleName);
      if( !fnELD.empty() && bELD ) 
	theElostDistribution[aMaterial] = new LeptsElossDistribution(fnELD);
    }

  }
  
}



void LeptsBaseModel::BuildMeanFreePathTable(const G4Material* aMaterial,
					    std::map< G4int, std::vector<G4double> >& integralXS) {
  int theColumn = theXSType;
  if( theNXSsub[aMaterial] == 1) theColumn =1;

  G4double LowEdgeEnergy, fValue, crossSection=0, eVEnergy;

  //BUILD MEAN FREE PATH TABLE FROM INTEGRAL CROSS SECTION
  unsigned int matIdx = aMaterial->GetIndex();
  G4PhysicsLogVector* ptrVector = new G4PhysicsLogVector(theLowestEnergyLimit,
							 theHighestEnergyLimit, theNumbBinTable);
    
  for (G4int ii=0; ii < theNumbBinTable; ii++) {
    LowEdgeEnergy = ptrVector->GetLowEdgeEnergy(ii);
    if( verboseLevel >= 2 )
      cout << GetName() << " " << ii << " LowEdgeEnergy " << LowEdgeEnergy << " > "
	     << theLowestEnergyLimit << " < " << theHighestEnergyLimit << endl;
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
	cout << " eVEnergy " << eVEnergy << " LowEdgeE " << LowEdgeEnergy
	       << " " << integralXS[theColumn][1] << endl;
	
      if(eVEnergy < integralXS[0][1] ) {
	crossSection = 0.;
      }
      else {
	G4int Bin = 0; // locate bin                                                                           
	G4double aa, bb;
	for( G4int jj=1; jj<theNXSdat[aMaterial]; jj++) {  // Extrapolate for E > Emax !!!
	  if( verboseLevel >= 3 )
	    cout << " GET BIN " << jj << " "<< eVEnergy << " > " << integralXS[0][jj] << endl; 
	  if( eVEnergy > integralXS[0][jj]) {
	    Bin = jj;
	  } else {
	    break;
	  }
	}
	aa = integralXS[0][Bin];
	bb = integralXS[0][Bin+1];
	crossSection = (integralXS[theColumn][Bin]
			+(integralXS[theColumn][Bin+1]-integralXS[theColumn][Bin])
			/ (bb-aa)*(eVEnergy-aa) ) * 1.e-16*cm2;
	  
	if( verboseLevel >= 3 )
	  cout << GetName() << " crossSection " <<  crossSection << " "
	       << integralXS[theColumn][Bin]
	       << " + " << (integralXS[theColumn][Bin+1]-integralXS[theColumn][Bin])
	       << " / " << (bb-aa) << " *" << (eVEnergy-aa) << " * " << 1.e-16*cm2 << endl;
	  
	//	  SIGMA += NbOfAtomsPerVolume[elm] * crossSection;
	SIGMA = NbOfMoleculesPerVolume * crossSection;
	if( verboseLevel >= 2 )
	  cout << GetName() << " ADDING SIGMA " << SIGMA << " NAtoms " << NbOfMoleculesPerVolume
	       << " Bin " << Bin << " TOTAL " << aa << " " << bb << " XS "
	       << integralXS[theColumn][Bin]  << " " << integralXS[theColumn][Bin+1] << endl;
      }
      
      fValue = SIGMA > DBL_MIN ? 1./SIGMA : DBL_MAX;
    }
    
    ptrVector->PutValue(ii, fValue);
    if( verboseLevel >= 2 )
      cout << GetName() << " ptrvector(" << ii << ") = " << eVEnergy << "\t" << fValue
	     << "\t" << crossSection/1e-16/cm2 << endl;
  }
  
  theMeanFreePathTable->insertAt( matIdx , ptrVector ) ;
}


G4double LeptsBaseModel::SampleAngle(const G4Material* aMat, G4double ei, G4double el) {
  G4double Ang, Mo, KR, se;
  
  if(ei > 10001 && bDXSMT) {
    Mo = sqrt( pow( (ei/27.2/137),2) +2*ei/27.2);
    KR = theDiffXSMT[aMat]->SampleAngle(ei, el);
    se = KR/(2*Mo);
    if( se > 1. ) {
      cout << "seseseError" << se << endl;
      se = 1.;
    }
    
    Ang = std::asin(se)*2.0;
    //cout << "xxxsa11 " << KR << " " << Ang*180/pi << endl;
  }
  else {
    Ang = theDiffXS[aMat]->SampleAngle(ei, el) * pi/180;
    //cout << "xxxsa22 " << Ang*180/pi << endl;
  }

  return(Ang);
}


// G4double LeptsBaseModel::SampleAngle(const G4Material* aMat, G4double ei, G4double el) {

//   if( (ei < 10010) || !bDXSMT) return( theDiffXS[aMat]->SampleAngle(ei, el) );

//   G4double Ed = ei -el;                                  //dispers
//   G4double Pi = sqrt( pow( (ei/27.2/137),2) +2*ei/27.2); //incident
//   G4double Pd = sqrt( pow( (Ed/27.2/137),2) +2*Ed/27.2); //dispers
//   G4double Kmin = Pi - Pd;
//   G4double Kmax = Pi + Pd;
//   G4double KR = theRMTDistribution[aMat]->Sample(Kmin, Kmax); //sorteo mom. transf.
//   G4double co = (Pi*Pi + Pd*Pd - KR*KR) / (2*Pi*Pd);
//   if( co > 1. ) co = 1.;
//   return( std::acos(co) );
// }


// G4double LeptsBaseModel::SampleAngle(const G4Material* aMat, G4double ei) {

//   if( (ei < 10010) || !bDXSMT) return( theDiffXS[aMat]->SampleAngle(ei, 0.0) );

//   G4double Pi = sqrt( pow( (ei/27.2/137),2) +2*ei/27.2); //incident
//   G4double KR = theRMTDistribution[aMat]->Sample(0.0, 2*Pi);
//   G4double se = KR/(2*Pi);
//   if( se > 1. ) se = 1.;
//   return( std::asin(se)*2 );
// }



G4ThreeVector LeptsBaseModel::SampleNewDirection(G4ThreeVector P0Dir, G4double x) {
  G4double cosTeta = std::cos(x); //*twopi/360.0);
  G4double sinTeta = std::sqrt(1.0-cosTeta*cosTeta);
  G4double Phi     = twopi * G4UniformRand() ;
  G4double dirx    = sinTeta*std::cos(Phi) , diry = sinTeta*std::sin(Phi) , dirz = cosTeta ;

  G4ThreeVector P1Dir( dirx,diry,dirz );
  P1Dir.rotateUz(P0Dir);

  return(P1Dir);
}


// Inversa de ElasticEnergyTransfer
double LeptsBaseModel::ElasticAngle(double E, double W, double MT, double MP) {
  double co = (E * (E+2*MP) - W * (E+MP+MT)) /
    std::sqrt( E * (E+2*MP) * (E-W) * (E-W+2*MP) );
  return( acos(co));
}


G4double LeptsBaseModel::ElasticEnergyTransfer(G4double E, G4double ang,
					      G4double MT, G4double MP) {
  G4double co = std::cos(ang);
  G4double si = std::sin(ang);

  G4double W = ( (E+MP)*si*si + MT - co*std::sqrt(MT*MT-MP*MP*si*si) ) * E*(E+2*MP)
    / ( (E+MP+MT)*(E+MP+MT) - E*(E+2*MP)*co*co );

  //G4double W2 = 2*MP/MT*(1-co)*E;
  //cout << "WWWWWWWWW: " << W/E << " " << E/W << " " << W2/W << endl;
  //cout << "Mm " << MT/MeV << " " << MP/MeV << endl;
  //if( W > E*0.9 )
  //cout << "EnergyTransfer: " << E/eV << " " << W/eV << " " << ang << endl;
  return W;
}


// FractionElasticEnergyTransferNonRelativistic (W/E)
G4double LeptsBaseModel::FractionET(G4double ang, G4double MT, G4double MP) {
  G4double co = std::cos(ang);
  G4double si = std::sin(ang);

  G4double F = ( MP*si*si + MT - co*std::sqrt(MT*MT-MP*MP*si*si) ) *2*MP / (MP+MT)/(MP+MT);
  return F;
}

// Ya no sirve pq la receta es invertible
double LeptsBaseModel::AdjustAngle(double angE, double W, double E, double MT, double MP) {
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
bool LeptsBaseModel::GetValues(string FnMaster, string pName, string theMaterial,
			      string& FnDXS, string& FnDXSMT, string& FnDXSSEC,
			       string& FnIXS, string& FnELD,
			      double& IonisPot, double& IonisPotInt, double& IonisPotInt2) {

  std::ifstream fin(FnMaster.c_str() );
  if (!fin.is_open()) {
    cout << "\nNo File " << FnMaster << " " << theMaterial << endl;
    return false;
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
      else if(w1 == pName && w2 == "DXS"    ) FnDXS    = w3;
      else if(w1 == pName && w2 == "DXSMT"  ) FnDXSMT  = w3;
      else if(w1 == pName && w2 == "DXSROT" && theXSType == XSRotation ) FnDXS = w3;
      else if(w1 == pName && w2 == "DXSSEC" ) FnDXSSEC = w3;
      else if(w1 == pName && w2 == "IXS"    ) FnIXS  = w3;
      else if(w1 == pName && w2 == "ELD"    ) FnELD  = w3;
      else if(w1 == pName && w2 == "ELDVIB" && theXSType == XSVibration   ) FnELD = w3;
      else if(w1 == pName && w2 == "ELDROT" && theXSType == XSRotation    ) FnELD = w3;
      else if(w1 == pName && w2 == "ELDEXC" && theXSType == XSExcitation  ) FnELD = w3;
      else if(w1 == pName && w2 == "ELDDIS" && theXSType == XSDissociation) FnELD = w3;
      else if(w1 == pName && w2 == "ELDION" && theXSType == XSIonisation  ) FnELD = w3;
      else if(w1 == pName && w2 == "ELDIS"  && theXSType == XSIonisationS ) FnELD = w3;
      else if(w1 == pName && w2 == "ELDII"  && theXSType == XSIonisationI ) FnELD = w3;
      else if(w1 == pName && w2 == "ELDID"  && theXSType == XSIonisationD ) FnELD = w3;
      else if(w1 == "inherit") {
	string pMaterial = w2;
	cout << theMaterial << " derived from " << pMaterial << endl;
	GetValues(FnMaster, pName, pMaterial,
		  FnDXS, FnDXSMT, FnDXSSEC, FnIXS, FnELD,
		  IonisPot, IonisPotInt, IonisPotInt2);
      }
    }
  }

  if( MaterialFound == false) {
    cout << "---not found " << pName << " | " << theMaterial << " | " << XSName[theXSType] << endl;
    return false;
  }
  else {
    cout << "************ " << pName << " | " << theMaterial << " | " << XSName[theXSType] << endl;
    return true;
  }
}

//AMR 2015-05
G4bool LeptsBaseModel::
ReadMasterFile(G4String FnMaster,
	       G4String pName, const G4Material* aMaterial,
	       string& FnDXS, string& FnDXSMT, string& FnDXSSEC, string& FnIXS, string& FnELD) {

  string theMaterial = aMaterial->GetName();
  double IonisPot, IonisPotInt, IonisPotInt2;
  
  GetValues(FnMaster, pName, theMaterial,
   	    FnDXS, FnDXSMT, FnDXSSEC,
	    FnIXS, FnELD, IonisPot, IonisPotInt, IonisPotInt2);

  if( verboseLevel >= 1 )
    cout << "ReadMaster" << FnMaster << endl
	 << "IP:      " << IonisPot << " IIP: "
	 << IonisPotInt << " IIP2: "  << IonisPotInt2 << endl
	 << "FnIXS    " << FnIXS << endl
	 << "FnDXS    " << FnDXS << endl
	 << "FnDXSMT  " << FnDXSMT << endl
	 << "FnDXSSEC " << FnDXSSEC << endl
	 << "FnELD    " << FnELD << endl;

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
    //    cout << " MMASS1 " << mmass/g << " " << aMaterial->GetElement(ii)->GetName() << " " << aMaterial->GetElement(ii)->GetA()/g << endl;
  }
  //  cout << " MMASS " << MolecularMass << " " << MolecularMass*g <<  " ME " << mmass << " " << mmass/g << endl; 
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


void LeptsBaseModel::InitMassArrays(const G4String theParticle) {
 
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

    cout << "\t" << aMaterial->GetName() << " " << theMolecularMass[aMaterial]/(g/mole)
	 << " g/mole " << theMassTarget[aMaterial]/MeV << " MeV "
	 << theParticle << " " << theMassProjectile[aMaterial]/MeV << " MeV "
	 << aMaterial->GetDensity()/(g/cm3) << " g/cm3 "
      //<< aMaterial->GetTemperature()/kelvin << " kelvin "
      //<< aMaterial->GetPressure()/atmosphere << " atmosphere "
      //<< aMaterial->GetPressure()/(atmosphere/760000) << " mTorr "
	 << endl;
   }
}




// old version
std::map< G4int, std::vector<G4double> > LeptsBaseModel::
ReadIXS(G4String fnIXS, const G4Material* aMaterial) {
  std::map< G4int, std::vector<G4double> > integralXS; // process type - energy
  //cout << "fnIXS (" << fnIXS << ")" << endl;

  std::ifstream fin(fnIXS);
  if (!fin.is_open()) {
    //G4Exception("LeptsBaseModel::ReadIXS", "", JustWarning,
    //(G4String("File not found: ")+ fnIXS).c_str());
    return integralXS;
  }

  cout << "    Reading\t" << fnIXS << endl;
  
  G4int nXSdat, nXSp;
  fin >> nXSdat >> nXSp;
  if( verboseLevel >= 1 )
    cout << "Read IXS (" << fnIXS << ")\t nXSdat: " << nXSdat << " nXSp: "  << nXSp << endl;
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
	cout << GetName() << " IXS(" << ie << " " << ip << ")= " << integralXS[ip][ie]
	     << " " << xsdat
	     << " / " << nXSdat << " " << nXSp << endl; 
      // xsdat 1e-16*cm2
    }
  }
  fin.close();

  return integralXS;
}


// new version
std::map< G4int, std::vector<G4double> >
LeptsBaseModel::ReadIXS(G4String fnIXS, const G4Material* aMaterial, G4String xsType)
{
  std::map< G4int, std::vector<G4double> > integralXS; // process type - energy
  //cout << "fnIXS (" << fnIXS << ")" << endl;

  std::ifstream fin(fnIXS);
  if (!fin.is_open()) {
    //G4Exception("LeptsBaseModel::ReadIXS", "", JustWarning,
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
  else {
    replace(line.begin(), line.end(), ',', ' '); // csv file
    replace(line.begin(), line.end(), '#', ' '); // comment
  }
  
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
    if( vline[i] == xsType) theColumn = i+1;
    // size_t found = xsType.find(vline[i]);
    // if (found != std::string::npos) theColumn = i+1;
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
