#include "colv.hh"


// constructor 
DataObject::DataObject() {
  cout << "constructor DataObject" << endl;
 
  //PointSize = 1;
  BallSize = 0.3;
  BallFactor = 0.2;
  DiscretizePalette = 16;
  CanvasPoint = vec3(0.0, 0.5, 1.0);

  Background = 0;
  LineWidth  = 1;
  Light    = vec4(0.4, 1.0, 1.0, 0.8);
  LightPos = vec3(-3, 3, 3);
  greenAttenuation = 0.25;
  
  ac = 1;
  vFrame = true;
  vAutoRotate = false;
  vSelection = false;
 
  Stride = 1;
  SmartStride = 1;

  IcosahedronLevel = 0;
  FontSize = 10;
  Scale = vec3(1,1,1);
  SpatialUnits = "mm";
  SpatialFactor = 1;

  FoV = 1.2;
  AR = 4.0/3.0;
  
  NeedUpdatePV = true;
  NeedUpdatePalette = true;
  FileName = string("callview");
};



DataObject::DataObject(int Nd) {
  DataObject();
  r.resize(Nd);
}

