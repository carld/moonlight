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
  Color.C

  Creation: SR, March 27th, 1996, from color.h

  Colors Classes

  From modeleur, Tue Aug 11 1992 /Kepler SCFR.
  Revision: April 1994
*/

#include <math.h>

#include "Color.h"
#include "Color8.h"
#include "HLSColor.h"
#include "HSVColor.h"

/////////////////////////////////////////////////////////////////////////////

// static
const Color Color::Black = Color( 0., 0., 0. );
const Color Color::White = Color( 1., 1., 1. );

/////////////////////////////////////////////////////////////////////////////

Color::Color( const Color8& color )
{
  s.r= real(color.r()) / 255.0;
  s.g= real(color.g()) / 255.0;
  s.b= real(color.b()) / 255.0;
}

/////////////////////////////////////////////////////////////////////////////
// Conversion: HSVColor -> Color(RGB)
// From DKB 2.12 file IBM.C
//
Color::Color( const HSVColor& color )
{
//  if (hue == 360.0)
//     hue = 0.0;                         /* (THIS LOOKS BACKWARDS BUT OK) */
//it's better with: fmod( hue, 360. );  /Kepler, Aug 11th 92

  real xh = fmod( color.h()*360., 360 ) / 60.0, // convert hue to be in [0..6]
       i = floor(xh),                         // i = greatest integer <= h
       f = xh - i,                            // f = fractional part of h
      p1 = color.v() * (1 - color.s()),
      p2 = color.v() * (1 -(color.s() * f)),
      p3 = color.v() * (1 -(color.s() * (1 - f))),
       v = color.v();

  switch( (int) i )
    {
    case 0: s.r=  v; s.g= p3; s.b= p1; break;
    case 1: s.r= p2; s.g=  v; s.b= p1; break;
    case 2: s.r= p1; s.g=  v; s.b= p3; break;
    case 3: s.r= p1; s.g= p2; s.b=  v; break;
    case 4: s.r= p3; s.g= p1; s.b=  v; break;
    case 5: s.r=  v; s.g= p1; s.b= p2; break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Conversion: HLSColor -> Color(RGB)
// From Computer Graphics
//
Color::Color( const HLSColor& color )
{
  #define value(_n1,_n2,_hue) { \
            real n1= (_n1), n2= (_n2), hue= (_hue); \
            while( hue >= 360. ) hue -= 360.;      \
            while( hue < 0. ) hue += 360.;         \
            if( hue < 60. )                        \
              v= n1+(n2-n1)*hue/60.;               \
             else                                  \
              if( hue < 180. )                     \
                v= n2;                             \
               else                                \
                if( hue < 240. )                   \
                  v= n1+(n2-n1)*(240.-hue)/60.;    \
                 else                              \
                  v= n1;                           \
            }

  real H= color.h()*359., L= color.l(), S= color.s(), v, m1, m2;

  if( L<= 0.5 )
    {
    m1= 0.; // to avoid a g++ warning on m1
    m2= L*(1.+S);
    }
   else
    {
    m2= L+S-L*S;
    m1= 2.*L-m2;
    }
  if( S == 0. )
    s.r= s.g= s.b= L;
   else
    {
    value( m1, m2, H+120. ); s.r= v;
    value( m1, m2, H ); s.g= v;
    value( m1, m2, H-120. ); s.b= v;
    }
}

/////////////////////////////////////////////////////////////////////////////

void Color::correctRange()
{
  if( s.r < 0. ) s.r= 0.; else if( s.r > 1. ) s.r= 1.;
  if( s.g < 0. ) s.g= 0.; else if( s.g > 1. ) s.g= 1.;
  if( s.b < 0. ) s.b= 0.; else if( s.b > 1. ) s.b= 1.;
}

/////////////////////////////////////////////////////////////////////////////

