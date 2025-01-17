/* This file is part of Atomes.

Atomes is free software: you can redistribute it and/or modify it under the terms
of the GNU Affero General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Atomes is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with Atomes.
If not, see <https://www.gnu.org/licenses/> */

#include "global.h"
#include "interface.h"
#include "glview.h"
#include "cbuild_edit.h"
#include "bind.h"
#include "project.h"
#include "workspace.h"
#include "readers.h"

extern G_MODULE_EXPORT void show_sg_info (GtkButton * but, gpointer data);

extern void get_origin (space_group * spg);
extern int test_lattice (builder_edition * cbuilder);
extern int build_crystal (gboolean visible, struct project * this_proj, gboolean to_wrap, gboolean show_clones, cell_info * cell, GtkWidget * widg);

gchar * crystal_sytems[7] = {"Triclinic", "Monoclinic", "Othorhombic", "Tetragonal", "Trigonal", "Hexagonal", "Cubic"};
gchar * bravais_keys[7][4] = {{"Primitive", NULL, NULL, NULL},
                              {"Primitive", "Base-centered", NULL, NULL},
                              {"Primitive", "Base-centered", "Body-centered", "Face-centered"},
                              {"Primitive", "Body-centered", NULL, NULL},
                              {"Hexagonal axes", "Rhombohedral axes", NULL, NULL},
                              {"Hexagonal", NULL, NULL, NULL},
                              {"Primitive", "Body-centered", "Face-centered", NULL}};

gchar * latt_info[7]={"<i>a</i> &#x2260; <i>b</i> &#x2260; <i>c</i>\n&#x3B1; &#x2260; &#x3B2; &#x2260; &#x263;",                                                 // Triclinic
                      "<i>a</i> &#x2260; <i>b</i> &#x2260; <i>c</i>\n&#x3B1; = &#x3B2; = 90° &#x2260; &#x263;\n\tor\n&#x3B1; = &#x263; = 90° &#x2260; &#x3B2;",  // Monoclinic
                      "<i>a</i> &#x2260; <i>b</i> &#x2260; <i>c</i>\n&#x3B1; = &#x3B2; = &#x263; = 90°",                                                         // Orthorhombic
                      "<i>a</i> = <i>b</i> &#x2260; <i>c</i>\n&#x3B1; = &#x3B2; = &#x263; = 90°",                                                                // Tetragonal
                      "<i>a</i> = <i>b</i> = <i>c</i>\n&#x3B1; = &#x3B2; = &#x3B3; &#x2260; 90°",                                                                // Trigonal R
                      "<i>a</i> = <i>b</i> &#x2260; <i>c</i>\n&#x3B1; = &#x3B2; = 90° and &#x263; = 120°",                                                       // Hexagonal
                      "<i>a</i> = <i>b</i> = <i>c</i>\n&#x3B1; = &#x3B2; = &#x263; = 90°"};                                                                      // Cubic
int nsg_bv[7]={2, 13, 59, 68,  25,  27,  36};
int min_bv[7]={0,  2, 15, 74, 142, 167, 194};

int get_bravais_id (int spg)
{
  if (spg < 3)
  {
    return 0;
  }
  else if (spg < 16)
  {
    return 1;
  }
  else if (spg < 75)
  {
    return 2;
  }
  else if (spg < 143)
  {
    return 3;
  }
  else if (spg < 168)
  {
    if (groups[spg-1][0] == 'R')
    {
      return 4;
    }
    else
    {
      return 5;
    }
  }
  else if (spg < 195)
  {
    return 5;
  }
  else
  {
    return 6;
  }
}

int get_bravais_img_id (int spg)
{
  if (spg < 3)
  {
    return 0;
  }
  else if (spg < 16)
  {
    if (groups[spg-1][0] == 'P')
    {
      return 1;
    }
    else
    {
      return 2;
    }
  }
  else if (spg < 75)
  {
    if (groups[spg-1][0] == 'P')
    {
      return 3;
    }
    else if (groups[spg-1][0] == 'I')
    {
      return 4;
    }
    else if (groups[spg-1][0] == 'A' || groups[spg-1][0] == 'C')
    {
      return 5;
    }
    else
    {
      return 6;
    }
  }
  else if (spg < 143)
  {
    if (groups[spg-1][0] == 'P')
    {
      return 7;
    }
    else
    {
      return 8;
    }
  }
  else if (spg < 168)
  {
    if (groups[spg-1][0] == 'R')
    {
      return 9;
    }
    else
    {
      return 10;
    }
  }
  else if (spg < 195)
  {
    return 10;
  }
  else
  {
    if (groups[spg-1][0] == 'P')
    {
      return 11;
    }
    else if (groups[spg-1][0] == 'I')
    {
      return 12;
    }
    else
    {
      return 13;
    }
  }
}

gchar * frac_string (gchar * init)
{
 gchar * end = substitute_string (init, "1/2", "&#189;");
 end = substitute_string (end, "1/4", "&#188;");
 end = substitute_string (end, "3/4", "&#190;");
 end = substitute_string (end, "1/3", "&#8531;");
 end = substitute_string (end, "2/3", "&#8532;");
 end = substitute_string (end, "1/6", "&#8537;");
 end = substitute_string (end, "5/6", "&#8538;");
 end = substitute_string (end, "1/8", "&#8539;");
 end = substitute_string (end, "3/8", "&#8540;");
 end = substitute_string (end, "5/8", "&#8541;");
 end = substitute_string (end, "7/8", "&#8542;");
 return g_strdup_printf ("%s", end);
}

gchar * get_num_string (gchar * str)
{
  str = replace_markup (str, "65", "6<sub>5</sub>");
  str = replace_markup (str, "64", "6<sub>4</sub>");
  str = replace_markup (str, "63", "6<sub>3</sub>");
  str = replace_markup (str, "62", "6<sub>2</sub>");
  str = replace_markup (str, "61", "6<sub>1</sub>");
  str = replace_markup (str, "43", "4<sub>3</sub>");
  str = replace_markup (str, "42", "4<sub>2</sub>");
  str = replace_markup (str, "41", "4<sub>1</sub>");
  str = replace_markup (str, "32", "3<sub>2</sub>");
  str = replace_markup (str, "31", "3<sub>1</sub>");
  return substitute_string (str, "21", "2<sub>1</sub>");
}

