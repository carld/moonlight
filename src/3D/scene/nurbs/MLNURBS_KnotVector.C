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
  MLNURBS_KnotVector.C

  Stephane Rehel

  January 7 1998
*/

#include <math.h>

#include "MLNURBS_KnotVector.h"

/////////////////////////////////////////////////////////////////////////////

MLNURBS_KnotVector::MLNURBS_KnotVector( int _m )
{
  m= _m;
  if( m < 0 )
    m= 0;

  U= 0;

  alloc();
}

/////////////////////////////////////////////////////////////////////////////

MLNURBS_KnotVector::~MLNURBS_KnotVector()
{
  destroy();
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_KnotVector::alloc()
{
  delete U;
  U= new MLNURBS::real [ m + 1 ];

  int middle= m/2;
  for( int i= 0; i <= m; ++i )
    U[i]= (i<=middle) ? 0. : 1.;
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_KnotVector::destroy()
{
  delete U;
  U= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_KnotVector::set( const MLNURBS::real* knots )
{
  if( knots == 0 )
    return;

  for( int i= 0; i <= m; ++i )
    U[i]= knots[i];
}

/////////////////////////////////////////////////////////////////////////////

// n+1 basis functions or n+1 control points
// degree = p
// The NURBS Book, p.68
int MLNURBS_KnotVector::findSpan( int n, int p, MLNURBS::real u ) const
{
//  if( u <= U[p+1] )
//     return p;
  if( u >= U[n+1] )
    return n;

  register int low= p;
  register int high= n+1;
  register int mid= (low+high) / 2;

  int infinite_loop= 10+n*2; // a la louche

  for(;;)
    {
    if( u < U[mid] )
      {
      high= mid;
      }
     else
      {
      if( u >= U[mid+1] )
        {
        low= mid;
        }
       else
        break;
      }

    if( ! --infinite_loop )
      break; // this should never happen, this a bug

    mid= (low+high) / 2;
    }

  return mid; // U[mid] <= u < U[mid+1]
}

/////////////////////////////////////////////////////////////////////////////

int MLNURBS_KnotVector::findSpan( MLNURBS::real u ) const
{
  return findSpan( m-1, 0, u );
}

/////////////////////////////////////////////////////////////////////////////

MLNURBS_KnotVector* MLNURBS_KnotVector::duplicate() const
{
  MLNURBS_KnotVector* kv= new MLNURBS_KnotVector(m);

  for( int i= 0; i <= m; ++i )
    (*kv)[i]= get(i);

  return kv;
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_KnotVector::insert( MLNURBS::real value,
                                 int span,
                                 int mul /* = 1 */ )
{
  if( U == 0 )
    return;
  if( span < 0 || span > m )
    return;

  int new_m= m + mul;
  MLNURBS::real* new_U= new MLNURBS::real [ new_m + 1 ];

  int i;
  int j= 0;

  for( i= 0; i <= span; ++i )
    new_U[j++]= U[i];

  for( i= 0; i < mul; ++i )
    new_U[j++]= value;

  for( i= span+1; i <= m; ++i )
    new_U[j++]= U[i];

  delete U;

  U= new_U;
  m= new_m;
}

/////////////////////////////////////////////////////////////////////////////

int MLNURBS_KnotVector::getMultiplicity( int span ) const
{
  if( span < 0 || span > m )
    return 0;

  const MLNURBS::real epsilon= fabs(U[m]) * 1e-6;

  MLNURBS::real u= U[span];
  int mul= 1;
  int i;

  i= span-1;
  while( i >= 0 )
    {
    if( fabs(u - U[i]) > epsilon )
      break;
    ++mul;
    --i;
    }

  i= span+1;
  while( i <= m )
    {
    if( fabs(u - U[i]) > epsilon )
      break;
    ++mul;
    ++i;
    }

  return mul;
}

/////////////////////////////////////////////////////////////////////////////

int MLNURBS_KnotVector::getMultiplicity( MLNURBS::real u ) const
{
  const MLNURBS::real epsilon= fabs(U[m]) * 1e-6;

  int span= -1;
  for( int i= 0; i <= m; ++i )
    if( fabs(u - U[i]) < epsilon )
      {
      span= i;
      break;
      }

  if( span == -1 )
    return 0;

  return getMultiplicity(span);
}

/////////////////////////////////////////////////////////////////////////////
