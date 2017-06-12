
#include "visu.hh"

void Invalidate(GtkWidget * w) {
  cout << endl << "Invalidate (" << gtk_widget_get_name(w) << ")" << endl;
  gdk_window_invalidate_rect(w->window, &w->allocation, false);
}


GdkGLDrawable * GetDrawable(GtkWidget *glw) {
  GdkGLContext * glcontext = gtk_widget_get_gl_context(glw);
  GdkGLDrawable * gldrw = gtk_widget_get_gl_drawable(glw);

  if(!gdk_gl_drawable_gl_begin(gldrw, glcontext)) {
    cout << "Error GlDrawable" << endl;
  }
  return(gldrw);
}


void FlushDrawable(GdkGLDrawable * gldrw) {
  if(gdk_gl_drawable_is_double_buffered(gldrw))
    gdk_gl_drawable_swap_buffers(gldrw);
  else
    glFlush();

  gdk_gl_drawable_gl_end(gldrw);
}


gboolean configure(GtkWidget *glw, GdkEventConfigure *event, gpointer user_data) {
 
  printDbg(3, "configure\n", "RED");

  GdkGLDrawable * gldrw = GetDrawable(glw);

  //Reshape(glw->allocation.width, glw->allocation.height);

  FlushDrawable(gldrw);
  return true;
}



gboolean configureInfo(GtkWidget *glw, GdkEventConfigure *event, gpointer user_data) {
  
  printDbg(3, "configureInfo\n", "RED");
  
  GdkGLDrawable * gldrw = GetDrawable(glw);

  //ReshapeInfo(glw->allocation.width, glw->allocation.height);

  FlushDrawable(gldrw);
  return true;
}



gboolean expose(GtkWidget *glw, GdkEventExpose *event, gpointer user_data) {

  printDbg(3, "expose\n", "RED");
  
  GdkGLDrawable * gldrw = GetDrawable(glw);

  /* draw only last expose */
  if (event->count > 0) return true;

  Draw();

  FlushDrawable(gldrw);
  return true;
}


gboolean exposeInfo(GtkWidget *glw, GdkEventExpose *event, gpointer user_data) { 

  printDbg(3, "exposeInfo\n", "RED");

  GdkGLDrawable * gldrw = GetDrawable(glw);

  /* draw only last expose */
  if (event->count > 0) return true;

  //DrawInfo();
  
  FlushDrawable(gldrw);
  return true;
}
