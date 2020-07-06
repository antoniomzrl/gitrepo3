#include "colv.hh"

extern DataObject * d;

gboolean AutoRotate( gpointer ang) {
  //cout << "AutoRotate" << endl;

  if(d->vAutoRotate) {
    RotateScene( 0, 10, 0);
    InvalidateGlw();
  }
  return TRUE;
}

void TimerRotate() {
  gpointer p = 0;
  g_timeout_add(100, AutoRotate, p);
}
