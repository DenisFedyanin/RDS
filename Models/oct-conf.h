// DO NOT EDIT!  Generated automatically from oct-conf.in.h by Make.
/*

Copyright (C) 1996-2015 John W. Eaton

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

Octave is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, see
<http://www.gnu.org/licenses/>.

*/

#if !defined (octave_conf_h)
#define octave_conf_h 1

#ifndef OCTAVE_CONF_ALL_CFLAGS
#define OCTAVE_CONF_ALL_CFLAGS ""
#endif

#ifndef OCTAVE_CONF_ALL_CXXFLAGS
#define OCTAVE_CONF_ALL_CXXFLAGS ""
#endif

#ifndef OCTAVE_CONF_ALL_FFLAGS
#define OCTAVE_CONF_ALL_FFLAGS "-O -mieee-fp"
#endif

#ifndef OCTAVE_CONF_ALL_LDFLAGS
#define OCTAVE_CONF_ALL_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_AMD_CPPFLAGS
#define OCTAVE_CONF_AMD_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_AMD_LDFLAGS
#define OCTAVE_CONF_AMD_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_AMD_LIBS
#define OCTAVE_CONF_AMD_LIBS "-lamd"
#endif

#ifndef OCTAVE_CONF_ARFLAGS
#define OCTAVE_CONF_ARFLAGS "cru"
#endif

#ifndef OCTAVE_CONF_AR
#define OCTAVE_CONF_AR "i686-w64-mingw32-ar"
#endif

#ifndef OCTAVE_CONF_ARPACK_CPPFLAGS
#define OCTAVE_CONF_ARPACK_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_ARPACK_LDFLAGS
#define OCTAVE_CONF_ARPACK_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_ARPACK_LIBS
#define OCTAVE_CONF_ARPACK_LIBS "-larpack"
#endif

#ifndef OCTAVE_CONF_BLAS_LIBS
#define OCTAVE_CONF_BLAS_LIBS "-lopenblas"
#endif

#ifndef OCTAVE_CONF_CAMD_CPPFLAGS
#define OCTAVE_CONF_CAMD_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_CAMD_LDFLAGS
#define OCTAVE_CONF_CAMD_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_CAMD_LIBS
#define OCTAVE_CONF_CAMD_LIBS "-lcamd"
#endif

#ifndef OCTAVE_CONF_CARBON_LIBS
#define OCTAVE_CONF_CARBON_LIBS ""
#endif

#ifndef OCTAVE_CONF_CC
#define OCTAVE_CONF_CC "i686-w64-mingw32-gcc"
#endif

// FIXME: OCTAVE_CONF_CCC_VERSION is deprecated.  Remove in version 3.12
#ifndef OCTAVE_CONF_CC_VERSION
#define OCTAVE_CONF_CC_VERSION %OCTAVE_CONF_CC_VERSION%
#endif

#ifndef OCTAVE_CONF_CCOLAMD_CPPFLAGS
#define OCTAVE_CONF_CCOLAMD_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_CCOLAMD_LDFLAGS
#define OCTAVE_CONF_CCOLAMD_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_CCOLAMD_LIBS
#define OCTAVE_CONF_CCOLAMD_LIBS "-lccolamd"
#endif

#ifndef OCTAVE_CONF_CFLAGS
#define OCTAVE_CONF_CFLAGS "-g -O2"
#endif

#ifndef OCTAVE_CONF_CHOLMOD_CPPFLAGS
#define OCTAVE_CONF_CHOLMOD_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_CHOLMOD_LDFLAGS
#define OCTAVE_CONF_CHOLMOD_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_CHOLMOD_LIBS
#define OCTAVE_CONF_CHOLMOD_LIBS "-lcholmod"
#endif

#ifndef OCTAVE_CONF_COLAMD_CPPFLAGS
#define OCTAVE_CONF_COLAMD_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_COLAMD_LDFLAGS
#define OCTAVE_CONF_COLAMD_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_COLAMD_LIBS
#define OCTAVE_CONF_COLAMD_LIBS "-lcolamd"
#endif

#ifndef OCTAVE_CONF_CPICFLAG
#define OCTAVE_CONF_CPICFLAG ""
#endif

