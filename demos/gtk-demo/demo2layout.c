#include "demo2layout.h"


struct _Demo2Layout
{
  GtkLayoutManager parent_instance;

  float position;
};

struct _Demo2LayoutClass
{
  GtkLayoutManagerClass parent_class;
};

G_DEFINE_TYPE (Demo2Layout, demo2_layout, GTK_TYPE_LAYOUT_MANAGER)

static void
demo2_layout_measure (GtkLayoutManager *layout_manager,
                      GtkWidget        *widget,
                      GtkOrientation    orientation,
                      int               for_size,
                      int              *minimum,
                      int              *natural,
                      int              *minimum_baseline,
                      int              *natural_baseline)
{
  GtkWidget *child;
  int minimum_size = 0;
  int natural_size = 0;

  for (child = gtk_widget_get_first_child (widget);
       child != NULL;
       child = gtk_widget_get_next_sibling (child))
    {
      int child_min = 0, child_nat = 0;

      if (!gtk_widget_should_layout (child))
        continue;

      gtk_widget_measure (child, orientation, -1,
                          &child_min, &child_nat,
                          NULL, NULL);
      minimum_size = MAX (minimum_size, child_min);
      natural_size = MAX (natural_size, child_nat);
    }

  *minimum = minimum_size;
  *natural = 3 * natural_size;
}

static void
demo2_layout_allocate (GtkLayoutManager *layout_manager,
                       GtkWidget        *widget,
                       int               width,
                       int               height,
                       int               baseline)
{
  Demo2Layout *self = DEMO2_LAYOUT (layout_manager);
  GtkWidget *child;
  float t = fmodf (self->position, 4.0);
  int x, y;

  for (child = gtk_widget_get_first_child (widget);
       child != NULL;
       child = gtk_widget_get_next_sibling (child))
    {
      GtkRequisition child_req;

      if (!gtk_widget_should_layout (child))
        continue;

      gtk_widget_get_preferred_size (child, &child_req, NULL);

      if (t < 1.0)
        {
          x = t * (width - child_req.width);
          y = 0;
        }
      else if (t < 2.0)
        {
          t -= 1.0;
          x = (1.0 - t) * (width - child_req.width);
          y = t * (height - child_req.height);
        }
      else if (t < 3.0)
        {
          t -= 2.0;
          x = t * (width - child_req.width);
          y = height - child_req.height;
        }
      else
        {
          t -= 3.0;
          x = (1.0 - t) * (width - child_req.width);
          y = (1.0 - t) * (height - child_req.height);
        }

      gtk_widget_size_allocate (child,
                                &(const GtkAllocation){ x, y, child_req.width, child_req.height},
                                -1);
    }
}

static GtkSizeRequestMode
demo2_layout_get_request_mode (GtkLayoutManager *layout_manager,
                               GtkWidget        *widget)
{
  return GTK_SIZE_REQUEST_CONSTANT_SIZE;
}

static void
demo2_layout_class_init (Demo2LayoutClass *klass)
{
  GtkLayoutManagerClass *layout_class = GTK_LAYOUT_MANAGER_CLASS (klass);

  layout_class->get_request_mode = demo2_layout_get_request_mode;
  layout_class->measure = demo2_layout_measure;
  layout_class->allocate = demo2_layout_allocate;
}

static void
demo2_layout_init (Demo2Layout *self)
{
}

GtkLayoutManager *
demo2_layout_new (void)
{
  return g_object_new (DEMO2_TYPE_LAYOUT, NULL);
}

void
demo2_layout_set_position (Demo2Layout *layout,
                           float        position)
{
  layout->position = position;
}
