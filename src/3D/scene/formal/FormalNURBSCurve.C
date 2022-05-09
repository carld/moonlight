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
   FormalNURBSCurve.C

   January 24 1997

   Stephane Rehel
*/

#include "FormalNURBSCurve.h"

#include "scene/nurbs/MLNURBS.h"
#include "scene/nurbs/MLNURBS_Curve.h"
#include "scene/CurveRep.h"
#include "scene/MLCurveVertex.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

FormalNURBSCurve::FormalNURBSCurve( MLScene* _scene ):
  FormalParamCurve(_scene)
{
  closed= false;

  nSegments= 20;

  minSegments= 1;

  nurbs= 0;
}

/////////////////////////////////////////////////////////////////////////////

FormalNURBSCurve::~FormalNURBSCurve()
{
  delete nurbs;
  nurbs= 0;
}

/////////////////////////////////////////////////////////////////////////////

FormalCurveRep::TYPE FormalNURBSCurve::getType() const
{
  return FormalCurveRep::NURBS;
}

/////////////////////////////////////////////////////////////////////////////

boolean FormalNURBSCurve::isNURBS() const
{
  return true;
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBSCurve::alloc( int n, int degree )
{
  if( nurbs != 0 )
    {
    delete nurbs;
    nurbs= 0;
    }

  nurbs= new MLNURBS_Curve( n, degree );
}

/////////////////////////////////////////////////////////////////////////////

FormalCurveRep* FormalNURBSCurve::duplicate()
{
  FormalNURBSCurve* f= new FormalNURBSCurve(scene);

  FormalParamCurve::copy(f);

  if( nurbs != 0 )
    f->nurbs= nurbs->duplicate();

  return f;
}

/////////////////////////////////////////////////////////////////////////////

// u, v >= 0
Point FormalNURBSCurve::getUPoint( int /*u*/, double t ) const
{
  if( nurbs == 0 )
    return Point(0,0,0);

//const double e= 1e-4;
//  return nurbs->get( double(u)/double(nFacesU) * (1.-e*2)+e,
//                     double(v)/double(nFacesV) * (1.-e*2)+e);

//Point p= nurbs->get(double(u)/double(nFacesU),double(v)/double(nFacesV));
//  printf( "(%d,%d)=[%.2f %.2f %.2f]\n",
//        u,v, p.x(), p.y(),p.z() );

  return nurbs->get(t);
}

/////////////////////////////////////////////////////////////////////////////

// called when moving a control point for example
void FormalNURBSCurve::regenerateVertices( CurveRep* curve )
{
  if( curve == 0 )
    return;

  int nVertices= closed ? nSegments : (nSegments+1);

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

  for( int u= 1; u <= nVertices; ++u )
    {
    double t= double(u-1) / double(nSegments);
    curve->vertices[u].set( t, getUPoint(u-1,t) );
    }
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBSCurve::_read( MLRFileBlock* block )
{
  FormalParamCurve::_read(block);

  int ok= block->getInteger();
  if( ! ok )
    {
    delete nurbs;
    nurbs= 0;
    return;
    }

  int n= block->getInteger();
  int degree= block->getInteger();

  alloc( n, degree );

  int i;

  int m= nurbs->getm();
  for( i= 0; i <= m; ++i )
    nurbs->getKnotVector()[i]= block->getDouble();

  for( i= 0; i <= n; ++i )
    {
    Point point;
    double weight;
    unsigned int flags;
    *block >> flags;
    *block >> point;
    *block >> weight;

    MLNURBS_ControlPoint& cp= nurbs->getControlPoint(i);
    cp.flags= MLNURBS_ControlPoint::Flag(flags);
    cp.set( point, weight );
    }
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBSCurve::_write( MLWFileBlock* block )
{
  FormalParamCurve::_write(block);

  *block << int((nurbs == 0 ) ? 0 : 1);
  if( nurbs == 0 )
    return;

  *block << int(nurbs->getn());
  *block << int(nurbs->get_degree());

  int i;

  int m= nurbs->getm();
  for( i= 0; i <= m; ++i )
    *block << double(nurbs->getKnot(i));

  int n= nurbs->getn();
  for( i= 0; i <= n; ++i )
    {
    const MLNURBS_ControlPoint& cp= nurbs->getConstControlPoint(i);
    *block << (unsigned int)cp.flags;
    *block << cp.point;
    *block << double(cp.w);
    }
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBSCurve::createSegment( const Point& p0, const Point& p1 )
{
  delete nurbs;

  nurbs= MLNURBS::getSegment( p0, p1 );
}

/////////////////////////////////////////////////////////////////////////////

void FormalNURBSCurve::createCircle()
{
  delete nurbs;

  nurbs= MLNURBS::getCircle9();
}

/////////////////////////////////////////////////////////////////////////////

/*
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
*/

/////////////////////////////////////////////////////////////////////////////

/*
void FormalNURBS::createSphere()
{
  delete nurbs;

//  nurbs= MLNURBS::getGeneralCylinder( *MLNURBS::getSemiCircle4(),
//                                      Vector(0,0,2) );
}
*/

/////////////////////////////////////////////////////////////////////////////

/*
void FormalNURBS::createCone()
{
  delete nurbs;

//  nurbs= MLNURBS::getGeneralCylinder( *MLNURBS::getSemiCircle4(),
//                                      Vector(0,0,2) );
}
*/

/////////////////////////////////////////////////////////////////////////////

/*
void FormalNURBS::createTorus()
{
  delete nurbs;

//  nurbs= MLNURBS::getGeneralCylinder( *MLNURBS::getSemiCircle4(),
//                                      Vector(0,0,2) );
}
*/

/////////////////////////////////////////////////////////////////////////////
