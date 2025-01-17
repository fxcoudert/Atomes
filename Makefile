# This file is part of Atomes.

# Atomes is free software: you can redistribute it and/or modify it under the terms
# of the GNU Affero General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.

# Atomes is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.

# You should have received a copy of the GNU Affero General Public License along with Atomes.
# If not, see <https://www.gnu.org/licenses/>

# The targets to build are 'atomes' or 'debug'

LINUX = 1
WINDOWS = 0
OSX = 0

# The next line defines the GTK version !
GTKV = 3
ifeq ($(GTKV),4)
  DGTK = -DGTK4 -DGTKGLAREA
  IGTK = `pkg-config --cflags gtk4 epoxy libxml-2.0 pangoft2 libavutil libavcodec libavformat libswscale`
  LGTK = `pkg-config --libs gtk4 epoxy libxml-2.0 pangoft2 libavutil libavcodec libavformat libswscale`
else
  DGTK = -DGTK3 -DGTKGLAREA -DMENU_ICONS
  IGTK = `pkg-config --cflags gtk+-3.0 epoxy libxml-2.0 pangoft2 libavutil libavcodec libavformat libswscale`
  LGTK = `pkg-config --libs gtk+-3.0 epoxy libxml-2.0 pangoft2 libavutil libavcodec libavformat libswscale`
endif

# OpenMP
OPENMP = 1
ifeq ($(OPENMP),1)
  DOMP = -DOPENMP -fopenmp
else
  DOMP =
endif

ifeq ($(LINUX),1)

  # Use a specific compiler version
  ifeq ($(GCCVER),)
    FC = gfortran
    CC = gcc
    LD = gcc
  else
    FC = gfortran-$(GCCVER)
    CC = gcc-$(GCCVER)
    LD = gcc-$(GCCVER)
  endif
  LIBS = -Wl,--export-dynamic $(LGTK) -lm -lgfortran

  DOS = -DLINUX
  CPPFLAGS =  -DPACKAGE_DATA_DIR=\"\" \
	-DPACKAGE_LOCALE_DIR=\"\" \
	-DPACKAGE_LIB_DIR=\""library"\" \
	-DPACKAGE_LOGO=\""pixmaps/logo.png"\" \
	-DPACKAGE_LAGPL=\""pixmaps/logo-agpl.png"\" \
	-DPACKAGE_LABOUT=\""pixmaps/logo-about.png"\" \
	-DPACKAGE_TD=\""pixmaps/td.png"\" \
	-DPACKAGE_MOL=\""pixmaps/molecule.png"\" \
	-DPACKAGE_IMP=\""pixmaps/import.png"\" \
	-DPACKAGE_CON=\""pixmaps/convert.png"\" \
	-DPACKAGE_IMG=\""pixmaps/image.png"\" \
	-DPACKAGE_PDF=\""pixmaps/pdf.png"\" \
	-DPACKAGE_SVG=\""pixmaps/svg.png"\" \
	-DPACKAGE_EPS=\""pixmaps/eps.png"\" \
	-DPACKAGE_PNG=\""pixmaps/png.png"\" \
	-DPACKAGE_JPG=\""pixmaps/jpg.png"\" \
	-DPACKAGE_BMP=\""pixmaps/bmp.png"\" \
	-DPACKAGE_TIFF=\""pixmaps/tiff.png"\" \
	-DPACKAGE_EDIT=\""pixmaps/cedit.png"\" \
	-DPACKAGE_VOID=\""pixmaps/void.png"\" \
	-DPACKAGE_GR=\""pixmaps/gr.png"\" \
	-DPACKAGE_SQ=\""pixmaps/sq.png"\" \
	-DPACKAGE_AN=\""pixmaps/an.png"\" \
	-DPACKAGE_BD=\""pixmaps/bd.png"\" \
	-DPACKAGE_RI=\""pixmaps/ri.png"\" \
	-DPACKAGE_CH=\""pixmaps/ch.png"\" \
	-DPACKAGE_SP=\""pixmaps/sp.png"\" \
	-DPACKAGE_MS=\""pixmaps/ms.png"\" \
	-DPACKAGE_OGL=\""pixmaps/opengl.png"\" \
	-DPACKAGE_OGLM=\""pixmaps/mol.png"\" \
	-DPACKAGE_OGLC=\""pixmaps/mol.png"\" \
	-DPACKAGE_PRO=\""pixmaps/prop.png"\" \
	-DPACKAGE_SET=\""pixmaps/settings.png"\" \
	-DPACKAGE_DOTA=\""pixmaps/dots/dots-a.png"\" \
	-DPACKAGE_DOTB=\""pixmaps/dots/dots-b.png"\" \
	-DPACKAGE_DOTC=\""pixmaps/dots/dots-c.png"\" \
	-DPACKAGE_DOTD=\""pixmaps/dots/dots-d.png"\" \
	-DPACKAGE_DOTE=\""pixmaps/dots/dots-e.png"\" \
	-DPACKAGE_DOTF=\""pixmaps/dots/dots-f.png"\" \
	-DPACKAGE_DOTG=\""pixmaps/dots/dots-g.png"\" \
	-DPACKAGE_DOTH=\""pixmaps/dots/dots-h.png"\" \
	-DPACKAGE_DOTI=\""pixmaps/dots/dots-i.png"\" \
	-DPACKAGE_DFBD=\""pixmaps/field/bd.png"\" \
	-DPACKAGE_DFAN=\""pixmaps/field/an.png"\" \
	-DPACKAGE_DFDI=\""pixmaps/field/di.png"\" \
	-DPACKAGE_DFTD=\""pixmaps/field/td.png"\" \
	-DPACKAGE_DFIN=\""pixmaps/field/in.png"\" \
	-DPACKAGE_SGCP=\""pixmaps/bravais/Cubic-P.png"\" \
	-DPACKAGE_SGCI=\""pixmaps/bravais/Cubic-I.png"\" \
	-DPACKAGE_SGCF=\""pixmaps/bravais/Cubic-F.png"\" \
	-DPACKAGE_SGHP=\""pixmaps/bravais/Hexagonal.png"\" \
	-DPACKAGE_SGTR=\""pixmaps/bravais/Trigonal-R.png"\" \
	-DPACKAGE_SGTI=\""pixmaps/bravais/Tetragonal-I.png"\" \
	-DPACKAGE_SGTP=\""pixmaps/bravais/Tetragonal-P.png"\" \
	-DPACKAGE_SGOP=\""pixmaps/bravais/Orthorhombic-P.png"\" \
	-DPACKAGE_SGOI=\""pixmaps/bravais/Orthorhombic-I.png"\" \
	-DPACKAGE_SGOC=\""pixmaps/bravais/Orthorhombic-C.png"\" \
	-DPACKAGE_SGOF=\""pixmaps/bravais/Orthorhombic-F.png"\" \
	-DPACKAGE_SGMP=\""pixmaps/bravais/Monoclinic-P.png"\" \
	-DPACKAGE_SGMI=\""pixmaps/bravais/Monoclinic-I.png"\" \
	-DPACKAGE_SGTC=\""pixmaps/bravais/Triclinic.png"\"

  LDFLGS = $(DOMP)

  RM = rm
  RMFLAGS = -f
  MV = mv
  CP = cp
  CPFLAGS = -f
  ECHO = echo

  EXT =

  FCVER = `$(FC) -dumpfullversion`
  CCVER = `$(CC) -dumpfullversion`

endif

