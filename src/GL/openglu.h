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

#ifndef __ml_glu_wrapper_h
#define __ml_glu_wrapper_h

#ifdef __WINDOWS__
#include <windows.h>
 #ifdef min
 #undef min
 #endif
 #ifdef max
 #undef max
 #endif

 #ifdef DELETE
 #undef DELETE
 #endif

 #ifdef USE_MESA
   #include "GL/glu.h"
 #else
   #include "wingl.h"
   #include "winglu.h"
   #define GLU_H
 #endif

#else
 #include "GL/glu.h"
 #define CALLBACK
#endif

#endif // ifndef __ml_glu_wrapper_h
