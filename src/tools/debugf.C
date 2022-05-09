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
  debugf.C

  Stephane Rehel
  July 15 1997
*/

#include <stdio.h>
#include <stdarg.h>

/////////////////////////////////////////////////////////////////////////////

int debugf( const char* format, ... )
{
  va_list args;
  int outcome;

  va_start( args, format );
  outcome= vfprintf( stderr, format, args );
  va_end( args );

  return outcome;
}

/////////////////////////////////////////////////////////////////////////////

