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
  Color8.h

  Creation: SR, March 27th, 1996, from color.h

  Colors Classes
  From modeleur, Tue Aug 11 1992 /Kepler SCFR.
  Revision: April 1994

  Revision: July 5 1996
*/


#ifndef __Color8_h
#define __Color8_h

#ifndef __boolean_h
#include "boolean.h"
#endif

class Color;

/////////////////////////////////////////////////////////////////////////////
// 8-bits defined color
//
class Color8
{
private:
  union
    {
    unsigned char c[4];
    struct { unsigned char r,g,b,a; } s;
    unsigned int l; // please _dont_use_ 16 bit MSDOG!
    };

public:
  static const Color8 black;
  static const Color8 blue;
  static const Color8 green;
  static const Color8 cyan;
  static const Color8 red;
  static const Color8 magenta;
  static const Color8 brown;
  static const Color8 lightgray;
  static const Color8 darkgray;
  static const Color8 lightblue;
  static const Color8 lightgreen;
  static const Color8 lightcyan;
  static const Color8 lightred;
  static const Color8 lightmagenta;
  static const Color8 yellow;
  static const Color8 white;

  Color8()
    {}
  Color8( unsigned char _r, unsigned char _g, unsigned char _b )
    {
    s.r= _r;
    s.g= _g;
    s.b= _b;
    s.a= 0;
    }
  Color8( unsigned char _r, unsigned char _g, unsigned char _b,
          unsigned char _a )
    {
    s.r= _r;
    s.g= _g;
    s.b= _b;
    s.a= _a;
    }
  Color8( const Color8& c )
    {
    l= c.l;
    }
  Color8( const Color& color );

  int r() const { return int(s.r); }
  int g() const { return int(s.g); }
  int b() const { return int(s.b); }
  int a() const { return int(s.a); }
  unsigned int getl() const { return l; }
  int geti( int i ) const
    {
    return (i==0) ? int(s.r) :
           (i==1) ? int(s.g) :
           (i==2) ? int(s.b) :
                    int(s.a);
    }

  unsigned char* getCoord() { return c; }
  const unsigned char* getConstCoord() const { return c; }

  unsigned char& operator [] ( int i )
    {
    return c[i];
    }

  void setr( unsigned char _r ) { s.r= _r; }
  void setg( unsigned char _g ) { s.g= _g; }
  void setb( unsigned char _b ) { s.b= _b; }
  void seta( unsigned char _a ) { s.a= _a; }
  void setn( int n, unsigned char c )
    {
    (n==1) ? (s.r=c) : (n==2) ? (s.g=c) : (n==3) ? (s.b=c) : (s.a=c);
    }
  void setl( int _l ) { l= _l; }

  Color8& operator = ( const Color8& c )
    {
    l= c.l;
    return *this;
    }
  boolean operator == ( const Color8& c ) const
    { return c.l == l; }
  boolean operator != ( const Color8& c ) const
    { return c.l != l; }
  int getGray() const
    {
//    return ( 35*int(getr()) + 55*int(getg()) + 10*int(getb()) ) / 100;
    return ( 45*int(s.r) + 70*int(s.g) + 13*int(s.b) ) >> 7;
    }
  Color8& operator+=( const Color8& c )
    {
    s.r+= c.s.r;
    s.g+= c.s.g;
    s.b+= c.s.b;
    s.a+= c.s.a;
    return *this;
    }

  int distance( const Color8& c ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Color8_h

