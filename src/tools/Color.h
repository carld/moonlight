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
  Color.h

  Creation: SR, March 27th, 1996, from color.h

  Colors Classes

  From modeleur, Tue Aug 11 1992 /Kepler SCFR.
  Revision: April 1994
*/

#ifndef __Color_h
#define __Color_h

// for fabs()
#include <math.h>

#ifndef __Common_h
#include "Common.h"
#endif

class Color8;
class HSVColor;
class HLSColor;


///////////////////////////////////////////////////////////////////////Color/
// Color class (RGB: default format)
//
class Color
{
public:
  typedef float real;

private:
  union
    {
    struct
      {
      real r, g, b;
      } s;
    real vect[3];
    };
public:
  static const Color Black;
  static const Color White;

  Color( )
    {}
  Color( const Color& c )
    {
    s.r= c.s.r;
    s.g= c.s.g;
    s.b= c.s.b;
    }
  Color( const real r, const real g, const real b )
    {
    s.r= r;
    s.g= g;
    s.b= b;
    }

  static Color gray( const real i )
    {
    return Color( i, i, i );
    }

  Color& operator=( const Color& c )
    {
    s.r= c.s.r;
    s.g= c.s.g;
    s.b= c.s.b;
    return *this;
    }

  Color( const HSVColor& color );
  Color( const HLSColor& color );
  Color( const Color8& color );

  real r() const { return s.r; }
  real g() const { return s.g; }
  real b() const { return s.b; }

  real* getCoord() { return vect; }
  const real* getConstCoord() const { return vect; }

  real& operator [] ( int i )
    {
    return vect[i];
    }

  void set( BYTE r, BYTE g, BYTE b )
    {
    static const real one_over_255 = 1. / 255.;
    s.r= real(r) * one_over_255;
    s.g= real(g) * one_over_255;
    s.b= real(b) * one_over_255;
    }

  boolean operator == ( const Color& c ) const
    {
    return s.r==c.s.r && s.g==c.s.g && s.b==c.s.b;
    }
  boolean operator != ( const Color& c ) const
    {
    return s.r!=c.s.r || s.g!=c.s.g || s.b!=c.s.b;
    }

  Color operator + ( const Color& c ) const
    {
    return Color( s.r + c.s.r, s.g + c.s.g, s.b + c.s.b );
    }
  Color& operator+=( const Color& c )
    {
    s.r+= c.s.r;
    s.g+= c.s.g;
    s.b+= c.s.b;
    return *this;
    }
  Color operator - ( const Color& c ) const
    {
    return Color( s.r - c.s.r, s.g - c.s.g, s.b - c.s.b );
    }
  Color& operator-=( const Color& c )
    {
    s.r-= c.s.r;
    s.g-= c.s.g;
    s.b-= c.s.b;
    return *this;
    }
  Color operator* ( const real k ) const
    {
    return Color( k*s.r, k*s.g, k*s.b );
    }
  Color operator/ ( const real k ) const
    {
    return Color( s.r/k, s.g/k, s.b/k );
    }
  Color& operator*=( const real d )
    {
    s.r *= d;
    s.g *= d;
    s.b *= d;
    return *this;
    }
  Color& operator/=( const real d )
    {
    s.r /= d;
    s.g /= d;
    s.b /= d;
    return *this;
    }

  Color operator & ( const Color& v ) const
    {
    return Color( s.r*v.s.r, s.g*v.s.g, s.b*v.s.b );
    }

  Color& operator &= ( const Color& v )
    {
    s.r *= v.s.r;
    s.g *= v.s.g;
    s.b *= v.s.b;
    return *this;
    }

  real getGray() const
    {
    return 0.35*s.r + 0.55*s.g + 0.10*s.b;
    }

  real cabs() const
    {
    return fabs(s.r) + fabs(s.g) + fabs(s.b);
    }

  void correctRange();
};

/////////////////////////////////////////////////////////////////////////////

inline Color operator* ( const Color::real k, const Color& c )
{
  return Color( k*c.r(), k*c.g(), k*c.b() );
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Color_h