gchar * get_so_string (space_group * spg, int id)
{
  gchar * str = replace_markup (spg -> settings[id].name, "s", "/");
  str = get_num_string (str);
  str = substitute_string (str, "_", NULL);
  if (spg -> settings[id].origin)
  {
    str = g_strdup_printf ("%s (%d)", str, spg -> settings[id].origin);
  }
  return str;
}

GtkTreeModel * so_combo_tree (space_group * spg)
{
  GtkTreeIter iter;
  GtkTreeStore * store;
  int i;
  store = gtk_tree_store_new (1, G_TYPE_STRING);
  if (! spg) return GTK_TREE_MODEL (store);
  if (spg -> nums > 1)
  {
    for (i=0; i<spg -> nums; i++)
    {
      gtk_tree_store_append (store, &iter, NULL);
      gtk_tree_store_set (store, & iter, 0, get_so_string (spg, i), -1);
    }
  }
  else
  {
    gtk_tree_store_append (store, &iter, NULL);
    gtk_tree_store_set (store, & iter, 0, groups[spg -> id-1], -1);
  }
  return GTK_TREE_MODEL (store);
}

GtkTreeModel * sg_combo_tree (int csid, int bvid)
{
  GtkTreeIter iter;
  GtkTreeStore * store;
  int i;
  gboolean doit;
  gchar * str;
  store = gtk_tree_store_new (1, G_TYPE_STRING);
  for (i=min_bv[csid]; i<min_bv[csid]+nsg_bv[csid]; i++)
  {
    doit = FALSE;
    if (! bvid || csid == 5)
    {
      doit = TRUE;
    }
    else
    {
      if (csid == 1 || csid == 3)
      {
        if ((bvid == 1 && groups[i][0] == 'P') || (bvid == 2 && groups[i][0] != 'P')) doit = TRUE;
      }
      else if (csid == 2)
      {
        if ((bvid == 1 && groups[i][0] == 'P') || (bvid == 2 && groups[i][0] == 'A') || (bvid == 2 && groups[i][0] == 'C') ||
            (bvid == 3 && groups[i][0] == 'I') || (bvid == 4 && groups[i][0] == 'F'))
        {
          doit = TRUE;
        }
      }
      else if (csid == 4)
      {
        if ((bvid == 1 && groups[i][0] == 'P') || (bvid == 2 && groups[i][0] == 'R')) doit = TRUE;
      }
      else if (csid == 6)
      {
        if ((bvid == 1 && groups[i][0] == 'P') || (bvid == 2 && groups[i][0] == 'I') || (bvid == 3 && groups[i][0] == 'F'))
        {
          doit = TRUE;
        }
      }
    }
    if (doit)
    {
      gtk_tree_store_append (store, &iter, NULL);
      str = g_strdup_printf ("%3d: %s", i+1, groups[i]);
      gtk_tree_store_set (store, & iter, 0, str, -1);
      g_free (str);
    }
  }
  return GTK_TREE_MODEL (store);
}

GtkTreeModel * la_combo_tree ()
{
  GtkTreeIter iter;
  GtkTreeStore *store;
  gchar * lat[2]={"<b><i>a</i></b>, <b><i>b</i></b>, <b><i>c</i></b>, &#x3B1; &#x3B2; &#x263;", "Vectors"};
  int i;
  store = gtk_tree_store_new (1, G_TYPE_STRING);
  for (i=0; i<2; i++)
  {
    gtk_tree_store_append (store, &iter, NULL);
    gtk_tree_store_set (store, & iter, 0, lat[i], -1);
  }
  return GTK_TREE_MODEL (store);
}

int get_sg_num (GtkComboBox * box)
{
  GValue val = {0, };
  GtkTreeModel * cmodel = gtk_combo_box_get_model (box);
  GtkTreeIter iter;
  gchar * str = NULL;
  gchar * num = NULL;
  int i = 0;
  if (gtk_combo_box_get_active_iter (box, & iter))
  {
    gtk_tree_model_get_value (cmodel, & iter, 0, & val);
    str = g_strdup_printf ("%s", (char *)g_value_get_string (& val));
    if (str)
    {
      num = g_strdup_printf ("%c%c%c", str[0], str[1], str[2]);
      i = (int) atof (num);
      g_free (str);
      g_free (num);
    }
  }
  return i;
}

