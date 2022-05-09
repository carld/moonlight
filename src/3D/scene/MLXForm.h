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
  MLXForm.h

  Simple ML transformation matrix

  Stephane Rehel

  December 13 1996
*/

#ifndef __MLXForm_h
#define __MLXForm_h

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __Transform_h
#include "tools/Transform.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLXForm
{
public:
  Transform preXForm;

  Vector scale;
  Vector rotate; // x, then y, then z. radians.
  Vector translate;

//  Transform trs; // scale, then rotate, then translate
  Transform transform; // preXForm * T.R.S

  MLXForm();

  ~MLXForm()
    {}

  MLXForm& operator = ( const MLXForm& fx );

  void setScale( const Vector& _scale )
    {
    scale= _scale;
    }
  void setRotate( const Vector& _rotate )
    {
    rotate= _rotate;
    }
  void setTranslate( const Vector& _translate )
    {
    translate= _translate;
    }
  void update(); // build 'transform' matrices
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLXForm_h
