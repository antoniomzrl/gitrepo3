#include "visu.hh"

extern DataObject * d;
extern GtkWidget * glw;
//extern GLfloat RefPoint[4];


gboolean keyPress(GtkWidget * glwNotUsed, GdkEventKey * event) {
  // GdkGLContext *  glcontext = gtk_widget_get_gl_context(glw);
  // GdkGLDrawable *  gldrawable = gtk_widget_get_gl_drawable(glw);

  // if(!gdk_gl_drawable_gl_begin(gldrawable, glcontext)) {
  //   g_assert_not_reached();
  // }

  cout << "keyPress " << event->keyval << endl;

  if(event->state & GDK_CONTROL_MASK) {
    if(     event->keyval == GDK_KEY_Up   ) d->LightPos.y -= 0.2;
    else if(event->keyval == GDK_KEY_Down ) d->LightPos.y += 0.2;
    else if(event->keyval == GDK_KEY_Right) d->LightPos.x -= 0.2;
    else if(event->keyval == GDK_KEY_Left ) d->LightPos.x += 0.2;
    else if(event->keyval == GDK_KEY_z    ) d->LightPos.z -= 0.2;
    else if(event->keyval == GDK_KEY_Z    ) d->LightPos.z += 0.2;
    cout << "LightPos: "; printgsl(d->LightPos);
  }
  else {
    if(     event->keyval == GDK_KEY_Up   ) RotateScene(-30,   0, 0);
    else if(event->keyval == GDK_KEY_Down ) RotateScene( 30,   0, 0);
    else if(event->keyval == GDK_KEY_Right) RotateScene(  0,  30, 0);
    else if(event->keyval == GDK_KEY_Left ) RotateScene(  0, -30, 0);

    else if(event->keyval == GDK_KEY_Return) CommitRotation();

  }

  Invalidate(glw);
  return true;
}