ifeq ($(WINDOWS),1)

  DISK = C:/
  ARCH = 64
  CPU = x86_64
  DEV = $(DISK)msys64/usr/
  COMP = $(DISK)msys64/mingw$(ARCH)/bin/
  GTK = $(DISK)msys64/mingw$(ARCH)/
  PKG = $(GTK)bin/pkg-config

  FC = $(COMP)$(CPU)-w64-mingw32-gfortran
  CC = $(COMP)$(CPU)-w64-mingw32-gcc
  LD = $(COMP)$(CPU)-w64-mingw32-gfortran $(DOMP)

  CPPFLAGS =
  LDFLG = -static-libgcc -static-libgfortran -lz -liphlpapi

  IGTK = -pthread -mms-bitfields -IC:/msys64/mingw64/include/gtk-3.0 -IC:/msys64/mingw64/include/cairo \
	-IC:/msys64/mingw64/include/pango-1.0 -IC:/msys64/mingw64/include/atk-1.0 -IC:/msys64/mingw64/include/cairo \
	-IC:/msys64/mingw64/include/gdk-pixbuf-2.0 -IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/libxml2 \
	-IC:/msys64/mingw64/include/pango-1.0 -IC:/msys64/mingw64/include/fribidi -IC:/msys64/mingw64/include \
	-IC:/msys64/mingw64/include/cairo -IC:/msys64/mingw64/include/pixman-1 -IC:/msys64/mingw64/include \
	-IC:/msys64/mingw64/lib/libffi-3.2.1/include -IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/freetype2 \
	-IC:/msys64/mingw64/include -IC:/msys64/mingw64/include/libpng16 -IC:/msys64/mingw64/include \
	-IC:/msys64/mingw64/include/harfbuzz -IC:/msys64/mingw64/include/glib-2.0 -IC:/msys64/mingw64/lib/glib-2.0/include -IC:/msys64/mingw64/include

  LGTK = -LC:/msys64/mingw64/lib -lgtk-3 -lgdk-3 -lz -lgdi32 -limm32 -lshell32 -lole32 \
	-Wl,-luuid -lwinmm -ldwmapi -lsetupapi -lcfgmgr32 -lpangowin32-1.0 -lpangocairo-1.0 \
	-latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lepoxy -lxml2 -lpangoft2-1.0 \
	-lpango-1.0 -lgobject-2.0 -lglib-2.0 -lintl -lfontconfig -lfreetype -lavutil -lavcodec -lavformat -lswscale

  LIB = $(LGTK)

  ifeq ($(MAKECMDGOALS), atomes)
    LIBS = $(LIB)
    LDFLGS = $(LDFLG)
  endif
  ifeq ($(MAKECMDGOALS), debug)
    LIBS = $(LIB) -lssp
    LDFLGS = $(LDFLG) -fno-stack-protector
  endif

  DOS =

  RM = $(DEV)bin/rm
  RMFLAGS = -f
  CP = $(DEV)bin/cp
  MV = $(DEV)bin/mv
  CAT = $(DEV)bin/cat
  CPFLAGS = -f
  ECHO = $(DEV)bin/echo

  WINDRES = $(COMP)windres
  WINATOMES = $(OBJ)winatomes.o

  EXT=".exe"
  FCVER = 10.2.0
  CVER = 10.2.0

endif

SRC = src/
LIC = src/lic/
GUI = src/gui/
WORK = src/workspace/
PROJ = src/project/
CALC = src/calc/
DLPOLY = src/calc/dl_poly/
LAMMPS = src/calc/lammps/
FIELDS = src/calc/force_fields/
CPMD = src/calc/cpmd/
CP2K = src/calc/cp2k/
CURVE = src/curve/
OGL = src/opengl/
GLWIN = src/opengl/win/
GLDRAW = src/opengl/draw/
GLEDIT = src/opengl/edit/
FOR = src/fortran/
OBJ = obj/
BIN = bin/

INC = -I$(SRC) -I$(LIC) -I$(GUI) -I$(WORK) -I$(PROJ) -I$(PROJ)readers/ -I$(CALC) -I$(DLPOLY) -I$(LAMMPS) -I$(FIELDS) -I$(CPMD) -I$(CP2K) -I$(CURVE) -I$(GLWIN) -I$(GLEDIT) -I$(GLDRAW) -I$(OGL) -I.
INCLUDES = $(INC) $(IGTK) -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED

ifeq ($(MAKECMDGOALS),atomes)
  FCFLAGS = -O3 -cpp
  CFLAGS = -O3
  LDFLAGS = $(LIBS) $(LDFLGS)
  DEFS = -DHAVE_CONFIG_H $(DGTK) $(DOS)
endif

ifeq ($(MAKECMDGOALS),debug)
  FCFLAGS = -fno-second-underscore -O0 -Wall -g3 -pg -ggdb3 -cpp -dA -dD -dH -dp -dP -fvar-tracking -fbounds-check -fstack-protector-all
  CFLAGS = -O0 -Wall -g3 -pg -ggdb3 -cpp -dA -dD -dH -dp -dP -fvar-tracking -fbounds-check -fstack-protector-all
  LDFLAGS = $(LIBS) $(LDFLGS) -pg
  DEFS = -DHAVE_CONFIG_H -DDEBUG $(DGTK) $(DOS)
endif

PROGRAM = atomes

# The objects which make the executable

OBJECTS_f = $(OBJECTS_f90) $(OBJECTS_F90)

OBJECTS_f90 = $(OBJ)parameters.o $(OBJ)mendeleiev.o \
	$(OBJ)utils.o \
	$(OBJ)xyz.o $(OBJ)c3d.o $(OBJ)trj.o $(OBJ)vas.o $(OBJ)pdb.o \
	$(OBJ)lattice.o $(OBJ)allochem.o $(OBJ)chemistry.o \
	$(OBJ)allocbonds.o $(OBJ)bonds.o $(OBJ)escs.o $(OBJ)angles.o \
	$(OBJ)threads.o $(OBJ)gr.o $(OBJ)sq.o $(OBJ)fzbt.o \
	$(OBJ)cqvf.o $(OBJ)sk.o $(OBJ)grfft.o \
	$(OBJ)allocmsd.o $(OBJ)msd.o \
	$(OBJ)initrings.o $(OBJ)resrings.o $(OBJ)rings_ogl.o \
	$(OBJ)rings-king.o $(OBJ)rings-guttman.o $(OBJ)rings-primitive.o \
	$(OBJ)initchains.o $(OBJ)chains.o $(OBJ)chains_ogl.o \
	$(OBJ)spherical.o \
	$(OBJ)dvtb.o $(OBJ)dmtx.o $(OBJ)molecules.o \
	$(OBJ)writedata.o

OBJECTS_F90 = $(OBJ)prepdata.o $(OBJ)init.o

OBJECTS_c = $(OBJ)global.o $(OBJ_LIC) $(OBJ_GUI) $(OBJ_WORK) $(OBJ_PROJ) $(OBJ_CURVE) \
			$(OBJ_CALC) $(OBJ_POLY) $(OBJ_LAMMPS) $(OBJ_FIELD) $(OBJ_CPMD) $(OBJ_CP2K) $(OBJ_OGL)

OBJ_LIC = $(OBJ)valid.o

OBJ_GUI = \
	$(OBJ)gtk-misc.o \
	$(OBJ)work_menu.o \
	$(OBJ)edit_menu.o \
	$(OBJ)calc_menu.o \
	$(OBJ)tools.o \
	$(OBJ)gui.o \
	$(OBJ)initc.o \
	$(OBJ)callbacks.o \
	$(OBJ)interface.o \
	$(OBJ)xmlrw.o \
	$(OBJ)bdcall.o \
	$(OBJ)grcall.o \
	$(OBJ)sqcall.o \
	$(OBJ)ringscall.o \
	$(OBJ)chainscall.o \
	$(OBJ)msdcall.o \
	$(OBJ)spcall.o \
	$(OBJ)main.o

OBJ_WORK = \
	$(OBJ)modelinfo.o \
	$(OBJ)workinfo.o \
	$(OBJ)workspace.o

OBJ_PROJ = \
	$(OBJ)read_cif.o \
	$(OBJ)read_coord.o \
	$(OBJ)read_xyz.o \
	$(OBJ)read_c3d.o \
	$(OBJ)read_trj.o \
	$(OBJ)read_vas.o \
	$(OBJ)read_pdb.o \
	$(OBJ)read_hist.o \
	$(OBJ)read_npt.o \
	$(OBJ)update_p.o \
	$(OBJ)init_p.o \
	$(OBJ)debugio.o \
	$(OBJ)read_field.o \
	$(OBJ)read_qm.o \
	$(OBJ)read_opengl.o \
	$(OBJ)read_curve.o \
	$(OBJ)read_mol.o \
	$(OBJ)read_bond.o \
	$(OBJ)open_p.o \
	$(OBJ)close_p.o \
	$(OBJ)save_field.o \
	$(OBJ)save_qm.o \
	$(OBJ)save_opengl.o \
	$(OBJ)save_curve.o \
	$(OBJ)save_mol.o \
	$(OBJ)save_bond.o \
	$(OBJ)save_p.o \
	$(OBJ)project.o

OBJ_CALC = \
	$(OBJ)calc.o

OBJ_CURVE = \
	$(OBJ)tab-1.o \
	$(OBJ)tab-2.o \
	$(OBJ)tab-3.o \
	$(OBJ)tab-4.o \
	$(OBJ)cedit.o \
	$(OBJ)datab.o \
	$(OBJ)cwidget.o \
	$(OBJ)glyph.o \
	$(OBJ)labels.o \
	$(OBJ)title.o \
	$(OBJ)legend.o \
	$(OBJ)xaxis.o \
	$(OBJ)yaxis.o \
	$(OBJ)frame.o \
	$(OBJ)draw.o \
	$(OBJ)show.o \
	$(OBJ)w_data.o \
	$(OBJ)w_img.o \
	$(OBJ)m_curve.o \
	$(OBJ)w_curve.o \
	$(OBJ)curve.o

