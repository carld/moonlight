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
  MLNURBS_Curve.C

  Stephane Rehel

  January 7 1998
*/

#include <math.h>

#include "tools/Matrix4x4.h"
#include "tools/HVector.h"

#include "MLNURBS_Curve.h"
#include "MLNURBS_BasisFunctions.h"

/////////////////////////////////////////////////////////////////////////////

MLNURBS_Curve::MLNURBS_Curve( int _n, int _degree ): kv(_n+_degree+1)
{
  left_right= 0;
  N_buffer= 0;

  n= _n;
  if( n < 0 )
    n= 0;

  cp= 0;

  alloc();
}

/////////////////////////////////////////////////////////////////////////////

MLNURBS_Curve::~MLNURBS_Curve()
{
  destroy();
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_Curve::alloc()
{
  destroy();

  cp= new MLNURBS_ControlPoint [ n + 1 ];

  int p= get_degree();
  left_right= new MLNURBS::real [ (p + 1) * 2 ];

  N_buffer= new MLNURBS::real [ p + 1 ];
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_Curve::destroy()
{
  if( cp != 0 )
    {
    delete [] cp;
    cp= 0;
    }

  delete left_right;
  left_right= 0;

  delete N_buffer;
  N_buffer= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_Curve::setControlPoints( const Point* points,
                                      const MLNURBS::real* weights /* = 0 */ )
{
  if( points == 0 && weights == 0 )
    return;

  if( points == 0 )
    {
    for( int i= 0; i <= n; ++i )
      cp[i].setWeight( weights[i] );
    return;
    }

  if( weights == 0 )
    {
    for( int i= 0; i <= n; ++i )
      cp[i].setPoint( points[i] );
    return;
    }

  for( int i= 0; i <= n; ++i )
    cp[i].set( points[i], weights[i] );
}

/////////////////////////////////////////////////////////////////////////////

Point MLNURBS_Curve::get( MLNURBS::real u ) const
{
  int p= get_degree();

  int span= kv.findSpan( getn(), p, u );

  MLNURBS_BasisFunctions bf(p,N_buffer);

  bf.compute( span, kv, u, left_right );

  HVector hpoint(0,0,0,0);

  int k= span-p+0;
  for( int i= 0; i <= p; ++i, ++k )
    hpoint+= cp[k].hpoint * bf.get(i);

  if( hpoint.h() == 0. )
    return Point( hpoint.x(), hpoint.y(), hpoint.z() );

  return Point(hpoint);
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_Curve::transform( const Matrix4x4& xform )
{
  for( int i= 0; i <= n; ++i )
    cp[i].setPoint( xform.apply(cp[i].point) );
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_Curve::getControlPointsBBox( Point& minPoint,
                                          double& minW,
                                          Point& maxPoint,
                                          double& maxW ) const
{
  if( n == 0 || cp == 0 )
    {
    minPoint= maxPoint= Point(0,0,0);
    minW= maxW= 0.;
    return;
    }

  minPoint= maxPoint= cp[0].point;
  minW= maxW= cp[0].w;

  for( int i= 1; i <= n; ++i )
    {
    const MLNURBS_ControlPoint& p= cp[i];
    minPoint= min( minPoint, p.point );
    maxPoint= max( maxPoint, p.point );
    minW= min((double)minW,(double)p.w );
    maxW= max((double)maxW,(double)p.w );
    }
}

/////////////////////////////////////////////////////////////////////////////

MLNURBS_Curve* MLNURBS_Curve::duplicate() const
{
  MLNURBS_Curve* c= new MLNURBS_Curve( n, get_degree() );

  for( int i= 0; i <= n; ++i )
    c->cp[i]= cp[i];

  return c;
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_Curve::insertKnot( MLNURBS::real u )
{
  if( u < kv.get(0) || u > kv.get(kv.getm()) )
    return;

  int p= get_degree();

  int span= kv.findSpan( getn(), p, u );

  int new_n= n + 1;

  MLNURBS_ControlPoint* new_cp= new MLNURBS_ControlPoint [ new_n + 1 ];

  int i;
  for( i= 0; i <= span - p; ++i )
    new_cp[i]= cp[i];

  for( i= span - p + 1; i <= span; ++i )
    {
    MLNURBS::real alpha= (u - kv.get(i)) / (kv.get(i+p) - kv.get(i));

    new_cp[i].set( cp[i].point * alpha + cp[i-1].point * (1.-alpha),
                   cp[i].w     * alpha + cp[i-1].w     * (1.-alpha) );
    }

  for( i= span+1; i <= new_n; ++i )
    new_cp[i]= cp[i];

  delete cp;

  cp= new_cp;
  n= new_n;

  kv.insert( u, span );
}

/////////////////////////////////////////////////////////////////////////////

// ask to remove a knot num times
// If u has been removed t times, returns t
int MLNURBS_Curve::removeKnot( int knot_index, int num )
{
  if( knot_index < 0 || knot_index > kv.getm() )
    return 0;

  Point minPoint, maxPoint;
  double minW, maxW;
  getControlPointsBBox( minPoint, minW, maxPoint, maxW );
  double tolerance= (maxPoint - minPoint).norm() * 1e-6;

  int m= getm();
  int n= getn();
  int p= get_degree();

  int ord= p+1;

  int r= knot_index;
  int s= kv.getMultiplicity(r);
  MLNURBS::real u= kv[r];

  if( s == 0 )
    return 0;

  int fout= (2*r-s-p) / 2;
  int last= r - s;
  int first= r - p;

  HVector* temp= new HVector [ 2 * p + 1 ];

  int t= 0;
  for( ; t < num; ++t )
    {
    int off= first - 1;
    temp[0]= cp[off].hpoint;
    temp[last+1-off]= cp[last+1].hpoint;
    int i= first;
    int j= last;

    int ii= 1;
    int jj= last - off;
    int remflag= 0;
    while( j - i > t )
      {
      MLNURBS::real alfi= (u-kv[i  ]) / (kv[i+ord+t]-kv[i  ]);
      MLNURBS::real alfj= (u-kv[j-t]) / (kv[j+ord  ]-kv[j-t]);
      temp[ii]= (cp[i].hpoint-(1.-alfi)*temp[ii-1])/alfi;
      temp[jj]= (cp[j].hpoint-alfj*temp[jj+1])/(1.-alfj);
      ++i;
      ++ii;
      --j;
      --jj;
      }

    if( j - i < t )
      {
      if( (temp[ii-1]-temp[jj+1]).absNorm() < tolerance )
        remflag= 1;
      }
     else
      {
      MLNURBS::real alfi= (u-kv[i]) / (kv[i+ord+t]-kv[i]);

      if( (cp[i].hpoint - (alfi*temp[ii+t+1]+(1.-alfi)*temp[ii-1])).absNorm()
          < tolerance )
        remflag= 1;
      }

    if( remflag == 0 )
      break;

    i= first;
    j= last;
    while( j - i > t )
      {
      cp[i].set( temp[i-off] );
      cp[j].set( temp[j-off] );
      ++i;
      --j;
      }

    --first;
    ++last;
    }

  delete temp;
  temp= 0;

  if( t == 0 )
    return 0;

  int k;
  for( k= r + 1; k <= m; ++k )
    kv.U[k-t]= kv.U[k];
  kv.m -= t;

  int j= fout;
  int i= j;
  for( k= 1; k < t; ++k )
    if( (k & 1) == 1 )
      ++i;
     else
      --j;

  // i like to move-it move-it
  for( k= i + 1; k <= n; ++k )
    {
    cp[j]= cp[k];
    ++j;
    }

  n -= t;

  return t;
}

/////////////////////////////////////////////////////////////////////////////
