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
  MLPColor.h

  Stephane Rehel
  April 15 1998
*/

#ifndef __MLPColor_h
#define __MLPColor_h

#include "MLProperty.h"

#include "tools/Color.h"

/////////////////////////////////////////////////////////////////////////////

class MLPColor: public MLProperty
{
  friend class MLNode;

protected:
  Color c;
  int frame;
  void* anim;

public:
  MLPColor( MLNode* _node, const char* _name ):
    MLProperty(_node,_name),
    c(0,0,0)
    {
    frame= 0;
    anim= 0;
    }

  MLPColor( MLNode* _node ):
    MLProperty(_node),
    c(0,0,0)
    {
    frame= 0;
    anim= 0;
    }

  virtual ~MLPColor()
    {}

  MLPropertyType::Type getType() const
    {
    return MLPropertyType::COLOR;
    }

  void setKey( int _frame );

  // return true is this property has changed
  boolean setFrame( int _frame );

  const Color& get() const
    {
    return c;
    }

  // return true if changed
  boolean set( const Color& _c )
    {
    if( c == _c )
      return false;

    c= _c;
    touch();

    return true;
    }

  MLPColor& operator = ( const Color& _c )
    {
    set(_c);
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

#endif // ifndef __MLPColor_h