OBJ_POLY = \
	$(OBJ)dlp_control.o \
	$(OBJ)dlp_init.o \
	$(OBJ)dlp_active.o \
	$(OBJ)dlp_copy.o \
	$(OBJ)dlp_comp.o \
	$(OBJ)dlp_viz.o \
	$(OBJ)dlp_mol.o \
	$(OBJ)dlp_atom.o \
	$(OBJ)dlp_ff_match.o \
	$(OBJ)dlp_edit.o \
	$(OBJ)dlp_print.o \
	$(OBJ)dlp_field.o

OBJ_LAMMPS = \
	$(OBJ)la_print.o \

OBJ_FIELD = \
	$(OBJ)force_fields.o

#	$(OBJ)amber94.o \
#	$(OBJ)amber96.o \
#	$(OBJ)amber98.o \
#	$(OBJ)amber99.o \
#	$(OBJ)cff91.o \
#	$(OBJ)charmm22_prot.o \
#	$(OBJ)charmm22_prot_metals.o \
#	$(OBJ)charmm35_ethers.o \
#	$(OBJ)charmm36_carb.o \
#	$(OBJ)charmm36_cgenff.o \
#	$(OBJ)charmm36_lipid.o \
#	$(OBJ)charmm36_na.o \
#	$(OBJ)charmm36_prot.o \
#	$(OBJ)charmm36_prot_metals.o \
#	$(OBJ)charmm_silicates.o \
#	$(OBJ)compass.o \
#	$(OBJ)cvff.o \
#	$(OBJ)cvff_aug.o \
#	$(OBJ)oplsaap.o \
#	$(OBJ)oplsaar.o \
#	$(OBJ)pcff.o


OBJ_CPMD = \
	$(OBJ)cpmd_print.o \
	$(OBJ)cpmd_nose.o \
	$(OBJ)cpmd_restart.o \
	$(OBJ)cpmd_atoms.o \
	$(OBJ)cpmd_init.o

OBJ_CP2K = \
	$(OBJ)cp2k_print.o \
	$(OBJ)cp2k_molopt-basis.o \
	$(OBJ)cp2k_gth-basis.o \
	$(OBJ)cp2k_basis.o \
	$(OBJ)cp2k_pot.o \
	$(OBJ)cp2k_files.o \
	$(OBJ)cp2k_mol.o \
	$(OBJ)cp2k_init.o

OBJ_WIN = \
	$(OBJ)color_box.o \
	$(OBJ)m_style.o \
	$(OBJ)m_map.o \
	$(OBJ)m_render.o \
	$(OBJ)m_quality.o \
	$(OBJ)m_atoms.o \
	$(OBJ)m_bonds.o \
	$(OBJ)m_clones.o \
	$(OBJ)m_box.o \
	$(OBJ)m_coord.o \
	$(OBJ)m_poly.o \
	$(OBJ)m_tools.o \
	$(OBJ)m_edit.o \
	$(OBJ)m_rep.o \
	$(OBJ)m_proj.o \
	$(OBJ)m_back.o \
	$(OBJ)m_axis.o \
	$(OBJ)m_anim.o \
	$(OBJ)menu_bar.o \
	$(OBJ)w_measures.o \
	$(OBJ)w_volumes.o \
	$(OBJ)w_colors.o \
	$(OBJ)w_atoms.o \
	$(OBJ)w_bonds.o \
	$(OBJ)w_labels.o \
	$(OBJ)w_search.o \
	$(OBJ)w_periodic.o \
	$(OBJ)w_library.o \
	$(OBJ)w_cutoffs.o \
	$(OBJ)w_rings.o \
	$(OBJ)w_chains.o \
	$(OBJ)w_coord.o \
	$(OBJ)w_box.o \
	$(OBJ)w_axis.o \
	$(OBJ)w_advance.o \
	$(OBJ)w_sequencer.o \
	$(OBJ)w_spiner.o \
	$(OBJ)w_encode.o \
	$(OBJ)w_record.o \
	$(OBJ)initcoord.o \
	$(OBJ)initmol.o \
	$(OBJ)initring.o \
	$(OBJ)initchain.o \
	$(OBJ)popup.o \
	$(OBJ)glwindow.o

OBJ_CEDIT = \
	$(OBJ)cell_shift.o \
	$(OBJ)cell_extra.o \
	$(OBJ)cell_super.o \
	$(OBJ)cell_density.o \
	$(OBJ)cell_cut.o \
	$(OBJ)cell_pixel.o \
	$(OBJ)cell_edit.o


OBJ_AEDIT = \
	$(OBJ)atom_action.o \
	$(OBJ)atom_coord.o \
	$(OBJ)atom_geo.o \
	$(OBJ)atom_insert.o \
	$(OBJ)atom_move.o \
	$(OBJ)atom_object.o \
	$(OBJ)atom_remove.o \
	$(OBJ)atom_search.o \
	$(OBJ)atom_species.o \
	$(OBJ)atom_edit.o

OBJ_CBUILD = \
	$(OBJ)cbuild_action.o \
	$(OBJ)cbuild_sg.o \
	$(OBJ)cbuild_info.o \
	$(OBJ)cbuild_edit.o

OBJ_DRAW = \
	$(OBJ)d_atoms.o \
	$(OBJ)d_bonds.o \
	$(OBJ)d_label.o \
	$(OBJ)d_selection.o \
	$(OBJ)d_poly.o \
	$(OBJ)d_rings.o \
	$(OBJ)d_box.o \
	$(OBJ)d_axis.o \
	$(OBJ)d_measures.o \
	$(OBJ)ogl_text.o \
	$(OBJ)movie.o \
	$(OBJ)image.o

OBJ_GL = \
	$(OBJ)arcball.o \
	$(OBJ)selection.o \
	$(OBJ)ogl_utils.o \
	$(OBJ)ogl_shaders.o \
	$(OBJ)ogl_shading.o \
	$(OBJ)ogl_draw.o \
	$(OBJ)glview.o

OBJ_OGL = $(OBJ_WIN) $(OBJ_CBUILD) $(OBJ_CEDIT) $(OBJ_AEDIT) $(OBJ_DRAW) $(OBJ_GL)

ifeq ($(WINDOWS),1)

  OBJECTS = $(WINATOMES) $(OBJECTS_f) $(OBJECTS_c)

else

  OBJECTS = $(OBJECTS_f) $(OBJECTS_c)

endif

SOURCES_h = \
	$(SRC)version.h \
	$(SRC)config.h \
	$(SRC)affero.h \
	$(SRC)bind.h \
	$(SRC)global.h \
	$(GUI)gui.h \
	$(GUI)interface.h \
	$(GUI)callbacks.h \
	$(PROJ)project.h \
	$(PROJ)/readers/readers.h \
	$(DLPOLY)dlp_field.h \
	$(FIELDS)force_field.h \
	$(OGL)glview.h \
	$(GLWIN)glwindow.h \
	$(GLWIN)submenus.h \
	$(GLWIN)color_box.h \
	$(GLWIN)initcoord.h \
	$(GLDRAW)movie.h \
	$(LIC)valid.h

# The rule to build the executable

atomes: version exe

debug: version exe

version:
	$(ECHO) "#define FC \""$(FC) $(FCVER)"\"" > src/version.h
	$(ECHO) "#define FCFLAGS \""$(FCFLAGS)"\""  >> src/version.h
	$(ECHO) "#define CC \""$(CC) $(CCVER)"\"" >> src/version.h
	$(ECHO) "#define CFLAGS \""$(CFLAGS)"\"" >> src/version.h

all:
	$(MAKE) debug
	$(CP) $(BIN)/atomes$(EXT) $(BIN)/atomes-debug$(EXT)
	$(MAKE) clean
	$(MAKE) atomes

exe:$(OBJECTS)
	$(LD) -o $(BIN)$(PROGRAM) $(OBJECTS) $(DEFS) $(LDFLAGS)

# Special targets

cleangui:
	$(RM) $(RMFLAGS) $(OBJ_GUI) $(BIN)$(PROGRAM)

cleanwork:
	$(RM) $(RMFLAGS) $(OBJ_WORK) $(BIN)$(PROGRAM)

cleanproj:
	$(RM) $(RMFLAGS) $(OBJ_PROJ) $(BIN)$(PROGRAM)

cleancalc:
	$(RM) $(RMFLAGS) $(OBJ_CALC) $(BIN)$(PROGRAM)