#ifndef OCTAVE_CONF_CPPFLAGS
#define OCTAVE_CONF_CPPFLAGS "-I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include"
#endif

#ifndef OCTAVE_CONF_CURL_CPPFLAGS
#define OCTAVE_CONF_CURL_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_CURL_LDFLAGS
#define OCTAVE_CONF_CURL_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_CURL_LIBS
#define OCTAVE_CONF_CURL_LIBS "-lcurl"
#endif

#ifndef OCTAVE_CONF_CXSPARSE_CPPFLAGS
#define OCTAVE_CONF_CXSPARSE_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_CXSPARSE_LDFLAGS
#define OCTAVE_CONF_CXSPARSE_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_CXSPARSE_LIBS
#define OCTAVE_CONF_CXSPARSE_LIBS "-lcxsparse"
#endif

#ifndef OCTAVE_CONF_CXXCPP
#define OCTAVE_CONF_CXXCPP "i686-w64-mingw32-g++ -E"
#endif

#ifndef OCTAVE_CONF_CXXFLAGS
#define OCTAVE_CONF_CXXFLAGS "-g -O2"
#endif

#ifndef OCTAVE_CONF_CXXPICFLAG
#define OCTAVE_CONF_CXXPICFLAG ""
#endif

#ifndef OCTAVE_CONF_CXX
#define OCTAVE_CONF_CXX "i686-w64-mingw32-g++"
#endif

// FIXME: OCTAVE_CONF_CXX_VERSION is deprecated.  Remove in version 3.12
#ifndef OCTAVE_CONF_CXX_VERSION
#define OCTAVE_CONF_CXX_VERSION ""
#endif

#ifndef OCTAVE_CONF_DEFAULT_PAGER
#define OCTAVE_CONF_DEFAULT_PAGER "less"
#endif

#ifndef OCTAVE_CONF_DEFS
#define OCTAVE_CONF_DEFS "-DHAVE_CONFIG_H"
#endif

#ifndef OCTAVE_CONF_DL_LD
#define OCTAVE_CONF_DL_LD "i686-w64-mingw32-g++"
#endif

#ifndef OCTAVE_CONF_DL_LDFLAGS
#define OCTAVE_CONF_DL_LDFLAGS "-shared -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc"
#endif

#ifndef OCTAVE_CONF_DL_LIBS
#define OCTAVE_CONF_DL_LIBS ""
#endif

#ifndef OCTAVE_CONF_ENABLE_DYNAMIC_LINKING
#define OCTAVE_CONF_ENABLE_DYNAMIC_LINKING "yes"
#endif

#ifndef OCTAVE_CONF_EXEEXT
#define OCTAVE_CONF_EXEEXT ".exe"
#endif

#ifndef OCTAVE_CONF_GCC_VERSION
#define OCTAVE_CONF_GCC_VERSION "4.9.2"
#endif

#ifndef OCTAVE_CONF_GXX_VERSION
#define OCTAVE_CONF_GXX_VERSION "4.9.2"
#endif

#ifndef OCTAVE_CONF_F77
#define OCTAVE_CONF_F77 "i686-w64-mingw32-gfortran"
#endif

#ifndef OCTAVE_CONF_F77_FLOAT_STORE_FLAG
#define OCTAVE_CONF_F77_FLOAT_STORE_FLAG "-ffloat-store"
#endif

#ifndef OCTAVE_CONF_F77_INTEGER_8_FLAG
#define OCTAVE_CONF_F77_INTEGER_8_FLAG ""
#endif

#ifndef OCTAVE_CONF_FC
#define OCTAVE_CONF_FC %OCTAVE_CONF_FC%
#endif

#ifndef OCTAVE_CONF_FFLAGS
#define OCTAVE_CONF_FFLAGS "-O -mieee-fp"
#endif

#ifndef OCTAVE_CONF_FFTW3_CPPFLAGSS
#define OCTAVE_CONF_FFTW3_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_FFTW3_LDFLAGSS
#define OCTAVE_CONF_FFTW3_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_FFTW3_LIBS
#define OCTAVE_CONF_FFTW3_LIBS "-lfftw3"
#endif

