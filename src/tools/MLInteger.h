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
  MLInteger.h

  Stephane Rehel
  March 19 1998
*/

#ifndef __MLInteger_h
#define __MLInteger_h

#include "boolean.h"

class MLString;

/////////////////////////////////////////////////////////////////////////////

class MLInteger
{
public:
  static const MLInteger MAX_VALUE;
  static const MLInteger MIN_VALUE;

protected:
  int i;

public:
  MLInteger( int _i )
    {
    i= _i;
    }

  MLInteger( const MLString& s )
    {
    i= parseInt(s);
    }

  virtual ~MLInteger()
    {}

  void toString( MLString& s ) const;

  int hashCode() const
    {
    return i;
    }

  boolean equals( MLInteger gi ) const
    {
    return i == gi.i;
    }

  static int parseInt( const MLString& s );

  static MLInteger valueOf( const MLString& s );

  int intValue() const
    {
    return i;
    }

  float floatValue() const
    {
    return float(i);
    }

  double doubleValue() const
    {
    return double(i);
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLInteger_h
