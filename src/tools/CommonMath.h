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
  CommonMath.h

  Stephane Rehel
  June 28, 1996

  from mathcls.h
*/

#ifndef __CommonMath_h
#define __CommonMath_h

/////////////////////////////////////////////////////////////////////////////

#include <math.h>

#ifndef M_2PI
#define M_2PI (2.*M_PI)
#endif

extern double doubleTolerance;
extern double distanceTolerance;
extern double angleTolerance;

#define DEGtoRAD(d) ((d) * (M_PI/180.))
#define RADtoDEG(d) ((d) * (180./M_PI))

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __CommonMath_h