cleanpoly:
	$(RM) $(RMFLAGS) $(OBJ_POLY) $(BIN)$(PROGRAM)

cleanfield:
	$(RM) $(RMFLAGS) $(OBJ_FIELD) $(BIN)$(PROGRAM)

cleancpmd:
	$(RM) $(RMFLAGS) $(OBJ_CPMD) $(BIN)$(PROGRAM)

cleancp2k:
	$(RM) $(RMFLAGS) $(OBJ_CP2K) $(BIN)$(PROGRAM)

cleancurve:
	$(RM) $(RMFLAGS) $(OBJ_CURVE) $(BIN)$(PROGRAM)

cleanedit:
	$(RM) $(RMFLAGS) $(OBJ_CEDIT) $(OBJ_AEDIT) $(OBJ_CBUILD) $(BIN)$(PROGRAM)

cleanogl:
	$(RM) $(RMFLAGS) $(OBJ_OGL) $(BIN)$(PROGRAM)

cleanc:
	$(RM) $(RMFLAGS) $(OBJECTS_c) $(BIN)$(PROGRAM)

cleanf:
	$(RM) $(RMFLAGS) $(OBJECTS_f) $(BIN)$(PROGRAM)
	$(RM) $(RMFLAGS) *.mod
	$(RM) $(RMFLAGS) $(OBJ)*.mod

clean:
	$(RM) $(RMFLAGS) $(OBJECTS) *.mod $(OBJ)*.mod $(BIN)$(PROGRAM)

clear:
	$(RM) $(RMFLAGS) $(OBJECTS) *.mod $(OBJ)*.mod

TouchSource:
	$(TOUCH) $(SOURCES)

$(OBJ)parameters.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)parameters.o $(FOR)parameters.f90
$(OBJ)mendeleiev.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)mendeleiev.o $(FOR)mendeleiev.f90
$(OBJ)utils.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)utils.o $(FOR)utils.f90
$(OBJ)xyz.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)xyz.o $(FOR)xyz.f90
$(OBJ)c3d.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)c3d.o $(FOR)c3d.f90
$(OBJ)trj.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)trj.o $(FOR)trj.f90
$(OBJ)vas.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)vas.o $(FOR)vas.f90
$(OBJ)pdb.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)pdb.o $(FOR)pdb.f90
$(OBJ)lattice.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)lattice.o $(FOR)lattice.f90
$(OBJ)allochem.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)allochem.o $(FOR)allochem.f90
$(OBJ)chemistry.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)chemistry.o $(FOR)chemistry.f90
$(OBJ)allocbonds.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)allocbonds.o $(FOR)allocbonds.f90
$(OBJ)bonds.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)bonds.o $(FOR)bonds.f90
$(OBJ)escs.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)escs.o $(FOR)escs.f90
$(OBJ)angles.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)angles.o $(FOR)angles.f90
$(OBJ)threads.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)threads.o $(FOR)threads.f90
$(OBJ)gr.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)gr.o $(FOR)gr.f90
$(OBJ)sq.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)sq.o $(FOR)sq.f90
$(OBJ)fzbt.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)fzbt.o $(FOR)fzbt.f90
$(OBJ)cqvf.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)cqvf.o $(FOR)cqvf.f90
$(OBJ)sk.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)sk.o $(FOR)sk.f90
$(OBJ)grfft.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)grfft.o $(FOR)grfft.f90
$(OBJ)allocmsd.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)allocmsd.o $(FOR)allocmsd.f90
$(OBJ)msd.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)msd.o $(FOR)msd.f90
$(OBJ)spherical.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)spherical.o $(FOR)spherical.f90
$(OBJ)initrings.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)initrings.o $(FOR)initrings.f90
$(OBJ)rings-king.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)rings-king.o $(FOR)rings-king.f90
$(OBJ)rings-guttman.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)rings-guttman.o $(FOR)rings-guttman.f90
$(OBJ)rings-primitive.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)rings-primitive.o $(FOR)rings-primitive.f90
$(OBJ)resrings.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)resrings.o $(FOR)resrings.f90
$(OBJ)rings_ogl.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)rings_ogl.o $(FOR)rings_ogl.f90
$(OBJ)initchains.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)initchains.o $(FOR)initchains.f90
$(OBJ)chains.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)chains.o $(FOR)chains.f90
$(OBJ)chains_ogl.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)chains_ogl.o $(FOR)chains_ogl.f90
$(OBJ)dvtb.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)dvtb.o $(FOR)dvtb.f90
$(OBJ)dmtx.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)dmtx.o $(FOR)dmtx.f90
$(OBJ)old-dmtx.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)old-dmtx.o $(FOR)old-dmtx.f90
$(OBJ)molecules.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DOMP) $(DEFS) -c -o $(OBJ)molecules.o $(FOR)molecules.f90
$(OBJ)writedata.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)writedata.o $(FOR)writedata.f90

# F90 files:

$(OBJ)prepdata.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)prepdata.o $(FOR)prepdata.F90 $(INC)
$(OBJ)init.o:
	$(FC) $(FCFLAGS) $(FCLIBS) $(DEFS) -c -o $(OBJ)init.o $(FOR)init.F90 $(INC)

# C files:
$(OBJ)global.o:
	$(CC) -c -MT $(OBJ)global.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)global.o $(SRC)global.c $(INCLUDES)

# license info:
$(OBJ)valid.o:
	$(CC) -c -MT $(OBJ)valid.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)valid.o $(LIC)valid.c $(INCLUDES)

# GUI
$(OBJ)gtk-misc.o:
	$(CC) -c -MT $(OBJ)gtk-misc.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)gtk-misc.o $(GUI)gtk-misc.c $(INCLUDES)
$(OBJ)work_menu.o:
	$(CC) -c -MT $(OBJ)work_menu.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)work_menu.o $(GUI)work_menu.c $(INCLUDES)
$(OBJ)edit_menu.o:
	$(CC) -c -MT $(OBJ)edit_menu.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)edit_menu.o $(GUI)edit_menu.c $(INCLUDES)
$(OBJ)calc_menu.o:
	$(CC) -c -MT $(OBJ)calc_menu.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)calc_menu.o $(GUI)calc_menu.c $(INCLUDES)
$(OBJ)tools.o:
	$(CC) -c -MT $(OBJ)tools.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)tools.o $(GUI)tools.c $(INCLUDES)
$(OBJ)gui.o:
	$(CC) -c -MT $(OBJ)gui.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)gui.o $(GUI)gui.c $(INCLUDES)
$(OBJ)initc.o:
	$(CC) -c -MT $(OBJ)initc.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)initc.o $(GUI)initc.c $(INCLUDES)

# Workspace
$(OBJ)modelinfo.o:
	$(CC) -c -MT $(OBJ)modelinfo.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)modelinfo.o $(WORK)modelinfo.c $(INCLUDES)
$(OBJ)expinfo.o:
	$(CC) -c -MT $(OBJ)expinfo.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)expinfo.o $(WORK)expinfo.c $(INCLUDES)
$(OBJ)workinfo.o:
	$(CC) -c -MT $(OBJ)workinfo.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)workinfo.o $(WORK)workinfo.c $(INCLUDES)
$(OBJ)workspace.o:
	$(CC) -c -MT $(OBJ)workspace.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)workspace.o $(WORK)workspace.c $(INCLUDES)

# GUI
$(OBJ)xmlrw.o:
	$(CC) -c -MT $(OBJ)xmlrw.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)xmlrw.o $(GUI)xmlrw.c $(INCLUDES)
$(OBJ)callbacks.o:
	$(CC) -c -MT $(OBJ)callbacks.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)callbacks.o $(GUI)callbacks.c $(INCLUDES)
$(OBJ)interface.o:
	$(CC) -c -MT $(OBJ)interface.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)interface.o $(GUI)interface.c $(INCLUDES)
$(OBJ)bdcall.o:
	$(CC) -c -MT $(OBJ)bdcall.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)bdcall.o $(GUI)bdcall.c $(INCLUDES)
$(OBJ)grcall.o:
	$(CC) -c -MT $(OBJ)grcall.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)grcall.o $(GUI)grcall.c $(INCLUDES)
$(OBJ)sqcall.o:
	$(CC) -c -MT $(OBJ)sqcall.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)sqcall.o $(GUI)sqcall.c $(INCLUDES)
$(OBJ)ringscall.o:
	$(CC) -c -MT $(OBJ)ringscall.o -MD -MP -MF $(CFLAGS) -DOPENMP $(DEFS) -o $(OBJ)ringscall.o $(GUI)ringscall.c $(INCLUDES)
