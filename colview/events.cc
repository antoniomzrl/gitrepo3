
#include "colv.hh"

extern GtkWidget * glw;

bool Invalidated;

void InvalidateGlw() {
  //cout << endl << "Invalidate " << gtk_widget_get_name(w) << endl;
  string text = "Invalidate " + (string) gtk_widget_get_name(glw) + "\n";
  printDbg(1, text, "RED");

  gtk_widget_queue_draw(GTK_WIDGET(glw) );
  Invalidated = true;
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
  printDbg(1, "configure\n", "GREEN");

  // We need to make the context current if we want to
  // call GL API
  gtk_gl_area_make_current (area);

  // If there were errors during the initialization or
  // when trying to make the context current, this
  // function will return a #GError for you to catch
  if (gtk_gl_area_get_error (area) != NULL) {
    g_message(" *** Error configure ***");
    return true;
  }

  return true;
}


gboolean resize(GtkGLArea *area, gint w, gint h, gpointer user_data) {
  ostringstream oss;
  oss << "resize " << w << " x " << h;

  static int wha=0;
  if( abs(w+h-wha) > 40 ) {
    wha = w+h;
    oss << " big -> draw";
    Draw();
    Invalidated = false;
  }

  oss << endl;
  printDbg(1, oss.str(), "GREEN");

  return true;
}

  
gboolean expose(GtkGLArea *area, GdkEventExpose *event, gpointer user_data) {

  static int NoCalls = 0, NoDraws=0;
  NoCalls++;

  // GdkEventType ty = gdk_event_get_event_type( GTK_EVENT(event) );
  // if( ty == GDK_BUTTON_PRESS ) cout << "butpress" << endl;
  // if( ty == GDK_KEY_PRESS ) cout << "keypress" << endl;
  if( event->type & GDK_EXPOSE ) cout << "expose expose" << endl;
  if( event->type & GDK_DAMAGE ) cout << "expose damage" << endl;
  
  /* draw only last expose */
  //if( event->count > 1) return true;

  if( Invalidated == true) {
    Draw();
    NoDraws++;
    Invalidated = false;
  }
  else if( NoCalls == 1) {
    cout << "        expose -> Draw NoCalls =1" << endl;
    Draw();
    NoDraws++;
    Invalidated = false;
  }
  //glFlush();
  
  ostringstream oss;
  oss << "expose " << gtk_widget_get_name(GTK_WIDGET(area) )
      << " (" << NoDraws << "/" << NoCalls << ")" << endl;
  printDbg(1, oss.str(), "RED");
  
  return true;
}
