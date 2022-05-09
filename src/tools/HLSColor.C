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
  HLSColor.C

  Creation: SR, March 27th, 1996, from color.h

  Colors Classes

  From modeleur, Tue Aug 11 1992 /Kepler SCFR.
  Revision: April 1994
*/

#include <math.h>

#include "Common.h"

#include "HLSColor.h"
#include "Color.h"


/////////////////////////////////////////////////////////////////////////////
// Conversion: Color(RGB) -> HLSColor
// From Computer Graphics
//

HLSColor::HLSColor( const Color& color )
{
  real maxc, minc;

  maxc= max( color.r(), max( color.g(), color.b() ) );
  minc= min( color.r(), min( color.g(), color.b() ) );
  _l= (maxc+minc)/2.0;
  if( fabs(maxc-minc) < 1e-8 )
    _s= _h= 0.;
   else
    {
    DBL delta= maxc-minc;
    _s= (_l<=0.5) ? (delta/(maxc+minc)) : (delta/(2.-maxc-minc));
    if( fabs(color.r() - maxc) < 1e-10 )
      _h= (color.g()-color.b())/delta;
     else
      if( fabs(color.g() - maxc) < 1e-10 )
        _h= 2.+(color.b()-color.r())/delta;
       else
        if( fabs(color.b() - maxc) < 1e-10 )
          _h= 4.+(color.r()-color.g())/delta;
    _h *= 60.;
    if( _h < 0. )
      _h += 360.;
    }

  _h /= 359.;
}

/////////////////////////////////////////////////////////////////////////////
