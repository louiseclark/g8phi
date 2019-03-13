/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef VECTORSDME
#define VECTORSDME

#include "RooAbsPdf.h"
#include "RooHSEventsPDF.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class VectorSDME : public RooHSEventsPDF {
public:
  VectorSDME() {} ; 
  VectorSDME(const char *name, const char *title,
	      RooAbsReal& _cosThetaKpHel,
	      RooAbsReal& _phiKpHel,
	      RooAbsReal& _PHI,
	      RooAbsReal& _pol,
	      RooAbsCategory& _polState,
	      RooAbsReal& _rho000,
	      RooAbsReal& _Rerho010,
	      RooAbsReal& _rho01m1,
	      RooAbsReal& _rho111,
	      RooAbsReal& _rho100,
	      RooAbsReal& _Rerho110,
	      RooAbsReal& _rho11m1,
	      RooAbsReal& _Imrho210,
	      RooAbsReal& _Imrho21m1);
  VectorSDME(const VectorSDME& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new VectorSDME(*this,newname); }
  inline virtual ~VectorSDME() { }

protected:

  RooRealProxy cosThetaKpHel ;
  RooRealProxy phiKpHel ;
  RooRealProxy PHI ;
  RooRealProxy pol ;
  RooCategoryProxy polState ;
  RooRealProxy rho000 ;
  RooRealProxy Rerho010 ;
  RooRealProxy rho01m1 ;
  RooRealProxy rho111 ;
  RooRealProxy rho100 ;
  RooRealProxy Rerho110 ;
  RooRealProxy rho11m1 ;
  RooRealProxy Imrho210 ;
  RooRealProxy Imrho21m1 ;
  
  Double_t evaluate() const ;
  Double_t evaluateMC(const vector<Float_t> *vars,const  vector<Int_t> *cats) const ;
  void MakeSets();

private:

  ClassDef(VectorSDME,1) // Your description goes here...
};
 
#endif