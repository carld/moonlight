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
  MLImage.h

  Stephane Rehel
  August 16 1996
  September 20 1997, from graphics/MLPicture.h
*/

#ifndef __MLImage_h
#define __MLImage_h

class MLGammaCorrection;

/////////////////////////////////////////////////////////////////////////////

class MLImage
{
protected:
  int width, height;
  bool with_alpha;

  unsigned char* data;
  bool own_data;

public:
  MLImage();
  virtual ~MLImage();

  void destroy();

  bool create( int _width, int _height, void* data, bool _with_alpha );

  void createRGB( int _width, int _height, void* data = 0 );
  void createRGBA( int _width, int _height, void* data = 0 );

  int getWidth() const
    {
    return width;
    }
  int getHeight() const
    {
    return height;
    }
  bool hasAlpha() const
    {
    return with_alpha;
    }
  unsigned char* getData() const
    {
    return data;
    }

  unsigned char* getPixelRGB( int i, int j ) const
    {
    return data + (i + j*width) * 3;
    }
  unsigned char* getPixelRGBA( int i, int j ) const
    {
    return data + (i + j*width) * 4;
    }
  unsigned char* getPixel( int i, int j ) const
    {
    return with_alpha ? getPixelRGBA(i,j) : getPixelRGB(i,j);
    }
  int getLineSize() const
    {
    return with_alpha ? (4 * width) : (3 * width);
    }
  void convertToRGB();

  void fill( int x1, int y1, int x2, int y2,
             int r, int g, int b, int a = 0 );

  void gammaCorrect( const MLGammaCorrection& gc );
  void gammaCorrect( double gamma );

  void upsidedown();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLImage_h

