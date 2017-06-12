
#ifndef TGmQGSP_BIC_HP_EMopt3_h
#define TGmQGSP_BIC_HP_EMopt3_h 1

#include <CLHEP/Units/SystemOfUnits.h>

#include "globals.hh"
#include "G4VModularPhysicsList.hh"
#include "CompileTimeConstraints.hh"

template<class T>
class TGmQGSP_BIC_HP_EMopt3: public T {
  
public:
  TGmQGSP_BIC_HP_EMopt3(G4int ver = 1);
  virtual ~TGmQGSP_BIC_HP_EMopt3();
  
public:
  // SetCuts() 
  virtual void SetCuts();

private:
  enum {ok = CompileTimeConstraints::IsA<T, G4VModularPhysicsList>::ok };
};

#include "GmQGSP_BIC_HP_EMopt3.icc"
typedef TGmQGSP_BIC_HP_EMopt3<G4VModularPhysicsList> GmQGSP_BIC_HP_EMopt3;

#endif



