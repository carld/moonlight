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
   Key.h

   Stephane Rehel
   June 28, 1996
*/


#ifndef __Key_h
#define __Key_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class KeyCodes;

/////////////////////////////////////////////////////////////////////////////

class Key
{
  friend class KeyCodes;

protected:
  typedef unsigned long KEY;
  KEY key;

public:
  static Key NULL_KEY;

  Key(): key(0)
    {}

  Key( const KEY _key ): key(_key)
    {}
  Key( const char ch ): key(KEY(ch))
    {}
  Key( const Key& _key ): key(_key.key)
    {}

  ~Key()
    {}

  Key& operator= ( const Key _key )
    {
    key= _key.key;
    return *this;
    }

  boolean operator == ( const Key _key ) const
    {
    return key == _key.key;
    }
  boolean operator != ( const Key _key ) const
    {
    return key != _key.key;
    }

  int getBase() const
    {
    return int(key & KEY(0xefff));
    }
  boolean hasModifiers() const
    {
    return (key & ~KEY(0xffff)) != KEY(0);
    }

  Key control() const
    {
    return Key( ((key>=KEY('a') && key<=KEY('z')) ? (key-KEY(0x20)) : key)
                 | KEY(0x10000UL));
    }
  Key uncontrol() const
    {
    return key & KEY(~0x10000UL);
    }
  boolean hasControl() const
    {
    return (key & KEY(0x10000UL)) != KEY(0);
    }

  Key shift() const
    {
    return Key(key | KEY(0x20000UL));
    }
  Key unshift() const
    {
    return Key(key & KEY(~0x20000UL));
    }
  boolean hasShift() const
    {
    return (key & KEY(0x20000UL)) != KEY(0);
    }

  Key alt() const
    {
    return Key( ((key>=KEY('a') && key<=KEY('z')) ? (key-KEY(0x20)) : key)
                 | KEY(0x40000UL));
    }
  Key unalt() const
    {
    return Key(key & KEY(~0x40000UL));
    }
  boolean hasAlt() const
    {
    return (key & KEY(0x40000UL)) != KEY(0);
    }

  static Key function( int i );
  boolean isFunction() const;
  int getFunctionNumber() const;
  Key lowerCase() const;
  Key upperCase() const;

  boolean isDigit() const;
  boolean isAlpha() const;
  boolean isAscii() const;

  static void init_KeyCodes();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Key_h
