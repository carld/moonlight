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
  MLSmallspherePicture.h

  Stephane Rehel

  July 5 1997
*/

#ifndef __MLSmallSpherePicture_h
#define __MLSmallSpherePicture_h

#ifndef __MLPicture_h
#include "MLPicture.h"
#endif

class Color;

/////////////////////////////////////////////////////////////////////////////

class MLSmallSpherePicture: public MLPicture
{
public:
  // Hues:
  static const double green,
                      yellow,
                      orange,
                      red,
                      magenta,
                      cyan,
                      blue;

  // default is green. Hue from -180 to 180
  MLSmallSpherePicture( double hue = 0. );

  MLSmallSpherePicture( const Color& color );

  virtual ~MLSmallSpherePicture();

  void set( double hue = 0. );
  void set( const Color& color );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLSmallSpherePicture_h
