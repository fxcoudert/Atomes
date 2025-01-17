/* This file is part of Atomes.

Atomes is free software: you can redistribute it and/or modify it under the terms
of the GNU Affero General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Atomes is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with Atomes.
If not, see <https://www.gnu.org/licenses/> */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <cairo-svg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "interface.h"
#include "callbacks.h"
#include "project.h"
#include "curve.h"
#include "cedit.h"

GtkWidget * axischoice = NULL;
GtkWidget * scale_box = NULL;
GtkWidget * ticks_inout_box = NULL;
GtkWidget * ticks_pos_box = NULL;
GtkWidget * labels_pos_box = NULL;
GtkWidget * ticks_labels_angle = NULL;
GtkWidget * ticks_labels_font = NULL;
GtkWidget * show_axis = NULL;
GtkWidget * show_grid = NULL;
GtkWidget * auto_scale = NULL;
GtkWidget * axis_default_title = NULL;
GtkWidget * axis_title = NULL;
GtkWidget * axis_title_font = NULL;
GtkWidget * majt = NULL;
GtkWidget * vmin = NULL;
GtkWidget * vmax = NULL;
GtkWidget * nmi[2], * ndi[2];
GtkWidget * mats[2];
GtkWidget * mits[2];
GtkWidget * nptx[2], * npty[2];
GtkWidget * tptx[2], * tpty[2];

extern qint dataxe[2];
extern qint framxe[4];
extern int a, b, c, d;

int get_active_axis ()
{
  return gtk_combo_box_get_active (GTK_COMBO_BOX(axischoice));
}

G_MODULE_EXPORT void set_axis_min (GtkEntry * res, gpointer data)
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  struct project * this_proj = get_project_by_id(a);
  int i = get_active_axis();
  if (i > -1)
  {
    const gchar * m;
    m = entry_get_text (res);
    if (atof(m) < this_proj -> curves[b][c] -> axmax[i])
    {
      this_proj -> curves[b][c] -> axmin[i] = atof(m);
    }
    else
    {
      show_warning ("Axis min must be < to axis max", this_proj -> curves[b][c] -> window);
    }
    update_entry_double (res, this_proj -> curves[b][c]  -> axmin[i]);
    update_curve (data);
  }
}

G_MODULE_EXPORT void set_axis_max (GtkEntry * res, gpointer data)
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  struct project * this_proj = get_project_by_id(a);
  int i = get_active_axis ();
  const gchar * m;
  m = entry_get_text (res);
  if (atof(m) > this_proj -> curves[b][c] -> axmin[i])
  {
    this_proj -> curves[b][c] -> axmax[i] = atof(m);
  }
  else
  {
    show_warning ("Axis max must be > to axis min", this_proj -> curves[b][c] -> window);
  }
  update_entry_double (res, this_proj -> curves[b][c] -> axmax[i]);
  update_curve (data);
}

G_MODULE_EXPORT void set_max_div (GtkEntry * maj, gpointer data)
{
  double tmp;
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  struct project * this_proj = get_project_by_id(a);
  int i = get_active_axis ();
  const gchar * m;
  m = entry_get_text (maj);
  tmp = atof(m);
  if (tmp != 0.0)
  {
    this_proj -> curves[b][c] -> majt[i] = tmp;
    update_curve (data);
  }
  else
  {
    show_warning ("Major tick must be > 0.0", this_proj -> curves[b][c] -> window);
  }
  update_entry_double (maj, this_proj -> curves[b][c] -> majt[i]);
}

G_MODULE_EXPORT void set_min_div_spin (GtkSpinButton * res, gpointer data)
{
  qint * ad = (qint *) data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  d = ad -> d;
  get_project_by_id(a) -> curves[b][c] -> mint[d] = gtk_spin_button_get_value_as_int(res) + 1;
  tint cd;
  cd.a = a;
  cd.b = b;
  cd.c = c;
  update_curve ((gpointer)& cd);
}

