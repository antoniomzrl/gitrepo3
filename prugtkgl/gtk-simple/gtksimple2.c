#include	<stdio.h>
#include	<stdlib.h>
#include	<gtk/gtk.h>

/* Prototypes */

int	main(int, char **);
void	add_button(GtkWidget *);
gint	destroyapp(GtkWidget *, gpointer);
gint	closeAppWindow(GtkWidget *, gpointer);
void	button_was_clicked(GtkWidget *, gpointer);

gint closeAppWindow(GtkWidget *widget, gpointer gdata)
{
  g_print("Quitting...\n");
  gtk_main_quit();

  /* ... Continue closing ... */
  return(FALSE);
}

void button_was_clicked(GtkWidget *widget, gpointer gdata)
{
  g_print("Button was clicked.\n");
}

gint destroyapp(GtkWidget *widget, gpointer gdata)
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

  /* ... Put some breathing room around objects in the container ... */
  gtk_container_border_width(GTK_CONTAINER(window),15);

  /* ... Add button to window ... */
  add_button(window);

  /* ... Now, make the window visible ... */
  gtk_widget_show(window);

  /* ... This is the event loop in gtk ... */
  /* ... Do not return until _gtk_main_quit_ is called ... */
  gtk_main();

  /* ... Exit status code ... */
  return(0);
}

void add_button(GtkWidget *parent_window)
{
	GtkWidget	*button;

  /* ... Create a button ... */
  button = gtk_button_new_with_label("Click me");

  /* ... Give the button an event handler - it'll call the function */
  /* ... button_was_clicked when the button is clicked ... */
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
                     GTK_SIGNAL_FUNC(button_was_clicked),NULL);

  /* ... The main window contains the button ... */
  gtk_container_add(GTK_CONTAINER(parent_window),button);

  /* ... Make the button visible ... */
  gtk_widget_show(button);
}
