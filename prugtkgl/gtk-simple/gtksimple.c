#include	<stdio.h>
#include	<stdlib.h>
#include	<gtk/gtk.h>

/* Prototypes */

int	main(int argc, char **argv);
gint	destroyapp(GtkWidget *widget, gpointer gdata);

gint	destroyapp(GtkWidget *widget, gpointer gdata)
{
  g_print("Quitting...\n");
  gtk_main_quit();

  /* ... Ok to be closed.  TRUE would prevent window from closing ... */
  return(FALSE);
}

int main(int argc, char **argv)
{
	GtkWidget	*window;

  /* ... Initialize gtk, handle command-line parameters ... */
  gtk_init(&argc, &argv);

  /* ... Create a window in gtk - note the window is NOT visible yet ... */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* ... Call destroyapp when the window gets a "delete_event" signal ... */
  gtk_signal_connect(GTK_OBJECT(window),"delete_event",
                     GTK_SIGNAL_FUNC(destroyapp), NULL);

  /* ... Now, make the window visible ... */
  gtk_widget_show(window);

  /* ... This is the event loop in gtk ... */
  /* ... Do not return until _gtk_main_quit_ is called ... */
  gtk_main();

  /* ... Exit status code ... */
  return(0);
}
