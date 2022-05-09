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
  MLPixmap.h

  Stephane Rehel
  July 9 1996
*/

#ifndef __MLPixmap_h
#define __MLPixmap_h

/////////////////////////////////////////////////////////////////////////////

class MLPixmap
{
protected:
  int lists, ok_list;
  int width, height;

public:
  MLPixmap();
  virtual ~MLPixmap();

  void make( const unsigned char* strings[], int _height );

  void print( int x, int y );

  int getWidth() const
    {
    return width;
    }
  int getHeight() const
    {
    return height;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPixmap_h