$(OBJ)chainscall.o:
	$(CC) -c -MT $(OBJ)chainscall.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)chainscall.o $(GUI)chainscall.c $(INCLUDES)
$(OBJ)msdcall.o:
	$(CC) -c -MT $(OBJ)msdcall.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)msdcall.o $(GUI)msdcall.c $(INCLUDES)
$(OBJ)spcall.o:
	$(CC) -c -MT $(OBJ)spcall.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)spcall.o $(GUI)spcall.c $(INCLUDES)
$(OBJ)main.o:
	$(CC) -c -MT $(OBJ)main.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DOMP) $(DEFS) -o $(OBJ)main.o $(GUI)main.c $(INCLUDES)

# Project
$(OBJ)read_cif.o:
	$(CC) -c -MT $(OBJ)read_cif.o -MD -MP -MF $(CFLAGS) $(DOMP) $(DEFS) -o $(OBJ)read_cif.o $(PROJ)readers/read_cif.c $(INCLUDES)
$(OBJ)read_coord.o:
	$(CC) -c -MT $(OBJ)read_coord.o -MD -MP -MF $(CFLAGS) $(DOMP) $(DEFS) -o $(OBJ)read_coord.o $(PROJ)readers/read_coord.c $(INCLUDES)
$(OBJ)read_xyz.o:
	$(CC) -c -MT $(OBJ)read_xyz.o -MD -MP -MF $(CFLAGS) $(DOMP) $(DEFS) -o $(OBJ)read_xyz.o $(PROJ)readers/read_xyz.c $(INCLUDES)
$(OBJ)read_c3d.o:
	$(CC) -c -MT $(OBJ)read_c3d.o -MD -MP -MF $(CFLAGS) $(DOMP) $(DEFS) -o $(OBJ)read_c3d.o $(PROJ)readers/read_c3d.c $(INCLUDES)
$(OBJ)read_trj.o:
	$(CC) -c -MT $(OBJ)read_trj.o -MD -MP -MF $(CFLAGS) $(DOMP) $(DEFS) -o $(OBJ)read_trj.o $(PROJ)readers/read_trj.c $(INCLUDES)
$(OBJ)read_vas.o:
	$(CC) -c -MT $(OBJ)read_vas.o -MD -MP -MF $(CFLAGS) $(DOMP) $(DEFS) -o $(OBJ)read_vas.o $(PROJ)readers/read_vas.c $(INCLUDES)
$(OBJ)read_pdb.o:
	$(CC) -c -MT $(OBJ)read_pdb.o -MD -MP -MF $(CFLAGS) $(DOMP) $(DEFS) -o $(OBJ)read_pdb.o $(PROJ)readers/read_pdb.c $(INCLUDES)
$(OBJ)read_hist.o:
	$(CC) -c -MT $(OBJ)read_hist.o -MD -MP -MF $(CFLAGS) $(DOMP) $(DEFS) -o $(OBJ)read_hist.o $(PROJ)readers/read_hist.c $(INCLUDES)
$(OBJ)read_npt.o:
	$(CC) -c -MT $(OBJ)read_npt.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)read_npt.o $(PROJ)readers/read_npt.c $(INCLUDES)
$(OBJ)update_p.o:
	$(CC) -c -MT $(OBJ)update_p.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)update_p.o $(PROJ)update_p.c $(INCLUDES)
$(OBJ)init_p.o:
	$(CC) -c -MT $(OBJ)init_p.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)init_p.o $(PROJ)init_p.c $(INCLUDES)
$(OBJ)debugio.o:
	$(CC) -c -MT $(OBJ)debugio.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)debugio.o $(PROJ)debugio.c $(INCLUDES)
$(OBJ)read_field.o:
	$(CC) -c -MT $(OBJ)read_field.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)read_field.o $(PROJ)read_field.c $(INCLUDES)
$(OBJ)read_qm.o:
	$(CC) -c -MT $(OBJ)read_qm.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)read_qm.o $(PROJ)read_qm.c $(INCLUDES)
$(OBJ)read_opengl.o:
	$(CC) -c -MT $(OBJ)read_opengl.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)read_opengl.o $(PROJ)read_opengl.c $(INCLUDES)
$(OBJ)read_curve.o:
	$(CC) -c -MT $(OBJ)read_curve.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)read_curve.o $(PROJ)read_curve.c $(INCLUDES)
$(OBJ)read_mol.o:
	$(CC) -c -MT $(OBJ)read_mol.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)read_mol.o $(PROJ)read_mol.c $(INCLUDES)
$(OBJ)read_bond.o:
	$(CC) -c -MT $(OBJ)read_bond.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)read_bond.o $(PROJ)read_bond.c $(INCLUDES)
$(OBJ)open_p.o:
	$(CC) -c -MT $(OBJ)open_p.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)open_p.o $(PROJ)open_p.c $(INCLUDES)
$(OBJ)close_p.o:
	$(CC) -c -MT $(OBJ)close_p.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)close_p.o $(PROJ)close_p.c $(INCLUDES)
$(OBJ)save_field.o:
	$(CC) -c -MT $(OBJ)save_field.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)save_field.o $(PROJ)save_field.c $(INCLUDES)
$(OBJ)save_qm.o:
	$(CC) -c -MT $(OBJ)save_qm.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)save_qm.o $(PROJ)save_qm.c $(INCLUDES)
$(OBJ)save_opengl.o:
	$(CC) -c -MT $(OBJ)save_opengl.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)save_opengl.o $(PROJ)save_opengl.c $(INCLUDES)
$(OBJ)save_curve.o:
	$(CC) -c -MT $(OBJ)save_curve.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)save_curve.o $(PROJ)save_curve.c $(INCLUDES)
$(OBJ)save_mol.o:
	$(CC) -c -MT $(OBJ)save_mol.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)save_mol.o $(PROJ)save_mol.c $(INCLUDES)
$(OBJ)save_bond.o:
	$(CC) -c -MT $(OBJ)save_bond.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)save_bond.o $(PROJ)save_bond.c $(INCLUDES)
$(OBJ)save_p.o:
	$(CC) -c -MT $(OBJ)save_p.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)save_p.o $(PROJ)save_p.c $(INCLUDES)
$(OBJ)project.o:
	$(CC) -c -MT $(OBJ)project.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)project.o $(PROJ)project.c $(INCLUDES)

# calc
$(OBJ)calc.o:
	$(CC) -c -MT $(OBJ)calc.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)calc.o $(CALC)calc.c $(INCLUDES)

# Curves
$(OBJ)tab-1.o:
	$(CC) -c -MT $(OBJ)tab-1.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)tab-1.o $(CURVE)tab-1.c $(INCLUDES)
$(OBJ)tab-2.o:
	$(CC) -c -MT $(OBJ)tab-2.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)tab-2.o $(CURVE)tab-2.c $(INCLUDES)
$(OBJ)tab-3.o:
	$(CC) -c -MT $(OBJ)tab-3.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)tab-3.o $(CURVE)tab-3.c $(INCLUDES)
$(OBJ)tab-4.o:
	$(CC) -c -MT $(OBJ)tab-4.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)tab-4.o $(CURVE)tab-4.c $(INCLUDES)
$(OBJ)cedit.o:
	$(CC) -c -MT $(OBJ)cedit.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)cedit.o $(CURVE)cedit.c $(INCLUDES)
$(OBJ)datab.o:
	$(CC) -c -MT $(OBJ)datab.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)datab.o $(CURVE)datab.c $(INCLUDES)
$(OBJ)cwidget.o:
	$(CC) -c -MT $(OBJ)cwidget.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)cwidget.o $(CURVE)cwidget.c $(INCLUDES)
$(OBJ)glyph.o:
	$(CC) -c -MT $(OBJ)glyph.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)glyph.o $(CURVE)glyph.c $(INCLUDES)
$(OBJ)labels.o:
	$(CC) -c -MT $(OBJ)labels.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)labels.o $(CURVE)labels.c $(INCLUDES)
$(OBJ)title.o:
	$(CC) -c -MT $(OBJ)title.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)title.o $(CURVE)title.c $(INCLUDES)
$(OBJ)legend.o:
	$(CC) -c -MT $(OBJ)legend.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)legend.o $(CURVE)legend.c $(INCLUDES)
$(OBJ)xaxis.o:
	$(CC) -c -MT $(OBJ)xaxis.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)xaxis.o $(CURVE)xaxis.c $(INCLUDES)
$(OBJ)yaxis.o:
	$(CC) -c -MT $(OBJ)yaxis.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)yaxis.o $(CURVE)yaxis.c $(INCLUDES)
$(OBJ)frame.o:
	$(CC) -c -MT $(OBJ)frame.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)frame.o $(CURVE)frame.c $(INCLUDES)
