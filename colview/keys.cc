#include "colv.hh"

extern DataObject * d;
extern GtkWidget * glw;
//extern GLfloat RefPoint[4];


gboolean keyPress(GtkWidget * glwNotUsed, GdkEventKey * event) {
  cout << "keyPress " << event->keyval << endl;
  return true;
}


gboolean keyRelease(GtkWidget * glwNotUsed, GdkEventKey * event) {
  cout << "keyRelease " << event->keyval << endl;

  // GdkGLContext *  glcontext = gtk_widget_get_gl_context(glw);
  // GdkGLDrawable *  gldrawable = gtk_widget_get_gl_drawable(glw);

  // if(!gdk_gl_drawable_gl_begin(gldrawable, glcontext)) {
  //   g_assert_not_reached();
  // }

  bool needInvalidate = true;
  
  if(event->state & GDK_CONTROL_MASK) {
    if(     event->keyval == GDK_KEY_y ) d->LightPos.y -= 0.2;
    else if(event->keyval == GDK_KEY_Y ) d->LightPos.y += 0.2;
    else if(event->keyval == GDK_KEY_x ) d->LightPos.x -= 0.2;
    else if(event->keyval == GDK_KEY_X ) d->LightPos.x += 0.2;
    else if(event->keyval == GDK_KEY_z ) d->LightPos.z -= 0.2;
    else if(event->keyval == GDK_KEY_Z ) d->LightPos.z += 0.2;
    else if(event->keyval == GDK_KEY_Return) CommitRotation();
    else needInvalidate = false;
    cout << "LightPos: "; printglm(d->LightPos);
  }
  else {
    if(     event->keyval == GDK_KEY_Up   ) RotateScene(-30,   0, 0);
    else if(event->keyval == GDK_KEY_Down ) RotateScene( 30,   0, 0);
    else if(event->keyval == GDK_KEY_Right) RotateScene(  0,  30, 0);
    else if(event->keyval == GDK_KEY_Left ) RotateScene(  0, -30, 0);
    else if(event->keyval == GDK_KEY_r)     RotateScene(  0,  0, 30);
    else if(event->keyval == GDK_KEY_R)     RotateScene(  0,  0,-30);
    else if(event->keyval == GDK_KEY_Return) CommitRotationHalfPi();
    else needInvalidate = false;
  }

  if(needInvalidate) Invalidate(glw);
  return true;
}