#ifndef OCTAVE_CONF_FFTW3F_CPPFLAGSS
#define OCTAVE_CONF_FFTW3F_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_FFTW3F_LDFLAGSS
#define OCTAVE_CONF_FFTW3F_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_FFTW3F_LIBS
#define OCTAVE_CONF_FFTW3F_LIBS "-lfftw3f"
#endif

#ifndef OCTAVE_CONF_FLIBS
#define OCTAVE_CONF_FLIBS "-L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib -L/scratch/jwe/src/mxe-octave/usr/lib/gcc/i686-w64-mingw32/4.9.2 -L/scratch/jwe/src/mxe-octave/usr/lib/gcc/i686-w64-mingw32/4.9.2/../../../../i686-w64-mingw32/lib/../lib -L/scratch/jwe/src/mxe-octave/usr/lib/gcc/i686-w64-mingw32/4.9.2/../../../../i686-w64-mingw32/lib -lm -lgfortran -lmingw32 -lmoldname -lmingwex -lmsvcrt -lquadmath -ladvapi32 -lshell32 -luser32 -lkernel32"
#endif

#ifndef OCTAVE_CONF_FLTK_CPPFLAGS
#define OCTAVE_CONF_FLTK_CPPFLAGS "-I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include -mwindows -DWIN32 -DUSE_OPENGL32 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64"
#endif

#ifndef OCTAVE_CONF_FLTK_LDFLAGS
#define OCTAVE_CONF_FLTK_LDFLAGS "-L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib  -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc -lfltk_gl -lglu32 -lopengl32 -lfltk -lole32 -lcomctl32"
#endif

#ifndef OCTAVE_CONF_FLTK_LIBS
#define OCTAVE_CONF_FLTK_LIBS ""
#endif

#ifndef OCTAVE_CONF_FONTCONFIG_CPPFLAGS
#define OCTAVE_CONF_FONTCONFIG_CPPFLAGS "-I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/freetype2 -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/libpng16 "
#endif

#ifndef OCTAVE_CONF_FONTCONFIG_LIBS
#define OCTAVE_CONF_FONTCONFIG_LIBS "-L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib -lfontconfig -lfreetype "
#endif

#ifndef OCTAVE_CONF_FPICFLAG
#define OCTAVE_CONF_FPICFLAG ""
#endif

#ifndef OCTAVE_CONF_FT2_CPPFLAGS
#define OCTAVE_CONF_FT2_CPPFLAGS "-I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/freetype2 -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/libpng16 "
#endif

#ifndef OCTAVE_CONF_FT2_LIBS
#define OCTAVE_CONF_FT2_LIBS "-L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib -lfreetype "
#endif

#ifndef OCTAVE_CONF_GLPK_CPPFLAGS
#define OCTAVE_CONF_GLPK_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_GLPK_LDFLAGS
#define OCTAVE_CONF_GLPK_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_GLPK_LIBS
#define OCTAVE_CONF_GLPK_LIBS "-lglpk"
#endif

#ifndef OCTAVE_CONF_GNUPLOT
#define OCTAVE_CONF_GNUPLOT "gnuplot"
#endif

#ifndef OCTAVE_CONF_HDF5_CPPFLAGS
#define OCTAVE_CONF_HDF5_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_HDF5_LDFLAGS
#define OCTAVE_CONF_HDF5_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_HDF5_LIBS
#define OCTAVE_CONF_HDF5_LIBS "-lhdf5"
#endif

#ifndef OCTAVE_CONF_INCFLAGS
#define OCTAVE_CONF_INCFLAGS %OCTAVE_CONF_INCFLAGS%
#endif

#ifndef OCTAVE_CONF_INCLUDEDIR
#define OCTAVE_CONF_INCLUDEDIR "/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include"
#endif

#ifndef OCTAVE_CONF_LAPACK_LIBS
#define OCTAVE_CONF_LAPACK_LIBS "-llapack"
#endif

#ifndef OCTAVE_CONF_LDFLAGS
#define OCTAVE_CONF_LDFLAGS "-Wl,-rpath-link,/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib -L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib -Wl,--export-all-symbols"
#endif

