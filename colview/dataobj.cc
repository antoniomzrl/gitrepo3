#include "visu.hh"

DataObject * d;


// constructor 
DataObject::DataObject() {
  cout << "constructor DataObject" << endl;
  NumDat = 0;
  MaxDat = 0;
  MinDat = 0;

 
  //PointSize = 1;
  BallSize = 0.3;
  BallFactor = 0.2;
  DiscretizePalette = 16;
  CanvasPoint[0] = 0.0;
  CanvasPoint[1] = 0.5;
  CanvasPoint[2] = 1.0;

  Background = 0;

  Light    = vec4(0.4, 1.0, 1.0, 0.8);
  LightPos = vec3(3, -3, 3);
  greenAttenuation = 0.2;
  
  vAxis = 0;
  vFrame = 1;
  vFrameLabels = 1;
  vColorBar = 0;
  vElastic = 1;
  vType = 1;
  
  Label[0] = "Particle type";
  Label[1] = "Interaction type";
  Label[2] = "Energy";
  Label[3] = "Energy Lost";
  Label[4] = "Energy Depo";
	  
  vAutoRotate = 0;
  vSelection = 0;
  vLorLines = 0;
  vBalls = 1; // 0=points, 1=balls
  vTracks = 0;
  vID = -2;
  vColorScale = 0;
  vFontList = 0;
  Stride = 1;
  SmartStride = 1;

  IcosahedronLevel = 0;
  FontSize = 10;
  ScaleX = 1;
  ScaleY = 1;
  ScaleZ = 1;
  SpatialUnits = "mm";
  SpatialFactor = 1;

  Wsize = 700;
  Fv = 1.2;

  verboseOut = 0;
  NeedUpdatePV = true;
  NeedRepaint = true;
  NeedUpdatePalette = true;
  FileName = string("callview");
};



DataObject::DataObject(int Nd) {
  DataObject();
  r.resize(Nd);
}