G_MODULE_EXPORT void set_ticks_size_major_spin (GtkSpinButton * res, gpointer data)
{
  qint * ad = (qint *) data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  d = ad -> d;
  get_project_by_id(a) -> curves[b][c] -> majt_size[d] = gtk_spin_button_get_value_as_int(res);
  tint cd;
  cd.a = a;
  cd.b = b;
  cd.c = c;
  update_curve ((gpointer)& cd);
}

G_MODULE_EXPORT void set_ticks_size_minor_spin (GtkSpinButton * res, gpointer data)
{
  qint * ad = (qint *) data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  d = ad -> d;
  struct project * this_proj = get_project_by_id(a);
  this_proj -> curves[b][c]-> mint_size[d] = gtk_spin_button_get_value_as_int(res);
  update_entry_int (GTK_ENTRY(res), this_proj -> curves[b][c] -> mint_size[d]);
  tint cd;
  cd.a = a;
  cd.b = b;
  cd.c = c;
  update_curve ((gpointer)& cd);
}

G_MODULE_EXPORT void set_lab_digit_spin (GtkSpinButton * res, gpointer data)
{
  qint * ad = (qint *) data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  d = ad -> d;
  struct project * this_proj = get_project_by_id(a);
  this_proj -> curves[b][c]-> labels_digit[d] = gtk_spin_button_get_value_as_int(res);
  update_entry_int (GTK_ENTRY(res), this_proj -> curves[b][c] -> labels_digit[d]);
  tint cd;
  cd.a = a;
  cd.b = b;
  cd.c = c;
  update_curve ((gpointer)& cd);
}

G_MODULE_EXPORT void set_lab_shift_x_spin (GtkSpinButton * res, gpointer data)
{
  qint * ad = (qint *) data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  d = ad -> d;
  struct project * this_proj = get_project_by_id(a);
  this_proj -> curves[b][c] -> labels_shift_x[d] = gtk_spin_button_get_value_as_int(res);
  update_entry_int (GTK_ENTRY(res), this_proj -> curves[b][c] -> labels_shift_x[d]);
  tint cd;
  cd.a = a;
  cd.b = b;
  cd.c = c;
  update_curve ((gpointer)& cd);
}

G_MODULE_EXPORT void set_lab_shift_y_spin (GtkSpinButton * res, gpointer data)
{
  qint * ad = (qint *) data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  d = ad -> d;
  struct project * this_proj = get_project_by_id(a);
  this_proj -> curves[b][c] -> labels_shift_y[d] = gtk_spin_button_get_value_as_int(res);
  update_entry_int (GTK_ENTRY(res), this_proj -> curves[b][c]-> labels_shift_y[d]);
  tint cd;
  cd.a = a;
  cd.b = b;
  cd.c = c;
  update_curve ((gpointer)& cd);
}

G_MODULE_EXPORT void set_axis_title_x_spin (GtkSpinButton * res, gpointer data)
{
  qint * ad = (qint *) data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  d = ad -> d;
  get_project_by_id(a) -> curves[b][c] -> axis_title_x[d] = gtk_spin_button_get_value_as_int(res);
  tint cd;
  cd.a = a;
  cd.b = b;
  cd.c = c;
  update_curve ((gpointer)& cd);
}

G_MODULE_EXPORT void set_axis_title_y_spin (GtkSpinButton * res, gpointer data)
{
  qint * ad = (qint *) data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  d = ad -> d;
  get_project_by_id(a) -> curves[b][c] -> axis_title_y[d] = gtk_spin_button_get_value_as_int(res);
  tint cd;
  cd.a = a;
  cd.b = b;
  cd.c = c;
  update_curve ((gpointer)& cd);
}

G_MODULE_EXPORT void set_io_ticks (GtkComboBox * box, gpointer data)
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  int i = get_active_axis ();
  get_project_by_id(a) -> curves[b][c] -> ticks_io[i] = gtk_combo_box_get_active (box);
  update_curve (data);
}

G_MODULE_EXPORT void set_pos_ticks (GtkComboBox * box, gpointer data)
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  int i = get_active_axis ();
  get_project_by_id(a) -> curves[b][c] -> ticks_pos[i] = gtk_combo_box_get_active (box);
  update_curve (data);
}

