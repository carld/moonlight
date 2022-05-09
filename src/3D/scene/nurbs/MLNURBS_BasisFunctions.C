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
  MLNURBS_BasisFunctions.C

  Stephane Rehel

  January 7 1998
*/

#include "MLNURBS_BasisFunctions.h"
#include "MLNURBS_KnotVector.h"

/////////////////////////////////////////////////////////////////////////////

MLNURBS_BasisFunctions::MLNURBS_BasisFunctions( int _p,
                                                MLNURBS::real* _N /* = 0 */ )
{
  p= _p;
  if( p < 0 )
    p= 0;

  N= _N;
  own_N= (_N == 0);

  alloc();
}

/////////////////////////////////////////////////////////////////////////////

MLNURBS_BasisFunctions::~MLNURBS_BasisFunctions()
{
  destroy();
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_BasisFunctions::alloc()
{
  destroy();

  if( own_N )
    N= new MLNURBS::real [ p + 1 ];

  // CAREFUL: it is left uninitialized for speed
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_BasisFunctions::destroy()
{
  if( own_N )
    {
    delete N;
    N= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

// The NURBS Book, p.70
void MLNURBS_BasisFunctions::compute( int span,
                                      const MLNURBS_KnotVector& kv,
                                      MLNURBS::real u,
                                      MLNURBS::real* left_right /* = 0 */ )
{
  MLNURBS::real* _left_right= 0;
  register MLNURBS::real* left;
  register MLNURBS::real* right;

  if( left_right != 0 )
    {
    left= left_right;
    right= left_right + p + 1;
    }
   else
    {
    _left_right= new MLNURBS::real [ 2*(p+1) ];
    left= _left_right;
    right= _left_right + p + 1;
    }

  N[0]= 1.;

  for( int j= 1; j <= p; ++j )
    {
    left [j]= u - kv.get(span+1-j);
    right[j]= kv.get(span+j) - u;

    register MLNURBS::real saved= 0.;
    for( register int r= 0; r < j; ++r )
      {
      register MLNURBS::real temp= N[r] / (right[r+1] + left[j-r]);
      N[r]= saved + right[r+1] * temp;
      saved= left[j-r] * temp;
      }

    N[j]= saved;
    }

  if( left_right == 0 )
    delete _left_right;
}

/////////////////////////////////////////////////////////////////////////////
