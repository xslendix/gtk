/* simple.c
 * Copyright (C) 2017  Red Hat, Inc
 * Author: Benjamin Otte
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#include "config.h"
#include <gtk/gtk.h>


static void
hello (void)
{
  g_print ("hello world\n");
}

static void
quit_cb (GtkWidget *widget,
         gpointer   data)
{
  gboolean *done = data;

  *done = TRUE;

  g_main_context_wakeup (NULL);
}

static void
invert3 (double m[9], double n[9])
{
  double det;
  double a = m[0];
  double b = m[1];
  double c = m[2];
  double d = m[3];
  double e = m[4];
  double f = m[5];
  double g = m[6];
  double h = m[7];
  double k = m[8];

  det = a*(e*k - f*h) - b*(d*k - f*g) + c*(d*h - e*g);

  n[0] =  (e*k - f*h) / det;
  n[1] = - (b*k - c*h) / det;
  n[2] = (b*f - c*e) / det;
  n[3] = - (d*k - f*g) / det;
  n[4] = (a*k - c*g) / det;
  n[5] = - (a*g - c*d) / det;
  n[6] = (d*h - e*g) / det;
  n[7] = - (a*h - b*g) / det;
  n[8] = (a*e - b*d) / det;
}

static void
perspective2d (double x0, double y0,
               double x1, double y1,
               double x2, double y2,
               double x3, double y3,
               double o[9])
{
  double mat[9], inv[9];
  double l, m, t;

  mat[0] = x0;
  mat[1] = x1;
  mat[2] = x2;
  mat[3] = y0;
  mat[4] = y1;
  mat[5] = y2;
  mat[6] = 1;
  mat[7] = 1;
  mat[8] = 1;

  invert3 (mat, inv);

  l = inv[0] * x3 + inv[1] * y3 + inv[2];
  m = inv[3] * x3 + inv[4] * y3 + inv[5];
  t = inv[6] * x3 + inv[7] * y3 + inv[8];

  o[0] = l*x0; o[1] = m*x1; o[2] = t*x2;
  o[3] = l*y0; o[4] = m*y1; o[5] = t*y2;
  o[6] = l;    o[7] = m;    o[8] = t;
}

#define RADIANS(angle) ((angle)*M_PI/180.0);
#define SX(r,t,p) ((r) * sin (t) * cos (p))
#define SZ(r,t,p) ((r) * sin (t) * sin (p))
#define SY(r,t,p) ((r) * cos (t))

static double offset;

static gboolean
tick_cb (GtkWidget *widget,
         GdkFrameClock *clock,
         gpointer data)
{
  offset += 1;
  gtk_widget_queue_draw (widget);
  return G_SOURCE_CONTINUE;
}

static void
draw_func (GtkDrawingArea *da,
           cairo_t *cr,
           int width,
           int height,
           gpointer data)
{
  double r = 500;
  double t1, t2, p1, p2;
  double x0, y0, x1, y1, x2, y2, x3, y3;
  int i, j;

  for (i = 0; i < 18; i++)
    for (j = 0; j < 36; j++)
      {
        if ((i + j) % 2)
          cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
        else
          cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);

        t1 = RADIANS (10 * i);
        t2 = RADIANS (10 * (i + 1));
        p1 = RADIANS (offset + 10 * j);
        p2 = RADIANS (offset + 10 * (j + 1));

        x0 = SX (r, t1, p1);
        y0 = SY (r, t1, p1);
        x1 = SX (r, t1, p2);
        y1 = SY (r, t1, p2);
        x2 = SX (r, t2, p2);
        y2 = SY (r, t2, p2);
        x3 = SX (r, t2, p1);
        y3 = SY (r, t2, p1);

        if (SZ (r, t1, p1) > 0)
          continue;

        cairo_move_to (cr, 150, 150);
        cairo_rel_move_to (cr, x0, y0);
        cairo_rel_line_to (cr, x1 - x0, y1 - y0);
        cairo_rel_line_to (cr, x2 - x1, y2 - y1);
        cairo_rel_line_to (cr, x3 - x2, y3 - y2);
        cairo_rel_line_to (cr, x0 - x3, y0 - y3);
        cairo_close_path (cr);

        cairo_fill (cr);
      }
}

int
main (int argc, char *argv[])
{
  GtkWidget *window, *box, *da;
  gboolean done = FALSE;

  gtk_init ();

  window = gtk_window_new ();
  gtk_window_set_title (GTK_WINDOW (window), "hello world");
  g_signal_connect (window, "destroy", G_CALLBACK (quit_cb), &done);

  box = gtk_box_new (1, 10);
  gtk_window_set_child (GTK_WINDOW (window), box);
  da = gtk_drawing_area_new ();
  gtk_widget_set_hexpand (da, TRUE);
  gtk_widget_set_vexpand (da, TRUE);
  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (da), draw_func, NULL, NULL);
  gtk_box_append (GTK_BOX (box), da);
  gtk_widget_add_tick_callback (da, tick_cb, NULL, NULL);

  gtk_widget_show (window);

  while (!done)
    g_main_context_iteration (NULL, TRUE);

  return 0;
}
