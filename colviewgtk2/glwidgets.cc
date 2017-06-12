
#include "visu.hh"

extern DataObject * d;
int wWidth, wHeight;


void InitGLEW() {
  glewExperimental = GL_TRUE;
  
  if( glewInit() != GLEW_OK) {
    cout << "Failed to init GLEW: " << glewGetErrorString(glewInit()) << endl;
    exit(-1);
  }
}


GtkWidget * CreateTopWidget(const char * title) {
  
  //int sw = glutGet(GLUT_SCREEN_WIDTH);
  //int sh = glutGet(GLUT_SCREEN_HEIGHT);
  //GtkWidget * gla = GlWidgetArea();

  wHeight = gdk_screen_height();
  wWidth  = gdk_screen_width();
  double fac = (double)wWidth/wHeight;
  
  d->FontSize = wHeight * 13.0/1000.0;
    
  cout << "Screen size: " << wWidth << " x " << wHeight << endl;

  if( fac > 1.7 )     cout << " 16:9" << endl;
  else if( fac > 1.3) cout << " 4:3" << endl;
  else                cout << "  less than 4:3" << endl;
  wHeight *= 0.92;
  wWidth  = wHeight *4/3;

  cout << "Widget size: " << wWidth << " x " << wHeight << endl;

  GtkWidget * twg = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(twg), title);
  gtk_window_set_default_size(GTK_WINDOW(twg), wWidth, wHeight);
  gtk_window_move(GTK_WINDOW(twg), gdk_screen_width()-wWidth-3, 3);

  g_signal_connect_swapped(twg, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(twg, "key_press_event", G_CALLBACK(keyPress),  NULL);
  //gtk_window_set_gravity(GTK_WINDOW(twg), GDK_GRAVITY_NORTH_WEAST);

  return(twg);
}




GtkWidget * CreateGlWidget(GtkWidget * twg) {

  GtkWidget * gla = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(twg), gla);

  gtk_widget_set_events(gla,
			GDK_EXPOSURE_MASK |
                        GDK_BUTTON_PRESS_MASK |
			GDK_BUTTON_RELEASE_MASK |
			GDK_POINTER_MOTION_MASK |
			GDK_POINTER_MOTION_HINT_MASK |
			GDK_CONTROL_MASK |
			GDK_KEY_PRESS_MASK |
			GDK_SCROLL_MASK);

  /* prepare GL */
  GdkGLConfig * glconfig = gdk_gl_config_new_by_mode( (GdkGLConfigMode)
						      (GDK_GL_MODE_RGB |
						       GDK_GL_MODE_DEPTH |
						       GDK_GL_MODE_DOUBLE));
  if( !glconfig) {
    cout << "Error glconfig" << endl;
    //g_assert_not_reached();
  }

  if( !gtk_widget_set_gl_capability(gla, glconfig, NULL, true, GDK_GL_RGBA_TYPE)) {
    cout << "Error gtkglcapability" << endl;
    //g_assert_not_reached();
  }
  
  gtk_widget_set_name(gla, "plot");
  
  // window has been created - initialization routines
  //  g_signal_connect(gla, "realize", G_CALLBACK(realize), NULL);
  // window was destroyed - cleanup, de-allocating data
  //  g_signal_connect(gla, "destroy", G_CALLBACK(destroy), NULL);

  // window was resized - reshape function
  g_signal_connect(gla, "configure-event", G_CALLBACK(configure), NULL);
 
  //window was exposed - need redraw
  g_signal_connect(gla, "expose-event", G_CALLBACK(expose), NULL);

  //Button events
  g_signal_connect(gla,   "button_press_event",   G_CALLBACK(butPress),    NULL);
  g_signal_connect(gla,   "button_release_event", G_CALLBACK(butRelease),  NULL);
  g_signal_connect(gla,   "motion_notify_event",  G_CALLBACK(mouseMotion), NULL);
  g_signal_connect(gla,   "scroll-event",         G_CALLBACK(butScroll),   NULL);
  

  return(gla);
}