void adjust_lattice_parameters (builder_edition * cbuilder)
{
  int i, j, k;
  j = get_sg_num(GTK_COMBO_BOX(cbuilder -> sg_combo));
  k = get_bravais_id(j);
  box_info * box = & cbuilder -> cell.box[0];
  switch (k)
  {
    case 1:
      box -> param[1][0] = 90.0;
      box -> vect[1][1] = box -> vect[2][2] = box -> vect[0][0];
      box -> vect[0][1] = box -> vect[0][2] = 0.0;
      break;
    case 2:
      box -> param[1][0] = box -> param[1][1] = box -> param[1][2] = 90.0;
      box -> vect[0][1] = box -> vect[0][2] = 0.0;
      box -> vect[1][0] = box -> vect[1][2] = 0.0;
      box -> vect[2][0] = box -> vect[2][1] = 0.0;
      break;
    case 3:
      box -> param[0][1] = box -> param[0][0];
      box -> param[1][0] = box -> param[1][1] = box -> param[1][2] = 90.0;
      break;
    case 4:
      box -> param[0][1] = box -> param[0][0];
      if (cbuilder -> cell.sp_group -> name[0] == 'R')
      {
        box -> param[1][0] = box -> param[1][1] = box -> param[1][2] = 0.0;
      }
      else
      {
        box -> param[1][0] = box -> param[1][1] = box -> param[1][2] = 90.0;
      }
      box -> vect[1][1] = box -> vect[0][0];
      box -> vect[0][1] = box -> vect[0][2] = 0.0;
      box -> vect[1][0] = box -> vect[1][2] = 0.0;
      box -> vect[2][0] = box -> vect[2][1] = 0.0;
      break;
    case 5:
      box -> param[0][1] = box -> param[0][0];
      box -> param[1][0] = box -> param[1][1] = 90.0;
      box -> param[1][2] = 120.0;
      box -> vect[0][1] = box -> vect[0][2] = 0.0;
      box -> vect[1][0] = box -> vect[1][2] = 0.0;
      break;
    case 6:
      box -> param[0][1] = box -> param[0][2] = box -> param[0][0];
      box -> param[1][0] = box -> param[1][1] = box -> param[1][2] = 90.0;
      box -> vect[1][1] = box -> vect[2][2] = box -> vect[0][0];
      box -> vect[0][1] = box -> vect[0][2] = 0.0;
      box -> vect[1][0] = box -> vect[1][2] = 0.0;
      box -> vect[2][0] = box -> vect[2][1] = 0.0;
      break;
    default:
      break;
  }
  if (cbuilder -> cell.ltype)
  {
    for (i=0; i<3; i++)
    {
      for (j=0; j<3; j++)
      {
        if (cbuilder -> ventry[i][j])
        {
          if (GTK_IS_WIDGET(cbuilder -> ventry[i][j]))
          {
            update_entry_double (GTK_ENTRY(cbuilder -> ventry[i][j]), box -> vect[i][j]);
          }
        }
      }
    }
  }
  else
  {
    for (i=0; i<2; i++)
    {
      for (j=0; j<3; j++)
      {
        if (cbuilder -> bentry[i][j])
        {
          if (GTK_IS_WIDGET(cbuilder -> bentry[i][j]))
          {
            update_entry_double (GTK_ENTRY(cbuilder -> bentry[i][j]), box -> param[i][j]);
          }
        }
      }
    }
    if (cbuilder -> bentry[0][1])
    {
      if (GTK_IS_WIDGET(cbuilder -> bentry[0][1]))
      {
        widget_set_sensitive (cbuilder -> bentry[0][1], (k > 2) ? 0 : 1);
        if (k > 2) update_entry_text (GTK_ENTRY(cbuilder -> bentry[0][1]), "");
      }
    }
    if (cbuilder -> bentry[0][2])
    {
      if (GTK_IS_WIDGET(cbuilder -> bentry[0][2]))
      {
        widget_set_sensitive (cbuilder -> bentry[0][2], (k > 5 || (k == 4 && cbuilder -> cell.sp_group -> name[0] == 'R')) ? 0 : 1);
        if (k > 5 || (k == 4 && cbuilder -> cell.sp_group -> name[0] == 'R')) update_entry_text (GTK_ENTRY(cbuilder -> bentry[0][2]), "");
      }
    }
    for (i=0; i<3; i++)
    {
      if (cbuilder -> bentry[1][i])
      {
        if (GTK_IS_WIDGET(cbuilder -> bentry[1][i]))
        {
          widget_set_sensitive (cbuilder -> bentry[1][i], (k > 1) || (k == 1 && ! i) ? 0 : 1);
          if (k == 4 && cbuilder -> cell.sp_group -> name[0] == 'R')
          {
            if (! i)
            {
              widget_set_sensitive (cbuilder -> bentry[1][i], 1);
            }
            else
            {
              update_entry_text (GTK_ENTRY(cbuilder -> bentry[1][i]), "");
            }
          }
        }
      }
    }
  }
}

G_MODULE_EXPORT void update_cb_vect (GtkEntry * entry, gpointer data)
{
  tint * id = (tint *)data;
  const gchar * m = entry_get_text (entry);
  double v = atof(m);
  get_project_by_id(id -> a) -> modelgl -> builder_win -> cell.box[0].vect[id -> b][id -> c] = v;
  update_entry_double (entry, v);
}

G_MODULE_EXPORT void update_cb_box (GtkEntry * entry, gpointer data)
{
  tint * id = (tint *)data;
  const gchar * m = entry_get_text (entry);
  double v = atof(m);
  if (v >= 0.0)
  {
    get_project_by_id(id -> a) -> modelgl -> builder_win -> cell.box[0].param[id -> b][id -> c] = v;
  }
  update_entry_double (entry, v);
}

GtkWidget * prepare_lattice_grid (int type, builder_edition * cbuilder, glwin * glview)
{
  GtkWidget * table = gtk_grid_new ();
  int i, j, k;
  k = 0;
  for (i=0; i<3; i++)
  {
    for (j=0; j<3; j++, k++)
    {
      if (i < 2) cbuilder -> cell.box[0].param[i][j] = 0.0;
      cbuilder -> cell.box[0].vect[i][j] = 0.0;
      t_box[k].a = i;
      t_box[k].b = j;
    }
  }
  cbuilder -> cell.ltype = type;
  if (cbuilder -> cell.ltype)
  {
    k = 0;
    for (i=0; i<4; i++)
    {
      if (i > 0)
      {
        gtk_grid_attach (GTK_GRID (table), markup_label(vect_name[i-1], 20, -1, 0.0, 0.5), 0, i, 1, 1);
      }
      for (j=0; j<4; j++)
      {
        if (j > 0)
        {
          if (i == 0)
          {
            gtk_grid_attach (GTK_GRID (table), markup_label(vect_comp[j-1], -1, 20, 0.5, 0.5), j, 0, 1, 1);
          }
          else
          {
            cbuilder -> ventry[i-1][j-1] = create_entry (G_CALLBACK(update_cb_vect), 100, 15, FALSE, & cbuilder -> pointers[i][j]);
            update_entry_double (GTK_ENTRY(cbuilder -> ventry[i-1][j-1]), cbuilder -> cell.box[0].vect[i-1][j-1]);
            gtk_grid_attach (GTK_GRID (table), cbuilder -> ventry[i-1][j-1], j, i, 1, 1);
            k ++;
          }
        }
      }
    }
  }
  else
  {
    k = 0;
    for (i=0; i<2; i++)
    {
      //gtk_grid_attach (GTK_GRID (table), markup_label(box_p[i], -1, 50, 0.5, 0.5), 1, i+2*i, 1, 1);
      gtk_grid_attach (GTK_GRID (table), markup_label(" ", 20, -1, 0.5, 0.5), 0, i, 1, 1);
      for (j=0; j<3; j++, k++)
      {
        gtk_grid_attach (GTK_GRID (table), markup_label(box_prop[i][j], -1, -1, 0.5, 0.5), j+1, i+2*i, 1, 1);
        cbuilder -> bentry[i][j] = create_entry (G_CALLBACK(update_cb_box), 100, 15, FALSE, & cbuilder -> pointers[i][j]);
        update_entry_double (GTK_ENTRY(cbuilder -> bentry[i][j]), cbuilder -> cell.box[0].param[i][j]);
        gtk_grid_attach (GTK_GRID (table), cbuilder -> bentry[i][j], j+1, i+2*i+1, 1, 1);
      }
    }
    adjust_lattice_parameters (cbuilder);
  }
  return table;
}

