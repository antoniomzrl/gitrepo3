
#include "uactions.hh"
#include "points.hh"


FILE * cfile;
int NoEvts = 0;
int WriteTransport, WriteUserCut;


UAWIF::UAWIF() : GmUserRunAction(), GmUserEventAction(),
			 GmUserSteppingAction() {
  cout << "UAWIF Constructor" << endl;
}



UAWIF::~UAWIF() {
  cout << "UAWIF Destructor" << endl;
}


void UAWIF::BeginOfRunAction( const G4Run* ) {
  cout << "UAWIF BeginOfRun" << endl;
  G4String JobName = GmParameterMgr::GetInstance()->GetStringValue("JobName", "job");
  WriteTransport = GmParameterMgr::GetInstance()->GetNumericValue("WriteTransport", 0);
  WriteUserCut  = GmParameterMgr::GetInstance()->GetNumericValue("WriteUserCut", 0);
  G4String fn = JobName + ".bin";
  G4String fn2 = JobName + "2.bin";

  cfile = fopen(fn.c_str(), "wb");

  if ( cfile==NULL )
    cout << "Error opening .bin file" << endl; 
}



void UAWIF::EndOfRunAction( const G4Run* ) {
  cout << "UAWIF EndOfRun" << endl;
  fclose(cfile);
}



void UAWIF::BeginOfEventAction( const G4Event* evt) {
  //cout << "UAWIF BeginOfEvent " << evt->GetEventID() << endl;
  NoEvts++;
}



void UAWIF::EndOfEventAction( const G4Event* evt) {
  //cout << "UAWIF EndOfEvent " << evt->GetEventID() << endl;
}



void UAWIF::UserSteppingAction(const G4Step* aStep) {
  G4String  partName = aStep->GetTrack()->GetDefinition()->GetParticleName();
  G4int      TrackID = aStep->GetTrack()->GetTrackID();
  G4ThreeVector Pos1 = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector Pos = aStep->GetPostStepPoint()->GetPosition();
  G4double     KinE = aStep->GetPostStepPoint()->GetKineticEnergy();
  G4double    KinE1 = aStep->GetPreStepPoint()->GetKineticEnergy();
  G4double     Edep = aStep->GetTotalEnergyDeposit();

  G4String procName1 = "null";
  G4String procName = "null";
  const G4VProcess * pro1 = aStep->GetPreStepPoint()->GetProcessDefinedStep();
  if(pro1)  procName = pro1->GetProcessName();
  const G4VProcess * pro = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  if(pro)  procName = pro->GetProcessName();

  if( procName.find("Transport") != std::string::npos && WriteTransport == 0) return;
  if( procName.find("SpecialCut") != std::string::npos && WriteUserCut == 0) return;

  //G4cout << "uawiffff " << procName << " " << WriteTransport<< " " << WriteUserCut << endl;
								
  G4String matName  = "null";
  G4Material * mat = aStep->GetPostStepPoint()->GetMaterial();
  G4double dens = 0;
  if(mat) {
    matName  = mat->GetName();
    dens = mat->GetDensity();
  }
 
  ColPoint p;
  p.x  = Pos[0]; //mm
  p.y  = Pos[1];
  p.z  = Pos[2];
  p.f1 = KinE/eV;
  p.f2 = (KinE1-KinE)/eV;
  p.f3 = Edep/eV;
  p.f4 = dens/(g/cm3);
  strncpy(p.tag, procName.c_str(), 64);

  fwrite(&p, sizeof(ColPoint), 1, cfile);
}

