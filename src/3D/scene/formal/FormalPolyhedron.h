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
   FormalPolyhedron.h

   March 1 1997
   Stephane Rehel
*/

#ifndef __FormalPolyhedron_h
#define __FormalPolyhedron_h

#ifndef __FormalRep_h
#include "FormalRep.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class FormalPolyhedron: public FormalRep
{
  friend class FormalRep;

public:

  enum PolyhedronType
    {
    first_polyhedron_type            = 0,
    tetrahedron                      = 1,
    cube                             = 2,
    octahedron                       = 3,
    icosahedron                      = 4,
    dodecahedron                     = 5,

    cuboctahedron                    = 6,

    icosidodecahedron                = 7,
    rhombicuboctahedron              = 8,
    rhombicosidodecahedron           = 9,
    rhombitruncated_cubeoctahedron   = 10,
    rhombitruncated_icosidodecahedron= 11,

    snub_cube                        = 12,
    snub_dodecahedron                = 13,

    truncated_tetrahedron            = 14,
    truncated_cube                   = 15,
    truncated_octahedron             = 16,
    truncated_icosahedron            = 17,
    truncated_dodecahedron           = 18,
    last_polyhedron_type
    };

protected:
  PolyhedronType polyhedron_type;

protected:
  FormalPolyhedron( MLScene* _scene );

public:
  virtual ~FormalPolyhedron();

  void setPolyhedronType( PolyhedronType _polyhedron_type );

  FormalRep::TYPE getType() const;

  void generate( MeshRep* mesh ) const;

  FormalRep* duplicate();

protected:
  virtual void _read( MLRFileBlock* block );
  virtual void _write( MLWFileBlock* block );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __FormalPolyhedron_h