G_MODULE_EXPORT void set_lattice (GtkWidget * widg, gpointer data)
{
  tint * id = (tint *) data;
  glwin * view = get_project_by_id(id -> a) -> modelgl;
  builder_edition * cbuilder = view -> builder_win;
  cbuilder -> lattice_grid = destroy_this_widget (cbuilder -> lattice_grid);
  cbuilder -> lattice_grid = prepare_lattice_grid (gtk_combo_box_get_active (GTK_COMBO_BOX(widg)), cbuilder, view);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, cbuilder -> lattice_box, cbuilder -> lattice_grid, FALSE, FALSE, 5);
  show_the_widgets (cbuilder -> lattice_grid);
}

G_MODULE_EXPORT void set_so (GtkComboBox * box, gpointer data)
{
  tint * id = (tint *) data;
  builder_edition * cbuilder = get_project_by_id(id -> a) -> modelgl -> builder_win;
  int i = gtk_combo_box_get_active(box);
  gchar * str = g_strdup_printf ("(%s,%s,%s)", cbuilder -> cell.sp_group -> settings[i].pos[0],
                                               cbuilder -> cell.sp_group -> settings[i].pos[1],
                                               cbuilder -> cell.sp_group -> settings[i].pos[2]);
  cbuilder -> so_info = destroy_this_widget(cbuilder -> so_info);
  cbuilder -> so_info = markup_label(frac_string(str), -1, -1, 0.5, 0.5);
  g_free (str);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> so_box, cbuilder -> so_info, FALSE, FALSE, 10);
  gtk_widget_show (cbuilder -> so_info);
  cbuilder -> cell.sp_group -> sid = i;
  get_origin (cbuilder -> cell.sp_group);
}

GtkWidget * sg_info (int sg, gpointer data)
{
  gchar * str = g_strdup_printf ("%s group info", groups[sg]);
  GtkWidget * mlab = markup_label(str, -1, -1, 0.5, 0.5);
  GtkWidget * mbut = create_button (NULL, IMG_NONE, NULL, 50, -1, GTK_RELIEF_NORMAL, G_CALLBACK(show_sg_info), data);
  add_container_child (CONTAINER_BUT, mbut, mlab);
  show_the_widgets (mbut);
  return mbut;
}

int read_space_group (builder_edition * cbuilder, int spg)
{
  // Read file
  gchar * sgfile = substitute_string(groups[spg], "<sub>", NULL);
  sgfile = substitute_string(sgfile, "</sub>", NULL);
  sgfile = substitute_string(sgfile, "/", "s");
#ifdef G_OS_WIN32
  sgfile  = g_strdup_printf ("%s\\space_groups\\%d-%s.sgl", PACKAGE_LIB_DIR, spg+1, sgfile);
#else
  sgfile = g_strdup_printf ("%s/space_groups/%d-%s.sgl", PACKAGE_LIB_DIR, spg+1, sgfile);
#endif
  int res;
  if (cbuilder)
  {
    cbuilder -> cell.sp_group = read_sg_xml_file (sgfile);
    res = (cbuilder -> cell.sp_group) ? 1 : 0;
  }
  else if (this_reader)
  {
    this_reader -> lattice.sp_group = read_sg_xml_file (sgfile);
    res = (this_reader -> lattice.sp_group) ? 1 : 0;
  }
  g_free (sgfile);
  return res;
}

int get_group_id_from_name (gchar * sg_name)
{
  int i;
  for (i=0; i<230; i++)
  {
    if (g_strcmp0(groups[i], sg_name) == 0) return i+1;
  }
  return 0;
}

void adjust_so_combo (builder_edition * cbuilder)
{
  GtkTreeModel * model = so_combo_tree (cbuilder -> cell.sp_group);
  gtk_combo_box_set_model (GTK_COMBO_BOX(cbuilder -> so_combo), model);
  g_object_unref (model);
  gtk_combo_box_set_active (GTK_COMBO_BOX(cbuilder -> so_combo), 0);

  GList * cell_list = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(cbuilder -> so_combo));
  if (cell_list && cell_list -> data)
  {
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(cbuilder -> so_combo), cell_list -> data, "markup", 0, NULL);
  }
  widget_set_sensitive (cbuilder -> so_combo, cbuilder -> cell.sp_group -> nums-1);
  if (cbuilder -> cell.sp_group) get_origin (cbuilder -> cell.sp_group);
}

