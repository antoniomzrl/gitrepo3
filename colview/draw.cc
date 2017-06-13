#include "visu.hh"

extern DataObject * d;
extern GtkWidget * glw;


void PlotBalls(DataObject * d) {
  
  double IcoLevel = d->IcosahedronLevel;
  if(d->vEconomic && d->NumDat > 100000) {
    IcoLevel = std::min(IcoLevel, 1.0);
  }
  
  static double IcoLevelAnt = -1;
  if(IcoLevel != IcoLevelAnt) {
    ComputeIcosahedronVertex(IcoLevel);
    IcoLevelAnt = IcoLevel;
  }
  
  PlotIcosahedrons();
}


void Draw(void) {

  double T1 = myclock();
  static double T0;
  static bool first = true;
  if(first) {
    T0 = T1;
    first = false;
  }
  
  static int NoCalls = 0;
  NoCalls++;
  
  glClearColor(d->Background, d->Background, d->Background, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  if( d->rMax < 1.0) BestSpatialUnit(d);

  PlotBalls(d);
  PlotColorBarText();
  PlotOrthoLines();
  PlotColorBarLines();

  cout << "Draw (" << NoCalls << ") (" << myclock() -T1 << ") sec Acum("
       << myclock() -T0 << " sec)" << endl;
}


void Reshape(int w, int h) {
  printDbg(3, "Reshape", "RED");

  //glViewport(0, 0, w, h);
  //glMatrixMode(GL_PROJECTION);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();                   // "limpiamos" con la matriz identidad.
  double R = d->Rv;
  glOrtho(-R,R, -R,R, -R,R);
  glMatrixMode(GL_MODELVIEW);         // Activamos matriz  modelado/visionado.
  glLoadIdentity();                   // "Limpiamos" la matriz
  Draw();
}
