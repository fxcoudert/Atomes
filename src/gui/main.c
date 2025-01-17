/* This file is part of Atomes.

Atomes is free software: you can redistribute it and/or modify it under the terms
of the GNU Affero General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Atomes is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with Atomes.
If not, see <https://www.gnu.org/licenses/> */

#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include "version.h"
#include "global.h"
#include "bind.h"
#include "gui.h"
#include "xmlrw.h"
#include "callbacks.h"
#include "interface.h"
#include "project.h"
#include "workspace.h"
#include "valid.h"

#ifdef G_OS_WIN32
#define APP_EXTENSION ".exe"
#else
#include <pwd.h>
#define APP_EXTENSION
#endif

extern GtkWidget * create_main_window (GApplication * app);

const gchar * dfi[2];
struct file_list {
  gchar * file_name;
  int file_type;
  struct file_list * next;
};

struct file_list * flist = NULL;
struct file_list * ftmp = NULL;
gboolean with_workspace = FALSE;

int test_this_arg (gchar * arg)
{
  char * fext[14]={"-awf", "-apf", " -xyz", "NULL", "-c3d", "-trj", "NULL", "-xdatcar", "NULL", "-pdb", "-ent", "-cif", "-hist", "-ipf"};
  int i, j;
  i = strlen(arg);
  gchar * str = g_ascii_strdown (arg, i);
  for (j=0; j<14; j++) if (g_strcmp0 (str, fext[j]) == 0) return j+1;
  gchar * aext = g_strdup_printf ("%c%c%c%c", str[i-4], str[i-3], str[i-2], str[i-1]);
  char * eext[14]={".awf", ".apf", ".xyz", "NULL", ".c3d", ".trj", "NULL", "tcar", "NULL", ".pdb", ".ent", ".cif", "hist", ".ipf"};
  for (j=0; j<14; j++) if (g_strcmp0 (aext, eext[j]) == 0) return -(j+1);
  g_free (str);
  g_free (aext);
  return 0;
}

void printhelp()
{
  char help[515] = "Usage: ATOMES [OPTION]\n       ATOMES [FILE]\n\n"
                   "3D model analysis and edition tool\n\n"
                   "options:\n"
                   "  -v, --version             version information\n"
                   "  -h, --help                display this help message\n"
                   "file:\n\n";
  char bug[20] = "\nReport a bug to <";
  char eh[4] = ">\n";

  printf("%s", help);
  printf("%s", bug);
  printf("%s", PACKAGE_BUGREPORT);
  printf("%s\n", eh);
}

void printversion ()
{
  char scanid[80]="\n3D model analysis and edition tool\n";
  char bug[20] = "\nReport a bug to <";
  char eh[4] = ">\n";

  printf ("%s", scanid);
  printf ("\n%s version         : %s\n", PACKAGE, VERSION);
  printf ("\nGTK+ version           : %1d.%1d.%1d\n",
          GTK_MAJOR_VERSION,
          GTK_MINOR_VERSION,
          GTK_MICRO_VERSION);
  printf ("Libavutil version      : %2d.%2d.%3d\n",
          LIBAVUTIL_VERSION_MAJOR,
          LIBAVUTIL_VERSION_MINOR,
          LIBAVUTIL_VERSION_MICRO);
  printf ("Libavformat version    : %2d.%2d.%3d\n",
          LIBAVFORMAT_VERSION_MAJOR,
          LIBAVFORMAT_VERSION_MINOR,
          LIBAVFORMAT_VERSION_MICRO);
  printf ("Libavcodec version     : %2d.%2d.%3d\n",
          LIBAVCODEC_VERSION_MAJOR,
          LIBAVCODEC_VERSION_MINOR,
          LIBAVCODEC_VERSION_MICRO);
  printf ("Libswscale version     : %2d.%2d.%3d\n",
          LIBSWSCALE_VERSION_MAJOR,
          LIBSWSCALE_VERSION_MINOR,
          LIBSWSCALE_VERSION_MICRO);
#ifdef OPENMP
  float v;
  char * v_string;
  switch (_OPENMP)
  {
    case 200505:
      v = 2.5;
      v_string = "2005-05";
      break;
    case 200805:
      v = 3.0;
      v_string = "2008-05";
      break;
    case 201107:
      v = 3.1;
      v_string = "2011-07";
      break;
    case 201307:
      v = 4.0;
      v_string = "2013-07";
      break;
    case 201511:
      v = 4.5;
      v_string = "2015-11";
      break;
    case 201811:
      v = 5.0;
      v_string = "2018-11";
      break;
    case 202011:
      v = 5.1;
      v_string = "2020-11";
      break;
  }
  printf ("OpenMP version         : %1.1f (%s)\n", v, v_string);
#endif
  printf ("FC    Compiler         : %s\n", FC);
  printf ("FC    Compiler flags   : %s\n", FCFLAGS);
  printf ("C     Compiler         : %s\n", CC);
  printf ("C     Compiler flags   : %s\n", CFLAGS);

  printf ("%s", bug);
  printf ("%s", PACKAGE_BUGREPORT);
  printf ("%s\n", eh);
}

