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
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "interface.h"
#include "callbacks.h"
#include "project.h"
#include "cedit.h"
#include "curve.h"

extern void adjust_tool_model (int calc, int curve, gchar * string_path);

gint32 etime;
int resol[2];
double XDRAW, YDRAW;
double DXDRAW, DYDRAW;
double xmax, ymax;
double x_min, x_max;
double y_max, y_min;
double ax, ay;
double cxy[2];
double mticks;
int nticks;
int xlog, ylog;
int dxlog, dylog;
char * sca;
int originp;

PangoLayout * layout;
gboolean dogrid;
int x_shift, y_shift;
int amint, amajt;
int tickpos, labpos;

// Marge sur X et Y dans les graphs
int xmarge = MARGX;
int ymarge = MARGY;
char * curve_image_file = NULL;

// Number of dash formats
int ndash = NDASHS;
int nglyph = NGLYPHS;

const double dashed0[] = {0.0, 0.0};
int len0 = sizeof(dashed0) / sizeof(dashed0[0]);

const double dashed1[] = {1.0, 0.0};
int len1 = sizeof(dashed1) / sizeof(dashed1[0]);

const double dashed2[] = {5.0, 2.5};
int len2 = sizeof(dashed2) / sizeof(dashed2[0]);

const double dashed3[] = {4.0, 6.0};
int len3 = sizeof(dashed3) / sizeof(dashed3[0]);

const double dashed4[] = {2.0, 5.0};
int len4 = sizeof(dashed4) / sizeof(dashed4[0]);

const double dashed5[] = {10.0, 5.0};
int len5 = sizeof(dashed5) / sizeof(dashed5[0]);;

const double dashed6[] = {10.0, 10.0};
int len6 = sizeof(dashed6) / sizeof(dashed6[0]);;

const double dashed7[] = {4.0, 1.0, 4.0};
int len7 = sizeof(dashed7) / sizeof(dashed7[0]);

const double dashed8[] = {10.0, 2.5, 10.0};
int len8 = sizeof(dashed8) / sizeof(dashed8[0]);;

const double dashed9[] = {10.0, 5.0, 2.0};
int len9 = sizeof(dashed9) / sizeof(dashed9[0]);;

const double pdashed[] = {1.0};
int lenp = 1;

thedash * selectdash (int iddash)
{
  thedash * dashtab;
  dashtab = g_malloc0 (sizeof*dashtab);

  if (iddash == 0)
  {
     dashtab -> a = dashed0;
     dashtab -> b = len0;
  }
  if (iddash == 1)
  {
     dashtab -> a = dashed1;
     dashtab -> b = len1;
  }
  if (iddash == 2)
  {
     dashtab -> a = dashed2;
     dashtab -> b = len2;
  }
  if (iddash == 3)
  {
     dashtab -> a = dashed3;
     dashtab -> b = len3;
  }
  if (iddash == 4)
  {
     dashtab -> a = dashed4;
     dashtab -> b = len4;
  }
  if (iddash == 5)
  {
     dashtab -> a = dashed5;
     dashtab -> b = len5;
  }
  if (iddash == 6)
  {
     dashtab -> a = dashed6;
     dashtab -> b = len6;
  }
  if (iddash == 7)
  {
     dashtab -> a = dashed7;
     dashtab -> b = len7;
  }
  if (iddash == 8)
  {
     dashtab -> a = dashed8;
     dashtab -> b = len8;
  }
  if (iddash == 9)
  {
     dashtab -> a = dashed9;
     dashtab -> b = len9;
  }
  if (iddash == 10)
  {
     dashtab -> a = pdashed;
     dashtab -> b = lenp;
  }
  return (dashtab);
}