G_MODULE_EXPORT void set_pos_labels (GtkComboBox * box, gpointer data)
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  int i = get_active_axis ();
  get_project_by_id(a) -> curves[b][c] -> labels_pos[i] = gtk_combo_box_get_active (box);
  update_curve (data);
}

G_MODULE_EXPORT void set_ticks_labels_font (GtkFontButton * fontb, gpointer data)
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  int i = get_active_axis ();
  struct project * this_proj = get_project_by_id(a);
  g_free (this_proj -> curves[b][c] -> labels_font[i]);
  this_proj -> curves[b][c] -> labels_font[i] = g_strdup_printf ("%s", gtk_font_chooser_get_font (GTK_FONT_CHOOSER(fontb)));
  update_curve (data);
}

void ticks_angle_has_changed (gpointer data, double value)
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  int i = get_active_axis ();
  get_project_by_id(a) -> curves[b][c] -> labels_angle[i] = value * (pi/180.0);
  update_curve (data);
}

G_MODULE_EXPORT gboolean scroll_set_ticks_angle (GtkRange * range, GtkScrollType scroll, gdouble value, gpointer data)
{
  ticks_angle_has_changed (data, value);
  return FALSE;
}

G_MODULE_EXPORT void set_ticks_angle (GtkRange * range, gpointer data)
{
  ticks_angle_has_changed (data, gtk_range_get_value (range));
}

#ifdef GTK4
G_MODULE_EXPORT void to_axis_title (GtkCheckButton * but, gpointer data)
#else
G_MODULE_EXPORT void to_axis_title (GtkToggleButton * but, gpointer data)
#endif
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  int i = get_active_axis ();
#ifdef GTK4
  if (gtk_check_button_get_active (but))
#else
  if (gtk_toggle_button_get_active (but))
#endif
  {
    widget_set_sensitive (axis_title, 0);
    struct project * this_proj = get_project_by_id(a);
    g_free (this_proj -> curves[b][c] -> axis_title[i]);
    this_proj -> curves[b][c] -> axis_title[i] = g_strdup_printf ("%s", default_title (i, c));
    update_entry_text (GTK_ENTRY(axis_title), this_proj -> curves[b][c] -> axis_title[i]);
  }
  else
  {
    widget_set_sensitive (axis_title, 1);
  }
  update_curve (data);
}

#ifdef GTK4
G_MODULE_EXPORT void set_grid (GtkCheckButton * grid, gpointer data)
#else
G_MODULE_EXPORT void set_grid (GtkToggleButton * grid, gpointer data)
#endif
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  int i = get_active_axis ();
#ifdef GTK4
  get_project_by_id(a) -> curves[b][c] -> show_grid[i] = gtk_check_button_get_active (grid);
#else
  get_project_by_id(a) -> curves[b][c] -> show_grid[i] = gtk_toggle_button_get_active (grid);
#endif
  update_curve (data);
}

G_MODULE_EXPORT void set_autoscale (GtkButton * autosc, gpointer data)
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  int i = get_active_axis ();
  get_project_by_id(a) -> curves[b][c] -> autoscale[i] = TRUE;
  update_curve (data);
}

#ifdef GTK4
G_MODULE_EXPORT void set_axis (GtkCheckButton * axis, gpointer data)
#else
G_MODULE_EXPORT void set_axis (GtkToggleButton * axis, gpointer data)
#endif
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  int i = get_active_axis ();
#ifdef GTK4
  get_project_by_id(a) -> curves[b][c] -> show_axis[i] = gtk_check_button_get_active (axis);
#else
  get_project_by_id(a) -> curves[b][c] -> show_axis[i] = gtk_toggle_button_get_active (axis);
#endif
  update_curve (data);
}

G_MODULE_EXPORT void set_axis_legend (GtkEntry * xtit, gpointer data)
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  int i = get_active_axis ();
  struct project * this_proj = get_project_by_id(a);
  g_free (this_proj -> curves[b][c] -> axis_title[i]);
  this_proj -> curves[b][c] -> axis_title[i] = g_strdup_printf ("%s", entry_get_text (xtit));
  update_curve (data);
}