#ifndef OCTAVE_CONF_LD_CXX
#define OCTAVE_CONF_LD_CXX "i686-w64-mingw32-g++"
#endif

#ifndef OCTAVE_CONF_LD_STATIC_FLAG
#define OCTAVE_CONF_LD_STATIC_FLAG ""
#endif

#ifndef OCTAVE_CONF_LEXLIB
#define OCTAVE_CONF_LEXLIB ""
#endif

#ifndef OCTAVE_CONF_LEX
#define OCTAVE_CONF_LEX "flex"
#endif

#ifndef OCTAVE_CONF_LFLAGS
#define OCTAVE_CONF_LFLAGS "-I"
#endif

#ifndef OCTAVE_CONF_LIBEXT
#define OCTAVE_CONF_LIBEXT "a"
#endif

#ifndef OCTAVE_CONF_LIBFLAGS
#define OCTAVE_CONF_LIBFLAGS "-L.."
#endif

#ifndef OCTAVE_CONF_LIBOCTAVE
#define OCTAVE_CONF_LIBOCTAVE "-loctave"
#endif

#ifndef OCTAVE_CONF_LIBOCTINTERP
#define OCTAVE_CONF_LIBOCTINTERP "-loctinterp"
#endif

#ifndef OCTAVE_CONF_LIBS
#define OCTAVE_CONF_LIBS "-lm   -lgdi32 -lws2_32 -luser32 -lkernel32 -lgdi32 -lws2_32 -luser32 -lkernel32"
#endif

#ifndef OCTAVE_CONF_LN_S
#define OCTAVE_CONF_LN_S "ln -s"
#endif

#ifndef OCTAVE_CONF_MAGICK_CPPFLAGS
#define OCTAVE_CONF_MAGICK_CPPFLAGS "-I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/GraphicsMagick -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/libxml2 -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include "
#endif

#ifndef OCTAVE_CONF_MAGICK_LDFLAGS
#define OCTAVE_CONF_MAGICK_LDFLAGS "-L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib "
#endif

#ifndef OCTAVE_CONF_MAGICK_LIBS
#define OCTAVE_CONF_MAGICK_LIBS "-lGraphicsMagick++ -lGraphicsMagick "
#endif

#ifndef OCTAVE_CONF_LLVM_CPPFLAGS
#define OCTAVE_CONF_LLVM_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_LLVM_LDFLAGS
#define OCTAVE_CONF_LLVM_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_LLVM_LIBS
#define OCTAVE_CONF_LLVM_LIBS ""
#endif

#ifndef OCTAVE_CONF_MKOCTFILE_DL_LDFLAGS
#define OCTAVE_CONF_MKOCTFILE_DL_LDFLAGS "-shared -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc"
#endif

#ifndef OCTAVE_CONF_OCTAVE_LINK_DEPS
#define OCTAVE_CONF_OCTAVE_LINK_DEPS ""
#endif

#ifndef OCTAVE_CONF_OCTAVE_LINK_OPTS
#define OCTAVE_CONF_OCTAVE_LINK_OPTS ""
#endif

#ifndef OCTAVE_CONF_OCTINCLUDEDIR
#define OCTAVE_CONF_OCTINCLUDEDIR "/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/octave-4.0.0/octave"
#endif

#ifndef OCTAVE_CONF_OCTLIBDIR
#define OCTAVE_CONF_OCTLIBDIR "/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib/octave/4.0.0"
#endif

#ifndef OCTAVE_CONF_OCT_LINK_DEPS
#define OCTAVE_CONF_OCT_LINK_DEPS ""
#endif

#ifndef OCTAVE_CONF_OCT_LINK_OPTS
#define OCTAVE_CONF_OCT_LINK_OPTS "-Wl,-rpath-link,/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib -L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib -Wl,--export-all-symbols "
#endif

#ifndef OCTAVE_CONF_OPENGL_LIBS
#define OCTAVE_CONF_OPENGL_LIBS "-L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib -lfontconfig -lfreetype  -lopengl32 -lglu32"
#endif

