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
  MLImageWriterTIF.C

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
#include <setjmp.h>

#include "tools/Common.h"

#include "tiffio.h"

#include "image/MLImage.h"

#include "MLImageWriterTIF.h"

/////////////////////////////////////////////////////////////////////////////

MLImageWriterTIF::MLImageWriterTIF()
{
  _tiff= 0;
  compressed= true;
}

/////////////////////////////////////////////////////////////////////////////

MLImageWriterTIF::~MLImageWriterTIF()
{}

/////////////////////////////////////////////////////////////////////////////

void MLImageWriterTIF::setCompressed( boolean yes )
{
  compressed= yes;
}

/////////////////////////////////////////////////////////////////////////////

void MLImageWriterTIF::close()
{
  if( _tiff != 0 )
    {
    TIFF* tiff= (TIFF*) _tiff;
    TIFFClose(tiff);
    _tiff= 0;
    }

  MLImageWriter::close();
}

/////////////////////////////////////////////////////////////////////////////

static const int MaxTextExtent= 2048;
static char message[MaxTextExtent];

static void TIFFWarningMessage( const char *module,
                                const char *format,
                                va_list warning )
{
  register char *p;

  p= message;
  if( module != (char *) NULL )
    {
    sprintf( p, "%s: ", module );
    p+= strlen(message);
    }
  vsprintf( p, format, warning );
}

/////////////////////////////////////////////////////////////////////////////

