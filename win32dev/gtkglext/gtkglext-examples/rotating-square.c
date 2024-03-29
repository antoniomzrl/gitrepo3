/**************************************************************************
 * rotatingSquare.c
 *
 * Copyright (c) 2002 Alif Wahid <awah005@users.sourceforge.net>
 *
 * A simple animation to demonstrate GtkGLExt.
 *
 * This program is in the public domain and you are using it at
 * your own risk.
 *
 **************************************************************************/

/* 
 * Modified by Naofumi Yasufuku  <naofumi@users.sourceforge.net>
 */

/**************************************************************************
 * Header file inclusions.
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <gtk/gtkgl.h>
/*** Use OpenGL extensions. ***/
/* #include <gdk/gdkglglext.h> */

#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>


/**************************************************************************
 * The following section contains all the macro definitions.
 **************************************************************************/

#define DEFAULT_WIDTH  200
#define DEFAULT_HEIGHT 200
#define DEFAULT_TITLE  "Rotating Square"


/**************************************************************************
 * Global variable declarations.
 **************************************************************************/

static gboolean animate = TRUE;

static float spin = 0.0;


/**************************************************************************
 * The following section contains the function prototype declarations.
 **************************************************************************/

static void         idle_add          (GtkWidget   *widget);
static void         idle_remove       (GtkWidget   *widget);

static void         toggle_animation  (GtkWidget   *widget);

static GdkGLConfig *configure_gl      (void);

static GtkWidget   *create_popup_menu (GtkWidget   *drawing_area);
static GtkWidget   *create_window     (GdkGLConfig *glconfig);


/**************************************************************************
 * The following section contains all the callback function definitions.
 **************************************************************************/

/***
 *** The "realize" signal handler. All the OpenGL initialization
 *** should be performed here, such as default background colour,
 *** certain states etc.
 ***/
static void
realize (GtkWidget *widget,
	 gpointer   data)
{
  GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

  /*** OpenGL BEGIN ***/
  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
    return;

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_FLAT);

  gdk_gl_drawable_gl_end (gldrawable);
  /*** OpenGL END ***/

  return;
}

/***
 *** The "configure_event" signal handler. Any processing required when
 *** the OpenGL-capable drawing area is re-configured should be done here.
 *** Almost always it will be used to resize the OpenGL viewport when
 *** the window is resized.
 ***/
static gboolean
configure_event (GtkWidget         *widget,
		 GdkEventConfigure *event,
		 gpointer           data)
{
  GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

  GLfloat w = widget->allocation.width;
  GLfloat h = widget->allocation.height;

  /*** OpenGL BEGIN ***/
  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
    return FALSE;

  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  gdk_gl_drawable_gl_end (gldrawable);
  /*** OpenGL END ***/

  return TRUE;
}

/***
 *** The "expose_event" signal handler. All the OpenGL re-drawing should
 *** be done here. This is repeatedly called as the painting routine
 *** every time the 'expose'/'draw' event is signalled.
 ***/
static gboolean
expose_event (GtkWidget      *widget,
	      GdkEventExpose *event,
	      gpointer        data)
{
  GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

  /*** OpenGL BEGIN ***/
  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
    return FALSE;

  glClear (GL_COLOR_BUFFER_BIT);

  glPushMatrix ();
    glRotatef (spin, 0.0, 0.0, 1.0);
    glColor3f (1.0, 1.0, 1.0);
    glRectf (-25.0, -25.0, 25.0, 25.0);
  glPopMatrix ();

  /* Swap buffers */
  if (gdk_gl_drawable_is_double_buffered (gldrawable))
    gdk_gl_drawable_swap_buffers (gldrawable);
  else
    glFlush ();

  gdk_gl_drawable_gl_end (gldrawable);
  /*** OpenGL END ***/

  return TRUE;
}

/***
 *** The idle function. Often in animations,
 *** idle functions are suitable for continous
 *** frame updates.
 ***/
static gboolean
idle (GtkWidget *widget)
{
  spin += 2.0;
  if (spin > 360.0) spin -= 360.0;

  gtk_widget_queue_draw (widget);

  return TRUE;
}

/***
 *** The "motion_notify_event" signal handler. Any processing required when
 *** the OpenGL-capable drawing area is under drag motion should be done here.
 ***/
static gboolean
motion_notify_event (GtkWidget      *widget,
		     GdkEventMotion *event,
		     gpointer        data)
{

  /*** Fill in the details here. ***/

  return FALSE;
}

/***
 *** The "button_press_event" signal handler. Any processing required when
 *** mouse buttons (only left and middle buttons) are pressed on the OpenGL-
 *** capable drawing area should be done here.
 ***/
static gboolean
button_press_event (GtkWidget      *widget,
		    GdkEventButton *event,
		    gpointer        data)
{
  /* Starts/Stops animation according mouse button clicks.*/
  if (event->button == 1)
    {
      toggle_animation (widget);
      return TRUE;
    }

  return FALSE;
}