#ifndef OCTAVE_CONF_OSMESA_CPPFLAGS
#define OCTAVE_CONF_OSMESA_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_OSMESA_LDFLAGS
#define OCTAVE_CONF_OSMESA_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_OSMESA_LIBS
#define OCTAVE_CONF_OSMESA_LIBS "-lOSMesa"
#endif

#ifndef OCTAVE_CONF_PCRE_CPPFLAGS
#define OCTAVE_CONF_PCRE_CPPFLAGS "-I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include"
#endif

#ifndef OCTAVE_CONF_PCRE_LIBS
#define OCTAVE_CONF_PCRE_LIBS "-L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib -lpcre"
#endif

#ifndef OCTAVE_CONF_PREFIX
#define OCTAVE_CONF_PREFIX "/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32"
#endif

#ifndef OCTAVE_CONF_PTHREAD_CFLAGS
#define OCTAVE_CONF_PTHREAD_CFLAGS "-pthread"
#endif

#ifndef OCTAVE_CONF_PTHREAD_LIBS
#define OCTAVE_CONF_PTHREAD_LIBS ""
#endif

#ifndef OCTAVE_CONF_QHULL_CPPFLAGS
#define OCTAVE_CONF_QHULL_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_QHULL_LDFLAGS
#define OCTAVE_CONF_QHULL_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_QHULL_LIBS
#define OCTAVE_CONF_QHULL_LIBS "-lqhull"
#endif

#ifndef OCTAVE_CONF_QRUPDATE_CPPFLAGS
#define OCTAVE_CONF_QRUPDATE_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_QRUPDATE_LDFLAGS
#define OCTAVE_CONF_QRUPDATE_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_QRUPDATE_LIBS
#define OCTAVE_CONF_QRUPDATE_LIBS "-lqrupdate"
#endif

#ifndef OCTAVE_CONF_QT_CPPFLAGS
#define OCTAVE_CONF_QT_CPPFLAGS "-I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/QtNetwork -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/QtOpenGL -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/QtGui -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include -I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/QtCore "
#endif

#ifndef OCTAVE_CONF_QT_LDFLAGS
#define OCTAVE_CONF_QT_LDFLAGS "-L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib "
#endif

#ifndef OCTAVE_CONF_QT_LIBS
#define OCTAVE_CONF_QT_LIBS "-lQtNetwork4 -lQtOpenGL4 -lQtGui4 -lQtCore4  -lqscintilla2"
#endif

#ifndef OCTAVE_CONF_RANLIB
#define OCTAVE_CONF_RANLIB "i686-w64-mingw32-ranlib"
#endif

#ifndef OCTAVE_CONF_RDYNAMIC_FLAG
#define OCTAVE_CONF_RDYNAMIC_FLAG ""
#endif

#ifndef OCTAVE_CONF_READLINE_LIBS
#define OCTAVE_CONF_READLINE_LIBS "-lreadline"
#endif

#ifndef OCTAVE_CONF_SED
#define OCTAVE_CONF_SED "/bin/sed"
#endif

#ifndef OCTAVE_CONF_SHARED_LIBS
#define OCTAVE_CONF_SHARED_LIBS ""
#endif

#ifndef OCTAVE_CONF_SHLEXT
#define OCTAVE_CONF_SHLEXT "dll"
#endif

#ifndef OCTAVE_CONF_SHLEXT_VER
#define OCTAVE_CONF_SHLEXT_VER "dll.4.0.0"
#endif

#ifndef OCTAVE_CONF_SH_LD
#define OCTAVE_CONF_SH_LD "i686-w64-mingw32-g++"
#endif

#ifndef OCTAVE_CONF_SH_LDFLAGS
#define OCTAVE_CONF_SH_LDFLAGS "-shared -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--enable-auto-image-base"
#endif

#ifndef OCTAVE_CONF_SONAME_FLAGS
#define OCTAVE_CONF_SONAME_FLAGS "-Wl,--out-implib=oct-conf.h.a"
#endif

#ifndef OCTAVE_CONF_STATIC_LIBS
#define OCTAVE_CONF_STATIC_LIBS ""
#endif

