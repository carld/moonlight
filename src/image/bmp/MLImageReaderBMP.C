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
  MLImageReaderBMP.C

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

#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#include "tools/Common.h"

#include "image/MLImage.h"

#include "MLImageReaderBMP.h"

/////////////////////////////////////////////////////////////////////////////

MLImageReaderBMP::MLImageReaderBMP()
{}

/////////////////////////////////////////////////////////////////////////////

MLImageReaderBMP::~MLImageReaderBMP()
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

static unsigned long LSBFirstReadLong( FILE *file )
{
  unsigned char
    buffer[4];

  assert(file != (FILE *) NULL);
  boolean status= ReadData( (char *) buffer, 1, 4, file );
  if( ! status )
    return (unsigned long) ~0;

  unsigned long
    value;
  value= (unsigned int) (buffer[3] << 24);
  value|= (unsigned int) (buffer[2] << 16);
  value|= (unsigned int) (buffer[1] << 8);
  value|= (unsigned int) (buffer[0]);

  return value;
}

/////////////////////////////////////////////////////////////////////////////

static boolean BMPDecodeImage( unsigned char *compressed_pixels,
                             unsigned char *pixels,
                             const unsigned int compression,
                             const unsigned int number_columns,
                             const unsigned int number_rows )
{
  register int
    i,
    x,
    y;

  register unsigned char
    *p,
    *q;

  unsigned char
    byte,
    count;

  assert(compressed_pixels != (unsigned char *) NULL);
  assert(pixels != (unsigned char *) NULL);
  byte=0;
  x=0;
  p=compressed_pixels;
  q=pixels;
  for (y=0; y < int(number_rows); )
    {
    count=(*p++);
    if (count != 0)
      {
      // Encoded mode.
      byte=(*p++);
      for (i=0; i < (int) count; i++)
        {
        if (compression == 1)
          *q++=byte;
        else
          *q++=(i & 0x01) ? (byte & 0x0f) : ((byte >> 4) & 0x0f);
        x++;
        }
      }
     else
      {
      // Escape mode.
      count=(*p++);
      if (count == 0x01)
        return true;
      switch(count)
        {
        case 0x00:
          {
          // End of line.
          x=0;
          y++;
          q=pixels+y*number_columns;
          break;
          }
        case 0x02:
          {
          // Delta mode.
          x+=(*p++);
          y+=(*p++);
          q=pixels+y*number_columns+x;
          break;
          }
        default:
          {
          // Absolute mode.
          for (i=0; i < (int) count; i++)
            {
            if (compression == 1)
              *q++=(*p++);
             else
              {
              if ((i & 0x01) == 0)
                byte=(*p++);
              *q++=(i & 0x01) ? (byte & 0x0f) : ((byte >> 4) & 0x0f);
              }
            x++;
            }

          // Read pad byte.
          if (compression == 1)
            {
            if (count & 0x01)
              p++;
            }
           else
            if (((count & 0x03) == 1) || ((count & 0x03) == 2))
              p++;
          break;
          }
        } // end switch(count)
      }
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLImageReaderBMP::read( MLImage* image )
{
  if( image == 0 || file == 0 )
    return false;

  typedef struct _BMPHeader
    {
    unsigned long
      file_size;

    unsigned short
      reserved[2];

    unsigned long
      offset_bits,
      size,
      width,
      height;

    unsigned short
      planes,
      bit_count;

    unsigned long
      compression,
      image_size,
      x_pixels,
      y_pixels,
      number_colors,
      colors_important;
    }
    BMPHeader;

  BMPHeader bmp_header;

  long
    start_position;

  unsigned char
    *bmp_data,
    *bmp_pixels,
    magick[12];

  unsigned int
    bytes_per_line,
    image_size,
    status;

  // Determine if this is a BMP file.
  status= ReadData((char *) magick,1,2,file);
  do{

    // Verify BMP identifier.
    start_position=ftell(file)-2;
    if( (!status) || (magick[0] | 0x20)!='b' || (magick[1] | 0x20)!='m' )
      {
      error= "Not a BMP image file";
      return false;
      }

    bmp_header.file_size=LSBFirstReadLong(file);
    bmp_header.reserved[0]=LSBFirstReadShort(file);
    bmp_header.reserved[1]=LSBFirstReadShort(file);
    bmp_header.offset_bits=LSBFirstReadLong(file);
    bmp_header.size=LSBFirstReadLong(file);
    if( bmp_header.size == 12 )
      {
      // OS/2 BMP image file.
      bmp_header.width=(unsigned long) LSBFirstReadShort(file);
      bmp_header.height=(unsigned long) LSBFirstReadShort(file);
      bmp_header.planes=LSBFirstReadShort(file);
      bmp_header.bit_count=LSBFirstReadShort(file);
      bmp_header.x_pixels=0;
      bmp_header.y_pixels=0;
      bmp_header.number_colors=0;
      bmp_header.compression=0;
      bmp_header.image_size=0;
      }
    else
      {
      // Microsoft Windows BMP image file.
      bmp_header.width=LSBFirstReadLong(file);
      bmp_header.height=LSBFirstReadLong(file);
      bmp_header.planes=LSBFirstReadShort(file);
      bmp_header.bit_count=LSBFirstReadShort(file);
      bmp_header.compression=LSBFirstReadLong(file);
      bmp_header.image_size=LSBFirstReadLong(file);
      bmp_header.x_pixels=LSBFirstReadLong(file);
      bmp_header.y_pixels=LSBFirstReadLong(file);
      bmp_header.number_colors=LSBFirstReadLong(file);
      bmp_header.colors_important=LSBFirstReadLong(file);
      for( int i=0; i < ((int) bmp_header.size-40); i++)
        fgetc(file);
      }

    struct ColorPacket
      {
      unsigned char red, green, blue;
      };
    ColorPacket* colormap= 0;
    int nColors= 0;

    if( bmp_header.bit_count < 24 )
      {
      unsigned char
        *bmp_colormap;

      unsigned int
        packet_size;

      // Read BMP raster colormap.
      nColors= bmp_header.number_colors;
      if( nColors == 0 )
        nColors= 1 << bmp_header.bit_count;

      colormap= new ColorPacket [ nColors ];
      bmp_colormap= new unsigned char [ 4*nColors ];

      packet_size=4;
      if (bmp_header.size == 12)
        packet_size= 3;
      ReadData( (char *) bmp_colormap,
                packet_size,
                nColors,
                file );
      unsigned char* p= bmp_colormap;
      for( int i=0; i < nColors; i++ )
        {
        colormap[i].blue= *p++;
        colormap[i].green= *p++;
        colormap[i].red= *p++;
        if( bmp_header.size != 12 )
          p++;
        }
      }

    // Read image data.
    while( ftell(file) < long(start_position+bmp_header.offset_bits) )
      fgetc(file);

    image_size=
      ((bmp_header.width*bmp_header.bit_count+31)/32)*4*bmp_header.height;
    if ((bmp_header.image_size == 0) || (bmp_header.image_size > image_size))
      bmp_header.image_size=image_size;
    bmp_data= new unsigned char [ bmp_header.image_size ];

    ReadData( (char *) bmp_data,
              1,
              (unsigned int) bmp_header.image_size,
              file );
    bmp_pixels= bmp_data;
    if( bmp_header.compression != 0 )
      {
      unsigned int
        packets;

      // Convert run-length encoded raster pixels.
      packets=(unsigned int)
        (((bmp_header.width*bmp_header.bit_count+31)/32)*4*bmp_header.height);
      if (bmp_header.compression == 2)
        packets<<=1;
      bmp_pixels= new unsigned char [ packets ];

      BMPDecodeImage( bmp_data,
                      bmp_pixels,
                      (unsigned int) bmp_header.compression,
                      (unsigned int) bmp_header.width,
                      (unsigned int) bmp_header.height );
      if (bmp_header.compression == 2)
        bmp_header.bit_count<<=1;
      delete bmp_data;
      bmp_data= 0;
      }

    // Initialize image structure.
    int columns= bmp_header.width;
    int rows= bmp_header.height;
//    image->units=PixelsPerCentimeterResolution;
//    image->x_resolution=bmp_header.x_pixels/100.0;
//    image->y_resolution=bmp_header.y_pixels/100.0;
//    image->packets=image->columns*image->rows;
//    image->pixels=(RunlengthPacket *)
//      malloc(image->packets*sizeof(RunlengthPacket));

    image->createRGB(columns,rows);

    // Convert BMP raster image to runlength-encoded packets.
    bytes_per_line=((columns*bmp_header.bit_count+31)/32)*4;
    switch( bmp_header.bit_count )
      {
      case 1:
        {
        // Convert bitmap scanline to runlength-encoded color packets.
        for( int y= rows-1; y >= 0; y-- )
          {
          unsigned char* p=bmp_pixels+(rows-y-1)*bytes_per_line;
          unsigned char* line= image->getPixel(0,y);
          for( int x=0; x < ((int) columns-7); x+=8 )
            {
            for( int bit=0; bit < 8; bit++ )
              {
              unsigned char index=((*p) & (0x80 >> bit) ? 0x01 : 0x00);
              if( colormap == 0 || index >= nColors )
                {
                index *= 255;
                line[0]= line[1]= line[2]= index;
                }
               else
                {
                line[0]= colormap[index].red;
                line[1]= colormap[index].green;
                line[2]= colormap[index].blue;
                }
              line+= 3;
              }
            p++;
            }
          if( (columns % 8) != 0 )
            {
            for( int bit=0; bit < (columns % 8); bit++ )
              {
              unsigned char index=((*p) & (0x80 >> bit) ? 0x01 : 0x00);
              if( colormap == 0 || index >= nColors )
                {
                index *= 255;
                line[0]= line[1]= line[2]= index;
                }
               else
                {
                line[0]= colormap[index].red;
                line[1]= colormap[index].green;
                line[2]= colormap[index].blue;
                }
              line+= 3;
              }
            p++;
            }
          }
        break;
        }

      case 4:
        {
        // Convert PseudoColor scanline to runlength-encoded color packets.
        for( int y= rows-1; y >= 0; y-- )
          {
          unsigned char* p=bmp_pixels+(rows-y-1)*bytes_per_line;
          unsigned char* line= image->getPixel(0,y);
          for( int x= 0; x < ((int) columns-1); x+=2 )
            {
            unsigned char index= (*p >> 4) & 0xf;
            if( colormap == 0 || index >= nColors )
              {
              index *= 15;
              line[0]= line[1]= line[2]= index;
              }
             else
              {
              line[0]= colormap[index].red;
              line[1]= colormap[index].green;
              line[2]= colormap[index].blue;
              }
            line+= 3;

            index=(*p) & 0xf;
            if( colormap == 0 || index >= nColors )
              {
              index *= 15;
              line[0]= line[1]= line[2]= index;
              }
             else
              {
              line[0]= colormap[index].red;
              line[1]= colormap[index].green;
              line[2]= colormap[index].blue;
              }
            line+= 3;
            p++;
            }
          if ((columns % 2) != 0)
            {
            unsigned char index=(*p >> 4) & 0xf;
            if( colormap == 0 || index >= nColors )
              {
              index *= 15;
              line[0]= line[1]= line[2]= index;
              }
             else
              {
              line[0]= colormap[index].red;
              line[1]= colormap[index].green;
              line[2]= colormap[index].blue;
              }
            line+= 3;
            p++;
            }
          }
        break;
        }
      case 8:
        {
        // Convert PseudoColor scanline to runlength-encoded color packets.
        if( bmp_header.compression == 1 )
          bytes_per_line= columns;

        for( int y= rows-1; y >= 0; y-- )
          {
          unsigned char* p=bmp_pixels+(rows-y-1)*bytes_per_line;
          unsigned char* line= image->getPixel(0,y);
          for( int x= 0; x < columns; x++ )
            {
            unsigned index=(*p++);
            if( colormap == 0 || int(index) >= nColors )
              {
              line[0]= line[1]= line[2]= index;
              }
             else
              {
              line[0]= colormap[index].red;
              line[1]= colormap[index].green;
              line[2]= colormap[index].blue;
              }

            line+= 3;
            }
          }
        break;
        }
      case 24:
        {
        // Convert DirectColor scanline to runlength-encoded color packets.
        for( int y= rows-1; y >= 0; y-- )
          {
          unsigned char* p=bmp_pixels+(rows-y-1)*bytes_per_line;
          unsigned char* line= image->getPixel(0,y);
          for( int x= 0; x < columns; x++ )
            {
//            if (image->matte)
//              q->index=UpScale(*p++);
            line[2]= (*p++);
            line[1]= (*p++);
            line[0]= (*p++);
            line+= 3;
            }
          }
        break;
        }

      default:
        {
        delete colormap;
        colormap= 0;
        error= "Not a BMP image file";
        return false;
        }
      }

    delete bmp_pixels; bmp_pixels= 0;

    delete colormap;
    colormap= 0;
    nColors= 0;

/*
    // Proceed to next image.
    status=ReadData((char *) magick,1,2,image->file);
    if ((status == True) && (strncmp((char *) magick,"BM",2) == 0))
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
break;
    }
  while(0); // ((status == True) && (strncmp((char *) magick,"BM",2) == 0));

  return true;
}

/////////////////////////////////////////////////////////////////////////////