G_MODULE_EXPORT void set_axis_title_font (GtkFontButton * fontb, gpointer data)
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  struct project * this_proj = get_project_by_id(a);
  int i = get_active_axis ();
  g_free (this_proj -> curves[b][c] -> axis_title_font[i]);
  this_proj -> curves[b][c] -> axis_title_font[i] = g_strdup_printf ("%s", gtk_font_chooser_get_font (GTK_FONT_CHOOSER(fontb)));
  update_curve (data);
}

G_MODULE_EXPORT void set_scale (GtkComboBox * sbox, gpointer data)
{
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  struct project * this_proj = get_project_by_id(a);
  int i = get_active_axis ();
  this_proj -> curves[b][c] -> scale[i] = gtk_combo_box_get_active(sbox);
  widget_set_sensitive (majt, ! gtk_combo_box_get_active(sbox));
  widget_set_sensitive (nmi[i], ! gtk_combo_box_get_active(sbox));
  this_proj -> curves[b][c]-> autoscale[i] = TRUE;
  update_curve (data);
}

int handler_id;

G_MODULE_EXPORT void update_axis (GtkComboBox * widg, gpointer data)
{
  int i;
  tint * ad = (tint *)data;
  a = ad -> a;
  b = ad -> b;
  c = ad -> c;
  struct project * this_proj = get_project_by_id(a);
  i = gtk_combo_box_get_active (widg);
  update_entry_double (GTK_ENTRY(vmin), this_proj -> curves[b][c] -> axmin[i]);
  update_entry_double (GTK_ENTRY(vmax), this_proj -> curves[b][c] -> axmax[i]);
  update_entry_double (GTK_ENTRY(majt), this_proj -> curves[b][c] -> majt[i]);
  gtk_combo_box_set_active (GTK_COMBO_BOX(ticks_inout_box), this_proj -> curves[b][c] -> ticks_io[i]);
  gtk_combo_box_set_active (GTK_COMBO_BOX(ticks_pos_box), this_proj -> curves[b][c] -> ticks_pos[i]);
  gtk_combo_box_set_active (GTK_COMBO_BOX(labels_pos_box), this_proj -> curves[b][c] -> labels_pos[i]);
  gtk_font_chooser_set_font (GTK_FONT_CHOOSER(ticks_labels_font), this_proj -> curves[b][c] -> labels_font[i]);
  gtk_range_set_value (GTK_RANGE(ticks_labels_angle), this_proj -> curves[b][c] -> labels_angle[i] * (180.0/pi));
  if (b < MS)
  {
    widget_set_sensitive (scale_box, 0);
  }
  else
  {
    g_signal_handler_disconnect (G_OBJECT(scale_box), handler_id);
    gtk_combo_box_set_active (GTK_COMBO_BOX(scale_box), this_proj -> curves[b][c] -> scale[i]);
    handler_id = g_signal_connect (G_OBJECT(scale_box), "changed", G_CALLBACK(set_scale), data);
    widget_set_sensitive (scale_box, 1);
    /* widget_set_sensitive (vmax, ! this_proj -> curves[b][c] -> scale[i]);
    widget_set_sensitive (vmin, ! this_proj -> curves[b][c] -> scale[i]); */
    widget_set_sensitive (majt, ! this_proj -> curves[b][c] -> scale[i]);
    widget_set_sensitive (nmi[i], ! this_proj -> curves[b][c] -> scale[i]);
  }
  gtk_widget_hide (nmi[! i]);
  gtk_widget_hide (ndi[! i]);
  gtk_widget_hide (mats[! i]);
  gtk_widget_hide (mits[! i]);
  gtk_widget_hide (nptx[! i]);
  gtk_widget_hide (npty[! i]);
  gtk_widget_hide (tptx[! i]);
  gtk_widget_hide (tpty[! i]);

  gtk_widget_show (nmi[i]);
  gtk_widget_show (ndi[i]);
  gtk_widget_show (mats[i]);
  gtk_widget_show (mits[i]);
  gtk_widget_show (nptx[i]);
  gtk_widget_show (npty[i]);
  gtk_widget_show (tptx[i]);
  gtk_widget_show (tpty[i]);

#ifdef GTK4
  gtk_check_button_set_active (GTK_CHECK_BUTTON(show_axis), this_proj -> curves[b][c] -> show_axis[i]);
  gtk_check_button_set_active (GTK_CHECK_BUTTON(show_grid), this_proj -> curves[b][c] -> show_grid[i]);
  gtk_check_button_set_active (GTK_CHECK_BUTTON(axis_default_title), this_proj -> curves[b][c] -> axis_defaut_title[i]);
#else
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(show_axis), this_proj -> curves[b][c] -> show_axis[i]);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(show_grid), this_proj -> curves[b][c] -> show_grid[i]);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(axis_default_title), this_proj -> curves[b][c] -> axis_defaut_title[i]);
#endif
  widget_set_sensitive (axis_title, ! this_proj -> curves[b][c] -> axis_defaut_title[i]);
  update_entry_text (GTK_ENTRY(axis_title), this_proj -> curves[b][c] -> axis_title[i]);
  gtk_font_chooser_set_font (GTK_FONT_CHOOSER(axis_title_font), this_proj -> curves[b][c] -> axis_title_font[i]);
}