void adjust_bv_img (builder_edition * cbuilder)
{
  if (cbuilder -> bv_img) cbuilder -> bv_img = destroy_this_widget(cbuilder -> bv_img);
  cbuilder -> bv_img = gtk_image_new_from_file (bravais_img[get_bravais_img_id(cbuilder -> cell.sp_group -> id)]);
#ifdef GTK4
  gtk_widget_set_size_request (cbuilder -> bv_img, 200, 200);
  gtk_widget_set_hexpand (cbuilder -> bv_img, TRUE);
  gtk_widget_set_vexpand (cbuilder -> bv_img, TRUE);
#endif
  add_box_child_start (GTK_ORIENTATION_VERTICAL, cbuilder -> bv_box[1], cbuilder -> bv_img, FALSE, FALSE, 5);
}

void adjust_lattice_constraints (builder_edition * cbuilder)
{
  if (cbuilder -> ltc_cons) cbuilder -> ltc_cons = destroy_this_widget(cbuilder -> ltc_cons);
  gchar * str = g_strdup_printf ("<b>%s</b>", latt_info[get_bravais_id (cbuilder -> cell.sp_group -> id)]);
  cbuilder -> ltc_cons = markup_label(str, 150, -1, 0.0, 0.5);
  g_free (str);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> ltc_box, cbuilder -> ltc_cons, FALSE, FALSE, 5);
}

G_MODULE_EXPORT void set_sg (GtkComboBox * box, gpointer data)
{
  tint * id = (tint *) data;
  builder_edition * cbuilder = get_project_by_id(id -> a) -> modelgl -> builder_win;
  int i = get_sg_num(box);
  if (i)
  {
    cbuilder -> sg_but = destroy_this_widget(cbuilder -> sg_but);
    if (read_space_group (cbuilder, i-1))
    {
      cbuilder -> sg_but = sg_info (i-1, data);
      add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> sg_box, cbuilder -> sg_but, FALSE, FALSE, 10);
      if (cbuilder -> bv_box[0]) adjust_lattice_constraints (cbuilder);
      if (cbuilder -> bv_box[1]) adjust_bv_img (cbuilder);
      show_the_widgets (cbuilder -> bh_box);
      if (cbuilder -> so_combo) adjust_so_combo (cbuilder);
      if (cbuilder -> lattice_grid) adjust_lattice_parameters (cbuilder);
    }
    else
    {
      // Error reading file
      show_error ("Cannot read space group *.sgl file ?!\nPlease check the program library !", 0, cbuilder -> win);
    }
  }
}

void adjust_sg_combo (builder_edition * cbuilder, int cs, int bl)
{
  GtkTreeModel * model = sg_combo_tree (cs, bl);
  gtk_combo_box_set_model (GTK_COMBO_BOX(cbuilder -> sg_combo), model);
  g_object_unref (model);
  gtk_combo_box_set_active (GTK_COMBO_BOX(cbuilder -> sg_combo), 0);
  GList * cell_list = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(cbuilder -> sg_combo));
  if (cell_list && cell_list -> data)
  {
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(cbuilder -> sg_combo), cell_list -> data, "markup", 0, NULL);
  }
  adjust_lattice_parameters (cbuilder);
}

G_MODULE_EXPORT void set_bl (GtkComboBox * combo, gpointer data)
{
  builder_edition * cbuilder = (builder_edition *)data;
  int i, j;
  i = gtk_combo_box_get_active (GTK_COMBO_BOX(cbuilder -> cs_combo));
  j = gtk_combo_box_get_active (combo);
  adjust_sg_combo (cbuilder, i, j);
}

GtkWidget * create_bl_combo (int cs, gpointer data)
{
  GtkWidget * cbox = create_combo();
  int i;
  combo_text_append (cbox, "All");
  for (i=0; i<4; i++)
  {
    if (bravais_keys[cs][i]) combo_text_append (cbox, bravais_keys[cs][i]);
  }
  i = (cs == 0 || cs == 5) ? 0 : 1;
  widget_set_sensitive (cbox, i);
  gtk_combo_box_set_active (GTK_COMBO_BOX(cbox), ! i);
  g_signal_connect (G_OBJECT(cbox), "changed", G_CALLBACK(set_bl), data);
  gtk_widget_set_size_request (cbox, 150, 25);
  return cbox;
}

G_MODULE_EXPORT void set_cs (GtkComboBox * combo, gpointer data)
{
  builder_edition * cbuilder = (builder_edition *)data;
  int i = gtk_combo_box_get_active (combo);
  cbuilder -> bl_combo = destroy_this_widget(cbuilder -> bl_combo);
  cbuilder -> bl_combo = create_bl_combo (i, data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> bl_box, cbuilder -> bl_combo, FALSE, FALSE, 0);
  adjust_sg_combo (cbuilder, i, 0);
  gtk_widget_show (cbuilder -> bl_combo);
}

#ifdef GTK4
G_MODULE_EXPORT void set_wr (GtkCheckButton * but, gpointer data)
#else
G_MODULE_EXPORT void set_wr (GtkToggleButton * but, gpointer data)
#endif
{
  builder_edition * cbuilder = (builder_edition * )data;
#ifdef GTK4
  cbuilder -> wrap = gtk_check_button_get_active (but);
#else
  cbuilder -> wrap = gtk_toggle_button_get_active (but);
#endif
}

#ifdef GTK4
G_MODULE_EXPORT void set_shc (GtkCheckButton * but, gpointer data)
#else
G_MODULE_EXPORT void set_shc (GtkToggleButton * but, gpointer data)
#endif
{
  builder_edition * cbuilder = (builder_edition * )data;
#ifdef GTK4
  cbuilder -> clones = gtk_check_button_get_active (but);
#else
  cbuilder -> clones = gtk_toggle_button_get_active (but);
#endif
}

G_MODULE_EXPORT void add_cryst_cell (GtkSpinButton * res, gpointer data)
{
  tint * bid = (tint *)data;
  builder_edition * cbuilder = get_project_by_id(bid -> a) -> modelgl -> builder_win;
  cbuilder -> cell.cextra[bid -> b] = gtk_spin_button_get_value_as_int(res);
}