double scale (double axe)
{
  double xs;
  double axis;

  axis = fabs(axe);

  if (axis < 0.0005)
  {
    xs=0.00005;
  }
  else if (axis < 0.0025)
  {
    xs=0.00025;
  }
  else if (axis < 0.005)
  {
    xs=0.0005;
  }
  else if (axis < 0.025)
  {
    xs=0.0025;
  }
  else if (axis < 0.05)
  {
    xs=0.005;
  }
  else if (axis < 0.25)
  {
    xs=0.025;
  }
  else if (axis < 0.5 )
  {
    xs=0.05;
  }
  else if (axis < 1.5 )
  {
    xs=0.1;
  }
  else if (axis < 2.5)
  {
    xs=0.25;
  }
  else if (axis < 5)
  {
    xs=0.5;
  }
  else if (axis < 15)
  {
    xs = 1.0;
  }
  else if (axis < 30)
  {
    xs = 2.0;
  }
  else if (axis < 50)
  {
    xs = 5.0;
  }
  else if (axis < 100)
  {
    xs = 10.0;
  }
  else if (axis < 250)
  {
    xs = 25.0;
  }
  else if (axis < 500)
  {
    xs = 50.0;
  }
  else if (axis < 1000)
  {
    xs = 100.0;
  }
  else if (axis < 5000)
  {
    xs = 500.0;
  }
  else if (axis < 10000)
  {
    xs = 1000.0;
  }
  else if (axis < 50000)
  {
    xs = 5000.0;
  }
  else if (axis < 100000)
  {
    xs = 10000.0;
  }
  else if (axis < 500000)
  {
    xs = 50000.0;
  }
  else if (axis < 1000000)
  {
    xs = 100000.0;
  }
  return (xs);
}

void prep_plot (struct project * this_proj, int rid, int cid)
{
  x_min = resol[0] * this_proj -> curves[rid][cid] -> frame_pos[0][0];
  x_max = resol[0] * this_proj -> curves[rid][cid] -> frame_pos[0][1];
  y_max = resol[1] * (1.0 - this_proj -> curves[rid][cid] -> frame_pos[1][1]);
  y_max = resol[1] - y_max;
  y_min = resol[1] * (1.0 - this_proj -> curves[rid][cid] -> frame_pos[1][0]);
  y_min = resol[1] - y_min;

  // The x size of the graph in pixels
  XDRAW = x_max - x_min;
// The y size of the graph in pixels
  YDRAW = y_max - y_min;
}

void clean_this_curve_window (int cid, int rid)
{
  /*if (active_project -> curves[rid][cid] -> window != NULL)
  {
    active_project -> curves[rid][cid] -> plot = destroy_this_widget (active_project -> curves[rid][cid] -> plot);
    active_project -> curves[rid][cid] -> window = destroy_this_widget (active_project -> curves[rid][cid] -> window);
  }*/
  if (active_project -> curves[rid][cid] -> ndata > 0)
  {
    int i;
    for (i=0; i<2; i++)
    {
      if (active_project -> curves[rid][cid] -> data[i] != NULL)
      {
        g_free (active_project -> curves[rid][cid] -> data[i]);
        active_project -> curves[rid][cid] -> data[i] = NULL;
      }
    }
  }
  active_project -> curves[rid][cid] -> ndata = 0;
}

void set_curve_data_zero (int rid, int cid, int interv)
{
  active_project -> curves[rid][cid] -> ndata = interv;
  active_project -> curves[rid][cid] -> data[0] = allocdouble (interv);
  int i;
  for (i=0; i<interv; i++)
  {
    active_project -> curves[rid][cid] -> data[0][i] = active_project -> min[rid] + i*active_project -> delta[rid];
  }
}

void save_curve_ (int * interv, double datacurve[* interv], int * cid, int * rid)
{
  int i, j;

#ifdef DEBUG
  /*g_debug ("SAVE_CURVE:: rid= %d, cid= %d, name= %s, interv= %d", * rid, * cid, active_project -> curves[* rid][* cid] -> name, * interv);
  for ( i=0 ; i < *interv ; i++ )
  {
    g_debug ("SAVECURVE:: i= %d, data[i]= %f", i, datacurve[i]);
  }*/
#endif // DEBUG

  clean_this_curve_window (* cid, * rid);
  if (* interv != 0)
  {
    int inter = (* rid == SP) ? * interv/2 + 1: * interv;
    if (* rid == SK)
    {
      active_project -> curves[* rid][* cid] -> ndata = inter;
      active_project -> curves[* rid][* cid] -> data[0] = duplicate_double (inter, xsk);
    }
    else
    {
      set_curve_data_zero (*  rid, * cid, inter);
    }
    if (* rid != SP)
    {
      active_project -> curves[* rid][* cid] -> data[1] = duplicate_double (inter, datacurve);
    }
    else
    {
      active_project -> curves[* rid][* cid] -> data[1] = allocdouble (inter);
      for (i=0; i<inter; i++)
      {
        active_project -> curves[* rid][* cid] -> data[1][i] = datacurve[i*2];
      }
    }
    for (i=0; i<2; i++)
    {
      j = active_project -> curves[* rid][* cid] -> extrac -> extras;
      active_project -> curves[* rid][* cid] -> extrac -> extras = 0;
      autoscale_axis (active_project, * rid, * cid, i);
      active_project -> curves[* rid][* cid] -> extrac -> extras = j;
      active_project -> curves[* rid][* cid] -> majt[i] = scale (active_project -> curves[* rid][* cid] -> axmax[i] - active_project -> curves[* rid][* cid] -> axmin[i]);
      active_project -> curves[* rid][* cid] -> mint[i] = 2;
    }
  }
  else
  {
    active_project -> curves[* rid][* cid] -> ndata = 0;
  }
}