GtkWidget * create_tab_4 (gpointer data)
{

  tint * cd = (tint *) data;
  int i;

  a = cd -> a;
  b = cd -> b;
  c = cd -> c;
  struct project * this_proj = get_project_by_id(a);

  GtkWidget * axisbox;
#ifdef GTK4
  axisbox = create_vbox (3);
#else
  axisbox = create_vbox (BSEP);
#endif
  GtkWidget * ahbox = create_hbox (0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, axisbox, ahbox, FALSE, FALSE, 5);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, markup_label("<b>Select axis:</b>", 175, -1, 1.0, 0.5), FALSE, FALSE, 0);
  axischoice = create_combo ();
  combo_text_append (axischoice, "X axis");
  combo_text_append (axischoice, "Y axis");
  gtk_combo_box_set_active (GTK_COMBO_BOX(axischoice), 0);
  gtk_widget_set_size_request (axischoice, 80, 35);
  g_signal_connect (G_OBJECT(axischoice), "changed", G_CALLBACK(update_axis), data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, axischoice, FALSE, FALSE, 25);

  add_box_child_start (GTK_ORIENTATION_VERTICAL, axisbox, create_hsep (), FALSE, FALSE, 5);

  ahbox = create_hbox (0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, axisbox, ahbox, FALSE, FALSE, 0);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox,  markup_label("Scale type:", 100, -1, 0.0, 0.5), FALSE, FALSE, 0);
  scale_box = create_combo ();
  combo_text_append (scale_box, "Linear");
  combo_text_append (scale_box, "Log");
  gtk_combo_box_set_active (GTK_COMBO_BOX(scale_box), this_proj -> curves[b][c] -> scale[0]);
  gtk_widget_set_size_request (scale_box, 80, -1);
  handler_id = g_signal_connect (G_OBJECT(scale_box), "changed", G_CALLBACK(set_scale), data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, scale_box, FALSE, FALSE, 0);
  auto_scale = create_button ("Autoscale axis", IMG_NONE, NULL, -1, -1, GTK_RELIEF_NORMAL, G_CALLBACK(set_autoscale), data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, auto_scale, FALSE, FALSE, 30);

// Axis min and max
  ahbox = create_hbox (0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, axisbox, ahbox, FALSE, FALSE, 5);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, markup_label("Min:", 30, -1, 0.5, 0.5), FALSE, FALSE, 20);
  vmin = create_entry (G_CALLBACK(set_axis_min), 100, 15, FALSE, data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, vmin, FALSE, FALSE, 0);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, markup_label("Max:", 30, -1, 0.5, 0.5), FALSE, FALSE, 20);
  vmax = create_entry (G_CALLBACK(set_axis_max), 100, 15, FALSE, data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, vmax, FALSE, FALSE, 0);

