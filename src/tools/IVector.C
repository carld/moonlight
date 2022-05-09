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
  IVector2.h

  Stephane Rehel, April 23rd, 1996
  from pntvect.h, from mathcls.h, etc, ..., from Aug 1992.
*/

#include "IVector.h"

/////////////////////////////////////////////////////////////////////////////

IVector min( const IVector& v1, const IVector& v2 )
{
  return IVector( min(v1.x(),v2.x()), min(v1.y(),v2.y()) );
}

/////////////////////////////////////////////////////////////////////////////

IVector max( const IVector& v1, const IVector& v2 )
{
  return IVector( max(v1.x(),v2.x()), max(v1.y(),v2.y()) );
}

/////////////////////////////////////////////////////////////////////////////

IPoint min( const IPoint& v1, const IPoint& v2 )
{
  return IPoint( min(v1.x(),v2.x()), min(v1.y(),v2.y()) );
}

/////////////////////////////////////////////////////////////////////////////

IPoint max( const IPoint& v1, const IPoint& v2 )
{
  return IPoint( max(v1.x(),v2.x()), max(v1.y(),v2.y()) );
}

/////////////////////////////////////////////////////////////////////////////
