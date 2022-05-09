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
  MLPictures.h

  Stephane Rehel
  August 16 1996
*/

#ifndef __MLPictures_h
#define __MLPictures_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class MLPicture;
class MLString;

/////////////////////////////////////////////////////////////////////////////

class MLPictures
{
public:
  MLPictures();

  static void ref( MLPicture* picture );
  static int unref( MLPicture* picture ); // return 0 when deleted

  static void add( MLPicture* picture, const MLString& name );
  static void add( MLPicture* picture );

  static MLPicture* find( void* data );
  static MLPicture* find( const MLString& name );
  static const MLString& getName( MLPicture* picture );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPictures_h

