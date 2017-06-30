#include "colv.hh"

DataObject * d;


// constructor 
DataObject::DataObject() {
  cout << "constructor DataObject" << endl;
 
  //PointSize = 1;
  BallSize = 0.3;
  BallFactor = 0.2;
  DiscretizePalette = 16;
  CanvasPoint.x = 0.0;
  CanvasPoint.y = 0.5;
  CanvasPoint.z = 1.0;

  Background = 0;

  Light    = vec4(0.4, 1.0, 1.0, 0.8);
  LightPos = vec3(-3, 3, 3);
  greenAttenuation = 0.25;
  
  ac = 1;
  vFrame = true;
  vAutoRotate = false;
  vSelection = false;
  
  Label[0] = "Particle type";
  Label[1] = "Interaction type";
  Label[2] = "Energy";
  Label[3] = "Energy Lost";
  Label[4] = "Energy Depo";
	  
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

