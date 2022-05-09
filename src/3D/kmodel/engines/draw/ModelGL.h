// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*
  ModelGL.h

  Stephane Rehel
  September 7 1997
*/

#ifndef __ModelGL_h
#define __ModelGL_h

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __Color_h
#include "tools/Color.h"
#endif

#ifndef __Spectrum_h
#include "tools/Spectrum.h"
#endif

#ifndef __ml_gl_wrapper_h
#include "GL/opengl.h"
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef DBL_IS_FLOAT
#define glPoint(p) glVertex3fv((p).getConstCoord())
#else
#define glPoint(p) glVertex3dv((p).getConstCoord())
#endif

#ifdef DBL_IS_FLOAT
#define glNormal(v) glNormal3fv((v).getConstCoord())
#else
#define glNormal(v) glNormal3dv((v).getConstCoord())
#endif

inline void glColor( const Color& c )
{
  glColor3fv((c).getConstCoord());
}

#define glSpectrum(s) glColor3fv((s).getConstCoord())

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelGL_h
