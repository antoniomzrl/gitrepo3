#include "visu.hh"

extern DataObject * d;
extern GtkWidget * glw;


void PlotBalls(DataObject * d) {
  
  double IcoLevel = d->IcosahedronLevel;
  if(d->vEconomic && (d->r).size() > 100000) {
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
  
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
  glClearColor(d->Background, d->Background, d->Background, 0.0);

  if( d->sw.w < 1.0) BestSpatialUnit(d);
  
  //glEnable(GL_LINE_SMOOTH);
  glLineWidth(d->LineWidth);
  
  PlotColorBarText();
  PlotBalls(d);
  PlotOrthoLines();
  PlotColorBarLines();

  cout << "Draw (" << NoCalls << ") (" << myclock() -T1 << ") sec Acum("
       << myclock() -T0 << " sec)" << endl;
}

