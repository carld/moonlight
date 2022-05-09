// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
   FormalNURBS.C

   January 8 1997

   Stephane Rehel
*/

#include "FormalNURBS.h"
#include <stdio.h>

#include "scene/nurbs/MLNURBS.h"
#include "scene/nurbs/MLNURBS_Surface.h"
#include "scene/MeshRep.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

FormalNURBS::FormalNURBS( MLScene* _scene ):
  FormalParamSurf(_scene)
{
  closedU= 0;
  closedV= 0;

  nFacesU= 8;
  nFacesV= 8;

  minFacesU= 1;
  minFacesV= 1;

  nurbs= 0;
}

/////////////////////////////////////////////////////////////////////////////

FormalNURBS::~FormalNURBS()
{
  delete nurbs;
  nurbs= 0;
}

/////////////////////////////////////////////////////////////////////////////

FormalRep::TYPE FormalNURBS::getType() const
{
  return FormalRep::NURBS;
}

/////////////////////////////////////////////////////////////////////////////

boolean FormalNURBS::isNURBS() const
{
  return true;
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBS::alloc( int n, int u_degree, int m, int v_degree )
{
  if( nurbs != 0 )
    {
    delete nurbs;
    nurbs= 0;
    }

  nurbs= new MLNURBS_Surface( n, u_degree, m, v_degree );
}

/////////////////////////////////////////////////////////////////////////////

FormalRep* FormalNURBS::duplicate()
{
  FormalNURBS* f= new FormalNURBS(scene);

  FormalParamSurf::copy(f);

  if( nurbs != 0 )
    f->nurbs= nurbs->duplicate();

  return f;
}

/////////////////////////////////////////////////////////////////////////////

// u, v >= 0
Point FormalNURBS::getUVPoint( int u, int v ) const
{
  if( nurbs == 0 )
    return Point(0,0,0);

//const double e= 1e-4;
//  return nurbs->get( double(u)/double(nFacesU) * (1.-e*2)+e,
//                     double(v)/double(nFacesV) * (1.-e*2)+e);

//Point p= nurbs->get(double(u)/double(nFacesU),double(v)/double(nFacesV));
//  printf( "(%d,%d)=[%.2f %.2f %.2f]\n",
//        u,v, p.x(), p.y(),p.z() );

  return nurbs->get( double(u)/double(nFacesU), double(v)/double(nFacesV) );
}

/////////////////////////////////////////////////////////////////////////////

// called when moving a control point for example
void FormalNURBS::regenerateVertices( MeshRep* mesh )
{
  if( mesh == 0 )
    return;

  int nVerticesU= closedU ? nFacesU : (nFacesU+1);
  int nVerticesV= closedV ? nFacesV : (nFacesV+1);

/*
  int nVertices= nVerticesU * nVerticesV;
  int nEdges= nVerticesV*nFacesU + nVerticesU*nFacesV;
  int nPolygons= nFacesU * nFacesV;

printf( "(%d %d) (%d %d) (%d %d) u=%d v=%d\n",
        mesh->nVertices , nVertices ,
      mesh->computeNBaseEdges(), nEdges,
      mesh->computeNBasePolygons(), nPolygons,
      nFacesU,nFacesV  );

  if( mesh->nVertices              != nVertices ||
      mesh->computeNBaseEdges()    != nEdges    ||
      mesh->computeNBasePolygons() != nPolygons )
    {
    // Mmmh, the mesh parameters aren't correct.
    // Let's regenerate the whole mesh
    FormalParamSurf::generate(mesh);
    return;
    }
*/

  for( int v= 1; v <= nVerticesV; ++v )
    for( int u= 1; u <= nVerticesU; ++u )
      mesh->vertices[ u + (v-1) * nVerticesU ].point= getUVPoint(u-1,v-1);

  int i;

  MLPolygonArray& polygons= mesh->polygons;
  for( i= 1; i <= polygons.getSize(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;

    p.updateNormalArea(mesh->vertices.getArray(),mesh->polygons.getArray());
    }

  MLSVertexArray& svertices= mesh->svertices;
  for( i= 1; i <= svertices.getSize(); ++i )
    {
    MLSVertex& sv= svertices[i];
    if( ! sv.allocated() )
      continue;

    sv.updateNormal(mesh->vertices.getArray(),mesh->polygons.getArray());
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean FormalNURBS::needToMergeVertices() const
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBS::_read( MLRFileBlock* block )
{
  FormalParamSurf::_read(block);

  int ok= block->getInteger();
  if( ! ok )
    {
    delete nurbs;
    nurbs= 0;
    return;
    }

  int n= block->getInteger();
  int u_degree= block->getInteger();
  int m= block->getInteger();
  int v_degree= block->getInteger();

  alloc( n, u_degree, m, v_degree );

  int i, j;

  int r= nurbs->getr();
  for( i= 0; i <= r; ++i )
    nurbs->getUKnotVector()[i]= block->getDouble();

  int s= nurbs->gets();
  for( j= 0; j <= s; ++j )
    nurbs->getVKnotVector()[j]= block->getDouble();

  for( i= 0; i <= n; ++i )
    for( j= 0; j <= m; ++j )
      {
      Point point;
      double weight;
      unsigned int flags;
      *block >> flags;
      *block >> point;
      *block >> weight;

      MLNURBS_ControlPoint& cp= nurbs->getControlPoint(i,j);
      cp.flags= MLNURBS_ControlPoint::Flag(flags);
      cp.set( point, weight );
      }
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBS::_write( MLWFileBlock* block )
{
  FormalParamSurf::_write(block);

  *block << int((nurbs == 0 ) ? 0 : 1);
  if( nurbs == 0 )
    return;

  *block << int(nurbs->getn());
  *block << int(nurbs->get_u_degree());
  *block << int(nurbs->getm());
  *block << int(nurbs->get_v_degree());

  int i, j;

  int r= nurbs->getr();
  for( i= 0; i <= r; ++i )
    *block << double(nurbs->getUKnot(i));

  int s= nurbs->gets();
  for( j= 0; j <= s; ++j )
    *block << double(nurbs->getVKnot(j));

  int n= nurbs->getn();
  int m= nurbs->getm();
  for( i= 0; i <= n; ++i )
    for( j= 0; j <= m; ++j )
      {
      const MLNURBS_ControlPoint& cp= nurbs->getConstControlPoint(i,j);
      *block << (unsigned int)cp.flags;
      *block << cp.point;
      *block << double(cp.w);
      }
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBS::createBilinear( const Point& p00, const Point& p10,
                                  const Point& p11, const Point& p01 )
{
  delete nurbs;

  nurbs= MLNURBS::getBilinear( p00, p10, p11, p01 );
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBS::createCylinder()
{
  delete nurbs;

//  nurbs= MLNURBS::getGeneralCylinder( *MLNURBS::getSemiCircle4(),
//                                      Vector(0,0,2) );
  nurbs= MLNURBS::getSurfaceRevolution( *(MLNURBS::getSegment(
                                          Point(1,0,0),
                                          Point(1,0,1) )),
                                      1. );
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBS::createSphere()
{
  delete nurbs;

  nurbs= MLNURBS::getSurfaceRevolution( *(MLNURBS::getSegment4(
                                          Point(0,0,0),
                                          Point(1,0,0),
                                          Point(1,0,2),
                                          Point(0,0,2) )),
                                      1. );
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBS::createCone()
{
  delete nurbs;
  nurbs= MLNURBS::getSurfaceRevolution( *(MLNURBS::getSegment(
                                          Point(1,0,0),
                                          Point(0,0,1) )),
                                      1. );
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBS::createTorus()
{
  delete nurbs;

printf("FormalNURBS::createTorus(): NOT IMPLEMENTED!!\n" );
createCylinder(); // DUMMY
//  nurbs= MLNURBS::getGeneralCylinder( *MLNURBS::getSemiCircle4(),
//                                      Vector(0,0,2) );
}

/////////////////////////////////////////////////////////////////////////////
