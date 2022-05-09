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
  IVector3.h

  Stephane Rehel, April 23rd, 1996
  from pntvect.h, from mathcls.h, etc, ..., from Aug 1992.
*/

#include "IVector3.h"

/////////////////////////////////////////////////////////////////////////////

// static
const IVector3 IVector3::X(1,0,0),
               IVector3::Y(0,1,0),
               IVector3::Z(0,0,1);

/////////////////////////////////////////////////////////////////////////////

IVector3 min( const IVector3& v1, const IVector3& v2 )
{
  return
  IVector3( min(v1.x(),v2.x()), min(v1.y(),v2.y()), min(v1.z(),v2.z()) );
}

/////////////////////////////////////////////////////////////////////////////

IVector3 max( const IVector3& v1, const IVector3& v2 )
{
  return
  IVector3( max(v1.x(),v2.x()), max(v1.y(),v2.y()), max(v1.z(),v2.z()) );
}

/////////////////////////////////////////////////////////////////////////////

IPoint3 min( const IPoint3& v1, const IPoint3& v2 )
{
  return
  IPoint3( min(v1.x(),v2.x()), min(v1.y(),v2.y()), min(v1.z(),v2.z()) );
}

/////////////////////////////////////////////////////////////////////////////

IPoint3 max( const IPoint3& v1, const IPoint3& v2 )
{
  return
  IPoint3( max(v1.x(),v2.x()), max(v1.y(),v2.y()), max(v1.z(),v2.z()) );
}

/////////////////////////////////////////////////////////////////////////////
