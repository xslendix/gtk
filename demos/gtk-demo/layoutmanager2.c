/* Layout Manager/Transformation
 *
 * This examples shows a simple example of a custom layout manager
 * and a widget using it. The layout manager places the children
 * of the widget in a grid or a circle, or something in between.
 *
 * The widget is animating the transition between the two layouts.
 * Click to start the transition.
 */

#include <gtk/gtk.h>

#include "demo2widget.h"

GtkWidget *
do_layoutmanager2 (GtkWidget *parent)
{
  static GtkWidget *window = NULL;

  if (!window)
    {
      GtkWidget *widget;
      GtkWidget *child;

      window = gtk_window_new ();
      gtk_window_set_title (GTK_WINDOW (window), "Transformation");
      gtk_window_set_default_size (GTK_WINDOW (window), 200, 100);
      g_object_add_weak_pointer (G_OBJECT (window), (gpointer *)&window);

      widget = demo2_widget_new ();

      child =  gtk_button_new_with_label ("Hello");
      gtk_widget_set_cursor_from_name (child, "crosshair");
      demo2_widget_add_child (DEMO2_WIDGET (widget), child);

      gtk_window_set_child (GTK_WINDOW (window), widget);
    }

  if (!gtk_widget_get_visible (window))
    gtk_widget_show (window);
  else
    gtk_window_destroy (GTK_WINDOW (window));

  return window;

}
