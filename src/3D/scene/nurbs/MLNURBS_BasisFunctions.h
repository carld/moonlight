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
  MLNURBS_BasisFunctions.h

  Stephane Rehel

  January 7 1998
*/

#ifndef __MLNURBS_BasisFunctions_h
#define __MLNURBS_BasisFunctions_h

#include <assert.h>

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLNURBS_h
#include "MLNURBS.h"
#endif

class MLNURBS_KnotVector;

/////////////////////////////////////////////////////////////////////////////

class MLNURBS_BasisFunctions
{
protected:
  int p;
  MLNURBS::real* N;

  boolean own_N;

public:
  MLNURBS_BasisFunctions( int _p, MLNURBS::real* _N = 0 );

  virtual ~MLNURBS_BasisFunctions();

protected:
  void alloc();
  void destroy();

public:
  int getp() const
    {
    return p;
    }

  // 0 <= i <= n
  MLNURBS::real get( int i ) const
    {
    assert( i >= 0 );
    assert( i <= p );
    return N[i];
    }

  void compute( int span,
                const MLNURBS_KnotVector& kv,
                MLNURBS::real u,
                MLNURBS::real* left_right = 0 );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLNURBS_BasisFunctions_h
