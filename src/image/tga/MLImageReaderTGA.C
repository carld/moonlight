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
  MLImageReaderTGA.C

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

#include "MLImageReaderTGA.h"

/////////////////////////////////////////////////////////////////////////////

MLImageReaderTGA::MLImageReaderTGA()
{}

/////////////////////////////////////////////////////////////////////////////

MLImageReaderTGA::~MLImageReaderTGA()
{}

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

boolean MLImageReaderTGA::read( MLImage* image )
{
  if( image == 0 || file == 0 )
    return false;

//  const int TGAColormap= 1;
//  const int TGARGB= 2;
//  const int TGAMonochrome= 3;
  const int TGARLEColormap= 9;
  const int TGARLERGB= 10;
  const int TGARLEMonochrome= 11;

  struct TGAHeader
    {
    unsigned char id_length,
                  colormap_type,
                  image_type;
    unsigned short colormap_index,
             colormap_length;
    unsigned char colormap_size;
    unsigned short x_origin,
                   y_origin,
                   width,
                   height;
    unsigned char pixel_size,
                  attributes;
    };

  Quantum blue, green, red;

  TGAHeader tga_header;

  unsigned char
    j,
    k,
    runlength;

  unsigned int
    base,
    flag,
    offset,
    real,
    skip,
    status;

  unsigned short
    index;


  // Read TGA header information.
  status= ReadData( (char*) &tga_header.id_length,1,1,file );
  tga_header.colormap_type= fgetc(file);
  tga_header.image_type= fgetc(file);
  do{
    if( !status ||
        (tga_header.image_type == 0) ||
        (tga_header.image_type > 11) )
      {
      error= "Not a TGA image file";
      return false;
      }
    tga_header.colormap_index= LSBFirstReadShort(file);
    tga_header.colormap_length= LSBFirstReadShort(file);
    tga_header.colormap_size= fgetc(file);
    tga_header.x_origin= LSBFirstReadShort(file);
    tga_header.y_origin= LSBFirstReadShort(file);
    tga_header.width= LSBFirstReadShort(file);
    tga_header.height= LSBFirstReadShort(file);
    tga_header.pixel_size= fgetc(file);
    tga_header.attributes= fgetc(file);

    // Initialize image structure.
//    image->matte=tga_header.pixel_size == 32;
    unsigned int columns= tga_header.width;
    unsigned int rows= tga_header.height;
    unsigned int nPackets= columns * rows;
    image->createRGB(columns,rows);

    if (tga_header.id_length != 0)
      {
      // TGA image comment.
      char* comments= new char [ tga_header.id_length+1 ];

      ReadData( comments,1,tga_header.id_length,file);
      delete comments;
      }

#define UpScale(p) p

    red=0;
    green=0;
    blue=0;
    const unsigned int MaxRGB= 255;

    if( tga_header.colormap_type != 0 )
      {
      // Read TGA raster colormap.
//      if( (tga_header.image_type == TGARLEColormap) ||
//          (tga_header.image_type == TGARLERGB) )
//        image->class=PseudoClass;

      int nColors= tga_header.colormap_length;
      colormap= new unsigned char [ nColors * 3 ];

      for( int i= 0; i < nColors; i++ )
        {
        switch( tga_header.colormap_size )
          {
          case 8:
          default:
            {
            // Gray scale.
            red=UpScale(fgetc(file));
            green=red;
            blue=red;
            break;
            }

          case 15:
          case 16:
            {
            // 5 bits each of red green and blue.
            j= fgetc(file);
            k= fgetc(file);
            red=(Quantum) ((MaxRGB*((int) (k & 0x7c) >> 2))/31);
            green=(Quantum)
              ((MaxRGB*(((int) (k & 0x03) << 3)+((int) (j & 0xe0) >> 5)))/31);
            blue=(Quantum) ((MaxRGB*((int) (j & 0x1f)))/31);
            break;
            }

          case 32:
          case 24:
            {
            // 8 bits each of blue, green and red.
            blue= UpScale(fgetc(file));
            green= UpScale(fgetc(file));
            red= UpScale(fgetc(file));
            break;
            }
          }
        colormap[i*3+0]= red;
        colormap[i*3+1]= green;
        colormap[i*3+2]= blue;
        }
      }

    // Convert TGA pixels to runlength-encoded packets.
    base= 0;
    flag= 0;
    index= 0;
    skip= false;
    real= 0;
    runlength= 0;
    offset= 0;

    unsigned char* q= image->getData();
    unsigned int i;
    for( i= 0; i < nPackets; i++ )
      {
      q[0]= 0;
      q[1]= 0;
      q[2]= 0;
      q+= 3;
      }

    for( unsigned int y= 0; y < rows; y++ )
      {
      real= offset;
      if( ((unsigned char) (tga_header.attributes & 0x20) >> 5) == 0 )
        real= rows-real-1;

      q= image->getPixel(0,real);
      for( unsigned int x= 0; x < columns; x++ )
        {
        if( (tga_header.image_type == TGARLEColormap) ||
            (tga_header.image_type == TGARLERGB) ||
            (tga_header.image_type == TGARLEMonochrome) )
          {
          if (runlength != 0)
            {
            runlength--;
            skip=flag != 0;
            }
           else
            {
            status= ReadData( (char *) &runlength,1,1,file );
            if( ! status )
              {
              error= "Unable to read image data";
              return false;
              }
            flag= runlength & 0x80;
            if( flag != 0 )
              runlength-= 128;
            skip= false;
            }
          }

        if( !skip )
          {
          switch( tga_header.pixel_size )
            {
            case 8:
            default:
              {
              // Gray scale.
              index= fgetc(file);
              if( tga_header.colormap_type == 0 )
                {
                red=(Quantum) UpScale(index);
                green=(Quantum) UpScale(index);
                blue=(Quantum) UpScale(index);
                }
               else
                {
                red=   colormap[index*3+0];
                green= colormap[index*3+1];
                blue=  colormap[index*3+2];
                }
              break;
              }

            case 15:
            case 16:
              {
              // 5 bits each of red green and blue.
              j= fgetc(file);
              k= fgetc(file);
              red=(Quantum) ((MaxRGB*((int) (k & 0x7c) >> 2))/31);
              green=(Quantum)
                ((MaxRGB*(((int) (k & 0x03) << 3)+((int) (j & 0xe0) >> 5)))/31);
              blue=(Quantum) ((MaxRGB*((int) (j & 0x1f)))/31);
              index=((unsigned short) k << 8)+j;
              break;
              }

            case 24:
            case 32:
              {
              // 8 bits each of blue green and red.
              blue= UpScale(fgetc(file));
              green= UpScale(fgetc(file));
              red= UpScale(fgetc(file));
              const unsigned short Opaque= 255;
              if( tga_header.pixel_size == 32 )
                index= Opaque - UpScale(fgetc(file));
              break;
              }
            }
          }

        if( !status )
          {
          error= "Unable to read image data";
          return false;
          }

        q[0]= red;
        q[1]= green;
        q[2]= blue;
        q+= 3;
        }

      if( ((unsigned char) (tga_header.attributes & 0xc0) >> 6) == 4 )
        offset+=4;
       else
        {
        if( ((unsigned char) (tga_header.attributes & 0xc0) >> 6) == 2 )
          offset+=2;
         else
          offset++;
        }

      if( offset >= rows )
        {
        base++;
        offset= base;
        }
      if( feof(file) )
        break;
//      if (image->previous == (Image *) NULL)
//        ProgressMonitor(LoadImageText,y,image->rows);
      }

/*
    (void) IsGrayImage(image);
    if (image->class == PseudoClass)
      SyncImage(image);
    CompressImage(image);
*/

    // Proceed to next image.
    status= ReadData( (char *) &tga_header.id_length,1,1,file );
    tga_header.colormap_type= fgetc(file);
    tga_header.image_type= fgetc(file);
    status &= ((tga_header.image_type != 0) && (tga_header.image_type <= 11));
status= false;
/*
    if( status  )
      {
      // Allocate next image structure.
      AllocateNextImage(image_info,image);
      if (image->next == (Image *) NULL)
        {
          DestroyImages(image);
          return((Image *) NULL);
        }
      image=image->next;
      ProgressMonitor(LoadImageText,(unsigned int) ftell(image->file),
        (unsigned int) image->filesize);
      }
*/
    }
  while(status);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

