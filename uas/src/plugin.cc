//gamos5
//#include "Reflex/PluginService.h" root caput

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


#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_USER_ACTION( UserActionPrueba  );
DEFINE_GAMOS_USER_ACTION( UAVerbose         );
DEFINE_GAMOS_USER_ACTION( UADepo            );
DEFINE_GAMOS_USER_ACTION( UADepoSpheric     );
DEFINE_GAMOS_USER_ACTION( UAInteraction     );
DEFINE_GAMOS_USER_ACTION( UAInteractionSp   );
DEFINE_GAMOS_USER_ACTION( UAEloss           );
DEFINE_GAMOS_USER_ACTION( UAWIF             );
DEFINE_GAMOS_USER_ACTION( UAContinuationFile);
DEFINE_GAMOS_USER_ACTION( UAExit            );
DEFINE_GAMOS_USER_ACTION( UAAnalyser        );
DEFINE_GAMOS_USER_ACTION( UAReflex          );
DEFINE_GAMOS_USER_ACTION( UAClock           );
DEFINE_GAMOS_USER_ACTION( UAVoxel           );
DEFINE_GAMOS_USER_ACTION( UAVolume          );
DEFINE_GAMOS_USER_ACTION( UAScoreVolume     );
DEFINE_GAMOS_USER_ACTION( UASelectVolume    );
DEFINE_GAMOS_USER_ACTION( UATargetVolume    );