$(OBJ)draw.o:
	$(CC) -c -MT $(OBJ)draw.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)draw.o $(CURVE)draw.c $(INCLUDES)
$(OBJ)show.o:
	$(CC) -c -MT $(OBJ)show.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)show.o $(CURVE)show.c $(INCLUDES)
$(OBJ)w_data.o:
	$(CC) -c -MT $(OBJ)w_data.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_data.o $(CURVE)w_data.c $(INCLUDES)
$(OBJ)w_img.o:
	$(CC) -c -MT $(OBJ)w_img.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)w_img.o $(CURVE)w_img.c $(INCLUDES)
$(OBJ)m_curve.o:
	$(CC) -c -MT $(OBJ)m_curve.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)m_curve.o $(CURVE)m_curve.c $(INCLUDES)
$(OBJ)w_curve.o:
	$(CC) -c -MT $(OBJ)w_curve.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)w_curve.o $(CURVE)w_curve.c $(INCLUDES)
$(OBJ)curve.o:
	$(CC) -c -MT $(OBJ)curve.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)curve.o $(CURVE)curve.c $(INCLUDES)

#DL Poly force field
$(OBJ)dlp_control.o:
	$(CC) -c -MT $(OBJ)dlp_control.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)dlp_control.o $(DLPOLY)dlp_control.c $(INCLUDES)
$(OBJ)dlp_init.o:
	$(CC) -c -MT $(OBJ)dlp_init.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)dlp_init.o $(DLPOLY)dlp_init.c $(INCLUDES)
$(OBJ)dlp_active.o:
	$(CC) -c -MT $(OBJ)dlp_active.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)dlp_active.o $(DLPOLY)dlp_active.c $(INCLUDES)
$(OBJ)dlp_copy.o:
	$(CC) -c -MT $(OBJ)dlp_copy.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)dlp_copy.o $(DLPOLY)dlp_copy.c $(INCLUDES)
$(OBJ)dlp_comp.o:
	$(CC) -c -MT $(OBJ)dlp_comp.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)dlp_comp.o $(DLPOLY)dlp_comp.c $(INCLUDES)
$(OBJ)dlp_viz.o:
	$(CC) -c -MT $(OBJ)dlp_viz.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)dlp_viz.o $(DLPOLY)dlp_viz.c $(INCLUDES)
$(OBJ)dlp_mol.o:
	$(CC) -c -MT $(OBJ)dlp_mol.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)dlp_mol.o $(DLPOLY)dlp_mol.c $(INCLUDES)
$(OBJ)dlp_atom.o:
	$(CC) -c -MT $(OBJ)dlp_atom.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)dlp_atom.o $(DLPOLY)dlp_atom.c $(INCLUDES)
$(OBJ)dlp_ff_match.o:
	$(CC) -c -MT $(OBJ)dlp_ff_match.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)dlp_ff_match.o $(DLPOLY)dlp_ff_match.c $(INCLUDES)
$(OBJ)dlp_edit.o:
	$(CC) -c -MT $(OBJ)dlp_edit.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)dlp_edit.o $(DLPOLY)dlp_edit.c $(INCLUDES)
$(OBJ)dlp_print.o:
	$(CC) -c -MT $(OBJ)dlp_print.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)dlp_print.o $(DLPOLY)dlp_print.c $(INCLUDES)
$(OBJ)dlp_field.o:
	$(CC) -c -MT $(OBJ)dlp_field.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)dlp_field.o $(DLPOLY)dlp_field.c $(INCLUDES)

#LAMMPS
$(OBJ)la_print.o:
	$(CC) -c -MT $(OBJ)la_print.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)la_print.o $(LAMMPS)la_print.c $(INCLUDES)

#Force fields
$(OBJ)force_fields.o:
	$(CC) -c -MT $(OBJ)force_fields.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)force_fields.o $(FIELDS)force_fields.c $(INCLUDES)
$(OBJ)amber94.o:
	$(CC) -c -MT $(OBJ)amber94.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)amber94.o $(FIELDS)amber94.c $(INCLUDES)
$(OBJ)amber96.o:
	$(CC) -c -MT $(OBJ)amber96.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)amber96.o $(FIELDS)amber96.c $(INCLUDES)
$(OBJ)amber98.o:
	$(CC) -c -MT $(OBJ)amber98.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)amber98.o $(FIELDS)amber98.c $(INCLUDES)
$(OBJ)amber99.o:
	$(CC) -c -MT $(OBJ)amber99.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)amber99.o $(FIELDS)amber99.c $(INCLUDES)
$(OBJ)cff91.o:
	$(CC) -c -MT $(OBJ)cff91.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)cff91.o $(FIELDS)cff91.c $(INCLUDES)
$(OBJ)charmm22_prot.o:
	$(CC) -c -MT $(OBJ)charmm22_prot.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)charmm22_prot.o $(FIELDS)charmm22_prot.c $(INCLUDES)
$(OBJ)charmm22_prot_metals.o:
	$(CC) -c -MT $(OBJ)charmm22_prot_metals.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)charmm22_prot_metals.o $(FIELDS)charmm22_prot_metals.c $(INCLUDES)
$(OBJ)charmm35_ethers.o:
	$(CC) -c -MT $(OBJ)charmm35_ethers.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)charmm35_ethers.o $(FIELDS)charmm35_ethers.c $(INCLUDES)
$(OBJ)charmm36_carb.o:
	$(CC) -c -MT $(OBJ)charmm36_carb.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)charmm36_carb.o $(FIELDS)charmm36_carb.c $(INCLUDES)
$(OBJ)charmm36_cgenff.o:
	$(CC) -c -MT $(OBJ)charmm36_cgenff.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)charmm36_cgenff.o $(FIELDS)charmm36_cgenff.c $(INCLUDES)
$(OBJ)charmm36_lipid.o:
	$(CC) -c -MT $(OBJ)charmm36_lipid.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)charmm36_lipid.o $(FIELDS)charmm36_lipid.c $(INCLUDES)
$(OBJ)charmm36_na.o:
	$(CC) -c -MT $(OBJ)charmm36_na.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)charmm36_na.o $(FIELDS)charmm36_na.c $(INCLUDES)
$(OBJ)charmm36_prot.o:
	$(CC) -c -MT $(OBJ)charmm36_prot.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)charmm36_prot.o $(FIELDS)charmm36_prot.c $(INCLUDES)
$(OBJ)charmm36_prot_metals.o:
	$(CC) -c -MT $(OBJ)charmm36_prot_metals.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)charmm36_prot_metals.o $(FIELDS)charmm36_prot_metals.c $(INCLUDES)
$(OBJ)charmm_silicates.o:
	$(CC) -c -MT $(OBJ)charmm_silicates.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)charmm_silicates.o $(FIELDS)charmm_silicates.c $(INCLUDES)
$(OBJ)compass.o:
	$(CC) -c -MT $(OBJ)compass.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)compass.o $(FIELDS)compass.c $(INCLUDES)
$(OBJ)cvff.o:
	$(CC) -c -MT $(OBJ)cvff.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)cvff.o $(FIELDS)cvff.c $(INCLUDES)
$(OBJ)cvff_aug.o:
	$(CC) -c -MT $(OBJ)cvff_aug.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)cvff_aug.o $(FIELDS)cvff_aug.c $(INCLUDES)
$(OBJ)oplsaap.o:
	$(CC) -c -MT $(OBJ)oplsaap.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)oplsaap.o $(FIELDS)oplsaap.c $(INCLUDES)
$(OBJ)oplsaar.o:
	$(CC) -c -MT $(OBJ)oplsaar.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)oplsaar.o $(FIELDS)oplsaar.c $(INCLUDES)
$(OBJ)pcff.o:
	$(CC) -c -MT $(OBJ)pcff.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)pcff.o $(FIELDS)pcff.c $(INCLUDES)

#CPMD input
$(OBJ)cpmd_print.o:
	$(CC) -c -MT $(OBJ)cpmd_print.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cpmd_print.o $(CPMD)cpmd_print.c $(INCLUDES)
$(OBJ)cpmd_nose.o:
	$(CC) -c -MT $(OBJ)cpmd_nose.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cpmd_nose.o $(CPMD)cpmd_nose.c $(INCLUDES)
$(OBJ)cpmd_restart.o:
	$(CC) -c -MT $(OBJ)cpmd_restart.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cpmd_restart.o $(CPMD)cpmd_restart.c $(INCLUDES)
$(OBJ)cpmd_atoms.o:
	$(CC) -c -MT $(OBJ)cpmd_atoms.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cpmd_atoms.o $(CPMD)cpmd_atoms.c $(INCLUDES)
