
#include "Reflex/PluginService.h"


#include "GmDummyPhysics.hh"
PLUGINSVC_FACTORY(GmDummyPhysics,G4VUserPhysicsList*())
#include "GmDNAPhysics.hh"
PLUGINSVC_FACTORY(GmDNAPhysics,G4VUserPhysicsList*())
#include "GmLeptsPhysics.hh"
PLUGINSVC_FACTORY(GmLeptsPhysics,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(GmLeptsSMFPhysics,G4VUserPhysicsList*())
#include "StandardAndDNAPhysics1.hh"
PLUGINSVC_FACTORY(StandardAndDNAPhysics1,G4VUserPhysicsList*())
#include "StandardAndDNAPhysics.hh"
PLUGINSVC_FACTORY(StandardAndDNAPhysics,G4VUserPhysicsList*())
#include "EMSSPhysics.hh"
PLUGINSVC_FACTORY(EMSSPhysics,G4VUserPhysicsList*())



#include "GmG4PhysicsLists.hh"
PLUGINSVC_FACTORY(G4FTFP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_BERT_TRV,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_INCLXX,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTFP_INCLXX_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4FTF_BIC,G4VUserPhysicsList*())
#ifndef WIN32
PLUGINSVC_FACTORY(G4G4GenericPhysicsList,G4VUserPhysicsList*())
#endif
PLUGINSVC_FACTORY(G4LBE,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QBBC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BERT_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BIC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_BIC_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_FTFP_BERT,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_INCLXX,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGSP_INCLXX_HP,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4QGS_BIC,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(G4Shielding,G4VUserPhysicsList*())

PLUGINSVC_FACTORY(G4QGSP_BIC_AllHP,G4VUserPhysicsList*())

#include "GmQGSP_BIC_HP_EMopt3.hh"
PLUGINSVC_FACTORY(GmQGSP_BIC_HP_EMopt3,G4VUserPhysicsList*())
 

