#include "visu.hh"

extern DataObject * d;
extern GtkWidget * glw;

gboolean AutoRotate( gpointer ang) {
  //cout << "AutoRotate" << endl;

  if(d->vAutoRotate) {
    RotateScene( 0, 10, 0);
    Invalidate(glw);
  }
  return TRUE;
}

void TimerRotate() {
  gpointer p = 0;
  g_timeout_add(100, AutoRotate, p);
}