/* For popup menu. */
static gboolean
button_press_event_popup_menu (GtkWidget      *widget,
			       GdkEventButton *event,
			       gpointer        data)
{
  if (event->button == 3)
    {
      /* Popup menu. */
      gtk_menu_popup (GTK_MENU (widget), NULL, NULL, NULL, NULL,
		      event->button, event->time);
      return TRUE;
    }

  return FALSE;
}

/***
 *** The "key_press_event" signal handler. Any processing required when key
 *** presses occur should be done here.
 ***/
static gboolean
key_press_event (GtkWidget   *widget,
		 GdkEventKey *event,
		 gpointer     data)
{
  switch (event->keyval)
    {
    case GDK_Left:
      /* Rotate left. */
      if (!animate)
	{
	  spin += 2.0;
	  if (spin > 360.0) spin -= 360.0;
	  gtk_widget_queue_draw (widget);
	}
      break;

    case GDK_Right:
      /* Rotate right. */
      if (!animate)
	{
	  spin -= 2.0;
	  if (spin < 360.0) spin += 360.0;
	  gtk_widget_queue_draw (widget);
	}
      break;

    case GDK_a:
      /* Toggle animation. */
      toggle_animation (widget);
      break;

    case GDK_Escape:
      /* Quit. */
      gtk_main_quit ();
      break;

    default:
      return FALSE;
    }

  return TRUE;
}

/***
 *** The "unrealize" signal handler. Any processing required when
 *** the OpenGL-capable window is unrealized should be done here.
 ***/
static void
unrealize (GtkWidget *widget,
	   gpointer   data)
{

  /*** Fill in the details here ***/

}


/**************************************************************************
 * The following section contains the idle function management routines.
 **************************************************************************/

/***
 *** Helper functions to add or remove the idle function.
 ***/

static guint idle_id = 0;

static void
idle_add (GtkWidget *widget)
{
  if (idle_id == 0)
    {
      idle_id = gtk_idle_add_priority (GDK_PRIORITY_REDRAW,
				       (GtkFunction) idle,
				       widget);
    }
}

static void
idle_remove (GtkWidget *widget)
{
  if (idle_id != 0)
    {
      gtk_idle_remove (idle_id);
      idle_id = 0;
    }
}

/***
 *** The "map_event" signal handler. Any processing required when the
 *** OpenGL-capable drawing area is mapped should be done here.
 ***/
static gboolean
map_event (GtkWidget *widget,
	   GdkEvent  *event,
	   gpointer   data)
{
  if (animate)
    idle_add (widget);

  return TRUE;
}

/***
 *** The "unmap_event" signal handler. Any processing required when the
 *** OpenGL-capable drawing area is unmapped should be done here.
 ***/
static gboolean
unmap_event (GtkWidget *widget,
	     GdkEvent  *event,
	     gpointer   data)
{
  idle_remove (widget);

  return TRUE;
}

/***
 *** The "visibility_notify_event" signal handler. Any processing required
 *** when the OpenGL-capable drawing area is visually obscured should be
 *** done here.
 ***/
static gboolean
visibility_notify_event (GtkWidget          *widget,
			 GdkEventVisibility *event,
			 gpointer            data)
{
  if (animate)
    {
      if (event->state == GDK_VISIBILITY_FULLY_OBSCURED)
	idle_remove (widget);
      else
	idle_add (widget);
    }

  return TRUE;
}


/**************************************************************************
 * The following section contains some miscellaneous utility functions.
 **************************************************************************/

/***
 *** Toggle animation.
 ***/
static void
toggle_animation (GtkWidget *widget)
{
  animate = !animate;

  if (animate)
    {
      idle_add (widget);
    }
  else
    {
      idle_remove (widget);
      gtk_widget_queue_draw (widget);
    }
}


/**************************************************************************
 * The following section contains the GUI building function definitions.
 **************************************************************************/

/***
 *** Creates the popup menu to be displayed.
 ***/
static GtkWidget *
create_popup_menu (GtkWidget *drawing_area)
{
  GtkWidget *menu;
  GtkWidget *menu_item;

  menu = gtk_menu_new ();

  /* Toggle animation */
  menu_item = gtk_menu_item_new_with_label ("Toggle Animation");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
  g_signal_connect_swapped (G_OBJECT (menu_item), "activate",
			    G_CALLBACK (toggle_animation), drawing_area);
  gtk_widget_show (menu_item);

  /* Quit */
  menu_item = gtk_menu_item_new_with_label ("Quit");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
  g_signal_connect (G_OBJECT (menu_item), "activate",
		    G_CALLBACK (gtk_main_quit), NULL);
  gtk_widget_show (menu_item);
	
  return menu;
}

