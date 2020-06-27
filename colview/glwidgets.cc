
#include "colv.hh"

extern DataObject * d;


vec4 WindowDimensions(GdkRectangle * screenSz) {
  vec4 wsz = vec4(0,0,0,0);
  wsz.y = screenSz->height * 0.85;
  wsz.x = screenSz->height * 0.85 *4/3;
  
  cout << "Widget size: " << wsz.x << " x " << wsz.y << endl;

  return wsz;
}



GdkRectangle * ScreenDimensions() {
  //int sw = glutGet(GLUT_SCREEN_WIDTH);
  //int sh = glutGet(GLUT_SCREEN_HEIGHT);
  //GtkWidget * gla = GlWidgetArea();

  //wWidth  = gdk_screen_width();
  //wHeight = gdk_screen_height();
  GtkWidget * tw   = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GdkDisplay * dpy = gtk_widget_get_display(tw);
  //GdkDisplay * dpy = gdk_display_get_default();
  
  //GdkWindow  * win = gtk_widget_get_window(widget);
  //GdkMonitor * mon = gdk_display_get_monitor_at_window(dpy, win);
  GdkMonitor * mon = gdk_display_get_primary_monitor(dpy);
  GdkRectangle * sc = new GdkRectangle();
  gdk_monitor_get_geometry(mon, sc);

  // Wayland
  if( sc->width == 0 || sc->height == 0) {
    GdkScreen * screen = gdk_screen_get_default();
    guint monitor = gdk_screen_get_primary_monitor(screen);
    gdk_screen_get_monitor_geometry(screen, monitor, sc);
    cout << "Wayland Size: " << sc->width << " x " << sc->height << endl;
  }

  
  if( sc->width == 0 ) sc->width  = 800;
  if( sc->height == 0) sc->height = 600;
  
  double fac = (double)sc->width/sc->height;
  cout << "Screen size: " << sc->width << " x " << sc->height;
  if( fac > 1.7 )     cout << " 16:9" << endl;
  else if( fac > 1.3) cout << " 4:3" << endl;
  else                cout << " less than 4:3" << endl;
  
  return (sc);
}



GtkWidget * GetTopWidget(const char * title) {

  GtkWidget * twg = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(twg), title);

  g_signal_connect(twg, "key_press_event",   G_CALLBACK(keyPress),    NULL);
  g_signal_connect(twg, "key_release_event", G_CALLBACK(keyRelease),  NULL);
  g_signal_connect_swapped(twg, "destroy",   G_CALLBACK(gtk_main_quit), NULL);
  //gtk_window_set_gravity(GTK_WINDOW(twg), GDK_GRAVITY_NORTH_WEAST);

  return(twg);
}


GtkWidget * GetGlWidget() {
  GtkWidget * gla = gtk_gl_area_new();

  gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(gla), true); //z-buffer
  //gtk_gl_area_set_has_stencil_buffer(GTK_GL_AREA(gla), true);
  
  gtk_widget_set_events(gla, GDK_EXPOSURE_MASK |
			GDK_BUTTON_PRESS_MASK |
   			GDK_BUTTON_RELEASE_MASK |
   			GDK_POINTER_MOTION_MASK |
   			//GDK_POINTER_MOTION_HINT_MASK |
   			GDK_CONTROL_MASK |
   			GDK_KEY_PRESS_MASK |
			GDK_KEY_RELEASE_MASK |
  			GDK_SCROLL_MASK
			);

  // /* prepare GL */
  // GdkGLConfig * glconfig = gdk_gl_config_new_by_mode( (GdkGLConfigMode)
  // 						      (GDK_GL_MODE_RGB |
  // 						       GDK_GL_MODE_DEPTH |
  // 						       GDK_GL_MODE_DOUBLE));
  // if( !glconfig) {
  //   cout << "Error glconfig" << endl;
  //   //g_assert_not_reached();
  // }

  // if( !gtk_widget_set_gl_capability(gla, glconfig, NULL, true, GDK_GL_RGBA_TYPE)) {
  //   cout << "Error gtkglcapability" << endl;
  //   //g_assert_not_reached();
  // }
  
  gtk_widget_set_name(gla, "plot");
  
  // window has been created - initialization routines
  //  g_signal_connect(gla, "realize", G_CALLBACK(realize), NULL);
  // window was destroyed - cleanup, de-allocating data
  //  g_signal_connect(gla, "destroy", G_CALLBACK(destroy), NULL);

  // window was resized - reshape function
  g_signal_connect(gla, "realize", G_CALLBACK(configure), NULL);
 
  //window was exposed - need redraw
  g_signal_connect(gla, "render", G_CALLBACK(expose), NULL);
  g_signal_connect(gla, "resize", G_CALLBACK(resize), NULL);
  
  //Button events
  g_signal_connect(gla, "button_press_event",   G_CALLBACK(butPress),    NULL);
  g_signal_connect(gla, "button_release_event", G_CALLBACK(butRelease),  NULL);
  g_signal_connect(gla, "motion_notify_event",  G_CALLBACK(mouseMotion), NULL);
  g_signal_connect(gla, "scroll-event",         G_CALLBACK(butScroll),   NULL);

  return(gla);
}
