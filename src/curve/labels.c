/* This file is part of Atomes.

Atomes is free software: you can redistribute it and/or modify it under the terms
of the GNU Affero General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Atomes is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with Atomes.
If not, see <https://www.gnu.org/licenses/> */

#include <string.h>
#include <math.h>
#include <cairo/cairo.h>
#include <pango/pangocairo.h>

#include "global.h"
#include "curve.h"

void label (cairo_t * cr, double val, int axe, int p, struct project * this_proj)
{
  gchar * label;
  gchar * lab;
  gchar * tmp;
  int i, j;
  double u, v;

  lab = g_strdup_printf ("%.10lf", val);
  for (i=0; i < strlen(lab); i++)
  {
    tmp = g_strdup_printf ("%c", lab[i]);
    if (g_strcmp0 ((char *)tmp, ".") == 0) j = i;
    g_free (tmp);
  }
  for (i=0; i < j + this_proj -> curves[activer][activec] -> labels_digit[axe] + 1; i++)
  {
    if (i == 0)
    {
      label = g_strdup_printf ("%c", lab[i]);
    }
    else
    {
      tmp = g_strdup_printf ("%s%c", label, lab[i]);
      g_free (label);
      label = g_strdup_printf ("%s", tmp);
      g_free (tmp);
    }
  }
  pango_layout_set_text (layout, label, -1);
  if (p == 1)
  {
    pango_layout_get_size (layout, & i, & j);
    if (axe == 0)
    {
      u = sin (this_proj -> curves[activer][activec] -> labels_angle[axe]) * (double)i;
      v = sin (pi / 2.0 - this_proj -> curves[activer][activec] -> labels_angle[axe]) * (double)j;
      ay = ay - (u + v)/PANGO_SCALE;
    }
    else
    {
      u = sqrt (1.0*i*i + 1.0*j*j);
      v = atan ((1.0*j) / (1.0*i));
      u = u * cos (this_proj -> curves[activer][activec] -> labels_angle[axe] + v);
      ax = ax - u  / PANGO_SCALE;
    }
  }
  cairo_move_to (cr, ax, ay);
  cairo_rotate (cr, this_proj -> curves[activer][activec] -> labels_angle[axe]);
  pango_cairo_update_layout (cr, layout);
  pango_cairo_show_layout (cr, layout);
  cairo_stroke (cr);
  cairo_rotate (cr, -this_proj -> curves[activer][activec] -> labels_angle[axe]);
  pango_cairo_update_layout (cr, layout);
}