/***
 *** Creates the simple application window with one
 *** drawing area that has an OpenGL-capable visual.
 ***/
static GtkWidget *
create_window (GdkGLConfig *glconfig)
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *drawing_area;
  GtkWidget *menu;
  GtkWidget *button;

  /*
   * Top-level window.
   */

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), DEFAULT_TITLE);

#ifndef G_OS_WIN32
  /* Perform the resizes immediately */
  gtk_container_set_resize_mode (GTK_CONTAINER (window), GTK_RESIZE_IMMEDIATE);
#endif
  /* Get automatically redrawn if any of their children changed allocation. */
  gtk_container_set_reallocate_redraws (GTK_CONTAINER (window), TRUE);

  /* Connect signal handlers to the window */
  g_signal_connect (G_OBJECT (window), "delete_event",
		    G_CALLBACK (gtk_main_quit), NULL);

  /*
   * VBox.
   */

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);

  /*
   * Drawing area to draw OpenGL scene.
   */

  drawing_area = gtk_drawing_area_new ();
  gtk_widget_set_size_request (drawing_area, DEFAULT_WIDTH, DEFAULT_HEIGHT);

  /* Set OpenGL-capability to the widget */
  gtk_widget_set_gl_capability (drawing_area,
				glconfig,
				NULL,
				TRUE,
				GDK_GL_RGBA_TYPE);

  gtk_widget_add_events (drawing_area,
			 GDK_BUTTON1_MOTION_MASK    |
			 GDK_BUTTON2_MOTION_MASK    |
			 GDK_BUTTON_PRESS_MASK      |
			 GDK_VISIBILITY_NOTIFY_MASK);

  /* Connect signal handlers to the drawing area */
  g_signal_connect_after (G_OBJECT (drawing_area), "realize",
                          G_CALLBACK (realize), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "configure_event",
		    G_CALLBACK (configure_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "expose_event",
		    G_CALLBACK (expose_event), NULL);

  g_signal_connect (G_OBJECT (drawing_area), "motion_notify_event",
		    G_CALLBACK (motion_notify_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "button_press_event",
		    G_CALLBACK (button_press_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "unrealize",
		    G_CALLBACK (unrealize), NULL);

  /* key_press_event handler for top-level window */
  g_signal_connect_swapped (G_OBJECT (window), "key_press_event",
			    G_CALLBACK (key_press_event), drawing_area);

  /* For idle function. */
  g_signal_connect (G_OBJECT (drawing_area), "map_event",
		    G_CALLBACK (map_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "unmap_event",
		    G_CALLBACK (unmap_event), NULL);
  g_signal_connect (G_OBJECT (drawing_area), "visibility_notify_event",
		    G_CALLBACK (visibility_notify_event), NULL);

  gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

  gtk_widget_show (drawing_area);

  /*
   * Popup menu.
   */

  menu = create_popup_menu (drawing_area);

  g_signal_connect_swapped (G_OBJECT (drawing_area), "button_press_event",
			    G_CALLBACK (button_press_event_popup_menu), menu);

  /*
   * Simple quit button.
   */

  button = gtk_button_new_with_label ("Quit");

  g_signal_connect (G_OBJECT (button), "clicked",
                    G_CALLBACK (gtk_main_quit), NULL);

  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  gtk_widget_show (button);

  return window;
}


/**************************************************************************
 * The following section contains utility function definitions.
 **************************************************************************/

/***
 *** Configure the OpenGL framebuffer.
 ***/
static GdkGLConfig *
configure_gl (void)
{
  GdkGLConfig *glconfig;

  /* Try double-buffered visual */
  glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB    |
					GDK_GL_MODE_DEPTH  |
					GDK_GL_MODE_DOUBLE);
  if (glconfig == NULL)
    {
      g_print ("\n*** Cannot find the double-buffered visual.\n");
      g_print ("\n*** Trying single-buffered visual.\n");

      /* Try single-buffered visual */
      glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB   |
					    GDK_GL_MODE_DEPTH);
      if (glconfig == NULL)
	{
	  g_print ("*** No appropriate OpenGL-capable visual found.\n");
	  exit (1);
	}
    }

  return glconfig;
}


/**************************************************************************
 * The main function is rather trivial.
 **************************************************************************/

int
main (int   argc,
      char *argv[])
{
  GtkWidget *window;
  GdkGLConfig *glconfig;

  /* Initialize GTK. */
  gtk_init (&argc, &argv);

  /* Initialize GtkGLExt. */
  gtk_gl_init (&argc, &argv);

  /* Configure OpenGL framebuffer. */
  glconfig = configure_gl ();

  /* Create and show the application window. */
  window = create_window (glconfig);
  gtk_widget_show (window);

  gtk_main ();

  return 0;
}


/**************************************************************************
 * End of file.
 **************************************************************************/
