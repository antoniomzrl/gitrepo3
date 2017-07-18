#include "points.hh"
#include "uactions.hh"
#include "G4Gamma.hh"
#include "G4RunManager.hh"
#include "G4TransportationManager.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

UAScoreVolume::UAScoreVolume() : GmUserRunAction(), GmUserEventAction(), GmUserSteppingAction() {
  G4cout << "UAScoreVolume Constructor" << G4endl;
}

UAScoreVolume::~UAScoreVolume() {
  cout << "UAScoreVolume Destructor" << endl;
}



void UAScoreVolume::BeginOfRunAction( const G4Run* ) {
  cout << theName + " BeginOfRunAction:" << endl;

  theClock = new Clock();

  Activity = GmParameterMgr::GetInstance()->GetNumericValue(theName+":SourceActivity", 0.777*mCi);
  targetName = GmParameterMgr::GetInstance()->GetVStringValue(theName+":TargetName", targetName);
  
  if( targetName.size() == 0 ) {
    G4Exception("UAScoreVolume Constructor", " xxx", FatalException,
		("No touchable selected in " + theName).c_str());
  }

  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( "*" );
  for( size_t it = 0; it < touchs.size(); it++) {
    G4cout << "  Touchable: " << touchs[it]->GetName() << "\t" << touchs[it]->GetLongName();
    if( touchs[it]->GetName() == targetName[0] ) {
      G4VSolid *theTarget = (G4VSolid *)touchs[it]->GetSolid();
      targetVolume = theTarget->GetCubicVolume();
      G4cout << "\t<-- ScoreVol: " << targetVolume << " (mm3)"; 
    }
    G4cout << G4endl;
  }

  PrimCount=0;
  PhotCount=0;
  
  hgNp = new TH1D("Nproc", "Number of ints",      3, 0, 3);
  hgEd = new TH1D("Edep",  "Edep(eV) /int type",  3, 0, 3);
} 


