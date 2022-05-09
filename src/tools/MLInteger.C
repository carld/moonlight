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
  MLInteger.C

  Stephane Rehel
  March 19 1998
*/

#include <stdlib.h>

#include "MLInteger.h"
#include "MLString.h"

/////////////////////////////////////////////////////////////////////////////

// static
const MLInteger MLInteger::MIN_VALUE(-2147483647); //should be -2147483648 !!?
const MLInteger MLInteger::MAX_VALUE( 2147483647);

/////////////////////////////////////////////////////////////////////////////

void MLInteger::toString( MLString& s ) const
{
  s= MLString::valueOf(i);
}

/////////////////////////////////////////////////////////////////////////////

// static
int MLInteger::parseInt( const MLString& s )
{
  return atoi(s.getConstBytes());
}

/////////////////////////////////////////////////////////////////////////////

// static
MLInteger MLInteger::valueOf( const MLString& s )
{
  return MLInteger( MLInteger::parseInt(s) );
}

/////////////////////////////////////////////////////////////////////////////