G_MODULE_EXPORT void apply_build (GtkButton * but, gpointer data)
{
  int id = GPOINTER_TO_INT(data);
  struct project * this_proj = get_project_by_id(id);
  if (test_lattice(this_proj -> modelgl -> builder_win))
  {
    build_crystal (TRUE, this_proj, this_proj -> modelgl -> builder_win -> wrap, this_proj -> modelgl -> builder_win -> clones,
                                  & this_proj -> modelgl -> builder_win -> cell, this_proj -> modelgl -> builder_win -> win);
  }
}

G_MODULE_EXPORT void close_build (GtkButton * but, gpointer data)
{
  int id = GPOINTER_TO_INT(data);
  struct project * this_proj = get_project_by_id(id);
  this_proj -> modelgl -> search_widg[7] = free_this_search_data (this_proj -> modelgl -> search_widg[7]);
  this_proj -> modelgl -> builder_win -> win = destroy_this_widget(this_proj -> modelgl -> builder_win -> win);
  g_free (this_proj -> modelgl -> builder_win);
  this_proj -> modelgl -> builder_win = NULL;
}

#ifdef GTK4
G_MODULE_EXPORT gboolean delete_build (GtkWindow * widg, gpointer data)
#else
G_MODULE_EXPORT gboolean delete_build (GtkWidget * widg, GdkEvent * event, gpointer data)
#endif
{
  close_build (NULL, data);
  return TRUE;
}

dint occp[5];

#ifdef GTK4
G_MODULE_EXPORT void toggle_occ (GtkCheckButton * but, gpointer data)
#else
G_MODULE_EXPORT void toggle_occ (GtkToggleButton * but, gpointer data)
#endif
{
  dint * cid = (dint *)data;
  get_project_by_id(cid -> a) -> modelgl -> builder_win -> occupancy = cid -> b;
}

#ifdef GTK4
G_MODULE_EXPORT void toggle_overlap (GtkCheckButton * Button, gpointer data)
#else
G_MODULE_EXPORT void toggle_overlap (GtkToggleButton * Button, gpointer data)
#endif
{
  builder_edition * cbuilder = (builder_edition * )data;
#ifdef GTK4
  cbuilder -> overlapping = gtk_check_button_get_active (Button);
#else
  cbuilder -> overlapping = gtk_toggle_button_get_active (Button);
#endif
}

G_MODULE_EXPORT void adjust_occupancy (GtkButton * but, gpointer data)
{
  struct project * this_proj = (struct project *)data;
  builder_edition * cbuilder = this_proj -> modelgl -> builder_win;
  GtkWidget * info = dialogmodal ("Occupancy set-up", GTK_WINDOW(cbuilder -> win));
  GtkWidget * vbox, * hbox;
  vbox = dialog_get_content_area (info);
  gchar * boccup[5] = {"<b>Random for the initial cell only</b>",
                       "<b>Random cell by cell</b>",
                       "<b>Completely random</b>",
                       "<b>Successively</b>",
                       "<b>Alternatively</b>"};
  gchar * occup[5] = {"<i>Sites are filled randomly in the initial cell only,\n"
                      "   then the initial cell is simply replicated.</i>",
                      "<i>Sites are filled randomly for each cell, cell by cell separately.</i>",
                      "<i>Sites are filled randomly for the entire network,\n"
                      "   the final crystal is considered as a whole.</i>",
                      "<i>Sites are filled successively: all object(s) A, then all object(s) B ... </i>",
                      "<i>Sites are filled alternatively: object A, object B, object A ...</i>"};
  GtkWidget * occ_but[5];
  int i;
  for (i=0; i<5; i++)
  {
    hbox = create_hbox(0);
    occp[i].a = this_proj -> id;
    occp[i].b = i;
    add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, hbox, FALSE, FALSE, 0);
#ifdef GTK4
    if (!i)
    {
      occ_but[i] = check_button (boccup[i], -1, 25, FALSE, G_CALLBACK(toggle_occ), & occp[i]);
    }
    else
    {
      occ_but[i] = check_button (boccup[i], -1, 25, FALSE, G_CALLBACK(toggle_occ), & occp[i]);
      gtk_check_button_set_group ((GtkCheckButton *)occ_but[i], (GtkCheckButton *)occ_but[0]);
    }
#else
    if (!i)
    {
      occ_but[i] = radio_button (boccup[i], -1, 25, FALSE, G_CALLBACK(toggle_occ), & occp[i]);
    }
    else
    {
      occ_but[i] = gtk_radio_button_new_from_widget (GTK_RADIO_BUTTON(occ_but[0]));
      add_container_child (CONTAINER_BUT, occ_but[i], markup_label(boccup[i], -1, 25, 0.0, 0.5));
    }
#endif
    if (i) g_signal_connect (G_OBJECT(occ_but[i]), "toggled", G_CALLBACK(toggle_occ), & occp[i]);
#ifdef GTK4
    gtk_button_set_has_frame (GTK_BUTTON(occ_but[i]), FALSE);
#endif
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, occ_but[i], FALSE, FALSE, 5);
    hbox = create_hbox(0);
    add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, hbox, FALSE, FALSE, 0);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, markup_label (occup[i], 200, -1, 0.5, 0.5), FALSE, FALSE, 50);
  }
  i = cbuilder -> occupancy;
#ifdef GTK4
  gtk_check_button_set_active (GTK_CHECK_BUTTON(occ_but[i]), TRUE);
#else
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(occ_but[i]), TRUE);
#endif
  hbox = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, hbox, FALSE, FALSE, 5);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, check_button ("<b>Allow overlapping</b>", -1, 25, cbuilder -> overlapping, G_CALLBACK(toggle_overlap), (gpointer)cbuilder), FALSE, FALSE, 0);
  hbox = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, hbox, FALSE, FALSE, 0);
  gchar * overlap = "<i>Instead of ensuring that sites are filled by a single object,\n"
                    "this allows object(s) to share the same crystalline position. \n"
                    "The option above describes how filled and empty positions alternate.</i>";
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, markup_label (overlap, 200, -1, 0.5, 0.5), FALSE, FALSE, 50);

  show_the_widgets (info);
  run_this_gtk_dialog (info, G_CALLBACK(run_destroy_dialog), NULL);
}

