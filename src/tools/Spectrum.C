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
   Spectrum.C

   Stephane Rehel
   November 13 1996
*/

#include "Spectrum.h"

#include "tools/Color.h"

/////////////////////////////////////////////////////////////////////////////

Spectrum::Spectrum( const Color& c )
{
  s.r= c.r();
  s.g= c.g();
  s.b= c.b();
}

/////////////////////////////////////////////////////////////////////////////

Spectrum::Spectrum( const Color& c, double intensity )
{
  s.r= intensity * c.r();
  s.g= intensity * c.g();
  s.b= intensity * c.b();
}

/////////////////////////////////////////////////////////////////////////////

Color Spectrum::toColor() const
{
  return Color( s.r, s.g, s.b );
}

/////////////////////////////////////////////////////////////////////////////

Spectrum Spectrum::operator & ( const Color& c ) const
{
  return Spectrum( s.r*c.r(), s.g*c.g(), s.b*c.b() );
}

/////////////////////////////////////////////////////////////////////////////

Spectrum operator & ( const Color& c, const Spectrum& s )
{
  return Spectrum( s.r() * c.r(),
                   s.g() * c.g(),
                   s.b() * c.b() );
}

/////////////////////////////////////////////////////////////////////////////

Spectrum& Spectrum::operator &= ( const Color& c )
{
  s.r *= c.r();
  s.g *= c.g();
  s.b *= c.b();

  return *this;
}

/////////////////////////////////////////////////////////////////////////////