#ifndef OCTAVE_CONF_TERM_LIBS
#define OCTAVE_CONF_TERM_LIBS "-ltermcap"
#endif

#ifndef OCTAVE_CONF_UMFPACK_CPPFLAGS
#define OCTAVE_CONF_UMFPACK_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_UMFPACK_LDFLAGS
#define OCTAVE_CONF_UMFPACK_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_UMFPACK_LIBS
#define OCTAVE_CONF_UMFPACK_LIBS "-lumfpack"
#endif

#ifndef OCTAVE_CONF_USE_64_BIT_IDX_T
#define OCTAVE_CONF_USE_64_BIT_IDX_T "no"
#endif

#ifndef OCTAVE_CONF_WARN_CFLAGS
#define OCTAVE_CONF_WARN_CFLAGS "-Wall -W -Wshadow -Wformat -Wpointer-arith -Wmissing-prototypes -Wstrict-prototypes -Wwrite-strings -Wcast-align -Wcast-qual"
#endif

#ifndef OCTAVE_CONF_WARN_CXXFLAGS
#define OCTAVE_CONF_WARN_CXXFLAGS "-Wall -W -Wshadow -Wold-style-cast -Wformat -Wpointer-arith -Wwrite-strings -Wcast-align -Wcast-qual"
#endif

#ifndef OCTAVE_CONF_X11_INCFLAGS
#define OCTAVE_CONF_X11_INCFLAGS ""
#endif

#ifndef OCTAVE_CONF_X11_LIBS
#define OCTAVE_CONF_X11_LIBS ""
#endif

#ifndef OCTAVE_CONF_XTRA_CFLAGS
#define OCTAVE_CONF_XTRA_CFLAGS "-pthread -mieee-fp"
#endif

#ifndef OCTAVE_CONF_XTRA_CXXFLAGS
#define OCTAVE_CONF_XTRA_CXXFLAGS "-pthread -mieee-fp"
#endif

#ifndef OCTAVE_CONF_YACC
#define OCTAVE_CONF_YACC "bison -y"
#endif

#ifndef OCTAVE_CONF_YFLAGS
#define OCTAVE_CONF_YFLAGS ""
#endif

#ifndef OCTAVE_CONF_Z_CPPFLAGS
#define OCTAVE_CONF_Z_CPPFLAGS ""
#endif

#ifndef OCTAVE_CONF_Z_LDFLAGS
#define OCTAVE_CONF_Z_LDFLAGS ""
#endif

#ifndef OCTAVE_CONF_Z_LIBS
#define OCTAVE_CONF_Z_LIBS "-lz"
#endif

#ifndef OCTAVE_CONF_config_opts
#define OCTAVE_CONF_config_opts "'CPPFLAGS=-I/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include' 'LDFLAGS=-L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib' '--host=i686-w64-mingw32' '--build=x86_64-unknown-linux-gnu' '--prefix=/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32' '--disable-silent-rules' '--enable-install-build-logs' 'FLTK_CONFIG=/scratch/jwe/src/mxe-octave/usr/bin/i686-w64-mingw32-fltk-config' 'gl_cv_func_gettimeofday_clobber=no' '--with-java-includedir=/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/include/java' '--disable-jit' 'MOC=/scratch/jwe/src/mxe-octave/usr/bin/i686-w64-mingw32-moc' 'UIC=/scratch/jwe/src/mxe-octave/usr/bin/i686-w64-mingw32-uic' 'RCC=/scratch/jwe/src/mxe-octave/usr/bin/i686-w64-mingw32-rcc' 'LRELEASE=/scratch/jwe/src/mxe-octave/usr/bin/i686-w64-mingw32-lrelease' 'LDFLAGS=-Wl,-rpath-link,/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib -L/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib' '--with-x=no' '--enable-cross-tools' 'PKG_CONFIG=/scratch/jwe/src/mxe-octave/usr/bin/pkg-config' 'PKG_CONFIG_PATH=/scratch/jwe/src/mxe-octave/usr/i686-w64-mingw32/lib/pkgconfig' 'build_alias=x86_64-unknown-linux-gnu' 'host_alias=i686-w64-mingw32'"
#endif

#endif