GtkWidget * builder_win (struct project * this_proj, gpointer data)
{
  int  i, j;
  gchar * str = (! this_proj -> natomes) ? g_strdup_printf ("Crystal builder - %s", this_proj -> name) : g_strdup_printf ("Crystal builder");
  builder_edition * cbuilder = this_proj -> modelgl -> builder_win;
  GtkWidget * win = create_win (str, this_proj -> modelgl -> win, FALSE, FALSE);
  g_free (str);
  GtkWidget * vbox = create_vbox (BSEP);
  add_container_child (CONTAINER_WIN, win, vbox);

  cbuilder -> bh_box = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, cbuilder -> bh_box, FALSE, FALSE, 0);
  for (i=0; i<2; i++)
  {
    cbuilder -> bv_box[i] = create_vbox (BSEP);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> bh_box, cbuilder -> bv_box[i], FALSE, FALSE, 0);
  }

  // Crystal system
  GtkWidget * hbox = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, cbuilder -> bv_box[0], hbox, FALSE, FALSE, 0);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, markup_label("Crystal system:", 150, -1, 0.0, 0.5), FALSE, FALSE, 5);
  cbuilder -> cs_combo = create_combo();

  for (i=0; i<7;i++) combo_text_append (cbuilder -> cs_combo, crystal_sytems[i]);
  gtk_combo_box_set_active (GTK_COMBO_BOX(cbuilder -> cs_combo), 0);
  gtk_widget_set_size_request (cbuilder -> cs_combo, 150, 25);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, cbuilder -> cs_combo, FALSE, FALSE, 0);
  g_signal_connect (G_OBJECT(cbuilder -> cs_combo), "changed", G_CALLBACK(set_cs), (gpointer)cbuilder);

  // Bravais lattice
  cbuilder -> bl_box = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, cbuilder -> bv_box[0], cbuilder -> bl_box, FALSE, FALSE, 0);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> bl_box, markup_label("Bravais lattice:", 150, -1, 0.0, 0.5), FALSE, FALSE, 5);
  cbuilder -> bl_combo = create_bl_combo (0, (gpointer)cbuilder);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> bl_box, cbuilder -> bl_combo, FALSE, FALSE, 0);

  cbuilder -> ltc_box = create_hbox (0);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> ltc_box, markup_label("Lattice constraints: ", 150, -1, 0.0, 0.5), FALSE, FALSE, 5);
  str = g_strdup_printf ("<b>%s</b>", latt_info[get_bravais_id (1)]);
  cbuilder -> ltc_cons = markup_label(str, 150, -1, 0.0, 0.5);
  g_free (str);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> ltc_box, cbuilder -> ltc_cons, FALSE, FALSE, 5);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, cbuilder -> bv_box[0], cbuilder -> ltc_box, FALSE, FALSE, 5);

  // cbuilder -> bv_img = gtk_image_new_from_file (bravais_img[get_bravais_img_id(1)]);
  // add_box_child_start (GTK_ORIENTATION_VERTICAL, cbuilder -> bv_box[1], cbuilder -> bv_img, FALSE, FALSE, 5);

  // Space group
  cbuilder -> sg_box = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, cbuilder -> sg_box, FALSE, FALSE, 0);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> sg_box, markup_label("Space group:", 150, -1, 0.0, 0.5), FALSE, FALSE, 5);
  GtkTreeModel * model = sg_combo_tree (0, 0);
  cbuilder -> sg_combo = gtk_combo_box_new_with_model (model);
  g_object_unref (model);
  GtkCellRenderer * renderer = gtk_cell_renderer_combo_new ();
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (cbuilder -> sg_combo), renderer, TRUE);
  gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (cbuilder -> sg_combo), renderer, "text", 0, NULL);
  g_signal_connect (G_OBJECT(cbuilder -> sg_combo), "changed", G_CALLBACK(set_sg), data);
  gtk_widget_set_size_request (cbuilder -> sg_combo, 150, 25);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> sg_box, cbuilder -> sg_combo, FALSE, FALSE, 0);
  gtk_combo_box_set_active (GTK_COMBO_BOX(cbuilder -> sg_combo), 0);
  GList * cell_list = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(cbuilder -> sg_combo));
  if (cell_list && cell_list -> data)
  {
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(cbuilder -> sg_combo), cell_list -> data, "markup", 0, NULL);
  }

  // Space group option
  cbuilder -> so_box = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, cbuilder -> so_box, FALSE, FALSE, 0);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> so_box, markup_label("Settings:", 150, -1, 0.0, 0.5), FALSE, FALSE, 5);
  model = so_combo_tree (cbuilder -> cell.sp_group);
  cbuilder -> so_combo = gtk_combo_box_new_with_model (model);
  g_object_unref (model);
  renderer = gtk_cell_renderer_combo_new ();
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (cbuilder -> so_combo), renderer, TRUE);
  gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (cbuilder -> so_combo), renderer, "text", 0, NULL);
  gtk_widget_set_size_request (cbuilder -> so_combo, 150, 25);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, cbuilder -> so_box, cbuilder -> so_combo, FALSE, FALSE, 0);
  gtk_combo_box_set_active (GTK_COMBO_BOX(cbuilder -> so_combo), 0);
  widget_set_sensitive (cbuilder -> so_combo, 0);
  g_signal_connect (G_OBJECT(cbuilder -> so_combo), "changed", G_CALLBACK(set_so), data);
  cell_list = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(cbuilder -> so_combo));
  if (cell_list && cell_list -> data)
  {
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(cbuilder -> so_combo), cell_list -> data, "markup", 0, NULL);
  }

  for (i=0; i<3; i++)
  {
    cbuilder -> cell.cextra[i] = 1;
    for (j=0; j<3; j++)
    {
      cbuilder -> pointers[i][j].a = this_proj -> id;
      cbuilder -> pointers[i][j].b = i;
      cbuilder -> pointers[i][j].c = j;
    }
  }
  if (cbuilder -> cell.sp_group) get_origin (cbuilder -> cell.sp_group);
  // Lattice parameters
  hbox = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, hbox, FALSE, FALSE, 0);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, markup_label("Lattice parameters:", 150, -1, 0.0, 0.5), FALSE, FALSE, 5);
  cbuilder -> lattice_box = create_vbox (BSEP);
  GtkTreeModel * lmodel = la_combo_tree ();
  cbuilder -> la_combo = gtk_combo_box_new_with_model (lmodel);
  g_object_unref (lmodel);
  renderer = gtk_cell_renderer_combo_new ();
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (cbuilder -> la_combo), renderer, TRUE);
  gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (cbuilder -> la_combo), renderer, "text", 0, NULL);
  gtk_widget_set_size_request (cbuilder -> la_combo, 150, 25);
  g_signal_connect (G_OBJECT(cbuilder -> la_combo), "changed", G_CALLBACK(set_lattice), data);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, cbuilder -> la_combo, FALSE, FALSE, 0);
  gtk_combo_box_set_active (GTK_COMBO_BOX(cbuilder -> la_combo), 0);
  cell_list = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(cbuilder -> la_combo));
  if (cell_list && cell_list -> data)
  {
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(cbuilder -> la_combo), cell_list -> data, "markup", 0, NULL);
  }
  hbox = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, hbox, FALSE, FALSE, 5);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, cbuilder -> lattice_box, FALSE, FALSE, 50);

  hbox = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, hbox, FALSE, FALSE, 0);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, markup_label("Cell(s):", 150, -1, 0.0, 0.5), FALSE, FALSE, 5);
  hbox = create_hbox(0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, hbox, FALSE, FALSE, 0);
  GtkWidget * ax_cell[3];
  for (i=0; i<3; i++)
  {
    ax_cell[i] = spin_button (G_CALLBACK(add_cryst_cell), cbuilder -> cell.cextra[i], 1, 1000, 1, 0, 100, & cbuilder -> pointers[i][0]);
    str = g_strdup_printf ("%s x ", box_prop[0][i]);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, markup_label(str, 50, -1, 1.0, 0.5), FALSE, FALSE, 5);
    g_free (str);
    add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, ax_cell[i], FALSE, FALSE, 0);
  }

  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, check_button (" Wrap all atoms in the unit cell after building", -1, -1, FALSE, G_CALLBACK(set_wr), (gpointer)cbuilder), FALSE, FALSE, 10);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, check_button (" Show/hide clones after building", -1, -1, FALSE, G_CALLBACK(set_shc), (gpointer)cbuilder), FALSE, FALSE, 0);

  hbox = create_hbox (0);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, hbox, FALSE, FALSE, 10);
  this_proj -> modelgl -> search_widg[7] = allocate_atom_search (this_proj -> id, INSERT, 7, 0);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, markup_label("Add object(s):", 150, -1, 0.0, 0.5), FALSE, FALSE, 5);
  cbuilder -> add_combo = create_action_combo (5, this_proj);
  gtk_widget_set_size_request (cbuilder -> add_combo, 110, -1);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, cbuilder -> add_combo, FALSE, FALSE, 5);
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, create_button ("Occupancy", IMG_STOCK, DPROPERTIES, -1, -1, GTK_RELIEF_NORMAL, G_CALLBACK(adjust_occupancy), this_proj), FALSE, FALSE, 5);

  GtkWidget * cbscroll = create_scroll (vbox, 400, 200, GTK_SHADOW_NONE);
  add_container_child (CONTAINER_SCR, cbscroll, create_atoms_tree (this_proj -> modelgl -> search_widg[7], this_proj, 0));

  hbox = create_hbox (5);
  add_box_child_start (GTK_ORIENTATION_VERTICAL, vbox, hbox, TRUE, FALSE, 5);
  cbuilder -> pbut = create_button ((this_proj -> natomes) ? "Build (new project)" : "Build", IMG_STOCK, APPLY, -1, -1, GTK_RELIEF_NORMAL, G_CALLBACK(apply_build), GINT_TO_POINTER(this_proj -> id));
  add_box_child_end (hbox, cbuilder -> pbut, FALSE, FALSE, 5);
  GtkWidget * but = create_button ("Close", IMG_STOCK, FCLOSE, -1, -1, GTK_RELIEF_NORMAL, G_CALLBACK(close_build), GINT_TO_POINTER(this_proj -> id));
  add_box_child_start (GTK_ORIENTATION_HORIZONTAL, hbox, but, FALSE, FALSE, 5);
  add_gtk_close_event (win, G_CALLBACK(delete_build), GINT_TO_POINTER(this_proj -> id));

  return win;
}

void prepare_crystal_builder (gpointer data)
{
  tint * id = (tint *) data;
  struct project * this_proj = get_project_by_id(id -> a);
  if (this_proj -> modelgl -> builder_win == NULL)
  {
    // close_edit (NULL, GINT_TO_POINTER(this_proj -> id));
    prepare_atom_edition (data, FALSE);
    this_proj -> modelgl -> builder_win = g_malloc0(sizeof*this_proj -> modelgl -> builder_win);
    this_proj -> modelgl -> builder_win -> cell.box = g_malloc0(sizeof*this_proj -> modelgl -> builder_win -> cell.box);
    this_proj -> modelgl -> builder_win -> win = builder_win (this_proj, data);
  }
  show_the_widgets (this_proj -> modelgl -> builder_win -> win);
}

#ifdef GTK4
G_MODULE_EXPORT void crystal_window (GSimpleAction * action, GVariant * parameter, gpointer data)
#else
G_MODULE_EXPORT void crystal_window (GtkWidget * widg, gpointer data)
#endif
{
  prepare_crystal_builder (data);
}
