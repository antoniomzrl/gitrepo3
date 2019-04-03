// casa

#include "uactions.hh"


UAVerbose::UAVerbose() : GmUserRunAction(), GmUserEventAction(),
			 GmUserSteppingAction() {
  cout << "UAVerbose Constructor" << endl;

  double T = 300*kelvin;
  double Et = k_Boltzmann * T;
  double ve = sqrt( 2*Et/(electron_mass_c2/c_light/c_light));
  double vn = sqrt( 2*Et/(neutron_mass_c2/c_light/c_light));
  cout << "Thermalization: " << endl
       << T/kelvin << " (kelvin) " << endl
       << Et/eV << " (eV)" << endl
       << "e-    " << ve/(m/s) << " (m/s)" << endl
       << "neutr " << vn/(m/s) << " (m/s)" << endl;
}


UAVerbose::~UAVerbose() {
  cout << "UAVerbose Destructor" << endl;
}


void UAVerbose::BeginOfRunAction( const G4Run* ) {
  cout << "UAVerbose BeginOfRun" << endl;
} 


void UAVerbose::EndOfRunAction( const G4Run* ) {
  cout << "UAVerbose EndOfRun" << endl;
} 


void UAVerbose::BeginOfEventAction( const G4Event* evt) {
  cout << "UAVerbose BeginOfEvent " << evt->GetEventID() << endl;
}


void UAVerbose::EndOfEventAction( const G4Event* evt) {
  cout << "UAVerbose EndOfEvent " << evt->GetEventID() << endl;
}


void UAVerbose::UserSteppingAction(const G4Step* aStep) {
  G4String partName = aStep->GetTrack()->GetDefinition()->GetParticleName();
  G4double Edep     = aStep->GetTotalEnergyDeposit();

  G4StepPoint * Pt1  = aStep->GetPreStepPoint();
  G4StepPoint * Pt2  = aStep->GetPostStepPoint();

  G4ThreeVector Pos  = Pt1->GetPosition();
  G4ThreeVector Pos2 = Pt2->GetPosition();

  G4double      KinE = Pt1->GetKineticEnergy();
  G4double     KinE2 = Pt2->GetKineticEnergy();

  G4ThreeVector Mom1  = Pt1->GetMomentum();
  G4ThreeVector Mom2  = Pt2->GetMomentum();
  //if(Mom2.mag() == 0) return;

  double ang = Mom1.angle(Mom2)*360/twopi;

  G4String procName  = "null";
  G4String procName2 = "null";
  G4String phvlName  = "null";
  G4String phvlName2 = "null";
  G4String matName   = "null";
  G4String matName2  = "null";

  const G4VProcess * pro  = Pt1->GetProcessDefinedStep();
  const G4VProcess * pro2 = Pt2->GetProcessDefinedStep();
  G4VPhysicalVolume * pv  = Pt1->GetPhysicalVolume();
  G4VPhysicalVolume * pv2 = Pt2->GetPhysicalVolume();
  G4Material        * mat = Pt1->GetMaterial();
  G4Material        * mat2= Pt2->GetMaterial();
  G4double dens = 0, dens2 = 0;

  if(pro )  procName  = pro->GetProcessName(); else cout << "err preproc\n";
  if(pro2)  procName2 = pro2->GetProcessName();else cout << "err postproc\n";
  if(pv  )  phvlName  = pv->GetName();  else cout << "err pre phvl\n";
  if(pv2 )  phvlName2 = pv2->GetName(); else cout << "err post phvl\n";

  if(mat2)  {
    matName2  = mat2->GetName();
    dens2     = mat2->GetDensity();
  }
  else cout << "err post mat\n";

  if(mat )  {
    matName = mat->GetName();
    dens    = mat->GetDensity();
    const G4double * napv = mat->GetVecNbOfAtomsPerVolume();
    cout << matName << " " << *napv/cm3 << " atoms/cm3" << endl;
  }
  else cout << "err pre mat\n";
  
  //procName = proc->GetProcessName();


  cout << "UAVerbose::UserSteppingAction" << endl
       << "  Part      " << partName << endl
       << "  PhysVol   " << phvlName << "\t" << phvlName2 << "\t" << endl
       << "  material  " << matName  << "\t" << matName2  << endl
       << "  Dens      " << dens/(g/cm3) << " " << "(g/cm3) " << dens/(mg/m3) << "(mg/m3)" << endl
       << "  Dens2     " << dens2/(g/cm3) << " " << "(g/cm3) " << dens2/(mg/m3) << "(mg/m3)" << endl
       << "  Proc      " << procName << "\t" << procName2 << endl
       << "  Pos       " << Pos      << " -> " << Pos2      << endl
       << "  Delta     " << Pos2-Pos << "\t" << G4BestUnit(aStep->GetStepLength(), "Length") << endl
       << "  Mom       " << Mom1     << endl
       << "  Ang(deg)  " << ang     << endl
       << "  KinE(eV)  " << KinE/eV << "\t" << KinE2/eV << endl
       << "  Elost(eV) " << (KinE-KinE2)/eV << endl
       << "  Edep(eV)  " << Edep/eV << endl << endl
       << "eellxx " << KinE/eV << " " << (KinE-KinE2)/eV << " "<< Pos[0] << endl
    ;

  // if( procName2.find("Ioni") != std::string::npos ) {
  //   if( fpSteppingManager->GetfN2ndariesAtRestDoIt() +
  // 	fpSteppingManager->GetfN2ndariesAlongStepDoIt() +
  // 	fpSteppingManager->GetfN2ndariesPostStepDoIt() == 0) {
  //     cout << "NoSeco " << endl;
  //   }
  // }
}