$(OBJ)cpmd_init.o:
	$(CC) -c -MT $(OBJ)cpmd_init.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cpmd_init.o $(CPMD)cpmd_init.c $(INCLUDES)

#CP2K input
$(OBJ)cp2k_print.o:
	$(CC) -c -MT $(OBJ)cp2k_print.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cp2k_print.o $(CP2K)cp2k_print.c $(INCLUDES)
$(OBJ)cp2k_molopt-basis.o:
	$(CC) -c -MT $(OBJ)cp2k_molopt-basis.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cp2k_molopt-basis.o $(CP2K)cp2k_molopt-basis.c $(INCLUDES)
$(OBJ)cp2k_gth-basis.o:
	$(CC) -c -MT $(OBJ)cp2k_gth-basis.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cp2k_gth-basis.o $(CP2K)cp2k_gth-basis.c $(INCLUDES)
$(OBJ)cp2k_basis.o:
	$(CC) -c -MT $(OBJ)cp2k_basis.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cp2k_basis.o $(CP2K)cp2k_basis.c $(INCLUDES)
$(OBJ)cp2k_pot.o:
	$(CC) -c -MT $(OBJ)cp2k_pot.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cp2k_pot.o $(CP2K)cp2k_pot.c $(INCLUDES)
$(OBJ)cp2k_files.o:
	$(CC) -c -MT $(OBJ)cp2k_files.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cp2k_files.o $(CP2K)cp2k_files.c $(INCLUDES)
$(OBJ)cp2k_mol.o:
	$(CC) -c -MT $(OBJ)cp2k_mol.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cp2k_mol.o $(CP2K)cp2k_mol.c $(INCLUDES)
$(OBJ)cp2k_init.o:
	$(CC) -c -MT $(OBJ)cp2k_init.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cp2k_init.o $(CP2K)cp2k_init.c $(INCLUDES)

# OpenGL :: Window and menus
$(OBJ)color_box.o:
	$(CC) -c -MT $(OBJ)color_box.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)color_box.o $(GLWIN)color_box.c $(INCLUDES)
$(OBJ)m_style.o:
	$(CC) -c -MT $(OBJ)m_style.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)m_style.o $(GLWIN)m_style.c $(INCLUDES)
$(OBJ)m_map.o:
	$(CC) -c -MT $(OBJ)m_map.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_map.o $(GLWIN)m_map.c $(INCLUDES)
$(OBJ)m_render.o:
	$(CC) -c -MT $(OBJ)m_render.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_render.o $(GLWIN)m_render.c $(INCLUDES)
$(OBJ)m_quality.o:
	$(CC) -c -MT $(OBJ)m_quality.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_quality.o $(GLWIN)m_quality.c $(INCLUDES)
$(OBJ)m_atoms.o:
	$(CC) -c -MT $(OBJ)m_atoms.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_atoms.o $(GLWIN)m_atoms.c $(INCLUDES)
$(OBJ)m_bonds.o:
	$(CC) -c -MT $(OBJ)m_bonds.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_bonds.o $(GLWIN)m_bonds.c $(INCLUDES)
$(OBJ)m_clones.o:
	$(CC) -c -MT $(OBJ)m_clones.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_clones.o $(GLWIN)m_clones.c $(INCLUDES)
$(OBJ)m_box.o:
	$(CC) -c -MT $(OBJ)m_box.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_box.o $(GLWIN)m_box.c $(INCLUDES)
$(OBJ)m_coord.o:
	$(CC) -c -MT $(OBJ)m_coord.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_coord.o $(GLWIN)m_coord.c $(INCLUDES)
$(OBJ)m_poly.o:
	$(CC) -c -MT $(OBJ)m_poly.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_poly.o $(GLWIN)m_poly.c $(INCLUDES)
$(OBJ)m_tools.o:
	$(CC) -c -MT $(OBJ)m_tools.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_tools.o $(GLWIN)m_tools.c $(INCLUDES)
$(OBJ)m_edit.o:
	$(CC) -c -MT $(OBJ)m_edit.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_edit.o $(GLWIN)m_edit.c $(INCLUDES)
$(OBJ)m_rep.o:
	$(CC) -c -MT $(OBJ)m_rep.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_rep.o $(GLWIN)m_rep.c $(INCLUDES)
$(OBJ)m_proj.o:
	$(CC) -c -MT $(OBJ)m_proj.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_proj.o $(GLWIN)m_proj.c $(INCLUDES)
$(OBJ)m_back.o:
	$(CC) -c -MT $(OBJ)m_back.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_back.o $(GLWIN)m_back.c $(INCLUDES)
$(OBJ)m_axis.o:
	$(CC) -c -MT $(OBJ)m_axis.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_axis.o $(GLWIN)m_axis.c $(INCLUDES)
$(OBJ)m_anim.o:
	$(CC) -c -MT $(OBJ)m_anim.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)m_anim.o $(GLWIN)m_anim.c $(INCLUDES)
$(OBJ)menu_bar.o:
	$(CC) -c -MT $(OBJ)menu_bar.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)menu_bar.o $(GLWIN)menu_bar.c $(INCLUDES)
$(OBJ)w_colors.o:
	$(CC) -c -MT $(OBJ)w_colors.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_colors.o $(GLWIN)w_colors.c $(INCLUDES)
$(OBJ)w_atoms.o:
	$(CC) -c -MT $(OBJ)w_atoms.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_atoms.o $(GLWIN)w_atoms.c $(INCLUDES)
$(OBJ)w_labels.o:
	$(CC) -c -MT $(OBJ)w_labels.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_labels.o $(GLWIN)w_labels.c $(INCLUDES)
$(OBJ)w_search.o:
	$(CC) -c -MT $(OBJ)w_search.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_search.o $(GLWIN)w_search.c $(INCLUDES)
$(OBJ)w_periodic.o:
	$(CC) -c -MT $(OBJ)w_periodic.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_periodic.o $(GLWIN)w_periodic.c $(INCLUDES)
$(OBJ)w_library.o:
	$(CC) -c -MT $(OBJ)w_library.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)w_library.o $(GLWIN)w_library.c $(INCLUDES)
$(OBJ)w_cutoffs.o:
	$(CC) -c -MT $(OBJ)w_cutoffs.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_cutoffs.o $(GLWIN)w_cutoffs.c $(INCLUDES)
$(OBJ)w_bonds.o:
	$(CC) -c -MT $(OBJ)w_bonds.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_bonds.o $(GLWIN)w_bonds.c $(INCLUDES)
$(OBJ)w_rings.o:
	$(CC) -c -MT $(OBJ)w_rings.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_rings.o $(GLWIN)w_rings.c $(INCLUDES)
$(OBJ)w_chains.o:
	$(CC) -c -MT $(OBJ)w_chains.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_chains.o $(GLWIN)w_chains.c $(INCLUDES)
$(OBJ)w_coord.o:
	$(CC) -c -MT $(OBJ)w_coord.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_coord.o $(GLWIN)w_coord.c $(INCLUDES)
$(OBJ)w_box.o:
	$(CC) -c -MT $(OBJ)w_box.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_box.o $(GLWIN)w_box.c $(INCLUDES)
$(OBJ)w_axis.o:
	$(CC) -c -MT $(OBJ)w_axis.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_axis.o $(GLWIN)w_axis.c $(INCLUDES)
$(OBJ)w_measures.o:
	$(CC) -c -MT $(OBJ)w_measures.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_measures.o $(GLWIN)w_measures.c $(INCLUDES)
$(OBJ)w_volumes.o:
	$(CC) -c -MT $(OBJ)w_volumes.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_volumes.o $(GLWIN)w_volumes.c $(INCLUDES)
$(OBJ)w_advance.o:
	$(CC) -c -MT $(OBJ)w_advance.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)w_advance.o $(GLWIN)w_advance.c $(INCLUDES)
$(OBJ)w_sequencer.o:
	$(CC) -c -MT $(OBJ)w_sequencer.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)w_sequencer.o $(GLWIN)w_sequencer.c $(INCLUDES)
$(OBJ)w_spiner.o:
	$(CC) -c -MT $(OBJ)w_spiner.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)w_spiner.o $(GLWIN)w_spiner.c $(INCLUDES)
$(OBJ)w_encode.o:
	$(CC) -c -MT $(OBJ)w_encode.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)w_encode.o $(GLWIN)w_encode.c $(INCLUDES)
$(OBJ)w_record.o:
	$(CC) -c -MT $(OBJ)w_record.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)w_record.o $(GLWIN)w_record.c $(INCLUDES)
