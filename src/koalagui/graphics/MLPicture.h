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
  MLPicture.h

  Stephane Rehel
  August 16 1996
*/

#ifndef __MLPicture_h
#define __MLPicture_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLImage_h
#include "image/MLImage.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLPicture: public MLImage
{
private:
  friend class MLPictures;
  int refCount;

public:
  MLPicture();
  virtual ~MLPicture();

  static void draw( MLImage* image, int x, int y );

  void draw( int x, int y );

  void draw( int x, int y,
             int x1, int y1, int x2, int y2 );

  // get gamma correction from GraphicsSystem
  void gammaCorrect();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPicture_h
