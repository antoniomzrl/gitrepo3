
#include "visu.hh"


extern DataObject * d;
extern float verboseFlag;

void DrawInfo(void) {

  printDbg(3, "DrawInfo\n", "RED");
  if(d->NumDat <1) return;

  if( d->rMax < 1.0) BestSpatialUnit(d);

  if( d->NeedUpdatePalette) {
    UpdatePalette();
    d->NeedUpdatePalette = false;
  }
    
  glClearColor(d->Background, d->Background, d->Background, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //glPushMatrix();

  //PlotPru(d);
  PlotLegend(d);
  PlotColorBar(d);

  //glPopMatrix();
}


void ReshapeInfo(int w, int h) {
  printDbg(3, "ReshapeInfo\n", "RED");
  glViewport(0, 0, w*2, h);
  //glMatrixMode(GL_PROJECTION);
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();                   // "limpiamos" con la matriz identidad.
  //double R = d->Rv;
  //glOrtho(-R,R, -R,R, -R,R);
  //glMatrixMode(GL_MODELVIEW);         // Activamos matriz  modelado/visionado.
  //glLoadIdentity();                   // "Limpiamos" la matriz
  DrawInfo();
}
