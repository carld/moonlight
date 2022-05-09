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
  KWCallbackClass.h

  Stephane Rehel
  August 4 1996
*/

#ifndef __KWCallbackClass_h
#define __KWCallbackClass_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class KWEvent;

/////////////////////////////////////////////////////////////////////////////

/**
 * This is a base class defining a callback interface used all over the place.
 */
class KWCallbackClass
{
public:
  /**
   * This is a callback that's overloaded in it's subclases.
   */
  virtual boolean kwCallback( KWEvent& )
    {
    return false;
    }
};

/////////////////////////////////////////////////////////////////////////////

typedef unsigned int KWidget;

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __KWCallbackClass_h
