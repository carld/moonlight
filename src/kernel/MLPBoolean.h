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
  MLPBoolean.h

  Stephane Rehel
  April 15 1998
*/

#ifndef __MLPBoolean_h
#define __MLPBoolean_h

#include "MLProperty.h"

/////////////////////////////////////////////////////////////////////////////

class MLPBoolean: public MLProperty
{
  friend class MLNode;

public:
  boolean b;

public:
  MLPBoolean( MLNode* _node, const char* _name ): MLProperty(_node,_name)
    {
    b= false;
    }

  MLPBoolean( MLNode* _node ): MLProperty(_node)
    {
    b= false;
    }

  virtual ~MLPBoolean()
    {}

  MLPropertyType::Type getType() const
    {
    return MLPropertyType::BOOLEAN;
    }

  boolean get() const
    {
    return b;
    }

  // return true if changed
  boolean set( boolean _b )
    {
    if( b == _b )
      return false;

    b= _b;
    touch();

    return true;
    }

  MLPBoolean& operator = ( boolean _b )
    {
    set(_b);
    return *this;
    }

  // return false if error
  virtual boolean _serialWrite( XDRFile* file );

  // return false if error
  virtual boolean _serialRead( XDRFile* file );

  // return the size (in bytes) that will be read/written by
  // serialRead()/serialWrite() on this property
  virtual int _serialSize() const;

  virtual boolean copy( MLProperty* p ) const;

  virtual MLProperty* clone() const;

  virtual MLString toString() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPBoolean_h
