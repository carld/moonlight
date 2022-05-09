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
  MLPString.h

  Stephane Rehel
  April 15 1998
*/

#ifndef __MLPString_h
#define __MLPString_h

#include "MLProperty.h"

/////////////////////////////////////////////////////////////////////////////

class MLPString: public MLProperty
{
  friend class MLNode;

public:
  MLString s;

public:
  MLPString( MLNode* _node, const char* _name ): MLProperty(_node,_name)
    {}

  MLPString( MLNode* _node ): MLProperty(_node)
    {}

  virtual ~MLPString()
    {}

  MLPropertyType::Type getType() const
    {
    return MLPropertyType::STRING;
    }

  const MLString& get() const
    {
    return s;
    }

  // return true if changed
  boolean set( const MLString& _s )
    {
    if( s == _s )
      return false;

    s= _s;
    touch();

    return true;
    }

  MLPString& operator = ( const MLString& _s )
    {
    set(_s);
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

#endif // ifndef __MLPString_h
