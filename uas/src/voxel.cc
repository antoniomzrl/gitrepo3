#include "points.hh"
#include "uactions.hh"
#include "G4Gamma.hh"
#include "G4RunManager.hh"

UAVoxel::UAVoxel() : GmUserRunAction(), GmUserEventAction(), GmUserSteppingAction() {
  G4cout << "UAVoxel Constructor" << G4endl;
}

UAVoxel::~UAVoxel() {
  G4cout << "UAVoxel Destructor" << G4endl;
}


void UAVoxel::Init() {
  vector<double> v(3,1);
  Center = GmParameterMgr::GetInstance()->GetVNumericValue(theName+":Center", v);
  Radius = GmParameterMgr::GetInstance()->GetNumericValue(theName+":Radius", 0.0);
  ExternalRadius = GmParameterMgr::GetInstance()->GetNumericValue(theName+":ExternalRadius", 0.0);
  Volume = 4.0/3.0*pi*pow(Radius,3);  // mm3

  cout <<"center (" << Center[0] << " "  << Center[1] << " "<< Center[2] << ") "
       << "rads: " << Radius << " " << ExternalRadius << endl
       << "Dist: " << sqrt(pow(Center[0],2)+pow(Center[1],2)+pow(Center[2],2)) << endl
       << "Volume: " << Volume << " mm3" << endl;
}


void UAVoxel::BeginOfRunAction( const G4Run* ) {
  NoPrEv=0;
  NoInEv=0;

  theClock = new Clock();
  
  NoPhotInside = GmParameterMgr::GetInstance()->GetNumericValue(theName+":NoPhotInside", 10);
  Activity = GmParameterMgr::GetInstance()->GetNumericValue(theName+":SourceActivity", 0.777*mCi);
  
  cout << "UAVoxel BeginOfRunAction:" << endl;

  hgNiV = new TH1D("NintVoxel",  "Number of ints",      3, 0, 3);
  hgEiV = new TH1D("EdintVoxel", "Edep(eV) /int type",  3, 0, 3);
   
  Init();
} 


void UAVoxel::EndOfRunAction( const G4Run* ) {
  cout << "UAVoxel EndOfRunAction" << endl;

  hgNiV->LabelsOption("a", "X");
  hgEiV->LabelsOption("a", "X");

  G4String JobName = GmParameterMgr::GetInstance()->GetStringValue("JobName", "job");
  
  G4String fn = "voxel_" + JobName + ".root";
  TFile * Fintsp = new TFile(fn.c_str(), "RECREATE");
  hgNiV->Write();
  hgEiV->Write();
  Fintsp->Close();

  cout << right << setw(12) << "No Ints"
       << right << setw(16) << "Edep"
       << right << setw(16) << "Edep/Tot"
       << left  << "   Process" << endl;

  ostringstream pl, pn, pe, pem;
  for (int i=0; i <= hgNiV->GetNbinsX()+1; i++) {
    if(hgNiV->GetBinContent(i) != 0) {
      cout << right << setw(16) << hgNiV->GetBinContent(i)
	   << right << setw(16) << hgEiV->GetBinContent(i)/eV
	   << right << setw(16) << hgEiV->GetBinContent(i)/hgEiV->GetSum()
	   << "   " << left  << hgEiV->GetXaxis()->GetBinLabel(i) << endl;

      pl  << hgEiV->GetXaxis()->GetBinLabel(i)       << "\t";
      pn  << hgNiV->GetBinContent(i)                 << "\t";
      pe  << hgEiV->GetBinContent(i)/eV              << "\t";
      pem << hgEiV->GetBinContent(i)/hgEiV->GetSum() << "\t";
    }
  }

  cout << "vvxx1 " << pl.str()  << endl
       << "vvxx2 " << pn.str()  << endl
       << "vvxx3 " << pe.str()  << endl
       << "vvxx4 " << pem.str() << endl;
  
  double Depo = hgEiV->GetSum(); // Depo in voxel
  double Mass = Volume*1*g/cm3;
  double Dose = Depo/Mass;
  double Time = NoPrEv/Activity;

  cout << "act " << Activity/mCi << " mCi " << Activity/Bq << " Bq" << endl
       << "Time " << Time/second << endl;

  cout << left  << "Total:" << endl
       << right << setw(12) << hgEiV->GetEntries()
       << right << setw(16) << hgEiV->GetSum()/eV   << endl
       << "No Primaries:  " << NoPrEv                << endl
       << "No Secondarie: " << NoInEv                << endl
       << "No col inVoxel " << hgNiV->GetEntries()  << endl
       << "Voxel Volume:  " << Volume/mm3 << " mm3" << endl
       << "Voxel Mass:    " << Mass/g   << " g" << endl
       << "Edepo:         " << Depo/eV  << " eV " << endl
       << "Edepo/primary: " << Depo/eV/NoPrEv << " eV " << endl
       << "Dose:          " << Dose/(joule/kg) << " J/Kg" << endl
       << "Dose/primary:  " << Dose/(joule/kg)/NoPrEv << " J/Kg" << endl
       << "Dose/t:        " << Dose/(joule/kg)/(Time/second) << " J/Kg/sec" << endl
       << "act: " << Activity/mCi << " mCi " << Activity/Bq << " Bq" << endl
       << "Time: " << Time/second << " sec" << endl
       << endl;
} 


