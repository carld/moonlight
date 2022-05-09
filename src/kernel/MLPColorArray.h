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
  MLPColorArray.h

  Stephane Rehel
  April 15 1998
*/

#ifndef __MLPColorArray_h
#define __MLPColorArray_h

#include "MLPArray.h"
#include "tools/Color.h"

/////////////////////////////////////////////////////////////////////////////

class MLPColorArray: public MLPArray<Color>
{
public:
  MLPColorArray( MLNode* _node, const char* _name ):
    MLPArray<Color>(_node,_name)
    {}

  MLPColorArray( MLNode* _node ):
    MLPArray<Color>(_node)
    {}

  virtual ~MLPColorArray()
    {}

  MLPropertyType::Type getType() const
    {
    return MLPropertyType::COLOR_ARRAY;
    }

  // return false if error
  virtual boolean _serialWrite( XDRFile* file );

  // return false if error
  virtual boolean _serialRead( XDRFile* file );

  // return the size (in bytes) that will be read/written by
  // serialRead()/serialWrite() on this property
  virtual int _serialSize() const;

  virtual MLProperty* clone() const;

protected:
  virtual MLString elementToString( const Color& t ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPColorArray_h
