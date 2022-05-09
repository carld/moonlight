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
  HSVColor.C

  Creation: SR, March 27th, 1996, from color.h

  Colors Classes

  From modeleur, Tue Aug 11 1992 /Kepler SCFR.
  Revision: April 1994
*/

#include "HSVColor.h"
#include "Color.h"

/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////HSVColor/
// Conversion: Color(RGB) -> HSVColor
// From DKB 2.12 file IBM.C
//
HSVColor::HSVColor( const Color& color )
{
  real nh = 0., ns;

  real nv = max ( color.r(), max( color.g(), color.b() ) ),
       m  = min ( color.r(), min( color.g(), color.b() ) );
  if( nv != 0.0 )                       /* if no value, it's black! */
    ns = (nv - m) / nv;
   else
    ns = 0.0;                           /* black = no colour saturation */
  if( ns == 0.0 )                       /* hue undefined if no saturation */
    {
    _h= _s= 0.;
    _v= nv;
    return;                             /* return black level (?) */
    }

  real r1 = (nv - color.r()) / (nv - m),    /* distance of color from red   */
       g1 = (nv - color.g()) / (nv - m),    /* distance of color from green */
       b1 = (nv - color.b()) / (nv - m);    /* distance of color from blue  */
  if( nv == color.r() )
    {
    if( m == color.g() )
      nh = 5. + b1;
     else
      nh = 1. - g1;
    }
  if( nv == color.g() )
    {
    if( m == color.b() )
      nh = 1. + r1;
     else
      nh = 3. - b1;
    }
  if( nv == color.b() )
    {
    if( m == color.r() )
      nh = 3. + g1;
     else
      nh = 5. - r1;
    }

  _h= (nh * 60.) / 360.; /* return h converted to degrees / 360. */
  _s= ns;
  _v= nv;
}

/////////////////////////////////////////////////////////////////////////////

