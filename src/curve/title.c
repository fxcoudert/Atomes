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
#include <pango/pangocairo.h>

#include "global.h"
#include "curve.h"

const gchar * default_title (int ax, int c)
{
  if (ax == 0)
  {
    if (activer == GR || activer == GK)
    {
      return ("r [Å]");
    }
    else if (activer == SQ || activer == SK)
    {
      return ("q [Å-1]");
    }
    else if (activer == BD)
    {
      return ("Dij [Å]");
    }
    else if (activer == AN)
    {
      return ("θ [°]");
    }
    else if (activer == RI)
    {
      return ("Size n of the ring [total number of nodes]");
    }
    else if (activer == CH)
    {
      return ("Size n of the chain [total number of nodes]");
    }
    else if (activer == SP)
    {
      return ("Ql");
    }
    else
    {
      return g_strdup_printf ("t [%s]", untime[active_project -> tunit]);
    }
  }
  else
  {
    return active_project -> curves[activer][c] -> name;
  }
}

void show_title (cairo_t * cr, struct project * this_proj, int rid, int cid)
{
  double x, y;

  x = this_proj -> curves[rid][cid] -> title_pos[0] * resol[0];
  y = this_proj -> curves[rid][cid] -> title_pos[1] * resol[1];
  cairo_set_source_rgba (cr, this_proj -> curves[rid][cid] -> title_color.red,
                             this_proj -> curves[rid][cid] -> title_color.green,
                             this_proj -> curves[rid][cid] -> title_color.blue,
                             this_proj -> curves[rid][cid] -> title_color.alpha);
  pango_layout_set_font_description (layout, pango_font_description_from_string (this_proj -> curves[rid][cid] -> title_font));
  pango_layout_set_text (layout, this_proj -> curves[rid][cid] -> title, -1);
  cairo_move_to (cr, x, y);
  pango_cairo_update_layout (cr, layout);
  pango_cairo_show_layout (cr, layout);
  cairo_stroke (cr);
}