// Major ticks spacing
  ahbox = create_hbox (0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, axisbox, ahbox, FALSE, FALSE, 1);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, markup_label("Major ticks spacing:", 160, -1, 0.0, 0.5), FALSE, FALSE, 10);
  majt = create_entry (G_CALLBACK(set_max_div), 100, 15, FALSE, data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, majt, FALSE, FALSE, 0);

// Number of minors ticks
  ahbox = create_hbox (0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, axisbox, ahbox, FALSE, FALSE, 1);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, markup_label("Number of minor ticks:", 160, -1, 0.0, 0.5), FALSE, FALSE, 10);
  for (i=0; i<2; i++)
  {
    nmi[i] = spin_button (G_CALLBACK(set_min_div_spin), (double)this_proj -> curves[b][c] -> mint[i]-1.0, 0.0, 100.0, 1.0, 0, 50, & dataxe[i]);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, nmi[i], FALSE, FALSE, 0);
  }

// Ticks ...
  ahbox = create_hbox (0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, axisbox, ahbox, FALSE, FALSE, 5);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, markup_label("<u>Tick marks:</u>", -1, -1, 0.0, 0.5), FALSE, FALSE, 0);

// ... position ...
  ahbox = abox (axisbox, "Location:", 1);
  ticks_inout_box = create_combo ();
  combo_text_append (ticks_inout_box, "In");
  combo_text_append (ticks_inout_box, "Out");
  gtk_widget_set_size_request (ticks_inout_box, 60, -1);
  g_signal_connect (G_OBJECT(ticks_inout_box), "changed", G_CALLBACK(set_io_ticks), data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, ticks_inout_box, FALSE, FALSE, 3);

  ticks_pos_box = create_combo ();
  combo_text_append (ticks_pos_box, "Normal");
  combo_text_append (ticks_pos_box, "Opposite");
  combo_text_append (ticks_pos_box, "Both");
  combo_text_append (ticks_pos_box, "None");
  gtk_widget_set_size_request (ticks_pos_box, 100, -1);
  g_signal_connect (G_OBJECT(ticks_pos_box), "changed", G_CALLBACK(set_pos_ticks), data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, ticks_pos_box, FALSE, FALSE, 3);

//  ... sizes ...

  ahbox = abox (axisbox, "Major ticks size:", 1);
  for (i=0; i<2; i++)
  {
    mats[i] = spin_button (G_CALLBACK(set_ticks_size_major_spin), (double)this_proj -> curves[b][c] -> majt_size[i], 0.0, 100.0, 1.0, 0, 50, & dataxe[i]);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, mats[i], FALSE, FALSE, 0);
  }
  ahbox = abox (axisbox, "Minor ticks size:", 1);
  for (i=0; i<2; i++)
  {
    mits[i] = spin_button (G_CALLBACK(set_ticks_size_minor_spin), (double)this_proj -> curves[b][c] -> mint_size[i], 0.0, 100.0, 1.0, 0, 50, & dataxe[i]);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, mits[i], FALSE, FALSE, 0);
  }

// Ticks labels ...
  ahbox = create_hbox (0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, axisbox, ahbox, FALSE, FALSE, 5);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, markup_label("<u>Tick labels:</u>", -1, -1, 0.0, 0.5), FALSE, FALSE, 0);

// ... position ...
  ahbox = abox (axisbox, "Location:", 1);
  labels_pos_box = create_combo ();
  combo_text_append (labels_pos_box, "Normal");
  combo_text_append (labels_pos_box, "Opposite");
  combo_text_append (labels_pos_box, "Both");
  combo_text_append (labels_pos_box, "None");
  gtk_widget_set_size_request (labels_pos_box, 150, -1);
  g_signal_connect (G_OBJECT(labels_pos_box), "changed", G_CALLBACK(set_pos_labels), data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, labels_pos_box, FALSE, FALSE, 0);

