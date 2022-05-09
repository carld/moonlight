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
   CurveRep.C

   Stephane Rehel

   January 21 1998
*/

#include <stdio.h>
#include <math.h>

#include "MLCurveVertex.h"

#include "CurveRep.h"

#ifndef NDEBUG
// #define CURVEREP_DEBUG 1
#endif

/////////////////////////////////////////////////////////////////////////////

CurveRep::CurveRep()
{
  nVertices= 0;
  vertices= new MLCurveVertex [ 1 ];
  closed= false;
}

/////////////////////////////////////////////////////////////////////////////

CurveRep::~CurveRep()
{
  clear();

  delete [] vertices;
  vertices= 0;
}

/////////////////////////////////////////////////////////////////////////////

int CurveRep::getSizeOf() const
{
  int s= sizeof(*this) + nVertices * sizeof(vertices[0]);

  return s;
}

/////////////////////////////////////////////////////////////////////////////

CurveRep* CurveRep::duplicate()
{
  CurveRep* curve= new CurveRep;

  curve->alloc(nVertices);
  curve->closed= closed;

  for( int i= 1; i <= nVertices; ++i )
    curve->vertices[i]= vertices[i];

  return curve;
}

/////////////////////////////////////////////////////////////////////////////

void CurveRep::clear()
{
  if( nVertices == 0 )
    return;

  delete [] vertices;
  vertices= new MLCurveVertex [ 1 ];

  nVertices= 0;

  closed= false;
}

/////////////////////////////////////////////////////////////////////////////

void CurveRep::alloc( int _nVertices )
{
  clear();

  nVertices= max( 0, _nVertices );

  delete [] vertices;
  vertices= new MLCurveVertex [ nVertices + 1 ];
}

/////////////////////////////////////////////////////////////////////////////

int CurveRep::getParameterIndex( double t ) const
{
  if( nVertices == 0 )
    return 1;

  if( t <= vertices[1].parameter )
    return 1;

  if( t >= vertices[nVertices].parameter )
    return nVertices;

  int i1= 1;
  int i2= nVertices;
  for(;;)
    {
    if( i1 == i2 )
      break;
    int mid= (i1+i2) / 2;
    int mid1= mid + 1;
    if( mid1 > nVertices )
      mid= nVertices;

    if( t < vertices[mid].parameter )
      i2= mid;
     else
      {
      if( t < vertices[mid1].parameter )
        break;
       else
        i1= mid;
      }
    }

  return i1;
}

/////////////////////////////////////////////////////////////////////////////

Point CurveRep::interpolate( double parameter ) const
{
  if( nVertices == 0 )
    return Point(0,0,0);

  if( parameter <= vertices[1].parameter )

  if( !closed && parameter >= vertices[nVertices].parameter )
    return vertices[nVertices].point;

  int i= getParameterIndex(parameter);
  if( !closed && i >= nVertices )
    return vertices[nVertices].point;

  double dt;
  int i1;
  if( closed && i == nVertices )
     {
     i1= 1;
     dt= 1. - vertices[i].parameter;
     }
    else
     {
     i1= i+1;
     dt= vertices[i1].parameter - vertices[i].parameter;
     }

  if( fabs(dt) < 1e-20 )
    return vertices[i].point;

  double t= (parameter - vertices[i].parameter) / dt;

  return vertices[i].point * (1.-t) + vertices[i1].point * t;
}

/////////////////////////////////////////////////////////////////////////////
