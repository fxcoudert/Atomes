/* This file is part of Atomes.

Atomes is free software: you can redistribute it and/or modify it under the terms
of the GNU Affero General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Atomes is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with Atomes.
If not, see <https://www.gnu.org/licenses/> */

#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo-ps.h>
#include <cairo-svg.h>

#include "global.h"
#include "curve.h"

extern void adjust_tool_model (int calc, int curve, gchar * string_path);

#ifdef GTK3
gboolean show_curve (GtkWidget * grwdget, cairo_t * cr, gpointer curve)
#else
void show_curve (GtkDrawingArea * area, cairo_t * cr, int width, int height, gpointer curve)
#endif
{
  int pid, rid, cid;
  int where;
  int i, j, k;
  double alp;
  gboolean upm;
  tint * cd = (tint *)curve;

  pid = cd -> a;
  rid = cd -> b;
  cid = cd -> c;
  activec = cid;
  activer = rid;
  cairo_surface_t * surface;

  struct project * this_proj = get_project_by_id(pid);

  where = this_proj -> curves[rid][cid] -> format;
  // g_print ("where= %d, width = %d, height= %d\n", where, width, height);

  this_proj -> curves[rid][cid] -> format = 0;
  if (where == 0)
  {
    alp=1.0;
#ifdef GTK3
    this_proj -> curves[rid][cid] -> wsize[0] = gtk_widget_get_allocated_width (grwdget);
    this_proj -> curves[rid][cid] -> wsize[1] = gtk_widget_get_allocated_height (grwdget);
#else
    this_proj -> curves[rid][cid] -> wsize[0] = width;
    this_proj -> curves[rid][cid] -> wsize[1] = height;
#endif
    for (j=0; j<2; j++)
    {
      resol[j] = this_proj -> curves[rid][cid] -> wsize[j];
      if (xyp[j] != NULL) update_entry_int (GTK_ENTRY(xyp[j]), this_proj -> curves[rid][cid] -> wsize[j]);
    }
  }
  else
  {
    alp=0.0;
    if (where == 1)
    {
      alp = back_alpha;
      surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, resol[0], resol[1]);
    }
    else if (where == 2)
    {
      alp=1.0;
      surface = cairo_pdf_surface_create (curve_image_file, (double)resol[0], (double)resol[1]);
    }
    else if (where == 3)
    {
      alp = back_alpha;
      surface = cairo_svg_surface_create (curve_image_file, (double)resol[0], (double)resol[1]);
    }
    else if (where == 4)
    {
      surface = cairo_ps_surface_create (curve_image_file, (double)resol[0], (double)resol[1]);
    }
    cr = cairo_create (surface);
  }
// Painting the background
  cairo_set_source_rgba (cr, this_proj -> curves[rid][cid] -> backcolor.red,
                             this_proj -> curves[rid][cid] -> backcolor.green,
                             this_proj -> curves[rid][cid] -> backcolor.blue, alp);
  cairo_paint (cr);
  cairo_set_source_rgba (cr, this_proj -> curves[rid][cid] -> backcolor.red,
                             this_proj -> curves[rid][cid] -> backcolor.green,
                             this_proj -> curves[rid][cid] -> backcolor.blue, 1.0);

// Creating Pango Font Layout
  layout = pango_cairo_create_layout (cr);

  upm = FALSE;
// Autoscale and autoticking for X and or Y axis
  for (j=0; j<2; j++)
  {
    if (this_proj -> curves[rid][cid] -> autoscale[j])
    {
      // Autoscale
      autoscale_axis (this_proj, rid, cid, j);
      this_proj -> curves[rid][cid] -> autoscale[j] = FALSE;
      // Autoticking
      this_proj -> curves[rid][cid] -> majt[j] = scale (this_proj -> curves[rid][cid] -> axmax[j] - this_proj -> curves[rid][cid] -> axmin[j]);
      this_proj -> curves[rid][cid] -> mint[j] = 2;
      upm = TRUE;
    }
    cxy[j] = this_proj -> curves[rid][cid] -> axmin[j];
  }
  xmax = this_proj -> curves[rid][cid] -> axmax[0] - cxy[0];
  ymax = this_proj -> curves[rid][cid] -> axmax[1] - cxy[1];
// Ensuring no log scale issues
  if (this_proj -> curves[rid][cid] -> scale[0]) setup_xaxis_log (cr, this_proj, rid, cid, FALSE);
  if (this_proj -> curves[rid][cid] -> scale[1]) setup_yaxis_log (cr, this_proj, rid, cid, FALSE);

  if (upm)
  {
    if (axischoice != NULL)
    {
      j = gtk_combo_box_get_active (GTK_COMBO_BOX(axischoice));
      update_entry_double (GTK_ENTRY(vmin), this_proj -> curves[rid][cid] -> axmin[j]);
      update_entry_double (GTK_ENTRY(vmax), this_proj -> curves[rid][cid] -> axmax[j]);
      update_entry_double (GTK_ENTRY(majt), this_proj -> curves[rid][cid] -> majt[j]);
      update_entry_int (GTK_ENTRY(nmi[j]), this_proj -> curves[rid][cid] -> mint[j] - 1);
    }
  }

  prep_plot (this_proj, rid, cid);

