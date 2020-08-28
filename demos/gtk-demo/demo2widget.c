#include "demo2widget.h"
#include "demo2layout.h"

/* parent widget */

struct _Demo2Widget
{
  GtkWidget parent_instance;

  gint64 start_time; /* time the transition started */
  guint tick_id;     /* our tick cb */
};

struct _Demo2WidgetClass
{
  GtkWidgetClass parent_class;
};

G_DEFINE_TYPE (Demo2Widget, demo2_widget, GTK_TYPE_WIDGET)

/* The widget is controlling the transition by calling
 * demo_layout_set_position() in a tick callback.
 */

static gboolean
transition (GtkWidget     *widget,
            GdkFrameClock *frame_clock,
            gpointer       data)
{
  Demo2Widget *self = DEMO2_WIDGET (widget);
  Demo2Layout *demo2_layout = DEMO2_LAYOUT (gtk_widget_get_layout_manager (widget));
  gint64 now = g_get_monotonic_time ();

  gtk_widget_queue_allocate (widget);
  demo2_layout_set_position (demo2_layout, (now - self->start_time) * 30.0 / ((double)G_TIME_SPAN_SECOND));

  return G_SOURCE_CONTINUE;
}

static void
demo2_widget_init (Demo2Widget *self)
{
  self->start_time = g_get_monotonic_time ();
  self->tick_id = gtk_widget_add_tick_callback (GTK_WIDGET (self), transition, NULL, NULL);
}

static void
demo2_widget_dispose (GObject *object)
{
  GtkWidget *child;

  while ((child = gtk_widget_get_first_child (GTK_WIDGET (object))))
    gtk_widget_unparent (child);

  G_OBJECT_CLASS (demo2_widget_parent_class)->dispose (object);
}

static void
demo2_widget_class_init (Demo2WidgetClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

  object_class->dispose = demo2_widget_dispose;

  /* here is where we use our custom layout manager */
  gtk_widget_class_set_layout_manager_type (widget_class, DEMO2_TYPE_LAYOUT);
}

GtkWidget *
demo2_widget_new (void)
{
  return g_object_new (DEMO2_TYPE_WIDGET, NULL);
}

void
demo2_widget_add_child (Demo2Widget *self,
                        GtkWidget  *child)
{
  gtk_widget_set_parent (child, GTK_WIDGET (self));
}
