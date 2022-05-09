// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  MLImage.C

  Stephane Rehel
  August 16 1996
  September 20 1997, from graphics/MLPicture.h
*/

#include <math.h>
#include <string.h>

#include "tools/MLGammaCorrection.h"

#include "MLImage.h"

/////////////////////////////////////////////////////////////////////////////

MLImage::MLImage()
{
  width= height= 0;
  data= 0;
  own_data= false;

  with_alpha= false;
}

/////////////////////////////////////////////////////////////////////////////

MLImage::~MLImage()
{
  destroy();
}

/////////////////////////////////////////////////////////////////////////////

void MLImage::destroy()
{
  if( own_data )
    delete data;

  data= 0;
  own_data= false;

  width= height= 0;

  with_alpha= false;
}

/////////////////////////////////////////////////////////////////////////////

bool MLImage::create( int _width, int _height, void* _data, bool _with_alpha )
{
  width= _width;
  height= _height;
  with_alpha= _with_alpha;

  if( width < 1 || height < 1 )
    return false;

  data= (unsigned char*) _data;
  if( data == 0 )
    {
    int line_length= width * (with_alpha ? 4 : 3);
    data= new unsigned char [ height * line_length ];

    unsigned char* p= data;
    for( int i= height * line_length; i; --i )
      *(p++)= 0;

    own_data= true;
    }
   else
    own_data= false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLImage::createRGB( int _width, int _height, void* data /* = 0 */ )
{
  create( _width, _height, data, false );
}

/////////////////////////////////////////////////////////////////////////////

void MLImage::createRGBA( int _width, int _height, void* data /* = 0 */ )
{
  create( _width, _height, data, true );
}

/////////////////////////////////////////////////////////////////////////////

void MLImage::convertToRGB()
{
  if( ! with_alpha || data == 0 )
    return;

  for( int y= 0; y < height; ++y )
    {
    register unsigned char* rgb_line= getPixelRGB(0,y);
    register unsigned char* rgba_line= getPixelRGBA(0,y);
    for( int x= 0; x < width; ++x )
      {
      rgb_line[0]= rgba_line[0];
      rgb_line[1]= rgba_line[1];
      rgb_line[2]= rgba_line[2];
      rgb_line += 3;
      rgba_line+= 4;
      }
    }

  with_alpha= false;
}

/////////////////////////////////////////////////////////////////////////////

void MLImage::fill( int x1, int y1, int x2, int y2,
                    int r, int g, int b, int a)
{
  if( data == 0 )
    return;

  if( x1 < 0 ) x1= 0;
  if( y1 < 0 ) y1= 0;
  if( x2 >= width ) x2= width-1;
  if( y2 >=height ) y2=height-1;

  if( x1 > x2 || y1 > y2 )
    return;

  for( int j= y1; j <= y2; ++j )
    {
    unsigned char* p= getPixel(x1,j);
    for( int i= x1; i <= x2; ++i )
      {
      *(p++)= r;
      *(p++)= g;
      *(p++)= b;
      if( with_alpha )
        *(p++)= a;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLImage::gammaCorrect( const MLGammaCorrection& gc )
{
  if( data == 0 )
    return;

  if( ! gc.hasCorrection() )
    return;

  if( with_alpha )
    {
    int length= width * height;
    register unsigned char* p= data;
    register const unsigned char* table= gc.getTable();
    for( register int i= length; i; --i )
      {
      *p= table[ *p ]; ++p;
      *p= table[ *p ]; ++p;
      *p= table[ *p ]; ++p;
      ++p;
      }
    }
   else
    {
    int length= width * height * 3;
    register unsigned char* p= data;
    register const unsigned char* table= gc.getTable();
    for( register int i= length; i; --i, ++p )
      *p= table[ *p ];
    }

}

/////////////////////////////////////////////////////////////////////////////

void MLImage::gammaCorrect( double gamma )
{
  MLGammaCorrection gc(gamma);

  gammaCorrect(gc);
}

/////////////////////////////////////////////////////////////////////////////

void MLImage::upsidedown()
{
  if( data == 0 )
    return;

  int line_length= width * (with_alpha ? 4 : 3);
  unsigned char* dummy= new unsigned char [ line_length + 1 ];

  for( int i= height/2-1; i >= 0; --i )
    {
    int j= height - 1 - i;

    unsigned char* line1= data + line_length * i;
    unsigned char* line2= data + line_length * j;

    memcpy( (void*) dummy, (void*) line1, line_length );
    memcpy( (void*) line1, (void*) line2, line_length );
    memcpy( (void*) line2, (void*) dummy, line_length );
    }

  delete dummy;
  dummy= 0;
}

/////////////////////////////////////////////////////////////////////////////