// let's go for the plot
// first we start by the possible extra curves
  i = 0;
  struct cextra * ctmp = this_proj -> curves[rid][cid] -> extrac -> last;
  for ( j=0 ; j < this_proj -> curves[rid][cid] -> extrac -> extras + 1; j++ )
  {
    if (this_proj -> curves[rid][cid] -> draw_id == j)
    {
      if (this_proj -> curves[rid][cid] -> bshift && this_proj -> curves[rid][cid] -> layout -> aspect && this_proj -> curves[rid][cid] -> extrac -> extras) i ++;
      k = (this_proj -> curves[rid][cid] -> layout -> aspect) ? i : 0;
      draw_curve (cr,
                  cid,
                  rid,
                  this_proj,
                  this_proj -> curves[rid][cid] -> ndata,
                  this_proj -> curves[rid][cid] -> layout -> datacolor,
                  this_proj -> curves[rid][cid] -> scale[0],
                  this_proj -> curves[rid][cid] -> scale[1],
                  this_proj -> curves[rid][cid] -> layout -> aspect,
                  this_proj -> curves[rid][cid] -> layout -> dash,
                  this_proj -> curves[rid][cid] -> layout -> thickness,
                  this_proj -> curves[rid][cid] -> layout -> glyph,
                  this_proj -> curves[rid][cid] -> layout -> gsize,
                  this_proj -> curves[rid][cid] -> layout -> gfreq,
                  this_proj -> curves[rid][cid] -> layout -> hwidth,
                  this_proj -> curves[rid][cid] -> layout -> hopac,
                  this_proj -> curves[rid][cid] -> layout -> hpos,
                  this_proj -> curves[rid][cid] -> extrac -> extras,
                  k);

    }
    else
    {
      if (this_proj -> curves[rid][cid] -> bshift && ctmp -> layout -> aspect) i++;
      k = (ctmp -> layout -> aspect) ? i : 0;
      draw_curve (cr,
                  ctmp -> id.c,
                  ctmp -> id.b,
                  get_project_by_id(ctmp -> id.a),
                  get_project_by_id(ctmp -> id.a) -> curves[ctmp -> id.b][ctmp -> id.c] -> ndata,
                  ctmp -> layout -> datacolor,
                  this_proj -> curves[rid][cid] -> scale[0],
                  this_proj -> curves[rid][cid] -> scale[1],
                  ctmp -> layout -> aspect,
                  ctmp -> layout -> dash,
                  ctmp -> layout -> thickness,
                  ctmp -> layout -> glyph,
                  ctmp -> layout -> gsize,
                  ctmp -> layout -> gfreq,
                  ctmp -> layout -> hwidth,
                  ctmp -> layout -> hopac,
                  ctmp -> layout -> hpos,
                  this_proj -> curves[rid][cid] -> extrac -> extras,
                  k);
      if (ctmp -> prev != NULL) ctmp = ctmp -> prev;
    }
  }

  // Draw the axis bars - in option
  if (this_proj -> curves[rid][cid] -> show_axis[1])
  {
    if (this_proj -> curves[rid][cid] -> axmin[0] < 0.0 && this_proj -> curves[rid][cid] -> axmax[0] > 0.0)
    {
      cairo_set_line_width (cr, 1.0);
      cairo_set_dash (cr, pdashed, lenp, 0.0);
      cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 1.0);
      cairo_move_to (cr, x_min - this_proj -> curves[rid][cid] -> axmin[0] * XDRAW / xmax, y_min);
      cairo_line_to (cr, x_min - this_proj -> curves[rid][cid] -> axmin[0] * XDRAW / xmax, y_max);
      cairo_stroke (cr);
    }
  }

  if (this_proj -> curves[rid][cid] -> show_axis[0])
  {
    if (this_proj -> curves[rid][cid] -> axmin[1] < 0.0 && this_proj -> curves[rid][cid] -> axmax[1] > 0.0)
    {
      cairo_set_line_width (cr, 1.0);
      cairo_set_dash (cr, pdashed, lenp, 0.0);
      cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 1.0);
      cairo_move_to (cr, x_min, y_min - this_proj -> curves[rid][cid] -> axmin[1] * YDRAW / ymax);
      cairo_line_to (cr, x_max, y_min - this_proj -> curves[rid][cid] -> axmin[1] * YDRAW / ymax);
      cairo_stroke (cr);
    }
  }
  // Maybe show the frame
  if (this_proj -> curves[rid][cid] -> show_frame) draw_frame (cr, this_proj, rid, cid);
  // maybe for the title
  if (this_proj -> curves[rid][cid] -> show_title) show_title (cr, this_proj, rid, cid);
  // and maybe for the legend
  if (this_proj -> curves[rid][cid] -> show_legend) show_legend (cr, this_proj, rid, cid);

  // and we plot the complete curve
  if (where == 0)
  {
    this_proj -> curves[rid][cid] -> surface = cairo_surface_reference (cairo_get_target (cr));
  }
  else if (where == 1)
  {
     cairo_surface_write_to_png (surface, curve_image_file);
     cairo_surface_destroy (surface);
  }
  else if (where == 2 || where == 4)
  {
    cairo_show_page (cr);
    cairo_surface_destroy (surface);
  }
  else if (where == 3)
  {
    cairo_surface_destroy (surface);
  }
  if (where > 0) cairo_destroy (cr);
  this_proj -> curves[rid][cid] -> displayed = TRUE;
#ifdef GTK3
  return TRUE;
#endif
}

G_MODULE_EXPORT void hide_curve (GtkWidget * curve, gpointer data)
{
  tint * idc = (tint *)data;
  gtk_widget_hide (get_project_by_id(idc -> a) -> curves[idc -> b][idc -> c] -> window);
  if (idc -> a == activep)
  {
    adjust_tool_model (idc -> b, idc -> c, get_project_by_id(idc -> a) -> curves[idc -> b][idc -> c] -> path);
  }
}

#ifdef GTK4
G_MODULE_EXPORT gboolean to_hide_curve (GtkWindow * thecurve, gpointer data)
#else
G_MODULE_EXPORT gboolean to_hide_curve (GtkWidget * thecurve, GdkEvent * event, gpointer data)
#endif
{
  hide_curve ((GtkWidget *)thecurve, data);
  return TRUE;
}
