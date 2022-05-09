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
   MLEdge.C

   Stephane Rehel
   November 11 1996

   Moved to tools/ on April 3 1998
*/

#include "tools/MLDArray.h"
#include "tools/xdr/XDRFile.h"

#include "MLEdge.h"
#include "MLPolygon.h"

/////////////////////////////////////////////////////////////////////////////

MLEdge& MLEdge::operator = ( const MLEdge& e )
{
  if( &e == this )
    return *this;

  v1= e.v1;
  v2= e.v2;
  polygon1= e.polygon1;
  polygon2= e.polygon2;
  flags= e.flags;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

int MLEdge::getSizeOf() const
{
  return sizeof(*this);
}

/////////////////////////////////////////////////////////////////////////////

void MLEdge::unallocate()
{
  flags &= ~Flag(ALLOCATED);
}

/////////////////////////////////////////////////////////////////////////////

// test if this a discontinuity edge
void MLEdge::computeDisc( const MLDArray<MLPolygon>& all_polygons,
                          double cos_discAngle )
{
  if( polygon1 == 0 || polygon2 == 0 )
    {
    flags &= ~Flag(DISC);
    flags |= Flag(BORDER);
    return;
    }

  flags &= ~Flag(BORDER);

  const MLPolygon& p1= all_polygons.get(polygon1);
  const MLPolygon& p2= all_polygons.get(polygon2);

  float cos_a;

  if( p1.material != p2.material )
    goto disc;

  cos_a= (p1.normal | p2.normal);
  if( cos_a <= cos_discAngle )
    goto disc;

  flags &= ~Flag(DISC);
  return;

disc:
  flags |= Flag(DISC);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLEdge::serialWrite( XDRFile* file )
{
#define write_int(it) if( ! file->putInteger(it) ) return false;

  write_int(flags);

  write_int(v1);
  write_int(v2);

  write_int(polygon1);
  write_int(polygon2);

#undef write_int

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLEdge::serialRead( XDRFile* file )
{
#define read_int(it) { int _it; if(!file->getInteger(&_it)) return false; it=_it; }

  read_int(flags);

  read_int(v1);
  read_int(v2);

  read_int(polygon1);
  read_int(polygon2);

#undef read_int

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLEdge::serialSize() const
{
  int s= 0;

  // flags
  s+= XDROp::getIntegerSize();

  // vertices
  s+= XDROp::getIntegerSize() * 2;

  // polygons
  s+= XDROp::getIntegerSize() * 2;

  return s;
}

/////////////////////////////////////////////////////////////////////////////
