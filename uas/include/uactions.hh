#ifndef UserActions_HH
#define UserActions_HH

#include "G4Run.hh"
#include "G4Event.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"

#include <TH3D.h>
#include "G4UnitsTable.hh"
#include <time.h>

#include <iostream>
#include <string>
using namespace std;
using namespace CLHEP; //xxx



class Clock {
  clock_t ti, tf;
  
public:
  Clock() {
    ti = clock();
  }
  
  double Elapsed() {
    tf = clock();
    return (tf-ti)/(double)CLOCKS_PER_SEC;
  }

  string FormattedElapsed() {
    ostringstream ft;
    double t = Elapsed();
    ft << t << " sec";
    if( t > 3600)
      ft << " = " << (int)t/3600 << " h " << ((int)t%3600)/60 << " min";
    else if( t > 60)
      ft << " = " << (int)t/60 << " min " << (int)t%60 << " sec";

    return ft.str();
  }
};



class UserActionPrueba : public GmUserRunAction,
			 public GmUserEventAction,
			 public GmUserSteppingAction {
public:
  UserActionPrueba();
  ~UserActionPrueba();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
};



class UAVerbose : public GmUserRunAction,
		  public GmUserEventAction,
		  public GmUserSteppingAction {
public:
  UAVerbose();
  ~UAVerbose();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
};



class UADepo : public GmUserRunAction,
	       public GmUserEventAction,
	       public GmUserSteppingAction {
public:
  UADepo();
  ~UADepo();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
};



class UAInteraction : public GmUserRunAction,
		      public GmUserEventAction,
		      public GmUserSteppingAction {
public:
  UAInteraction();
  ~UAInteraction();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
};




class UAInteractionSp : public GmUserRunAction,
			public GmUserEventAction,
			public GmUserSteppingAction {
public:
  UAInteractionSp();
  ~UAInteractionSp();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
};


class UADepoSpheric : public GmUserRunAction,
		      public GmUserEventAction,
		      public GmUserSteppingAction {
public:
  UADepoSpheric();
  ~UADepoSpheric();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
};



class UAEloss : public GmUserRunAction,
			public GmUserEventAction,
			public GmUserSteppingAction {
public:
  UAEloss();
  ~UAEloss();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
};



class UAWIF : public GmUserRunAction,
	      public GmUserEventAction,
	      public GmUserSteppingAction {
public:
  UAWIF();
  ~UAWIF();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
};


class UAContinuationFile : public GmUserRunAction,
			   public GmUserEventAction,
			   public GmUserSteppingAction {
public:
  UAContinuationFile();
  ~UAContinuationFile();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
};



class UAExit : public GmUserRunAction,
	       public GmUserEventAction,
	       public GmUserSteppingAction {
public:
  UAExit();
  ~UAExit();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
};


class UAReflex : public GmUserRunAction,
		 public GmUserEventAction,
		 public GmUserSteppingAction {
public:
  UAReflex();
  ~UAReflex();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
};


class UAClock :  public GmUserRunAction, public GmUserEventAction {
public:
  UAClock();
  ~UAClock();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);

  Clock * clk;
  int TimeMark, TimeLimit, NEv;

};



class UAVoxel : public GmUserStackingAction, public GmUserRunAction,
		public GmUserEventAction, public GmUserSteppingAction
{
public:
  UAVoxel();
  ~UAVoxel();
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *);
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);

  virtual bool IsInsideVoxel(const G4Step* );
  virtual bool IsInsideBigVoxel(const G4Track* );
  virtual void Init();
  
  unsigned long NoPrEv, NoInEv, NoPhotInside;
  Clock * theClock;
  TH1D *hgNiV, *hgEiV;
  vector<double> Center;
  double Volume, Radius, ExternalRadius, Activity;
};



class UAVolume : public UAVoxel
{
  bool IsInsideVoxel(const G4Step* );
  bool IsInsideBigVoxel(const G4Track* );
  void Init();
  
public:
  //G4String targetName;
  std::vector<G4String> targetName;
  G4double              targetReach;
  G4VSolid *            theTarget;
  G4ThreeVector         theTargetPosition;
  G4Navigator * theNavigator;
};





class UAScoreVolume : public GmUserRunAction,
		      public GmUserEventAction,
		      public GmUserSteppingAction
{
public:
  UAScoreVolume();
  ~UAScoreVolume();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(  const G4Event *);
  void UserSteppingAction(const G4Step *);
  bool IsInside(const G4Step *);
  void WriteHistograms();
    
  unsigned PrimCount, PhotCount;
  TH1D *hgNp, *hgEd;
  double targetVolume, Activity;
  std::vector<G4String> targetName;
  Clock *theClock;
  int TimeMark;
};



class UASelectVolume: public GmUserStackingAction,
		      public GmUserRunAction
{
public:
  UASelectVolume();
  ~UASelectVolume();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *);
  virtual bool IsNearVolume(const G4Track*, double);
  
  unsigned NoPhotInside, PhotCount, TimeLimit;

  std::vector<G4String> targetName;
  G4VSolid            * theTarget;
  G4ThreeVector         theTargetPosition;
  G4double              targetReach;
  G4Navigator         * theNavigator;
};


class UATargetVolume: public GmUserStackingAction,
		      public GmUserRunAction
{
public:
  UATargetVolume();
  ~UATargetVolume();
  void BeginOfRunAction(  const G4Run *);
  void EndOfRunAction(    const G4Run *);
  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *);
  
  unsigned NoPhotInside, PhotCount;

  std::vector<G4String> targetName;
  G4VSolid            * theTarget;
  G4ThreeVector         theTargetPosition;
  G4double              targetReach;
  G4Navigator         * theNavigator;
};

#endif  // UserActions_HH