gboolean destroy_func (gpointer user_data)
{
  GtkWidget * splashi = (GtkWidget*) user_data;
  destroy_this_widget (splashi);
  return FALSE;
}

#ifdef GTK3
G_MODULE_EXPORT gboolean splashdraw (GtkWidget * widget, cairo_t * cr, gpointer data)
{
  cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0); /* transparent */
  cairo_paint (cr);
  return FALSE;
}
#endif

GtkWidget * create_splash_window ()
{
  GtkWidget * splash_window = new_gtk_window ();
  gtk_window_set_decorated (GTK_WINDOW (splash_window), FALSE);
  GtkWidget * image;
#ifdef GTK4
  image = gtk_picture_new_for_filename (PACKAGE_LOGO);
  gchar * backcol = g_strdup_printf ("window#background {\n"
                                     "  background-color: rgba(255, 255, 255, 0.0);}");
  provide_gtk_css (backcol);
  gtk_widget_set_name (splash_window, "background");
  g_free (backcol);
#else
  gtk_window_set_type_hint (GTK_WINDOW (splash_window), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
  gtk_window_set_position (GTK_WINDOW (splash_window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_widget_set_app_paintable (splash_window, TRUE);
  GdkScreen * screen = gtk_widget_get_screen (splash_window);
  GdkVisual * visual = gdk_screen_get_rgba_visual (screen);
  gtk_widget_set_visual (splash_window, visual);
  // Next line might be optional for total transparency
  g_signal_connect(G_OBJECT(splash_window), "draw", G_CALLBACK(splashdraw), NULL);
  image = gtk_image_new_from_file (PACKAGE_LOGO);
#endif
  add_container_child (CONTAINER_WIN, splash_window, image);

  show_the_widgets (splash_window);
  gtk_window_set_transient_for ((GtkWindow *)splash_window, (GtkWindow *)MainWindow);
  return splash_window;
}

void read_this_file (int file_type, gchar * this_file)
{
  FILE * fp = fopen (this_file, dfi[0]);
  if (file_type == 1)
  {
    int i = open_save_workspace (fp, 0);
    if (i != 0)
    {
      gchar * err = g_strdup_printf ("Error while reading workspace file\n%s\nError code: %d\n", this_file, i);
      show_error (err, 0, MainWindow);
      g_free (err);
    }
  }
  else
  {
    init_project (FALSE);
    open_save (fp, 0, activep, activep, 0, this_file);
  }
  fclose (fp);
}

void open_this_data_file (int file_type, gchar * file_name)
{
  gchar * end;
  gchar * str;
  gchar * filedir;
  int i;
#ifdef G_OS_WIN32
  WIN32_FIND_DATA ffd;
  HANDLE hFind;
#else
  DIR * d;
  struct dirent * dir;
#endif

  switch (file_type)
  {
    case 1:
      read_this_file (1, file_name);
      break;
    case 2:
      end = g_strdup_printf ("%c", file_name[strlen(file_name)-1]);
      if (g_strcmp0 (file_name, "*") == 0 || g_strcmp0 (end, "*") == 0)
      {
        if (g_strcmp0 (file_name, "*") == 0)
        {
          filedir = g_strdup_printf ("./");
        }
        else
        {
          filedir = g_strdup_printf ("%c", file_name[0]);
          for (i=1; i<strlen(file_name)-1; i++) filedir = g_strdup_printf ("%s%c", filedir, file_name[i]);
        }
#ifdef G_OS_WIN32
        ffd;
        hFind = FindFirstFile (filedir, & ffd);
        if (hFind != INVALID_HANDLE_VALUE)
        {
          if (ffd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
          {
            str = g_strdup_printf ("%s\\%s", filedir, (gchar *)ffd.cFileName);
            read_this_file (2, str);
            g_free (str);
          }
          while (FindNextFile(hFind, &ffd) != 0)
          {
            if (ffd.dwFileAttributes & ! FILE_ATTRIBUTE_DIRECTORY)
            {
              str = g_strdup_printf ("%s\\%s", filedir, (gchar *)ffd.cFileName);
              read_this_file (2, str);
              g_free (str);
            }
          }
        }
        FindClose(hFind);
#else
        d = opendir (filedir);
        if (d)
        {
          while ((dir = readdir(d)) != NULL)
          {
            if (dir -> d_type == DT_REG)
            {
              str = g_strdup_printf ("%s/%s", filedir, dir -> d_name);
              read_this_file (2, str);
              g_free (str);
            }
          }
          closedir(d);
        }
#endif
        g_free (filedir);
      }
      else
      {
        read_this_file (2, file_name);
      }
      break;
    case 14:
      init_project (TRUE);
      open_this_isaacs_xml_file (file_name, activep, FALSE);
      break;
    default:
      end = g_strdup_printf ("%c", file_name[strlen(file_name)-1]);
      if (g_strcmp0 (file_name, "*") == 0 || g_strcmp0 (end, "*") == 0)
      {
        if (g_strcmp0 (file_name, "*") == 0)
        {
          filedir = g_strdup_printf ("./");
        }
        else
        {
          filedir = g_strdup_printf ("%c", file_name[0]);
          for (i=1; i<strlen(file_name)-1; i++) filedir = g_strdup_printf ("%s%c", filedir, file_name[i]);
        }
#ifdef G_OS_WIN32
        ffd;
        hFind = FindFirstFile (filedir, & ffd);
        if (hFind != INVALID_HANDLE_VALUE)
        {
          if (ffd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
          {
            init_project (TRUE);
            active_project -> coordfile =  g_strdup_printf ("%s\\%s", filedir, (gchar *)ffd.cFileName);
            open_this_coordinate_file (file_type-3);
          }
          while (FindNextFile(hFind, &ffd) != 0)
          {
            if (ffd.dwFileAttributes & ! FILE_ATTRIBUTE_DIRECTORY)
            {
              init_project (TRUE);
              active_project -> coordfile = g_strdup_printf ("%s\\%s", filedir, (gchar *)ffd.cFileName);
              open_this_coordinate_file (file_type-3);
            }
          }
        }
        FindClose(hFind);
#else
        d = opendir (filedir);
        if (d)
        {
          while ((dir = readdir(d)) != NULL)
          {
            if (dir -> d_type == DT_REG)
            {
              init_project (TRUE);
              active_project -> coordfile = g_strdup_printf ("%s/%s", filedir, dir -> d_name);
              open_this_coordinate_file (file_type-3);
            }
          }
          closedir(d);
        }
#endif
        g_free (filedir);
      }
      else
      {
        init_project (TRUE);
        active_project -> coordfile = g_strdup_printf ("%s", file_name);
        open_this_coordinate_file (file_type-3);
      }
      break;
  }
}

G_MODULE_EXPORT void run_program (GApplication * app, gpointer data)
{
#ifdef GTK3
  GtkSettings * default_settings = gtk_settings_get_default ();
  g_object_set (default_settings, "gtk-button-images", TRUE, NULL);
#endif
#ifdef G_OS_WIN32
#ifdef GTK3
  g_object_set (default_settings, "gtk-key-theme-name", "win32", NULL);
#endif
  dfi[0]="rb";
  dfi[1]="wb";
#else
  dfi[0]="r";
  dfi[1]="w";
#endif

#ifdef OSX
  GtkosxApplication * ProgOSX;
  ProgOSX = g_object_new (GTKOSX_TYPE_APPLICATION, NULL);
  gtkosx_application_set_use_quartz_accelerators (ProgOSX, FALSE);
  gtkosx_application_ready (ProgOSX);
#endif
#ifdef DEBUG
  printversion ();
#endif // DEBUG
  MainWindow = create_main_window (app);
  GtkWidget * isplash = create_splash_window ();
  if (isplash == NULL)
  {
    g_print ("Impossible to load the splash screen\n");
  }
  else
  {
    g_timeout_add_seconds (2, destroy_func, isplash);
  }
  char * closure = "Starting Atomes <b>demo</b> version:\n"
                   "\n"
                   // "  *  this evaluation version will stop in <b>5 min</b>\n"
                   "\t- saving features have been disabled !\n"
                   "\n"
                   "Interested in purchasing the Atomes program ?\n";
  registered_atomes =TRUE;
 // (testing_atomes) ? TRUE : validate ();
  if (! registered_atomes)
  {
    show_info (closure, 1, MainWindow);
    correct_this_window_title (MainWindow, g_strdup_printf ("%s - demo version", PACKAGE));
    // g_timeout_add_seconds (300, (GSourceFunc )quit_gtk, NULL);
  }
  else
  {
    // gtk_widget_hide (register_button);
  }

  if (flist)
  {
    ftmp = flist;
    silent_input = TRUE;
    if (with_workspace)
    {
      while (ftmp)
      {
        if (ftmp -> file_type == 1)
        {
          // Open the workspace
          open_this_data_file (ftmp -> file_type, ftmp -> file_name);
        }
        ftmp = ftmp -> next;
      }
    }
    ftmp = flist;
    while (ftmp)
    {
      if (ftmp -> file_type != 1)
      {
        // Add project(s) to workspace
        open_this_data_file (ftmp -> file_type, ftmp -> file_name);
      }
      ftmp = ftmp -> next;
    }
    g_free (flist);
    flist = NULL;
    silent_input = FALSE;
  }
#ifdef OSX
  g_object_unref (ProgOSX);
#endif
}

int main (int argc, char *argv[])
{
  gboolean RUNC = FALSE;

#ifdef G_OS_WIN32
  FreeConsole ();
  PACKAGE_PREFIX = g_win32_get_package_installation_directory_of_module (NULL);
  // g_win32_get_package_installation_directory (NULL, NULL);
  PACKAGE_LIB_DIR = g_build_filename (PACKAGE_PREFIX, "library", NULL);
  PACKAGE_DATA_DIR = g_build_filename (PACKAGE_PREFIX, "pixmaps", NULL);
  PACKAGE_LOCALE_DIR = g_build_filename (PACKAGE_PREFIX, "locale", NULL);
  PACKAGE_IMP = g_build_filename (PACKAGE_PREFIX, "pixmaps/import.png", NULL);
  PACKAGE_CON = g_build_filename (PACKAGE_PREFIX, "pixmaps/convert.png", NULL);
  PACKAGE_IMG = g_build_filename (PACKAGE_PREFIX, "pixmaps/image.png", NULL);
  PACKAGE_PDF = g_build_filename (PACKAGE_PREFIX, "pixmaps/pdf.png", NULL);
  PACKAGE_SVG = g_build_filename (PACKAGE_PREFIX, "pixmaps/svg.png", NULL);
  PACKAGE_EPS = g_build_filename (PACKAGE_PREFIX, "pixmaps/eps.png", NULL);
  PACKAGE_PNG = g_build_filename (PACKAGE_PREFIX, "pixmaps/png.png", NULL);
  PACKAGE_JPG = g_build_filename (PACKAGE_PREFIX, "pixmaps/jpg.png", NULL);
  PACKAGE_BMP = g_build_filename (PACKAGE_PREFIX, "pixmaps/bmp.png", NULL);
  PACKAGE_TIFF = g_build_filename (PACKAGE_PREFIX, "pixmaps/tiff.png", NULL);
  PACKAGE_VOID = g_build_filename (PACKAGE_PREFIX, "pixmaps/void.png", NULL);
  PACKAGE_GR = g_build_filename (PACKAGE_PREFIX, "pixmaps/gr.png", NULL);
  PACKAGE_SQ = g_build_filename (PACKAGE_PREFIX, "pixmaps/sq.png", NULL);
  PACKAGE_BD = g_build_filename (PACKAGE_PREFIX, "pixmaps/bd.png", NULL);
  PACKAGE_AN = g_build_filename (PACKAGE_PREFIX, "pixmaps/an.png", NULL);
  PACKAGE_RI = g_build_filename (PACKAGE_PREFIX, "pixmaps/ri.png", NULL);
  PACKAGE_CH = g_build_filename (PACKAGE_PREFIX, "pixmaps/ch.png", NULL);
  PACKAGE_SP = g_build_filename (PACKAGE_PREFIX, "pixmaps/sp.png", NULL);
  PACKAGE_MS = g_build_filename (PACKAGE_PREFIX, "pixmaps/ms.png", NULL);
  PACKAGE_TD = g_build_filename (PACKAGE_PREFIX, "pixmaps/td.png", NULL);
  PACKAGE_MOL = g_build_filename (PACKAGE_PREFIX, "pixmaps/molecule.png", NULL);
  PACKAGE_OGL = g_build_filename (PACKAGE_PREFIX, "pixmaps/opengl.png", NULL);
  PACKAGE_OGLM = g_build_filename (PACKAGE_PREFIX, "pixmaps/mol.png", NULL);
  PACKAGE_OGLC = g_build_filename (PACKAGE_PREFIX, "pixmaps/mol.png", NULL);
  PACKAGE_PRO = g_build_filename (PACKAGE_PREFIX, "pixmaps/prop.png", NULL);
  PACKAGE_SET = g_build_filename (PACKAGE_PREFIX, "pixmaps/settings.png", NULL);
  PACKAGE_LOGO = g_build_filename (PACKAGE_PREFIX, "pixmaps/logo.png", NULL);
  PACKAGE_LAGPL = g_build_filename (PACKAGE_PREFIX, "pixmaps/logo-agpl.png", NULL);
  PACKAGE_LABOUT = g_build_filename (PACKAGE_PREFIX, "pixmaps/logo-about.png", NULL);
  PACKAGE_DOTA = g_build_filename (PACKAGE_PREFIX, "pixmaps/dots/dots-a.png", NULL);
  PACKAGE_DOTB = g_build_filename (PACKAGE_PREFIX, "pixmaps/dots/dots-b.png", NULL);
  PACKAGE_DOTC = g_build_filename (PACKAGE_PREFIX, "pixmaps/dots/dots-c.png", NULL);
  PACKAGE_DOTD = g_build_filename (PACKAGE_PREFIX, "pixmaps/dots/dots-d.png", NULL);
  PACKAGE_DOTE = g_build_filename (PACKAGE_PREFIX, "pixmaps/dots/dots-e.png", NULL);
  PACKAGE_DOTF = g_build_filename (PACKAGE_PREFIX, "pixmaps/dots/dots-f.png", NULL);
  PACKAGE_DOTG = g_build_filename (PACKAGE_PREFIX, "pixmaps/dots/dots-g.png", NULL);
  PACKAGE_DOTH = g_build_filename (PACKAGE_PREFIX, "pixmaps/dots/dots-h.png", NULL);
  PACKAGE_DFBD = g_build_filename (PACKAGE_PREFIX, "pixmaps/field/bd.png", NULL);
  PACKAGE_DFAN = g_build_filename (PACKAGE_PREFIX, "pixmaps/field/an.png", NULL);
  PACKAGE_DFDI = g_build_filename (PACKAGE_PREFIX, "pixmaps/field/di.png", NULL);
  PACKAGE_DFTD = g_build_filename (PACKAGE_PREFIX, "pixmaps/field/td.png", NULL);
  PACKAGE_DFIN = g_build_filename (PACKAGE_PREFIX, "pixmaps/field/in.png", NULL);
  PACKAGE_SGCP = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Cubic-P.png", NULL);
  PACKAGE_SGCI = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Cubic-I.png", NULL);
  PACKAGE_SGCF = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Cubic-F.png", NULL);
  PACKAGE_SGHP = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Hexagonal.png", NULL);
  PACKAGE_SGTR = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Trigonal-R.png", NULL);
  PACKAGE_SGTI = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Tetragonal-I.png", NULL);
  PACKAGE_SGTP = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Tetragonal-P.png", NULL);
  PACKAGE_SGOP = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Orthorhombic-P.png", NULL);
  PACKAGE_SGOI = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Orthorhombic-I.png", NULL);
  PACKAGE_SGOC = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Orthorhombic-C.png", NULL);
  PACKAGE_SGOF = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Orthorhombic-F.png", NULL);
  PACKAGE_SGMP = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Monoclinic-P.png", NULL);
  PACKAGE_SGMI = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Monoclinic-I.png", NULL);
  PACKAGE_SGTC = g_build_filename (PACKAGE_PREFIX, "pixmaps/bravais/Triclinic.png", NULL);
  ATOMES_CONFIG = g_build_filename (PACKAGE_PREFIX, "atomes.cfg", NULL);
#endif // G_OS_WIN32
#ifdef LINUX
  struct passwd * pw = getpwuid(getuid());
  ATOMES_CONFIG = g_strdup_printf ("%s/.atomes.cfg", pw -> pw_dir);
#endif
  int i, j, k;
  switch (argc)
  {
    case 1:
      RUNC=TRUE;
      break;
    case 2:
      if (g_strcmp0 (argv[1], "-h") == 0 || g_strcmp0 (argv[1], "--help") == 0)
      {
        printhelp();
        RUNC=FALSE;
      }
      else if (g_strcmp0 (argv[1], "-v") == 0 || g_strcmp0 (argv[1], "--version") == 0)
      {
        printversion();
        RUNC=FALSE;
      }
      else
      {
        RUNC=TRUE;
        i = test_this_arg (argv[1]);
        if (i !=0)
        {
          flist = g_malloc0(sizeof*flist);
          flist -> file_name = g_strdup_printf ("%s", argv[1]);
          flist -> file_type = -i;
          if (flist -> file_type == 1) with_workspace = TRUE;
        }
      }
      break;
    default:
      RUNC=TRUE;
      i=0;
      for (j=1; j<argc; j++)
      {
        k = test_this_arg (argv[j]);
        if (! (abs(k) == 1 && with_workspace))
        {
          if (k > 0 && j < argc-1)
          {
            if (! flist)
            {
              flist = g_malloc0(sizeof*flist);
              ftmp = flist;
            }
            else
            {
              ftmp -> next = g_malloc0(sizeof*ftmp -> next);
              ftmp = ftmp -> next;
            }
            ftmp -> file_name = g_strdup_printf ("%s", argv[j+1]);
            ftmp -> file_type = k;
            j ++;
          }
          else if (k < 0)
          {
            if (! flist)
            {
              flist = g_malloc0(sizeof*flist);
              ftmp = flist;
            }
            else
            {
              ftmp -> next = g_malloc0(sizeof*ftmp -> next);
              ftmp = ftmp -> next;
            }
            ftmp -> file_name = g_strdup_printf ("%s", argv[j]);
            ftmp -> file_type = -k;
          }
          if (abs(k) == 1) with_workspace = TRUE;
        }
        else if (k == 1)
        {
          j ++;
        }
      }
      break;
  }

  if (RUNC)
  {
    // setlocale(LC_ALL,"en_US");
    gtk_disable_setlocale ();
    AtomesApp = gtk_application_new (g_strdup_printf ("atomes.prog-%d", (int)clock()), G_APPLICATION_FLAGS_NONE);
    g_signal_connect (G_OBJECT(AtomesApp), "activate", G_CALLBACK(run_program), NULL);
    int status = g_application_run (G_APPLICATION (AtomesApp), 0, NULL);
    g_object_unref (AtomesApp);
    return status;
  }
  return 0;
}