void hide_curves (struct project * this_proj, int c)
{
  int i;

  for ( i = 0 ; i < this_proj -> numc[c] ; i ++ )
  {
    if (this_proj -> curves[c][i])
    {
      if (this_proj -> curves[c][i] -> window)
      {
        if (is_the_widget_visible(this_proj -> curves[c][i] -> window))
        {
          gtk_widget_hide(this_proj -> curves[c][i] -> window);
          adjust_tool_model (c, i, this_proj -> curves[c][i] -> path);
          g_free (this_proj -> curves[c][i] -> path);
          this_proj -> curves[c][i] -> path = NULL;
        }
      }
    }
  }
}

void remove_this_curve_from_extras (int a, int b, int c)
{
  int i, j, k, l;
  struct project * this_proj;
  struct cextra * ctmp;
  for (i=0; i<nprojects; i++)
  {
    if (i != a)
    {
      this_proj = get_project_by_id (i);
      for (j=0; j<NGRAPHS; j++)
      {
        if (this_proj -> idcc[j] != NULL)
        {
          for (k=0; k<this_proj -> numc[j]; k++)
          {
            if (this_proj -> curves[j][k] -> extrac > 0)
            {
              ctmp = this_proj -> curves[j][k] -> extrac -> first;
              for (l=0; l<this_proj -> curves[j][k] -> extrac -> extras; l++)
              {
                if (ctmp -> id.a == a && ctmp -> id.b == b && ctmp -> id.c == c)
                {
                  remove_extra (this_proj -> curves[j][k] -> extrac, ctmp);
                  break;
                }
                if (ctmp -> next != NULL) ctmp = ctmp -> next;
              }
            }
          }
        }
      }
    }
  }
}

void erase_curves (struct project * this_proj, int c)
{
  int i, j;

  for (i=0 ; i<this_proj -> numc[c]; i ++)
  {
    if (this_proj -> curves[c][i])
    {
      remove_this_curve_from_extras (this_proj -> id, c, i);
      for (j=0; j<2; j++)
      {
        if (this_proj -> curves[c][i] -> data[j])
        {
          free (this_proj -> curves[c][i] -> data[j]);
          this_proj -> curves[c][i] -> data[j] = NULL;
        }
      }
      if (this_proj -> curves[c][i] -> name)
      {
        g_free (this_proj -> curves[c][i] -> name);
        this_proj -> curves[c][i] -> name = NULL;
      }
      g_free (this_proj -> curves[c][i]);
      this_proj -> curves[c][i] = NULL;
    }
  }
}

void update_curves ()
{
  int i, j, k;
  struct project * this_proj;
  for (i=0; i<nprojects; i++)
  {
    this_proj = get_project_by_id(i);
    for (j=0; j<NGRAPHS; j++)
    {
      for (k=0; k<this_proj -> numc[j]; k++)
      {
        if (this_proj -> curves[j][k] -> plot != NULL)
        {
          if (is_the_widget_visible(this_proj -> curves[j][k] -> plot))
          {
            gtk_widget_queue_draw (this_proj -> curves[j][k] -> plot);
          }
        }
      }
    }
  }
}

void update_curve (gpointer data)
{
  tint * cd = (tint *)data;
  gtk_widget_queue_draw (get_project_by_id(cd -> a) -> curves[cd -> b][cd -> c] -> plot);
}
