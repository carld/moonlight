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
  MLImageReaderPNM.C

  Stephane Rehel

  August 12 1997

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
#include <assert.h>
#include <ctype.h>

#include "tools/Common.h"

#include "image/MLImage.h"

#include "MLImageReaderPNM.h"

/////////////////////////////////////////////////////////////////////////////

MLImageReaderPNM::MLImageReaderPNM()
{}

/////////////////////////////////////////////////////////////////////////////

MLImageReaderPNM::~MLImageReaderPNM()
{}

/////////////////////////////////////////////////////////////////////////////

unsigned int MLImageReaderPNM::PNMInteger( const unsigned int base )
{
  int c;

  // Skip any leading whitespace.
  do{
    c= fgetc(file);
    if( c == EOF )
      return 0;
    if( c == int('#') )
      {
      // Read comment.
      for(;;)
        {
        c= fgetc(file);
        if( (c == EOF) || (c == int('\n')) )
          break;
        }
      }
    }
  while( !isdigit(c) );

  if( base == 2 )
    return c - int('0');

  //  Evaluate number.
  unsigned int value= 0;
  do{
    value*= 10;
    value+= c - int('0');
    c= fgetc(file);
    if( c == EOF )
      return 0;
    }
  while( isdigit(c) );

  return value;
}

/////////////////////////////////////////////////////////////////////////////

typedef unsigned char Quantum;

/////////////////////////////////////////////////////////////////////////////

static unsigned int ReadData( char *data,
                              const unsigned int size,
                              const unsigned int number_items,
                              FILE *file )
{
  long
    bytes,
    count;

  assert(data != (char *) NULL);
  assert(file != (FILE *) NULL);
  count=0;
  for (bytes=size*number_items; bytes > 0; bytes-=count)
  {
    count=(long) fread(data,1,bytes,file);
    if (count <= 0)
      return false;
    data+=count;
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////

static unsigned short LSBFirstReadShort( FILE *file )
{
  unsigned char
    buffer[2];

  unsigned int
    status;

  unsigned short
    value;

  assert(file != (FILE *) NULL);
  status=ReadData((char *) buffer,1,2,file);
  if (status == false)
    return((unsigned short) ~0);
  value=(unsigned short) (buffer[1] << 8);
  value|=(unsigned short) (buffer[0]);
  return(value);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLImageReaderPNM::read( MLImage* image )
{
  if( image == 0 || file == 0 )
    return false;

  const unsigned int MaxRawValue= 255;

  // Read PNM image.
  char format;
  boolean status;
  status= ReadData( (char*) &format,1,1,file);

  // Verify PNM identifier.
  if( !status || (format != 'P') )
    {
    error= "Not a PNM image file";
    return false;
    }

  // Initialize image structure.
  format= fgetc(file);
  if( format == '7' )
    PNMInteger(10);

  int columns= PNMInteger(10);
  int rows= PNMInteger(10);
  if( columns*rows <= 0 )
    {
    error= "Unable to read image: image dimensions are zero";
    return false;
    }

  unsigned int max_value;
  if( (format == int('1')) || (format == int('4')) )
    max_value= 1; // bitmap
  else
    max_value= PNMInteger(10);

  Quantum* scale= 0;

  const unsigned int MaxRGB= 255;
  if( max_value != MaxRGB )
    {
    // Compute pixel scaling table.
    scale= new Quantum [ max_value+1 ];
    for( unsigned int i= 0; i <= max_value; i++ )
      scale[i]= Quantum( (i*MaxRGB+(max_value >> 1))/max_value );
    }

  switch( format )
    {
    case int('1'):
      {
      // Convert PBM image to runlength-encoded packets.
      delete scale;
      scale= 0;
      error= "unsupported PNM-P1 format";
      return false;
      }

    case int('2'):
      {
      // Convert PGM image to runlength-encoded packets.
      delete scale;
      scale= 0;
      error= "unsupported PNM-P2 format";
      return false;
      }

    case int('3'):
      {
      image->createRGB( columns, rows );

      // Convert PNM image to runlength-encoded packets.
      for( int y= 0; y < rows; y++ )
        {
        unsigned char* line= image->getPixel(0,y);
        for( int x= 0; x < columns; x++ )
          {
          unsigned short red, green, blue;
          red= PNMInteger(10);
          green= PNMInteger(10);
          blue= PNMInteger(10);
          if( scale != 0 )
            {
            red= scale[red];
            green= scale[green];
            blue= scale[blue];
            }
          line[0]= red;
          line[1]= green;
          line[2]= blue;
          line+= 3;
          }
        }
      break;
      }

    case int('4'):
      {
      // Convert PBM raw image to runlength-encoded packets.
      delete scale;
      scale= 0;
      error= "unsupported PNM-P4 format";
      return false;
      }

    case int('5'):
    case int('7'):
      {
      image->createRGB( columns, rows );

      // Convert PGM raw image to runlength-encoded packets.

      for( int y= 0; y < rows; y++ )
        {
        unsigned char* line= image->getPixel(0,y);
        for( int x= 0; x < columns; x++ )
          {
          unsigned short index;
          if( max_value <= MaxRawValue )
            index= fgetc(file);
           else
            {
            index= LSBFirstReadShort(file);
            index= ((unsigned int)index) * 255 / int(MaxRawValue);
            }

          line[0]= index;
          line[1]= index;
          line[2]= index;
          line+= 3;
          }
        }

      break;
      }

    case int('6'):
      {
      image->createRGB( columns, rows );

      // Convert PNM raster image to runlength-encoded packets.
      for( int y= 0; y < rows; y++ )
        {
        unsigned char* line= image->getPixel(0,y);
        for( int x= 0; x < columns; x++ )
          {
          unsigned short red, green, blue;
          if( max_value <= MaxRawValue )
            {
            red= fgetc(file);
            green= fgetc(file);
            blue= fgetc(file);
            }
           else
            {
            red= LSBFirstReadShort(file);
            green= LSBFirstReadShort(file);
            blue= LSBFirstReadShort(file);
            }
          if( scale != 0 )
            {
            red= scale[red];
            green= scale[green];
            blue= scale[blue];
            }
          line[0]= red;
          line[1]= green;
          line[2]= blue;
          line+= 3;
          }
        }
      break;
      }

    default:
      {
      delete scale;
      error= "Not a PNM image file";
      return false;
      }
    }

  delete scale;

  return true;
}

/////////////////////////////////////////////////////////////////////////////