bool MLImageWriterTIF::open( const std::string& filename )
{
  close();
  error= "";

  if( ! MLImageWriter::open(filename) )
    return false;

  close();

  TIFFSetErrorHandler(TIFFWarningMessage);
  TIFFSetWarningHandler(TIFFWarningMessage);
  TIFF* tiff= TIFFOpen( filename.c_str(), "wb" );

  if( tiff == 0 )
    {
    error= "Unable to create file";
    return false;
    }

  _tiff= (void*) tiff;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

typedef unsigned char Quantum;

/////////////////////////////////////////////////////////////////////////////

bool MLImageWriterTIF::write( MLImage* image )
{
  if( image == 0 || _tiff == 0 )
    return false;

  if( image->getData() == 0 )
    return false;

  boolean hasAlpha= image->hasAlpha();

  message[0]= '\0';

#ifndef TIFFDefaultStripSize
#define TIFFDefaultStripSize(tiff,request)  ((8*1024)/TIFFScanlineSize(tiff))
#endif

  register unsigned char
    *q;

  TIFF
    *tiff;

  uint16 compression, photometric;

  unsigned char
    *scanline;

/*
  unsigned int
    scene,
    x_resolution,
    y_resolution;

  unsigned short
    units,
    value;
*/

  tiff= (TIFF*) _tiff;

  int columns= image->getWidth();
  int rows= image->getHeight();

  do{

    // Initialize TIFF fields.
    TIFFSetField(tiff,TIFFTAG_IMAGELENGTH,(uint32) rows);
    TIFFSetField(tiff,TIFFTAG_IMAGEWIDTH,(uint32) columns);
    TIFFSetField(tiff,TIFFTAG_BITSPERSAMPLE,8);
//    if (image->depth == 16)
//      TIFFSetField(tiff,TIFFTAG_BITSPERSAMPLE,16);

    compression= compressed ? COMPRESSION_LZW : COMPRESSION_NONE;

    TIFFSetField(tiff,TIFFTAG_COMPRESSION,compression);

    // Full color TIFF raster.
    photometric=PHOTOMETRIC_RGB;
    TIFFSetField(tiff,TIFFTAG_SAMPLESPERPIXEL,(hasAlpha ? 4 : 3));
    if( hasAlpha )
      {
      uint16 extra_samples,
             sample_info[1];

      // TIFF has a matte channel.

      extra_samples=1;
      sample_info[0]= EXTRASAMPLE_ASSOCALPHA;
      TIFFSetField(tiff,TIFFTAG_EXTRASAMPLES,extra_samples,&sample_info);
      }

    TIFFSetField(tiff,TIFFTAG_PHOTOMETRIC,photometric);
    TIFFSetField(tiff,TIFFTAG_FILLORDER,FILLORDER_MSB2LSB);
    TIFFSetField(tiff,TIFFTAG_ORIENTATION,ORIENTATION_TOPLEFT);
    TIFFSetField(tiff,TIFFTAG_PLANARCONFIG,PLANARCONFIG_CONTIG);
    TIFFSetField(tiff,TIFFTAG_ROWSPERSTRIP,rows);

/*
    if (photometric == PHOTOMETRIC_RGB)
      if ((image_info->interlace == PlaneInterlace) ||
          (image_info->interlace == PartitionInterlace))
        TIFFSetField(tiff,TIFFTAG_PLANARCONFIG,PLANARCONFIG_SEPARATE);
*/

/*
    flags=NoValue;
    if (image_info->density != (char *) NULL)
      flags=XParseGeometry(image_info->density,&sans_offset,&sans_offset,
        &x_resolution,&y_resolution);
    if (flags & WidthValue)
      image->x_resolution=x_resolution;
    if (flags & HeightValue)
      image->y_resolution=y_resolution;
    units=RESUNIT_NONE;
    if (image->units == PixelsPerInchResolution)
      units=RESUNIT_INCH;
    if (image->units == PixelsPerCentimeterResolution)
      units=RESUNIT_CENTIMETER;
    if ((image->x_resolution == 0.0) || (image->y_resolution == 0.0))
      {
        units=RESUNIT_NONE;
        image->x_resolution=image->columns;
        image->y_resolution=image->rows;
      }
    TIFFSetField(tiff,TIFFTAG_RESOLUTIONUNIT,(uint16) units);
    TIFFSetField(tiff,TIFFTAG_XRESOLUTION,image->x_resolution);
    TIFFSetField(tiff,TIFFTAG_YRESOLUTION,image->y_resolution);
    TIFFSetField(tiff,TIFFTAG_DOCUMENTNAME,image->filename);
    TIFFSetField(tiff,TIFFTAG_SOFTWARE,Version);
*/

/*
    if (image->number_scenes > 1)
      {
        TIFFSetField(tiff,TIFFTAG_SUBFILETYPE,FILETYPE_PAGE);
        TIFFSetField(tiff,TIFFTAG_PAGENUMBER,(unsigned short) image->scene,
          image->number_scenes);
      }
    if (image->label != (char *) NULL)
      TIFFSetField(tiff,TIFFTAG_PAGENAME,image->label);
    if (image->comments != (char *) NULL)
      TIFFSetField(tiff,TIFFTAG_IMAGEDESCRIPTION,image->comments);
*/

    // Write image scanlines.

    scanline= new unsigned char [ TIFFScanlineSize(tiff) ];

    for( int y= 0; y < rows; ++y )
      {
      unsigned char* line= image->getPixel(0,y);
      q= scanline;

      for( int x= 0; x < columns; x++ )
        {
        // Convert to contiguous RGB scanlines.
#define WriteQuantum(quantum,q) *((q)++)=(quantum);
        WriteQuantum(line[0],q);
        WriteQuantum(line[1],q);
        WriteQuantum(line[2],q);
        if( hasAlpha )
          {
          WriteQuantum(line[3],q);
          line+= 4;
          }
         else
          line+= 3;
        }
      if( TIFFWriteScanline( tiff, (char *) scanline, y, 0 ) < 0 )
        {
        error= "Error writing image";
        TIFFClose(tiff);
        _tiff= 0;
        return false;
        }
      }

    delete scanline; scanline= 0;

    TIFFWriteDirectory(tiff);

    break;

/*
    if (image->next == (Image *) NULL)
      break;
    image->next->file=image->file;
    image=image->next;
    ProgressMonitor(SaveImageText,scene++,image->number_scenes);
*/
    }
  while(0);//image_info->adjoin);

  TIFFClose(tiff);
  _tiff= 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////
