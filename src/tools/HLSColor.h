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
  HLSColor.h

  Creation: SR, March 27th, 1996, from color.h

  Colors Classes

  From modeleur, Tue Aug 11 1992 /Kepler SCFR.
  Revision: April 1994
*/


#ifndef __HLSColor_h
#define __HLSColor_h

#ifndef __boolean_h
#include "boolean.h"
#endif

class Color;

/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////HLSColor/
// HLS Color class (Hue Luminosity Saturation)
//
class HLSColor
{
public:
  typedef float real;

private:
  real _h, _l, _s;

public:
  HLSColor()
    {}
  HLSColor( const real __h, const real __l, const real __s )
    {
    _h= __h;
    _l= __l;
    _s= __s;
    }
  HLSColor( const Color& color );

  real h() const { return _h; }
  real l() const { return _l; }
  real s() const { return _s; }

  void seth( const real __h ) { _h= __h; }
  void setl( const real __l ) { _l= __l; }
  void sets( const real __s ) { _s= __s; }

  boolean operator == ( const HLSColor& c ) const
    {
    return _h==c._h && _l==c._l && _s==c._s;
    }
  boolean operator != ( const HLSColor& c ) const
    {
    return _h!=c._h || _l!=c._l || _s!=c._s;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __HLSColor_h