$(OBJ)initcoord.o:
	$(CC) -c -MT $(OBJ)initcoord.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)initcoord.o $(GLWIN)initcoord.c $(INCLUDES)
$(OBJ)initmol.o:
	$(CC) -c -MT $(OBJ)initmol.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)initmol.o $(GLWIN)initmol.c $(INCLUDES)
$(OBJ)initring.o:
	$(CC) -c -MT $(OBJ)initring.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)initring.o $(GLWIN)initring.c $(INCLUDES)
$(OBJ)initchain.o:
	$(CC) -c -MT $(OBJ)initchain.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)initchain.o $(GLWIN)initchain.c $(INCLUDES)
$(OBJ)popup.o:
	$(CC) -c -MT $(OBJ)popup.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)popup.o $(GLWIN)popup.c $(INCLUDES)
$(OBJ)glwindow.o:
	$(CC) -c -MT $(OBJ)glwindow.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)glwindow.o $(GLWIN)glwindow.c $(INCLUDES)

# OpenGL :: Cell edit
$(OBJ)cell_shift.o:
	$(CC) -c -MT $(OBJ)cell_shift.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cell_shift.o $(GLEDIT)cell_shift.c $(INCLUDES)
$(OBJ)cell_extra.o:
	$(CC) -c -MT $(OBJ)cell_extra.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cell_extra.o $(GLEDIT)cell_extra.c $(INCLUDES)
$(OBJ)cell_super.o:
	$(CC) -c -MT $(OBJ)cell_super.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cell_super.o $(GLEDIT)cell_super.c $(INCLUDES)
$(OBJ)cell_density.o:
	$(CC) -c -MT $(OBJ)cell_density.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cell_density.o $(GLEDIT)cell_density.c $(INCLUDES)
$(OBJ)cell_cut.o:
	$(CC) -c -MT $(OBJ)cell_cut.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cell_cut.o $(GLEDIT)cell_cut.c $(INCLUDES)
$(OBJ)cell_pixel.o:
	$(CC) -c -MT $(OBJ)cell_pixel.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cell_pixel.o $(GLEDIT)cell_pixel.c $(INCLUDES)
$(OBJ)cell_edit.o:
	$(CC) -c -MT $(OBJ)cell_edit.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cell_edit.o $(GLEDIT)cell_edit.c $(INCLUDES)

# OpenGL :: Atom edit
$(OBJ)atom_action.o:
	$(CC) -c -MT $(OBJ)atom_action.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)atom_action.o $(GLEDIT)atom_action.c $(INCLUDES)
$(OBJ)atom_coord.o:
	$(CC) -c -MT $(OBJ)atom_coord.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)atom_coord.o $(GLEDIT)atom_coord.c $(INCLUDES)
$(OBJ)atom_geo.o:
	$(CC) -c -MT $(OBJ)atom_geo.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)atom_geo.o $(GLEDIT)atom_geo.c $(INCLUDES)
$(OBJ)atom_insert.o:
	$(CC) -c -MT $(OBJ)atom_insert.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)atom_insert.o $(GLEDIT)atom_insert.c $(INCLUDES)
$(OBJ)atom_move.o:
	$(CC) -c -MT $(OBJ)atom_move.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)atom_move.o $(GLEDIT)atom_move.c $(INCLUDES)
$(OBJ)atom_object.o:
	$(CC) -c -MT $(OBJ)atom_object.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)atom_object.o $(GLEDIT)atom_object.c $(INCLUDES)
$(OBJ)atom_remove.o:
	$(CC) -c -MT $(OBJ)atom_remove.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)atom_remove.o $(GLEDIT)atom_remove.c $(INCLUDES)
$(OBJ)atom_search.o:
	$(CC) -c -MT $(OBJ)atom_search.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)atom_search.o $(GLEDIT)atom_search.c $(INCLUDES)
$(OBJ)atom_species.o:
	$(CC) -c -MT $(OBJ)atom_species.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)atom_species.o $(GLEDIT)atom_species.c $(INCLUDES)
$(OBJ)atom_edit.o:
	$(CC) -c -MT $(OBJ)atom_edit.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)atom_edit.o $(GLEDIT)atom_edit.c $(INCLUDES)

# OpenGL :: Crystal builder
$(OBJ)cbuild_action.o:
	$(CC) -c -MT $(OBJ)cbuild_action.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cbuild_action.o $(GLEDIT)cbuild_action.c $(INCLUDES)
$(OBJ)cbuild_sg.o:
	$(CC) -c -MT $(OBJ)cbuild_sg.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cbuild_sg.o $(GLEDIT)cbuild_sg.c $(INCLUDES)
$(OBJ)cbuild_info.o:
	$(CC) -c -MT $(OBJ)cbuild_info.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cbuild_info.o $(GLEDIT)cbuild_info.c $(INCLUDES)
$(OBJ)cbuild_edit.o:
	$(CC) -c -MT $(OBJ)cbuild_edit.o -MD -MP -MF $(CPPFLAGS) $(CFLAGS) $(DEFS) -o $(OBJ)cbuild_edit.o $(GLEDIT)cbuild_edit.c $(INCLUDES)

# OpenGL :: Draw
$(OBJ)d_bonds.o:
	$(CC) -c -MT $(OBJ)d_bonds.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)d_bonds.o $(GLDRAW)d_bonds.c $(INCLUDES)
$(OBJ)d_atoms.o:
	$(CC) -c -MT $(OBJ)d_atoms.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)d_atoms.o $(GLDRAW)d_atoms.c $(INCLUDES)
$(OBJ)d_label.o:
	$(CC) -c -MT $(OBJ)d_label.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)d_label.o $(GLDRAW)d_label.c $(INCLUDES)
$(OBJ)d_selection.o:
	$(CC) -c -MT $(OBJ)d_selection.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)d_selection.o $(GLDRAW)d_selection.c $(INCLUDES)
$(OBJ)d_poly.o:
	$(CC) -c -MT $(OBJ)d_poly.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)d_poly.o $(GLDRAW)d_poly.c $(INCLUDES)
$(OBJ)d_rings.o:
	$(CC) -c -MT $(OBJ)d_rings.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)d_rings.o $(GLDRAW)d_rings.c $(INCLUDES)
$(OBJ)d_box.o:
	$(CC) -c -MT $(OBJ)d_box.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)d_box.o $(GLDRAW)d_box.c $(INCLUDES)
$(OBJ)d_axis.o:
	$(CC) -c -MT $(OBJ)d_axis.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)d_axis.o $(GLDRAW)d_axis.c $(INCLUDES)
$(OBJ)d_measures.o:
	$(CC) -c -MT $(OBJ)d_measures.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)d_measures.o $(GLDRAW)d_measures.c $(INCLUDES)
$(OBJ)ogl_text.o:
	$(CC) -c -MT $(OBJ)ogl_text.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)ogl_text.o $(GLDRAW)ogl_text.c $(INCLUDES)
$(OBJ)movie.o:
	$(CC) -c -MT $(OBJ)movie.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)movie.o $(GLDRAW)movie.c $(INCLUDES)
$(OBJ)image.o:
	$(CC) -c -MT $(OBJ)image.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)image.o $(GLDRAW)image.c $(INCLUDES)

# OpengGL :: GL
$(OBJ)glview.o:
	$(CC) -c -MT $(OBJ)glview.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)glview.o $(OGL)glview.c $(INCLUDES)
$(OBJ)arcball.o:
	$(CC) -c -MT $(OBJ)arcball.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)arcball.o $(OGL)arcball.c $(INCLUDES)
$(OBJ)ogl_utils.o:
	$(CC) -c -MT $(OBJ)ogl_utils.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)ogl_utils.o $(OGL)ogl_utils.c $(INCLUDES)
$(OBJ)ogl_shaders.o:
	$(CC) -c -MT $(OBJ)ogl_shaders.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)ogl_shaders.o $(OGL)ogl_shaders.c $(INCLUDES)
$(OBJ)ogl_shading.o:
	$(CC) -c -MT $(OBJ)ogl_shading.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)ogl_shading.o $(OGL)ogl_shading.c $(INCLUDES)
$(OBJ)selection.o:
	$(CC) -c -MT $(OBJ)selection.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)selection.o $(OGL)selection.c $(INCLUDES)
$(OBJ)ogl_draw.o:
	$(CC) -c -MT $(OBJ)ogl_draw.o -MD -MP -MF $(CFLAGS) $(DEFS) -o $(OBJ)ogl_draw.o $(OGL)ogl_draw.c $(INCLUDES)


# Win file:

$(OBJ)winatomes.o:
	$(WINDRES) $(SRC)atomes.rc -o $(OBJ)winatomes.o
