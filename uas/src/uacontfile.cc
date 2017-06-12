
#include "uactions.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

G4String ContFile;
std::ofstream contFile;

UAContinuationFile::UAContinuationFile() : GmUserRunAction(), GmUserEventAction(),
					   GmUserSteppingAction() {
  cout << "UAContinuationFile Constructor" << endl;
}


UAContinuationFile::~UAContinuationFile() {
  cout << "UAContinuationFile Destructor" << endl;
}


void UAContinuationFile::BeginOfRunAction( const G4Run* ) {
  cout << "UAContinuationFile BeginOfRun" << endl;
  //ContFile = "pru.txt";
  ContFile = GmParameterMgr::GetInstance()->GetStringValue("ContinuationFileName", "contfil.txt");
  contFile.open(ContFile.c_str() );
} 


void UAContinuationFile::EndOfRunAction( const G4Run* ) {
  cout << "UAContinuationFile EndOfRun" << endl;
  contFile.close();
} 


void UAContinuationFile::BeginOfEventAction( const G4Event* evt) {
  cout << "UAContinuationFile BeginOfEvent " << evt->GetEventID() << endl;
}


void UAContinuationFile::EndOfEventAction( const G4Event* evt) {
  cout << "UAContinuationFile EndOfEvent " << evt->GetEventID() << endl;
}


void UAContinuationFile::UserSteppingAction(const G4Step* aStep) {

  G4String procName2 = "null";
  G4StepPoint * Pt2  = aStep->GetPostStepPoint();
  const G4VProcess * pro2 = Pt2->GetProcessDefinedStep();
  if(pro2)  procName2 = pro2->GetProcessName();

  static int n=0;
  if(procName2 == "UserSpecialCut") {
    G4String partName  = aStep->GetTrack()->GetDefinition()->GetParticleName();
    G4StepPoint * preP = aStep->GetPreStepPoint();
    //G4double KinE = preP->GetKineticEnergy();
    G4ThreeVector r = preP->GetPosition();
    G4ThreeVector m = preP->GetMomentum();
    n++;
    contFile << n << "," << partName << "," << r[0] << "," << r[1] << "," << r[2] << ","
	     << m[0] << "," << m[1] << "," << m[2] << "," << n << ",1" << endl;
  }

      // double dx = 1*cm;
      // double zu = -5*cm;
      // double zl = -5*cm - dx;
      // double yu = 0.5*dx;
      // double yl = -0.5*dx;
      // double xu1 = -49*cm +0.5*dx;
      // double xl1 = -49*cm -0.5*dx;
      // double xu2 = -20*cm +0.5*dx;
      // double xl2 = -20*cm -0.5*dx;
      // double xu3 = 10*cm +0.5*dx;
      // double xl3 = 10*cm -0.5*dx;
      // if(r[2] > zl && r[2] < zu && r[1] > yl && r[1] < yu ) {
      // 	//if( r[0] < xl1 && r[0] > xu1)
      // 	  contFile << n << "," << pn << "," << r[0] << "," << r[1] << "," << r[2] << ","
      // 		   << m[0] << "," << m[1] << "," << m[2] << "," << n << ",1" << endl;
      // }
}
