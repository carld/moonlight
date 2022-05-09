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
  Color8.C

  Creation: SR, March 27th, 1996, from color.h

  Colors Classes
  From modeleur, Tue Aug 11 1992 /Kepler SCFR.
  Revision: April 1994

  Revision: July 5 1996
*/

#include "Color8.h"

#include "Color.h"

/////////////////////////////////////////////////////////////////////////////

// static
const Color8 Color8::black       = Color8(  0,   0,   0);
const Color8 Color8::blue        = Color8(  0,   0, 128);
const Color8 Color8::green       = Color8(  0, 128,   0);
const Color8 Color8::cyan        = Color8(  0, 128, 128);
const Color8 Color8::red         = Color8(128,   0,   0);
const Color8 Color8::magenta     = Color8(128,   0, 128);
const Color8 Color8::brown       = Color8(128, 128,   0);
const Color8 Color8::lightgray   = Color8(170, 170, 170);
const Color8 Color8::darkgray    = Color8( 85,  85,  85);
const Color8 Color8::lightblue   = Color8(  0,   0, 255);
const Color8 Color8::lightgreen  = Color8(  0, 255,   0);
const Color8 Color8::lightcyan   = Color8(  0, 255, 255);
const Color8 Color8::lightred    = Color8(255,   0,   0);
const Color8 Color8::lightmagenta= Color8(255,   0, 255);
const Color8 Color8::yellow      = Color8(255, 255,   0);
const Color8 Color8::white       = Color8(255, 255, 255);

/////////////////////////////////////////////////////////////////////////////

Color8::Color8( const Color& color )
{
  s.r= (color.r()<=0.) ? 0 : (color.r()>=1.) ? 255 : BYTE(color.r()*255.);
  s.g= (color.g()<=0.) ? 0 : (color.g()>=1.) ? 255 : BYTE(color.g()*255.);
  s.b= (color.b()<=0.) ? 0 : (color.b()>=1.) ? 255 : BYTE(color.b()*255.);
  s.a= 0;
}

/////////////////////////////////////////////////////////////////////////////

int Color8::distance( const Color8& c ) const
{
//    return Color8( abs(c.r()-r()),
//                   abs(c.g()-g()),
//                   abs(c.b()-b()) ).getGray();

//    return ( 45*int(abs(c.r()-r()))
//            +70*int(abs(c.g()-g()))
//            +13*int(abs(c.b()-b())) ) >> 7;

  register int d, v;

  v= 45*(int(c.s.r) - int(s.r)); d= (v<0) ? -v : v;
  v= 70*(int(c.s.g) - int(s.g)); if( v<0 ) d-=v; else d+=v;
  v= 13*(int(c.s.b) - int(s.b));

  return (v<0) ? (d-v) : (d+v);
}

/////////////////////////////////////////////////////////////////////////////

