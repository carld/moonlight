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
  MLNURBS.C

  Stephane Rehel

  January 7 1998
*/

#include "tools/Matrix3x3.h"

#include "MLNURBS.h"

#include "MLNURBS_ControlPoint.h"
#include "MLNURBS_KnotVector.h"

#include "MLNURBS_Curve.h"
#include "MLNURBS_Surface.h"

/////////////////////////////////////////////////////////////////////////////

// static
MLNURBS_Curve* MLNURBS::getSegment( const Point& p0, const Point& p1 )
{
  MLNURBS_Curve* c= new MLNURBS_Curve( 1, 1 );

  MLNURBS::real knots[4] = { 0., 0., 1., 1. };
  c->getKnotVector().set(knots);

  MLNURBS::real weights[4] = { 1., 1., 1., 1. };
  Point points[2]= { p0, p1 };

  c->setControlPoints( points, weights );

  return c;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLNURBS_Curve* MLNURBS::getSegment4( const Point& p0, const Point& p1, const Point& p2, const Point& p3 )
{
  MLNURBS_Curve* c= new MLNURBS_Curve( 3, 2 );

  MLNURBS::real knots[7] = {
                           0., 0., 0.,
                           0.5,
                           1., 1., 1.
                           };
  c->getKnotVector().set(knots);

  double w1= 1.;
  double w2= 1. / 2.;
  MLNURBS::real weights[4] = { w1, w2, w2, w1 };

  Point points[4]= { p0,p1,p2,p3 };

  c->setControlPoints( points, weights );

  return c;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLNURBS_Curve* MLNURBS::getSemiCircle4()
{
  MLNURBS_Curve* c= new MLNURBS_Curve( 3, 2 );

  MLNURBS::real knots[7] = {
                           0., 0., 0.,
                           0.5,
                           1., 1., 1.
                           };
  c->getKnotVector().set(knots);

  double w1= 1.;
  double w2= 1. / 2.;
  MLNURBS::real weights[4] = { w1, w2, w2, w1 };

  double r= 1.;
  Point center(0,0,0);
  Point points[4]= {
                   center + r*Vector(1,0,0),
                   center + r*Vector(1,1,0),
                   center + r*Vector(-1,1,0),
                   center + r*Vector(-1,0,0)
                   };

  c->setControlPoints( points, weights );

  return c;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLNURBS_Curve* MLNURBS::getCircle9()
{
  MLNURBS_Curve* c= new MLNURBS_Curve( 8, 2 );

  MLNURBS::real knots[12] = {
                            0., 0., 0.,
                            0.25, 0.25, 0.5, 0.5, 0.75, 0.75,
                            1., 1., 1.
                            };
  c->getKnotVector().set(knots);

  double w1= 1.;
  double w2= sqrt(2.) / 2.;
  MLNURBS::real weights[9] = { w1, w2, w1, w2, w1, w2, w1, w2, w1 };

  double r= 1.;
  Point center(0,0,0);
  Point points[9]= {
                   Point(center.x()+r,center.y(),  0.),
                   Point(center.x()+r,center.y()+r,0.),
                   Point(center.x()  ,center.y()+r,0.),
                   Point(center.x()-r,center.y()+r,0.),
                   Point(center.x()-r,center.y()  ,0.),
                   Point(center.x()-r,center.y()-r,0.),
                   Point(center.x()  ,center.y()-r,0.),
                   Point(center.x()+r,center.y()-r,0.),
                   Point(center.x()+r,center.y()  ,0.)
                   };

  c->setControlPoints( points, weights );

  return c;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLNURBS_Curve* MLNURBS::getCircle7Triangle()
{
  MLNURBS_Curve* c= new MLNURBS_Curve( 6, 2 );

  MLNURBS::real knots[10] = {
                            0., 0., 0.,
                            1./3., 1./3., 2./3., 2./3.,
                            1., 1., 1.
                            };
  c->getKnotVector().set(knots);

  double w1= 1.;
  double w2= 1. / 2.;
  MLNURBS::real weights[7] = { w1, w2, w1, w2, w1, w2, w1 };

//  double r= 1.;
//  Point center(0,0,0);
  double a= cos( 30. / 180. * M_PI );
  Point points[7]= {
                   Point(a,0.5,0.),
                   Point(0.,2.,0.),
                   Point(-a,0.5,0.),
                   Point(-2.*a,-1.,0.),
                   Point(0.,-1.,0.),
                   Point(-2.*a,-1.,0.),
                   Point(a,0.5,0.)
                   };

  c->setControlPoints( points, weights );

  return c;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLNURBS_Curve* MLNURBS::getCircle7Square()
{
  MLNURBS_Curve* c= new MLNURBS_Curve( 6, 2 );

  MLNURBS::real knots[10] = {
                            0., 0., 0.,
                            0.25, 0.5, 0.5, 0.75,
                            1., 1., 1.
                            };
  c->getKnotVector().set(knots);

  double w1= 1.;
  double w2= 1. / 2.;
  MLNURBS::real weights[7] = { w1, w2, w2, w1, w2, w2, w1 };

  double r= 1.;
  Point center(0,0,0);
  Point points[7]= {
                   Point(center.x()+r,center.y()  ,0.),
                   Point(center.x()+r,center.y()+r,0.),
                   Point(center.x()-r,center.y()+r,0.),
                   Point(center.x()-r,center.y()  ,0.),
                   Point(center.x()-r,center.y()-r,0.),
                   Point(center.x()+r,center.y()-r,0.),
                   Point(center.x()+r,center.y()  ,0.),
                   };

  c->setControlPoints( points, weights );

  return c;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLNURBS_Surface* MLNURBS::getBilinear( const Point& p00,
                                       const Point& p10,
                                       const Point& p11,
                                       const Point& p01 )
{
  MLNURBS_Surface* s= new MLNURBS_Surface( 1, 1, 1, 1 );

  MLNURBS::real knots[4] = { 0., 0., 1., 1. };

  s->getUKnotVector().set(knots);
  s->getVKnotVector().set(knots);

  s->getControlPoint(0,0).set(p00,1.);
  s->getControlPoint(1,0).set(p10,1.);
  s->getControlPoint(1,1).set(p11,1.);
  s->getControlPoint(0,1).set(p01,1.);

  return s;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLNURBS_Surface* MLNURBS::getGeneralCylinder( MLNURBS_Curve& curve,
                                              const Vector& distance )
{
  MLNURBS_Surface* s= new MLNURBS_Surface( curve.getn(),
                                           curve.get_degree(),
                                           1,
                                           1 );

  s->getUKnotVector().set( &(curve.getKnotVector()[0]) );

  MLNURBS::real vknots[4] = { 0., 0., 1., 1. };
  s->getVKnotVector().set( vknots );

  for( int i= 0; i <= curve.getn(); ++i )
    {
    const MLNURBS_ControlPoint& cp= curve.getConstControlPoint(i);

    MLNURBS_ControlPoint& sp0= s->getControlPoint(i,0);
    MLNURBS_ControlPoint& sp1= s->getControlPoint(i,1);

    sp0= cp;
    sp1.set( cp.point + distance, cp.w );
    }

  return s;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLNURBS_Surface* MLNURBS::getSurfaceRevolution( MLNURBS_Curve& curve,
                                                double /*radius*/ )
{
  MLNURBS_Surface* s= new MLNURBS_Surface( 8, 2,
                                           curve.getn(),
                                           curve.get_degree() );

  MLNURBS::real uknots[12] = {
                             0., 0., 0.,
                             0.25, 0.25, 0.5, 0.5, 0.75, 0.75,
                             1., 1., 1.
                             };
  s->getUKnotVector().set(uknots);
  s->getVKnotVector().set( &(curve.getKnotVector()[0]) );

  double w1= 1.;
  double w2= sqrt(2.) / 2.;

  for( int j= 0; j <= curve.getn(); ++j )
    {
    const MLNURBS_ControlPoint& cp= curve.getConstControlPoint(j);

    for( int i= 0; i <= 8; ++i )
      {
      MLNURBS_ControlPoint& sp= s->getControlPoint(i,j);

      if( (i & 1) == 0 )
        sp.setWeight( cp.w * w1 );
       else
        sp.setWeight( cp.w * w2 );
      }
    }

  for( int i= 0; i <= 8; ++i )
    {
    double theta= - M_PI*2. * double(i) / 8.;
    Matrix3x3 m;
    m.rotation( Vector(0,0,1), theta );

    Matrix3x3 m1;
    m1.scaling( Vector( ((i&1)==0) ? w1 : (1./w2),
                        ((i&1)==0) ? w1 : (1./w2),
                        1. ) );
    m= m1 * m;

    for( int j= 0; j <= curve.getn(); ++j )
      {
      const MLNURBS_ControlPoint& cp= curve.getConstControlPoint(j);

      Vector v= m * Vector( cp.point.x(), cp.point.y(), cp.point.z() );

      MLNURBS_ControlPoint& sp= s->getControlPoint(i,j);

      sp.setPoint( Point(v.x(),v.y(),v.z()) );
      }
    }

  return s;
}

/////////////////////////////////////////////////////////////////////////////
