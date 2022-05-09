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
  HSVColor.h

  Creation: SR, March 27th, 1996, from color.h

  Colors Classes

  From modeleur, Tue Aug 11 1992 /Kepler SCFR.
  Revision: April 1994
*/


#ifndef __HSVColor_h
#define __HSVColor_h

#ifndef __boolean_h
#include "boolean.h"
#endif

class Color;

////////////////////////////////////////////////////////////////////HSVColor/
// HSV Color class (Hue Saturation Value)
//
class HSVColor
{
private:
  typedef float real;

  real _h, _s, _v;

public:
  HSVColor()
    {}

  HSVColor( const real __h, const real __s, const real __v )
    {
    _h= __h;
    _s= __s;
    _v= __v;
    }

  HSVColor( const Color& color );

  real h() const { return _h; }
  real s() const { return _s; }
  real v() const { return _v; }

  void seth( const real __h ) { _h= __h; }
  void sets( const real __s ) { _s= __s; }
  void setv( const real __v ) { _v= __v; }

  real geti( int i ) const
    {
    return (i==0) ? _h : (i==1) ? _s : _v;
    }

  boolean operator == ( const HSVColor& c ) const
    {
    return _h==c.h() && _s==c.s() && _v==c.v();
    }

  boolean operator != ( const HSVColor& c ) const
    {
    return _h!=c.h() || _s!=c.s() || _v!=c.v();
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __HSVColor_h

