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
   Key.C

   Stephane Rehel
   June 28, 1996
*/

#include <assert.h>

#include "Key.h"
#include "KeyCodes.h"

/////////////////////////////////////////////////////////////////////////////

// static
Key Key::NULL_KEY;

/////////////////////////////////////////////////////////////////////////////

// static
Key Key::function( int i )
{
  return Key( KEY(0x3b00) + KEY(i)*KEY(0x100) );
}

/////////////////////////////////////////////////////////////////////////////

boolean Key::isFunction() const
{
  return key >= function(1).key && key <= function(12).key;
}

/////////////////////////////////////////////////////////////////////////////

int Key::getFunctionNumber() const
{
  KEY i= (key - KEY(0x3b00)) / KEY(0x100);
  if( i >= KEY(1) && i <= KEY(12) )
    return int(i);
   else
    return 0;
}

/////////////////////////////////////////////////////////////////////////////

Key Key::lowerCase() const
{
  if( key>=KEY('A') && key<=KEY('Z') )
    return Key(key | KEY(0x20));
   else
    return Key(key);
}

/////////////////////////////////////////////////////////////////////////////

Key Key::upperCase() const
{
  if( key>=KEY('a') && key<=KEY('z') )
    return Key(key & KEY(~0x20));
   else
    return Key(key);
}

/////////////////////////////////////////////////////////////////////////////

boolean Key::isDigit() const
{
  return key>=KEY('0') && key<=KEY('9');
}

/////////////////////////////////////////////////////////////////////////////

boolean Key::isAlpha() const
{
  return (key >= KEY('A') && key <= KEY('Z')) ||
         (key >= KEY('a') && key <= KEY('z'));
}

/////////////////////////////////////////////////////////////////////////////

boolean Key::isAscii() const
{
  return (key >= KEY(' ') && key <= KEY(126));
}

/////////////////////////////////////////////////////////////////////////////

// static
void Key::init_KeyCodes()
{
  Key::NULL_KEY= Key(KEY(0));

  KeyCodes::init_keys();
}

/////////////////////////////////////////////////////////////////////////////
