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
  MLGammaCorrection.C

  Stephane Rehel
  July 30 1997
*/

#include <math.h>

#include "tools/Common.h"
#include "tools/Color.h"

#include "MLGammaCorrection.h"

/////////////////////////////////////////////////////////////////////////////

MLGammaCorrection::MLGammaCorrection( double _gamma )
{
  gamma= _gamma;
  gamma= max( gamma, 0.01 );
  gamma= min( gamma, 10. );
  one_over_gamma= 1. / gamma;

  _hasCorrection= fabs(gamma - 1.) > 1e-4;

  for( int i= 0; i < 256; ++i )
    table[i]= int(.5 + 255. * pow( double(i) / 255., one_over_gamma ));
}

/////////////////////////////////////////////////////////////////////////////
static void foo() {}
double MLGammaCorrection::do_f( double d ) const
{
  foo(); // this is a bug in gcc 2.8.1 / linux !!!
  if( d <= 0. )
    return 0.;

  if( d >= 1. )
    return 1.;

  return pow( d, one_over_gamma );
}

/////////////////////////////////////////////////////////////////////////////

Color& MLGammaCorrection::do_c( Color& color ) const
{
  color[0]= do_f(color[0]);
  color[1]= do_f(color[1]);
  color[2]= do_f(color[2]);

  return color;
}

/////////////////////////////////////////////////////////////////////////////
