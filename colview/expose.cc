
#include "colv.hh"

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

  if(event->type ==GDK_NOTHING          ) cout << "NOTHING          " << endl;
  if(event->type ==GDK_DELETE           ) cout << "DELETE           " << endl;
  if(event->type ==GDK_DESTROY          ) cout << "DESTROY          " << endl;
  if(event->type ==GDK_EXPOSE           ) cout << "EXPOSE           " << endl;
  if(event->type ==GDK_MOTION_NOTIFY    ) cout << "MOTION_NOTIFY    " << endl;
  if(event->type ==GDK_BUTTON_PRESS     ) cout << "BUTTON_PRESS     " << endl;
  if(event->type ==GDK_2BUTTON_PRESS    ) cout << "2BUTTON_PRESS    " << endl;
  if(event->type ==GDK_3BUTTON_PRESS    ) cout << "3BUTTON_PRESS    " << endl;
  if(event->type ==GDK_BUTTON_RELEASE   ) cout << "BUTTON_RELEASE   " << endl;
  if(event->type ==GDK_KEY_PRESS        ) cout << "KEY_PRESS        " << endl;
  if(event->type ==GDK_KEY_RELEASE      ) cout << "KEY_RELEASE      " << endl;
  if(event->type ==GDK_ENTER_NOTIFY     ) cout << "ENTER_NOTIFY     " << endl;
  if(event->type ==GDK_LEAVE_NOTIFY     ) cout << "LEAVE_NOTIFY     " << endl;
  if(event->type ==GDK_FOCUS_CHANGE     ) cout << "FOCUS_CHANGE     " << endl;
  if(event->type ==GDK_CONFIGURE        ) cout << "CONFIGURE        " << endl;
  if(event->type ==GDK_MAP              ) cout << "MAP              " << endl;
  if(event->type ==GDK_UNMAP            ) cout << "UNMAP            " << endl;
  if(event->type ==GDK_PROPERTY_NOTIFY  ) cout << "PROPERTY_NOTIFY  " << endl;
  if(event->type ==GDK_SELECTION_CLEAR  ) cout << "SELECTION_CLEAR  " << endl;
  if(event->type ==GDK_SELECTION_REQUEST) cout << "SELECTION_REQUEST" << endl;
  if(event->type ==GDK_SELECTION_NOTIFY ) cout << "SELECTION_NOTIFY " << endl;
  if(event->type ==GDK_PROXIMITY_IN     ) cout << "PROXIMITY_IN     " << endl;
  if(event->type ==GDK_PROXIMITY_OUT    ) cout << "PROXIMITY_OUT    " << endl;
  if(event->type ==GDK_CLIENT_EVENT     ) cout << "CLIENT_EVENT     " << endl;
  if(event->type ==GDK_VISIBILITY_NOTIFY) cout << "VISIBILITY_NOTIFY" << endl;

  
  gtk_gl_area_make_current(area);

  /* draw only last expose */
  //if( event->count > 1) return true;

  Draw();
  //glFlush();

  return true;
}