void UAVoxel::BeginOfEventAction( const G4Event* ) {
  //theEDepo1 = 0.;
}


void UAVoxel::EndOfEventAction( const G4Event* evt ) {
  static long  N = 1;
  bool MustPrint = false;
  long eid = evt->GetEventID() +1;
  NoPrEv++;

  if(eid%N == 0) {
    MustPrint = true;
    N *= 10;
  }
  
  if(MustPrint) {
    cout << NoPrEv << " / " << NoInEv << " Ed " << hgEiV->GetSum()/eV << " Ed/p "
	 << hgEiV->GetSum()/eV/NoPrEv << " " << theClock->FormattedElapsed() << endl;
    for (int i=0; i <= hgNiV->GetNbinsX()+1; i++)
      if(hgNiV->GetBinContent(i) != 0)
	cout << hgNiV->GetBinContent(i) << "\t" << hgNiV->GetXaxis()->GetBinLabel(i) << endl;
  }
}



bool UAVoxel::IsInsideVoxel(const G4Step* aStep) {
  G4ThreeVector r = aStep->GetPostStepPoint()->GetPosition();
  G4double d = sqrt( (r[0]-Center[0]) * (r[0]-Center[0]) +
		     (r[1]-Center[1]) * (r[1]-Center[1]) +
		     (r[2]-Center[2]) * (r[2]-Center[2]) );

  if( d > Radius) return false;
  else            return true;
}

bool UAVoxel::IsInsideBigVoxel(const G4Track* aTrack) {
  G4ThreeVector r = aTrack->GetPosition();
  
  G4double d = sqrt( (r[0]-Center[0]) * (r[0]-Center[0]) +
		     (r[1]-Center[1]) * (r[1]-Center[1]) +
		     (r[2]-Center[2]) * (r[2]-Center[2]) );

  if( d > ExternalRadius) return false;
  else                    return true;
}

void UAVoxel::UserSteppingAction(const G4Step* aStep ) {
  static G4ParticleDefinition * gammaDef = G4Gamma::GammaDefinition();
  
  if( !IsInsideVoxel(aStep) ) return;

  G4double Ed = aStep->GetTotalEnergyDeposit();
  G4String procName;
  const G4VProcess * pro = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  if(pro )  procName = pro->GetProcessName();
  else      procName = "null";

  //if( aStep->GetTrack()->GetDefinition() == gammaDef)
  if( procName == "phot")
    NoInEv++;
  
  hgEiV->Fill(procName, Ed);
  hgNiV->Fill(procName, 1);
}



G4ClassificationOfNewTrack UAVoxel::ClassifyNewTrack(const G4Track* aTrack ) {
  static G4ParticleDefinition * gammaDef = G4Gamma::GammaDefinition();
  //G4String partName = aTrack->GetDefinition()->GetParticleName();

  if( NoInEv >= NoPhotInside) {
    //cout << "RunManagerAbortRun" << endl;
    NoInEv--;
    G4RunManager::GetRunManager()->AbortRun();
  }

  if( aTrack->GetDefinition() == gammaDef) {
    return fUrgent;
  }
  
  if( !IsInsideBigVoxel(aTrack) )
    return fKill;
  else {
    return fUrgent;
  }
}
