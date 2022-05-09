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
  Token.C

  CTL
  Stephane Rehel, January 22nd, 1996
*/

#include "Token.h"

/////////////////////////////////////////////////////////////////////////////

// static private
boolean* Token::charsArray= 0;

/////////////////////////////////////////////////////////////////////////////

// static
void Token::initCharsArray()
{
  if( charsArray != 0 )
    return;
  charsArray= new boolean [ 128 ];

  for( int i= 0; i < 128; ++i )
    charsArray[i]= false;

  charsArray[int('.')]= true;
  charsArray[int(',')]= true;
  charsArray[int(';')]= true;
  charsArray[int(':')]= true;
  charsArray[int('#')]= true;
  charsArray[int('$')]= true;
  charsArray[int('%')]= true;
  charsArray[int('&')]= true;
  charsArray[int('\'')]= true;
  charsArray[int('(')]= true;
  charsArray[int(')')]= true;
  charsArray[int('{')]= true;
  charsArray[int('}')]= true;
  charsArray[int('[')]= true;
  charsArray[int(']')]= true;
  charsArray[int('*')]= true;
  charsArray[int('+')]= true;
  charsArray[int('-')]= true;
  charsArray[int('/')]= true;
  charsArray[int('<')]= true;
  charsArray[int('>')]= true;
  charsArray[int('=')]= true;
  charsArray[int('?')]= true;
  charsArray[int('@')]= true;
  charsArray[int('!')]= true;
  charsArray[int('"')]= true;
  charsArray[int('^')]= true;
  charsArray[int('_')]= true;
  charsArray[int('|')]= true;
  charsArray[int('~')]= true;
  charsArray[int('`')]= true;
  charsArray[int('\\')]= true;
}

/////////////////////////////////////////////////////////////////////////////

// static
// can merge 'tc1 + tc2' into a new token char ??
// return null_char if not
Token::Char Token::canMerge( Token::Char tc1, Token::Char tc )
{
  switch( tc1 )
    {
    case ampersand:
      if( tc == ampersand )
        return ampersand2; // '&&'
      if( tc == equal )
        return ampersand_equal; // '&='
      break;

    case bar:
      if( tc == bar )
        return bar2; // '||'
      if( tc == equal )
        return bar_equal; // '|='
      break;

    case equal:
      if( tc == equal )
        return equal2; // '=='
      break;

    case star:
      if( tc == star )
        return star2; // '**'
      if( tc == equal )
        return star_equal; // '*='
      if( tc == slash )
        return star_slash; // '*/'
      break;

    case o_angle:
      if( tc == o_angle )
        return o_angle2; // '<<'
      break;

    case c_angle:
      if( tc == c_angle )
        return c_angle2; // '>>'
      break;

    case plus:
      if( tc == plus )
        return plus2; // '++'
      if( tc == equal )
        return plus_equal; // '+='
      break;

    case minus:
      if( tc == minus )
        return minus2; // '--'
      if( tc == equal )
        return minus_equal; // '-='
      if( tc == c_angle )
        return arrow; // '->'
      break;

    case slash:
      if( tc == slash )
        return slash2; // '//'
      if( tc == equal )
        return slash_equal; // '/='
      if( tc == star )
        return slash_star; // '/*'
      break;

    default:
      break;
    }

  return null_char; // failed
}

/////////////////////////////////////////////////////////////////////////////