void UAScoreVolume::EndOfRunAction( const G4Run* ) {
  cout << "UAScoreVolume EndOfRunAction" << endl;

  hgNp->LabelsOption("a", "X");
  hgEd->LabelsOption("a", "X");

  unsigned int NoIonis = 0;
  for (int i=0; i <= hgNp->GetNbinsX()+1; i++) {
    string theLabel = hgNp->GetXaxis()->GetBinLabel(i);
    if( theLabel.find("IonisationS") != std::string::npos)
      NoIonis += hgNp->GetBinContent(i);
    if( theLabel.find("IonisationD") != std::string::npos)
      NoIonis += hgNp->GetBinContent(i) *2;
    if( theLabel.find("IonisationK") != std::string::npos)
      NoIonis += hgNp->GetBinContent(i) *2;
  }
  
  double Depo = hgEd->GetSum(); // Depo in voxel
  double Mass = targetVolume*1*g/cm3;
  double Dose = Depo/Mass;
  double Time = PrimCount/Activity;
  double Charge = NoIonis * eplus;
  double ctv    = Charge/targetVolume/Time;
  double ctm    = Charge/Mass/Time;

  double hour = second*3600;
  double Gy   = joule/kg;
  double pC   = coulomb *1e-12;
  double ug   = g *1e-6;

  cout << "Activity       " << Activity/mCi << " mCi = " << Activity/Bq << " Bq" << endl
       << "No Primaries:  " << PrimCount            << endl
       << "Time           " << Time/second << " seg  = " << Time/millisecond << " ms" << endl
       << "Voxel No Coll: " << hgNp->GetEntries()  << endl
       << "No Photoel:    " << PhotCount            << endl
       << "Volume:        " << targetVolume/mm3 << " mm3" << endl
       << "Mass:          " << Mass/g   << " g" << endl
       << "Navogadro:     " << Avogadro << endl
       << "P1molec:       " << 18.0/Avogadro << " g" << endl
       << "NoMolec:       " << (Mass/g)/(18.0/Avogadro) << endl
       << "Edep:          " << Depo/eV  << " eV " << endl
       << "Edep/primary:  " << Depo/eV/PrimCount << " eV " << endl
       << "Dose:          " << Dose/Gy << " Gy" << endl
       << "Dose/primary:  " << Dose/Gy/PrimCount << " Gy" << endl
       << "               " << Dose/Gy/PrimCount*100.0/3600.0/1e6 << " cGy/(h*MBq)" << endl
       << "Dose/t:        " << Dose/Time/(Gy/second) << " Gy/sec" << endl
       << "               " << Dose/Time/(Gy/hour)*100 << " cGy/hour" << endl
       << "No Ionis:      " << NoIonis << endl
       << "Charge         " << Charge/coulomb << " C " << Charge/pC << " pC" << endl
       << "Charge/vol/t   " << ctv/(pC/mm3/second) << " pC/mm3/s" << endl
       << "               " << ctv/(pC/mm3/hour) << " pC/mm3/hour" << endl
       << "Charge/M/t     " << ctm/(pC/ug/hour) << " pC/ug/hour" << endl
       << "W(eV/pair)     " << Depo/NoIonis/eV << endl
       << endl;

  cout << " MultiFunctionalDet: theScoreVolume" << endl
       << " PrimitiveScorer: theScore" << endl
       << " Number of entries= 4"      << endl
       << "  index: NoPrimaries  = "   << PrimCount << " +-(REL) 0 sumV2= 0" << endl
       << "  index: Dose_cGyHour  = "  << Dose/Time/(Gy/hour)*100   << " +-(REL) 0 sumV2= 0" << endl
       << "  index: Charge_pCmm3s  = " << ctv/(pC/mm3/second) << " +-(REL) 0 sumV2= 0" << endl
       << "  index: Charge_pCugHour  = " << ctm/(pC/ug/hour) << " +-(REL) 0 sumV2= 0" << endl
       << " theScore SUM ALL: "          << PrimCount << " +-(REL) 0" << endl;

  hgNp->Fill( (G4String)"#1_NoPrimaries",     PrimCount);
  hgNp->Fill( (G4String)"#2_Edep(MeV)",       Depo);
  hgNp->Fill( (G4String)"#3_Volume(mm3)",     targetVolume/mm3);
  hgNp->Fill( (G4String)"#4_Mass(g)",         Mass/g);
  hgNp->Fill( (G4String)"#5_Dose(Gy)",        Dose/Gy);
  hgNp->Fill( (G4String)"#6_Dose(cGy/h*MBq)", Dose/Gy*100.0/3600.0/1e6);
  hgNp->Fill( (G4String)"#7_Charge(Coulomb)", Charge/coulomb);
  hgNp->Fill( (G4String)"#8_NoIonisations",   NoIonis);

  cout << "UAScoreVolume summary:" << endl;
  for (int i=0; i <= hgNp->GetNbinsX()+1; i++) {
    if(hgNp->GetBinContent(i) != 0) {
      cout << right << setw(16) << hgNp->GetBinContent(i)
	   << "\t" << left << hgNp->GetXaxis()->GetBinLabel(i)
	   << endl;
    }
  }
  
  G4String JobName = GmParameterMgr::GetInstance()->GetStringValue("JobName", "job");
  G4String fn = theName + "_" + JobName + ".root";
  TFile * Fintsp = new TFile(fn.c_str(), "RECREATE");
  hgNp->Write();
  hgEd->Write();
  Fintsp->Close();
}


void UAScoreVolume::BeginOfEventAction( const G4Event* ) {
  //theEDepo1 = 0.;
}


void UAScoreVolume::EndOfEventAction( const G4Event* evt ) {
  PrimCount++;
}


void UAScoreVolume::UserSteppingAction(const G4Step* aStep ) {
  static G4ParticleDefinition * gammaDef = G4Gamma::GammaDefinition();
  
  if( !IsInside(aStep) ) return;

  G4double Ed = aStep->GetTotalEnergyDeposit();
  G4String procName;
  const G4VProcess * pro = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  if(pro )  procName = pro->GetProcessName();
  else      procName = "null";

  hgNp->Fill(procName, 1);
  hgEd->Fill(procName, Ed);
  
  //if( aStep->GetTrack()->GetDefinition() == gammaDef) {
  if( procName == "phot")
    PhotCount++;
}




bool UAScoreVolume::IsInside(const G4Step* aStep) {
  G4VPhysicalVolume * pv = aStep->GetPostStepPoint()->GetPhysicalVolume();
  G4String pvName;
  if( pv ) pvName = pv->GetName();
  else     pvName = "null";

  if( pvName == targetName[0] )  return true;
  else                           return false;
}
