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
   MLSVertex.C

   November 10 1996
   Stephane Rehel

   Moved to tools/ on April 3 1998
*/

#include "MLSVertex.h"
#include "MLVertex.h"
#include "MLPolygon.h"

#include "tools/MLDArray.h"
#include "tools/xdr/XDRFile.h"

/////////////////////////////////////////////////////////////////////////////

MLSVertex& MLSVertex::operator = ( const MLSVertex& sv )
{
  if( &sv == this )
    return *this;

  vertex= sv.vertex;
  normal= sv.normal;
  material= sv.material;
  txtCoord= sv.txtCoord;

  flags= sv.flags;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

int MLSVertex::getSizeOf() const
{
  return sizeof(*this);
}

/////////////////////////////////////////////////////////////////////////////

void MLSVertex::updateNormal( const MLDArray<MLVertex>& all_vertices,
                              const MLDArray<MLPolygon>& all_polygons )
{
  const MLVertex& v= all_vertices.get(vertex);

  const int* polygons= v.polygons;
  const int nPolygons= v.nPolygons;

  normal= Vector(0,0,0);
  for( int i= 0; i < nPolygons; ++i )
    {
    int pi= polygons[i];
    if( pi == 0 )
      continue;
    normal+= all_polygons.get(pi).normal;
    }

  double norm= normal.norm();
  if( norm > 1e-30 )
    normal /= norm;
}

/////////////////////////////////////////////////////////////////////////////

void MLSVertex::unallocate()
{
  flags &= ~Flag(ALLOCATED);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLSVertex::serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

#define write_double(r) if( ! file->putDouble(r) ) return false;
#define write_int(it) if( ! file->putInteger(it) ) return false;

  write_int(int(flags));
  write_int(vertex);

  write_double(normal.x());
  write_double(normal.y());
  write_double(normal.z());

  write_int(material);
  write_double(txtCoord.x());
  write_double(txtCoord.y());

#undef write_double
#undef write_int

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLSVertex::serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

#define read_double(c) { double r; if(!file->getDouble(&r)) return false; c=r; }
#define read_int(it) { int _it; if(!file->getInteger(&_it)) return false; it=_it; }

  read_int(flags);
  read_int(vertex);

  read_double(normal[0]);
  read_double(normal[1]);
  read_double(normal[2]);

  read_int(material);
  read_double(txtCoord[0]);
  read_double(txtCoord[1]);

#undef read_double
#undef read_int

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLSVertex::serialSize() const
{
  int s= 0;

  // flags
  s+= XDROp::getIntegerSize();

  // vertex
  s+= XDROp::getIntegerSize();

  // normal
  s+= XDROp::getDoubleSize() * 3;

  // material
  s+= XDROp::getIntegerSize();

  // normal
  s+= XDROp::getDoubleSize() * 2;

  return s;
}

/////////////////////////////////////////////////////////////////////////////