// ... significant digits ...
  ahbox = abox (axisbox, "Significant digits:", 1);
  for (i=0; i<2; i++)
  {
    ndi[i] = spin_button (G_CALLBACK(set_lab_digit_spin), (double)this_proj -> curves[b][c] -> labels_digit[i], 0.0, 100.0, 1.0, 0, 50, & dataxe[i]);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, ndi[i], FALSE, FALSE, 0);
  }

// ... font ...
  ticks_labels_font = font_button (this_proj -> curves[b][c] -> labels_font[0], 150, 35, G_CALLBACK(set_ticks_labels_font), data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, abox (axisbox, "Font:", 1), ticks_labels_font, FALSE, FALSE, 5);

// ... angle ..
  ticks_labels_angle = create_hscale (-180.0, 180.0, 1.0, 0.0, GTK_POS_LEFT, 0, 150, G_CALLBACK(set_ticks_angle), G_CALLBACK(scroll_set_ticks_angle), data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, abox (axisbox, "Angle:", 1), ticks_labels_angle, FALSE, FALSE, 5);

// ...distance to axis
  ahbox = abox (axisbox, "Position: ", 1);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, markup_label("x:", -1, -1, 0.9, 0.5), FALSE, FALSE, 10);
  for (i=0; i<2; i++)
  {
    nptx[i] = spin_button (G_CALLBACK(set_lab_shift_x_spin), (double)this_proj -> curves[b][c] -> labels_shift_x[i], -100.0, 100.0, 1.0, 0, 50, & dataxe[i]);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, nptx[i], FALSE, FALSE, 0);
  }
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, markup_label("y:", -1, -1, 0.9, 0.5), FALSE, FALSE, 10);
  for (i=0; i<2; i++)
  {
    npty[i] = spin_button (G_CALLBACK(set_lab_shift_y_spin), (double) this_proj -> curves[b][c] -> labels_shift_y[i], -100.0, 100.0, 1.0, 0, 50, & dataxe[i]);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, npty[i], FALSE, FALSE, 0);
  }

  show_axis = check_button ("Show/hide axis (if min[axis] &lt; 0.0 &lt; max[axis])", 100, 30, FALSE, G_CALLBACK(set_axis), data);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, axisbox, show_axis, FALSE, FALSE, 3);
  show_grid = check_button ("Show/hide axis grid", 100, 30, FALSE, G_CALLBACK(set_grid), data);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, axisbox, show_grid, FALSE, FALSE, 1);

// Axis title
  ahbox = create_hbox (0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, axisbox, ahbox, FALSE, FALSE,3);
  axis_default_title = check_button ("Use default axis title", 175, -1, FALSE, G_CALLBACK(to_axis_title), data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, axis_default_title, FALSE, FALSE, 0);
  axis_title = create_entry (G_CALLBACK(set_axis_legend), 150, 15, FALSE, data);
  gtk_entry_set_alignment (GTK_ENTRY(axis_title), 0.0);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, axis_title, FALSE, FALSE, 0);

  axis_title_font = font_button (this_proj -> curves[b][c] -> axis_title_font[0], 150, 35, G_CALLBACK(set_axis_title_font), data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, abox (axisbox, "Font:", 3), axis_title_font, FALSE, FALSE, 0);

  ahbox = abox (axisbox, "Position: ", 3);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, markup_label("x:", -1, -1, 0.9, 0.5), FALSE, FALSE, 10);
  for (i=0; i<2; i++)
  {
    tptx[i] = spin_button (G_CALLBACK(set_axis_title_x_spin), (double)this_proj -> curves[b][c] -> axis_title_x[i], -500.0, 500.0, 1.0, 0, 50, & dataxe[i]);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, tptx[i], FALSE, FALSE, 0);
  }
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, markup_label("y:", -1, -1, 0.9, 0.5), FALSE, FALSE, 10);
  for (i=0; i<2; i++)
  {
    tpty[i] = spin_button (G_CALLBACK(set_axis_title_y_spin), (double)this_proj -> curves[b][c] -> axis_title_y[i], -500.0, 500.0, 1.0, 0, 50, & dataxe[i]);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, ahbox, tpty[i], FALSE, FALSE, 0);
  }
  return axisbox;
}
