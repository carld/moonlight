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
  MLNURBS_Surface.C

  Stephane Rehel

  January 8 1998
*/

#include "tools/Matrix4x4.h"

#include "MLNURBS_Surface.h"
#include "MLNURBS_BasisFunctions.h"
#include "MLNURBS_ControlPoint.h"

/////////////////////////////////////////////////////////////////////////////

MLNURBS_Surface::MLNURBS_Surface( int _n, int _u_degree,
                                  int _m, int _v_degree ):
  kvu(_n+_u_degree+1),
  kvv(_m+_v_degree+1)
{
  left_right= 0;
  Nu_buffer= 0;
  Nv_buffer= 0;

  n= _n;
  if( n < 0 )
    n= 0;

  m= _m;
  if( m < 0 )
    m= 0;

  cp= 0;

  alloc();
}

/////////////////////////////////////////////////////////////////////////////

MLNURBS_Surface::~MLNURBS_Surface()
{
  destroy();
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_Surface::alloc()
{
  destroy();

  cp= new MLNURBS_ControlPoint* [ n + 1 ];

  for( int i= 0; i <= n; ++i )
    cp[i]= new MLNURBS_ControlPoint [ m + 1 ];

  int p= get_u_degree();
  int q= get_v_degree();
  left_right= new MLNURBS::real [ (((p>q) ? p : q) + 1) * 2 ];

  Nu_buffer= new MLNURBS::real [ p + 1 ];
  Nv_buffer= new MLNURBS::real [ q + 1 ];
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_Surface::destroy()
{
  if( cp != 0 )
    {
    for( int i= 0; i <= n; ++i )
      {
      if( cp[i] != 0 )
        delete [] cp[i];
      }

    delete cp;
    cp= 0;
    }

  delete left_right;
  left_right= 0;

  delete Nu_buffer;
  Nu_buffer= 0;

  delete Nv_buffer;
  Nv_buffer= 0;
}

/////////////////////////////////////////////////////////////////////////////

Point MLNURBS_Surface::get( MLNURBS::real u, MLNURBS::real v ) const
{
  int p= get_u_degree();
  int q= get_v_degree();

  MLNURBS_BasisFunctions ubf(p,Nu_buffer);
  int uspan= kvu.findSpan( getn(), p, u );
  ubf.compute( uspan, kvu, u, left_right );

  MLNURBS_BasisFunctions vbf(q,Nv_buffer);
  int vspan= kvv.findSpan( getm(), q, v );
  vbf.compute( vspan, kvv, v, left_right );

  HVector hpoint(0,0,0,0);

/*
  int uind= uspan - p;
  for( int j= 0; j <= q; ++j )
    {
    HVector temp(0,0,0,0);

    int vind= vspan - q + j;
    for( int i= 0; i <= p; ++i )
      temp += cp[uind+i][vind].hpoint * ubf.get(i);

    hpoint+= temp * vbf.get(j);
    }
*/

  int k= uspan - p;
  for( int i= 0; i <= p; ++i, ++k )
    {
    HVector temp(0,0,0,0);

    register MLNURBS_ControlPoint* _cp= cp[k] + vspan-q;
    for( register int j= 0; j <= q; ++j, ++_cp )
      temp += _cp->hpoint * vbf.get(j);

    hpoint+= temp * ubf.get(i);
    }

  if( hpoint.h() == 0. )
    return Point( hpoint.x(), hpoint.y(), hpoint.z() );

  return Point(hpoint);
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_Surface::transform( const Matrix4x4& xform )
{
  for( int i= 0; i <= n; ++i )
    for( int j= 0; j <= m; ++j )
      cp[i][j].setPoint( xform.apply(cp[i][j].point) );
}

/////////////////////////////////////////////////////////////////////////////

MLNURBS_Surface* MLNURBS_Surface::duplicate() const
{
  MLNURBS_Surface* s= new MLNURBS_Surface( n, get_u_degree(),
                                           m, get_v_degree() );

  for( int i= 0; i <= n; ++i )
    for( int j= 0; j <= m; ++j )
      s->cp[i][j]= cp[i][j];

  return s;
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_Surface::insertUKnot( MLNURBS::real u )
{
  if( u < kvu.get(0) || u > kvu.get(kvu.getm()) )
    return;

  int p= get_u_degree();

  int span= kvu.findSpan( getn(), p, u );

  int i, j;

  int new_n= n + 1;

  MLNURBS_ControlPoint** new_cp= new MLNURBS_ControlPoint* [ new_n + 1 ];
  for( i= 0; i <= new_n; ++i )
    new_cp[i]= new MLNURBS_ControlPoint [ m + 1 ];

  for( i= 0; i <= span - p; ++i )
    {
    for( j= 0; j <= m; ++j )
      new_cp[i][j]= cp[i][j];
    }

  for( i= span - p + 1; i <= span; ++i )
    {
    MLNURBS::real alpha= (u - kvu.get(i)) / (kvu.get(i+p) - kvu.get(i));

    for( j= 0; j <= m; ++j )
      {
      new_cp[i][j].set( cp[i][j].point*alpha + cp[i-1][j].point*(1.-alpha),
                        cp[i][j].w    *alpha + cp[i-1][j].w    *(1.-alpha) );
      }
    }

  for( i= span+1; i <= new_n; ++i )
    for( j= 0; j <= m; ++j )
      new_cp[i][j]= cp[i][j];

  delete cp;

  cp= new_cp;
  n= new_n;

  kvu.insert( u, span );
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_Surface::insertVKnot( MLNURBS::real v )
{
  if( v < kvv.get(0) || v > kvv.get(kvv.getm()) )
    return;

  int q= get_v_degree();

  int span= kvu.findSpan( getn(), q, v );

  int i, j;

  int new_m= m + 1;

  MLNURBS_ControlPoint** new_cp= new MLNURBS_ControlPoint* [ n + 1 ];
  for( i= 0; i <= n; ++i )
    new_cp[i]= new MLNURBS_ControlPoint [ new_m + 1 ];

  for( j= 0; j <= span - q; ++j )
    {
    for( i= 0; i <= n; ++i )
      new_cp[i][j]= cp[i][j];
    }

  for( j= span - q + 1; j <= span; ++j )
    {
    MLNURBS::real alpha= (v - kvv.get(j)) / (kvv.get(j+q) - kvv.get(j));

    for( i= 0; i <= n; ++i )
      {
      new_cp[i][j].set( cp[i][j].point*alpha + cp[i-1][j].point*(1.-alpha),
                        cp[i][j].w    *alpha + cp[i-1][j].w    *(1.-alpha) );
      }
    }

  for( j= span+1; j <= new_m; ++j )
    for( i= 0; i <= n; ++i )
      new_cp[i][j]= cp[i][j];

  delete cp;

  cp= new_cp;
  m= new_m;

  kvv.insert( v, span );
}

/////////////////////////////////////////////////////////////////////////////
