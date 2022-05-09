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
  MLImageWriterPNM.C

  Stephane Rehel

  August 19 1997

  FROM: ImageMagick-3.8.6
  COPYRIGHT
    Copyright 1997 E. I. du Pont de Nemours and Company

    Permission to use, copy, modify, distribute, and sell this
    software and its documentation for any purpose is hereby
    granted without fee, provided that the above copyright notice
    appear in all copies and that both that copyright notice and
    this permission notice appear in supporting documentation, and
    that the name of E. I. du Pont de Nemours and Company not be
    used in advertising or publicity pertaining to distribution of
    the software without specific, written prior permission.  E.
    I. du Pont de Nemours and Company makes no representations
    about the suitability of this software for any purpose.  It is
    provided "as is" without express or implied warranty.

    E. I. du Pont de Nemours and Company disclaims all warranties
    with regard to this software, including all implied warranties
    of merchantability and fitness, in no event shall E. I. du
    Pont de Nemours and Company be liable for any special,
    indirect or consequential damages or any damages whatsoever
    resulting from loss of use, data or profits, whether in an
    action of contract, negligence or other tortious action,
    arising out of or in connection with the use or performance of
    this software.
*/

#include <stdio.h>
#include <errno.h>

#include "tools/Common.h"

#include "image/MLImage.h"

#include "MLImageWriterPNM.h"

/////////////////////////////////////////////////////////////////////////////

MLImageWriterPNM::MLImageWriterPNM()
{}

/////////////////////////////////////////////////////////////////////////////

MLImageWriterPNM::~MLImageWriterPNM()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLImageWriterPNM::write( MLImage* image )
{
  if( image == 0 || file == 0 )
    return false;

  if( image->getData() == 0 )
    return false;

  int width= image->getWidth();
  int height= image->getHeight();

  fprintf( file, "P6\n");
//  fprintf( file, "#%s\n", filename.get() );
  fprintf( file, "%d\n%d\n255\n", width, height );

  const int line_length= width * 3;
  if( image->hasAlpha() )
    {
    unsigned char* line= new unsigned char [ width * 3 ];
    for( int y= 0; y < height; ++y )
      {
      const unsigned char* line4= image->getPixel(0,y);
      for( int x= 0; x < width; ++x )
        {
        line[0]= line4[0];
        line[1]= line4[1];
        line[2]= line4[2];
        line+= 3;
        line4+= 4;
        }

      int outcome= fwrite( (void*) line, 1, line_length, file );
      if( outcome != line_length )
        {
        error= strerror(errno);
        return false;
        }
      }
    delete line;
    }
   else
    {
    for( int y= 0; y < height; ++y )
      {
      const unsigned char* line= image->getPixel(0,y);
      int outcome= fwrite( (void*) line, 1, line_length, file );
      if( outcome != line_length )
        {
        error= strerror(errno);
        return false;
        }
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
