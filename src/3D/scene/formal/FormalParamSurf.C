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
   FormalParamSurf.C

   November 12 1996
   Stephane Rehel
*/

#include "FormalParamSurf.h"
#include "scene/MeshRep.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

FormalParamSurf::FormalParamSurf( MLScene* _scene ): FormalRep(_scene)
{
  closedU= closedV= 0;
  nFacesU= nFacesV= 0;
  minFacesU= minFacesV= 3;
}

/////////////////////////////////////////////////////////////////////////////

FormalParamSurf::~FormalParamSurf()
{}

/////////////////////////////////////////////////////////////////////////////

void FormalParamSurf::generate( MeshRep* mesh ) const
{
  if( mesh == 0 )
    return;

  int nVerticesU= closedU ? nFacesU : (nFacesU+1);
  int nVerticesV= closedV ? nFacesV : (nFacesV+1);

  int nVertices= nVerticesU * nVerticesV;

  int nEdges= nVerticesV*nFacesU + nVerticesU*nFacesV;

  mesh->alloc( nVertices, nEdges, nFacesU * nFacesV );

  // we act as if we've got a U,V-open surface
  // We build a quick vertex index reference table:
  int* vertices= new int [ (nFacesU+1) * (nFacesV+1) ];
  #define VERTEX(u,v) (vertices[(u)-1+((v)-1)*(nFacesU+1)])

  int u, v;
  for( v= 1; v <= nFacesV+1; ++v )
    for( u= 1; u <= nFacesU+1; ++u )
      {
      int u0= (u<=nFacesU) ? u : (closedU ? 1 : u);
      int v0= (v<=nFacesV) ? v : (closedV ? 1 : v);
      VERTEX(u,v)= u0 + (v0-1) * nVerticesU;
      }

  // generate vertices
  for( v= 1; v <= nVerticesV; ++v )
    for( u= 1; u <= nVerticesU; ++u )
      mesh->vertices[ u + (v-1) * nVerticesU ].point= getUVPoint(u-1,v-1);

  // generate edges
  int i= 1;

  // U-horizontal edges
  for( v= 1; v <= nVerticesV; ++v )
    for( u= 1; u <= nFacesU; ++u )
      {
      mesh->edges[i].v1= VERTEX(u,v);
      mesh->edges[i].v2= VERTEX(u+1,v);
      ++i;
      }

  // V-horizontal edges
  for( u= 1; u <= nVerticesU; ++u )
    for( v= 1; v <= nFacesV; ++v )
      {
      mesh->edges[i].v1= VERTEX(u,v);
      mesh->edges[i].v2= VERTEX(u,v+1);
      ++i;
      }

  assert( i == nEdges+1 );

  // Generate polygons
  i= 1;
  for( v= 1; v <= nFacesV; ++v )
    for( u= 1; u <= nFacesU; ++u )
      {
      mesh->polygons[i].allocQuad( VERTEX(u  ,v  ),
                                   VERTEX(u+1,v  ),
                                   VERTEX(u+1,v+1),
                                   VERTEX(u  ,v+1) );
/*
      mesh->polygons[i].allocQuad( VERTEX(u  ,v+1),
                                   VERTEX(u+1,v+1),
                                   VERTEX(u+1,v  ),
                                   VERTEX(u  ,v  ) );
*/
      ++i;
      }

  #undef VERTEX
  delete vertices;

  if( needToMergeVertices() )
    mesh->mergeVertices(); // a sphere need to be cleaned

  mesh->finalizeRep();
}

/////////////////////////////////////////////////////////////////////////////

boolean FormalParamSurf::isParamSurf() const
{
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean FormalParamSurf::isNURBS() const
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamSurf::copy( FormalParamSurf* ps ) const
{
  if( ps == 0 )
    return;

  ps->nFacesU= nFacesU;
  ps->nFacesV= nFacesV;
  ps->minFacesU= minFacesU;
  ps->minFacesV= minFacesV;
  ps->closedU= closedU;
  ps->closedV= closedV;
}

/////////////////////////////////////////////////////////////////////////////

boolean FormalParamSurf::needToMergeVertices() const
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamSurf::_read( MLRFileBlock* block )
{
  *block >> nFacesU;
  *block >> nFacesV;
  *block >> closedU;
  *block >> closedV;
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamSurf::_write( MLWFileBlock* block )
{
  *block << nFacesU;
  *block << nFacesV;
  *block << closedU;
  *block << closedV;
}

/////////////////////////////////////////////////////////////////////////////

