
#include "visu.hh"

void Invalidate(GtkWidget * w) {
  //cout << endl << "Invalidate " << gtk_widget_get_name(w) << endl;
  string text = "Invalidate " + (string) gtk_widget_get_name(w) + "\n";
  printDbg(3, text, "RED");

  gtk_widget_queue_draw(GTK_WIDGET(w) );
}


// GdkGLDrawable * GetDrawable(GtkWidget *glw) {
//   GdkGLContext * glcontext = gtk_widget_get_gl_context(glw);
//   GdkGLDrawable * gldrw = gtk_widget_get_gl_drawable(glw);

//   if(!gdk_gl_drawable_gl_begin(gldrw, glcontext)) {
//     cout << "Error GlDrawable" << endl;
//   }
//   return(gldrw);
// }


// void FlushDrawable(GdkGLDrawable * gldrw) {
//   if(gdk_gl_drawable_is_double_buffered(gldrw))
//     gdk_gl_drawable_swap_buffers(gldrw);
//   else
//     glFlush();

//   gdk_gl_drawable_gl_end(gldrw);
// }


gboolean configure(GtkGLArea *area, GdkEventConfigure *event, gpointer user_data) {
 
  printDbg(3, "configure\n", "RED");

  // We need to make the context current if we want to
  // call GL API
  gtk_gl_area_make_current (area);

  // If there were errors during the initialization or
  // when trying to make the context current, this
  // function will return a #GError for you to catch
  if (gtk_gl_area_get_error (area) != NULL)
    return true;

  return true;
}


gboolean expose(GtkGLArea *area, GdkEventExpose *event, gpointer user_data) {

  printDbg(3, "expose\n", "RED");
  
  gtk_gl_area_make_current(area);

  /* draw only last expose */
  //if (event->count > 0) return true;

  Draw();
  //glFlush();

  return true;
}
