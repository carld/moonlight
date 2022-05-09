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
   Spectrum.h

   Stephane Rehel
   November 13 1996
*/

#ifndef __Spectrum_h
#define __Spectrum_h

class Color;

/////////////////////////////////////////////////////////////////////////////

class Spectrum
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
  Spectrum()
    {
    s.r= s.g= s.b= real(0.);
    }

  Spectrum( const Color& c );

  Spectrum( const Color& c, double intensity );

  Spectrum( const Spectrum& c )
    {
    s.r= c.s.r;
    s.g= c.s.g;
    s.b= c.s.b;
    }

  Spectrum( const real r, const real g, const real b )
    {
    s.r= r;
    s.g= g;
    s.b= b;
    }

  Spectrum& operator=( const Spectrum& c )
    {
    s.r= c.s.r;
    s.g= c.s.g;
    s.b= c.s.b;
    return *this;
    }

  real r() const { return s.r; }
  real g() const { return s.g; }
  real b() const { return s.b; }

  real* getCoord() { return vect; }
  const real* getConstCoord() const { return vect; }

  real& operator [] ( int i )
    {
    return vect[i];
    }

  void zero()
    {
    s.r= s.g= s.b= real(0.);
    }

  Spectrum operator + ( const Spectrum& c ) const
    {
    return Spectrum( s.r + c.s.r, s.g + c.s.g, s.b + c.s.b );
    }

  Spectrum& operator+=( const Spectrum& c )
    {
    s.r+= c.s.r;
    s.g+= c.s.g;
    s.b+= c.s.b;
    return *this;
    }

  Spectrum operator - ( const Spectrum& c ) const
    {
    return Spectrum( s.r - c.s.r,
                     s.g - c.s.g,
                     s.b - c.s.b );
    }

  Spectrum& operator-=( const Spectrum& c )
    {
    s.r-= c.s.r;
    s.g-= c.s.g;
    s.b-= c.s.b;
    return *this;
    }

  Spectrum operator* ( const real k ) const
    {
    return Spectrum( k*s.r, k*s.g, k*s.b );
    }

  Spectrum operator/ ( const real k ) const
    {
    return Spectrum( s.r/k, s.g/k, s.b/k );
    }

  Spectrum& operator *= ( const real d )
    {
    s.r *= d;
    s.g *= d;
    s.b *= d;
    return *this;
    }

  Spectrum& operator /= ( const real d )
    {
    s.r /= d;
    s.g /= d;
    s.b /= d;
    return *this;
    }

  Spectrum operator & ( const Spectrum& c ) const
    {
    return Spectrum( s.r*c.s.r, s.g*c.s.g, s.b*c.s.b );
    }

  Spectrum operator & ( const Color& c ) const;

  // multiply each component
  Spectrum& operator &= ( const Spectrum& c )
    {
    s.r *= c.s.r;
    s.g *= c.s.g;
    s.b *= c.s.b;
    return *this;
    }

  Spectrum& operator &= ( const Color& c );

  // return Watt power
  real power() const
    {
    static const real one_third= 1. / 3.;
    return (s.r+s.g+s.b) * one_third;
    }

  // return lumens power = 680*V(power)
  real luminousPower() const
    {
    // V(lambda): 35%(R) 55%(G) 10%(B)
    return 238.*s.r + 374.*s.g + 68.*s.b;
    }

  // return candelas power = luminousPower() / (4*PI)
  real candelasPower() const
    {
    return 18.94*s.r + 29.77*s.g + 5.41*s.b;
    }

  Color toColor() const;
};

/////////////////////////////////////////////////////////////////////////////

Spectrum operator & ( const Color& c, const Spectrum& s );

/////////////////////////////////////////////////////////////////////////////

inline Spectrum operator* ( const Spectrum::real k, const Spectrum& c )
{
  return Spectrum( k*c.r(), k*c.g(), k*c.b() );
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Spectrum_h
