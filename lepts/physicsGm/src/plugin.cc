
#include "GmDummyPhysics.hh"
#include "GmDNAPhysics.hh"
#include "GmLeptsPhysics.hh"
#include "StandardAndDNAPhysics1.hh"
#include "StandardAndDNAPhysics.hh"
#include "GmG4PhysicsLists.hh"
#include "GmQGSP_BIC_HP_EMopt3.hh"

//#include "EMSSPhysics.hh"
//PLUGINSVC_FACTORY(EMSSPhysics,G4VUserPhysicsList*())
//#include "Reflex/PluginService.h" root caput

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
#include "GmPhysicsFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmPhysicsVerbosity.hh"

DEFINE_SEAL_MODULE( );

DEFINE_GAMOS_PHYSICS( GmDummyPhysics );
DEFINE_GAMOS_PHYSICS( GmDNAPhysics );
DEFINE_GAMOS_PHYSICS( GmLeptsPhysics );
DEFINE_GAMOS_PHYSICS( GmLeptsSMFPhysics );
DEFINE_GAMOS_PHYSICS( StandardAndDNAPhysics1 );
DEFINE_GAMOS_PHYSICS( StandardAndDNAPhysics );
DEFINE_GAMOS_PHYSICS( G4FTFP_BERT );
DEFINE_GAMOS_PHYSICS( G4FTFP_BERT_HP );
DEFINE_GAMOS_PHYSICS( G4FTFP_BERT_TRV );
DEFINE_GAMOS_PHYSICS( G4FTFP_INCLXX );
DEFINE_GAMOS_PHYSICS( G4FTFP_INCLXX_HP );
DEFINE_GAMOS_PHYSICS( G4FTF_BIC );
DEFINE_GAMOS_PHYSICS( G4LBE );
DEFINE_GAMOS_PHYSICS( G4QBBC );
DEFINE_GAMOS_PHYSICS( G4QGSP_BERT );
DEFINE_GAMOS_PHYSICS( G4QGSP_BERT_HP );
DEFINE_GAMOS_PHYSICS( G4QGSP_BIC );
DEFINE_GAMOS_PHYSICS( G4QGSP_BIC_HP );
DEFINE_GAMOS_PHYSICS( G4QGSP_FTFP_BERT );
DEFINE_GAMOS_PHYSICS( G4QGSP_INCLXX );
DEFINE_GAMOS_PHYSICS( G4QGSP_INCLXX_HP );
DEFINE_GAMOS_PHYSICS( G4QGS_BIC );
DEFINE_GAMOS_PHYSICS( G4Shielding );
DEFINE_GAMOS_PHYSICS( G4QGSP_BIC_AllHP );
DEFINE_GAMOS_PHYSICS( GmQGSP_BIC_HP_EMopt3 );

DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmPhysicsVerbosity, "GmPhysicsVerbosity");
