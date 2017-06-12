#include "uactions.hh"
#include "G4RunManager.hh"


UAClock::UAClock() : GmUserRunAction(), GmUserEventAction() {
  cout << "UAClock Constructor" << endl;
}

UAClock::~UAClock() {
  cout << "UAClock Destructor" << endl;
}

void UAClock::BeginOfRunAction( const G4Run* ) {
  cout << "UAClock BeginOfRunAction" << endl;
  TimeMark = GmParameterMgr::GetInstance()->GetNumericValue(theName+":TimeMark",3600);
  TimeLimit = GmParameterMgr::GetInstance()->GetNumericValue(theName+":TimeLimit", 36000000);
  clk = new Clock();

  NEv = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
}

void UAClock::EndOfRunAction( const G4Run* ) {
  cout << "UAClock EndOfRunActnion" << endl;
} 


void UAClock::BeginOfEventAction( const G4Event* ) {
}



string fmtime(double ti) {
  ostringstream ss;

  if(    ti > 3600) {
    ss << (int)ti/3600 << "h";
    int min = ((int)ti%3600)/60;
    if(min >0) ss << min << "min";
  }
  else if( ti > 60) {
    ss  << (int)ti/60 << "min";
    int sec = (int)ti%60;
    if(sec >0) ss << sec << "sec";
  }
  else
    ss << ti << "sec";
  
  return( ss.str() );
}


  
void UAClock::EndOfEventAction( const G4Event* evt ) {
  static long   N = 1;
  static double TimeWrite = TimeMark;
  double eTime = clk->Elapsed();
  int eid = evt->GetEventID() +1;
  bool MustPrint = false;

  if(eid%N == 0) {
    MustPrint = true;
    N *= 10;
  }

  if(eTime > TimeWrite) {
    MustPrint = true;
    TimeWrite += TimeMark;
  }

  if(MustPrint) {
    double eta = (double)NEv * eTime / (double)eid;
    
    cout << "Clk Ev " << eid << "\t" << fmtime(eTime)
	 << "\t" << eid/eTime << " part/sec"
	 << "\t(ETA: " << fmtime(eta-eTime) << " / " << fmtime(eta) << ")"
	 << endl;
  }

  if( eTime >= TimeLimit) {
    cout << "Time limit reached, killing RunManager" << endl;
    G4RunManager::GetRunManager()->AbortRun();
  }
}
