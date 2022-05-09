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
  MLGammaCorrection.h

  Stephane Rehel
  July 30 1997
*/

#ifndef __MLGammaCorrection_h
#define __MLGammaCorrection_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class Color;

/////////////////////////////////////////////////////////////////////////////

class MLGammaCorrection
{
protected:
  boolean _hasCorrection;
  double gamma;
  double one_over_gamma;
  unsigned char table[256];

public:
  MLGammaCorrection( double _gamma );

  virtual ~MLGammaCorrection()
    {}

  boolean hasCorrection() const
    {
    return _hasCorrection;
    }
  double getGamma() const
    {
    return gamma;
    }
  double getOneOverGamma() const
    {
    return one_over_gamma;
    }
  const unsigned char* getTable() const
    {
    return table;
    }

  unsigned char do_ub( unsigned char c ) const
    {
    return table[c];
    }

  // d clamped in [0..1]
  double do_f( double d ) const;

  void do_ub( unsigned char& r,
              unsigned char& g,
              unsigned char& b ) const
    {
    r= table[r];
    g= table[g];
    b= table[b];
    }

  Color& do_c( Color& color ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLGammaCorrection_h
