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
  MLImageReaderRGB.C

  Stephane Rehel
  August 20 1997

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

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "MLImageReaderRGB.h"

#include "image/MLImage.h"

/////////////////////////////////////////////////////////////////////////////

MLImageReaderRGB::MLImageReaderRGB()
{}

/////////////////////////////////////////////////////////////////////////////

MLImageReaderRGB::~MLImageReaderRGB()
{}

/////////////////////////////////////////////////////////////////////////////

static unsigned int ReadData( char *data,
                              const unsigned int size,
                              const unsigned int number_items,
                              FILE *file )
{
  assert(data != (char *) NULL);
  assert(file != (FILE *) NULL);

  long count= 0;
  for( long bytes= size*number_items; bytes > 0; bytes-= count )
    {
    count= (long) fread( data, 1, bytes, file );
    if( count <= 0 )
      return false;
    data+= count;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

static unsigned short MSBFirstReadShort( FILE *file )
{
  assert( file != (FILE *) NULL );

  unsigned char buffer[2];
  boolean status= ReadData( (char*) buffer, 1, 2, file );

  if( ! status )
    return (unsigned short) ~0;

  unsigned short value= (unsigned int) (buffer[0] << 8);
  value |= (unsigned int) (buffer[1]);

  return value;
}

/////////////////////////////////////////////////////////////////////////////

static unsigned long MSBFirstReadLong( FILE *file )
{
  assert( file != (FILE *) NULL );

  unsigned char buffer[4];
  boolean status= ReadData( (char*) buffer, 1, 4, file );
  if( ! status )
    return (unsigned long) ~0;

  unsigned long value= (unsigned int) (buffer[0] << 24);
  value |= (unsigned int) (buffer[1] << 16);
  value |= (unsigned int) (buffer[2] << 8);
  value |= (unsigned int) (buffer[3]);

  return value;
}

/////////////////////////////////////////////////////////////////////////////

static void SGIDecode( unsigned char *packets, unsigned char *pixels )
{
  for(;;)
    {
    unsigned char pixel= (*packets++);
    unsigned char count= pixel & 0x7f;
    if( count == 0 )
      break;
    if( pixel & 0x80 )
      {
      for( ; count != 0; count-- )
        {
        *pixels= (*packets++);
        pixels+= 4;
        }
      }
     else
      {
      pixel=(*packets++);
      for(  ; count != 0; count-- )
        {
          *pixels=pixel;
          pixels+=4;
        }
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLImageReaderRGB::read( MLImage* image )
{
  if( image == 0 || file == 0 )
    return false;

  typedef struct _SGIHeader
    {
    unsigned short magic;
    unsigned char storage, bytes_per_pixel;
    unsigned short dimension,
                   columns,
                   rows,
                   depth;
    unsigned long minimum_value,
                  maximum_value;
    unsigned char filler[492];
    }
    SGIHeader;

  SGIHeader iris_header;

  // Read SGI raster header.
  iris_header.magic= MSBFirstReadShort(file);

  do{
    // Verify SGI identifier.
    if( iris_header.magic != 0x01DA )
      {
      error= "Not a SGI RGB image";
      return false;
      }

    iris_header.storage= fgetc(file);
    iris_header.bytes_per_pixel= fgetc(file);
    if( iris_header.bytes_per_pixel != 1 )
      {
      error= "Image must have 1 byte per pixel channel";
      return false;
      }

    iris_header.dimension= MSBFirstReadShort(file);
    iris_header.columns= MSBFirstReadShort(file);
    iris_header.rows= MSBFirstReadShort(file);
    iris_header.depth= MSBFirstReadShort(file);
    iris_header.minimum_value= MSBFirstReadLong(file);
    iris_header.maximum_value= MSBFirstReadLong(file);
    ReadData( (char*) iris_header.filler,
              1,
              (unsigned int) sizeof(iris_header.filler),
              file );

    // Allocate SGI pixels.
    unsigned char* iris_pixels= new unsigned char
                                  [ 4*iris_header.columns*iris_header.rows ];

    if( iris_header.storage != 0x01 )
      {
      // Read standard image format.

      unsigned char* scanline= new unsigned char [ iris_header.columns ];
      for( int z= 0; z < int(iris_header.depth); z++ )
        {
        unsigned char* p= iris_pixels+z;
        for( int y= 0; y < int(iris_header.rows); y++ )
          {
          ReadData( (char*) scanline, 1, iris_header.columns, file );
          for( int x= 0; x < int(iris_header.columns); x++ )
            {
            *p= scanline[x];
            p+= 4;
            }
          }
        }
      delete scanline;
      scanline= 0;
      }
     else
      {
      // Read runlength-encoded image format.

      int rows_depth= iris_header.rows*iris_header.depth;
      unsigned long* offsets= new unsigned long [ rows_depth ];
      unsigned char* packets= new unsigned char
                                  [ (iris_header.columns << 1) + 10 ];
      unsigned long* runlength= new unsigned long [ rows_depth ];

      int i;
      for( i= 0; i < rows_depth; i++ )
        offsets[i]= MSBFirstReadLong(file);

      for (i= 0; i < rows_depth; i++ )
        runlength[i]= MSBFirstReadLong(file);

      // Check data order.

      unsigned long offset= 0;
      unsigned int data_order= 0;
      for( int y= 0; ((y < int(iris_header.rows)) && !data_order); y++ )
        {
        register int index= y;
        for( int z= 0; ((z < int(iris_header.depth)) && !data_order); z++ )
          {
          if( offsets[index] < offset )
            data_order= 1;

          offset= offsets[index];

          index+= iris_header.rows;
          }
        }

      offset= 512 + 4*((iris_header.rows*iris_header.depth) << 1);

      if( data_order == 1 )
        {
        for( int z= 0; z < int(iris_header.depth); z++ )
          {
          unsigned char* p= iris_pixels;
          int index= z * iris_header.rows;
          for( int y= 0; y < int(iris_header.rows); y++ )
            {
            if( offset != offsets[index] )
              {
              offset= offsets[index];
              fseek( file, long(offset), SEEK_SET );
              }
            ReadData( (char*) packets,
                      1,
                      (unsigned int) runlength[index],
                      file );
            offset+= runlength[index];

            SGIDecode( packets, p+z );
            p+= iris_header.columns*4;

            ++index;
            }
          }
        }
       else
        {
        unsigned char* p= iris_pixels;
        for( int y= 0; y < int(iris_header.rows); y++ )
          {
          int index= y;
          for( int z= 0; z < int(iris_header.depth); z++ )
            {
            if( offset != offsets[index] )
              {
              offset= offsets[index];
              fseek( file, long(offset), SEEK_SET );
              }
            ReadData( (char*) packets,
                      1,
                      (unsigned int) runlength[index],
                      file );
            offset+= runlength[index];
            SGIDecode( packets, p+z );

            index+= iris_header.rows;
            }
          p+= iris_header.columns*4;
          }
        }

      delete runlength; runlength= 0;
      delete packets; packets= 0;
      delete offsets; offsets= 0;
      }

    // Initialize image structure.
    boolean hasAlpha= (iris_header.depth == 4);

    int columns= iris_header.columns;
    int rows= iris_header.rows;

    if( hasAlpha )
      image->createRGBA(columns,rows);
     else
      image->createRGB(columns,rows);

    // Convert SGI raster image

    if( iris_header.depth >= 3 )
      {
      int delta= hasAlpha ? 4 : 3;
      for( int y= 0; y < rows; y++ )
        {
        register unsigned char* line= image->getPixel(0,y);
        register unsigned char* p= iris_pixels+((rows-1)-y)*(columns*4);
        for( register int x= 0; x < columns; x++ )
          {
          line[0]= p[0];
          line[1]= p[1];
          line[2]= p[2];
          if( hasAlpha )
            line[3]= p[3];
          line+= delta;
          p+= 4;
          }
        }
      }
     else
      {
      // Grayscale.
      int delta= hasAlpha ? 4 : 3;
      for( int y= 0; y < rows; y++ )
        {
        register unsigned char* line= image->getPixel(0,y);
        register unsigned char* p= iris_pixels+((rows-1)-y)*(columns*4);
        for( register int x= 0; x < columns; x++ )
          {
          line[0]= *p;
          line[1]= *p;
          line[2]= *p;
          line += delta;

          p+=4;
          }
        }
      }

    delete iris_pixels;
    iris_pixels= 0;

iris_header.magic= 0;
/*
    // Proceed to next image.
    iris_header.magic= MSBFirstReadShort(file);
    if( iris_header.magic == 0x01DA )
      {
      // Allocate next image structure.
      AllocateNextImage(image_info,image);
      if (image->next == (Image *) NULL)
        {
          DestroyImages(image);
          return((Image *) NULL);
        }
      image=image->next;
      }
*/
    }
  while( iris_header.magic == 0x01DA );

  return true;
}

