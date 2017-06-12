//gamos5
#include "Reflex/PluginService.h"

//#include "phlists.hh"
#include "uactions.hh"

//PLUGINSVC_FACTORY(LeptsPhysicsList,    G4VUserPhysicsList*())
//PLUGINSVC_FACTORY(SallyDNAPhysicsList, G4VUserPhysicsList*())

//xxx #include "GmDNAPhysicsList.hh"
//xxx PLUGINSVC_FACTORY(GmDNAPhysicsList,    G4VUserPhysicsList*())


//DEFINE_GAMOS_USER_ACTION(Run);
//DEFINE_GAMOS_USER_ACTION(Event);
//DEFINE_GAMOS_GENERATOR(Generator);
//DEFINE_GAMOS_GEOMETRY(Geometry);
//DEFINE_GAMOS_SENSDET(Detector);

PLUGINSVC_FACTORY(UserActionPrueba,  GmUserAction*());
PLUGINSVC_FACTORY(UAVerbose,         GmUserAction*());
PLUGINSVC_FACTORY(UADepo,            GmUserAction*());
PLUGINSVC_FACTORY(UADepoSpheric,     GmUserAction*());

PLUGINSVC_FACTORY(UAInteraction,     GmUserAction*());
PLUGINSVC_FACTORY(UAInteractionSp,   GmUserAction*());
PLUGINSVC_FACTORY(UAEloss,           GmUserAction*());
PLUGINSVC_FACTORY(UAWIF,             GmUserAction*());
PLUGINSVC_FACTORY(UAContinuationFile,GmUserAction*());
PLUGINSVC_FACTORY(UAExit,            GmUserAction*());
PLUGINSVC_FACTORY(UAReflex,          GmUserAction*());

PLUGINSVC_FACTORY(UAClock,           GmUserAction*());
PLUGINSVC_FACTORY(UAVoxel,           GmUserAction*());
PLUGINSVC_FACTORY(UAVolume,          GmUserAction*());
PLUGINSVC_FACTORY(UAScoreVolume,     GmUserAction*());
PLUGINSVC_FACTORY(UASelectVolume,    GmUserAction*());
PLUGINSVC_FACTORY(UATargetVolume,    GmUserAction*());
