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
  MLImageWriterJPG.C

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

extern "C"
{
#define boolean jpg_boolean
#include "jpeglib.h"
#undef boolean
}

#include "tools/Common.h"

#include "image/MLImage.h"

#include "MLImageWriterJPG.h"

/////////////////////////////////////////////////////////////////////////////

MLImageWriterJPG::MLImageWriterJPG()
{
  quality= 75;
  progressive= true;
}

/////////////////////////////////////////////////////////////////////////////

MLImageWriterJPG::~MLImageWriterJPG()
{}

/////////////////////////////////////////////////////////////////////////////

void MLImageWriterJPG::setQuality( int _quality )
{
  quality= _quality;
  if( quality <= 0 )
    quality= 1;
  if( quality > 100 )
    quality= 100;
}

/////////////////////////////////////////////////////////////////////////////

void MLImageWriterJPG::setProgressive( boolean _progressive )
{
  progressive= _progressive;
}

/////////////////////////////////////////////////////////////////////////////

typedef unsigned char Quantum;

/////////////////////////////////////////////////////////////////////////////

static char message[JMSG_LENGTH_MAX];

/////////////////////////////////////////////////////////////////////////////

static void EmitMessage( j_common_ptr jpeg_info,int level )
{
  struct jpeg_error_mgr *jpeg_error;

  jpeg_error=jpeg_info->err;
  (jpeg_error->format_message)( jpeg_info, ::message );

  if( level < 0 )
    {
//    if( jpeg_error->num_warnings == 0 || jpeg_error->trace_level >= 3 )
//        Warning((char *) message,image->filename);
    jpeg_error->num_warnings++;
    }
  else
    {
//    if( jpeg_error->trace_level >= level )
//      Warning((char *) message,image->filename);
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLImageWriterJPG::write( MLImage* image )
{
  if( image == 0 || file == 0 )
    return false;

  if( image->getData() == 0 )
    return false;

  JSAMPLE* jpeg_pixels;

  struct jpeg_compress_struct jpeg_info;
  struct jpeg_error_mgr jpeg_error;

  int width= image->getWidth();
  int height= image->getHeight();

  // Initialize JPEG parameters.
  jpeg_info.err= jpeg_std_error( &jpeg_error );
  jpeg_info.err->emit_message= EmitMessage;
  jpeg_create_compress( &jpeg_info );
  jpeg_stdio_dest( &jpeg_info, file );
  jpeg_info.image_width= width;
  jpeg_info.image_height= height;
  jpeg_info.input_components= 3;
  jpeg_info.in_color_space= JCS_RGB;

/*
  if( IsGrayImage(image) )
    {
      jpeg_info.input_components=1;
      jpeg_info.in_color_space=JCS_GRAYSCALE;
    }
*/
  jpeg_set_defaults( &jpeg_info );
  jpeg_info.density_unit= 0;
//  jpeg_info.X_density=(short) image->x_resolution;
//  jpeg_info.Y_density=(short) image->y_resolution;
//  if (image->units == PixelsPerInchResolution)
//    jpeg_info.density_unit=1;
//  if (image->units == PixelsPerCentimeterResolution)
//    jpeg_info.density_unit=2;

  for( int i= 0; i < MAX_COMPONENTS; i++ )
    {
    jpeg_info.comp_info[i].h_samp_factor=1;
    jpeg_info.comp_info[i].v_samp_factor=1;
    }

  jpeg_set_quality( &jpeg_info, quality, TRUE );
  jpeg_info.optimize_coding= TRUE;

#if (JPEG_LIB_VERSION >= 61)
  jpeg_info.dct_method= JDCT_FLOAT;
//  if (image_info->interlace != NoneInterlace)
  if( progressive )
    jpeg_simple_progression( &jpeg_info );
#endif

  jpeg_start_compress( &jpeg_info, TRUE );

//  if (image->comments != (char *) NULL)
//    for (i=0; i < Extent(image->comments); i+=65533)
//      jpeg_write_marker(&jpeg_info,JPEG_COM,(unsigned char *) image->comments+i,
//        (unsigned int) Min(Extent(image->comments+i),65533));

  // Convert MIFF to JPEG raster pixels.

  jpeg_pixels= new JSAMPLE [ width * 3 ];

  for( int y= 0; y < height; ++y )
    {
    JSAMPROW scanline[1];
    scanline[0]= (JSAMPROW) jpeg_pixels;
    JSAMPLE* q= jpeg_pixels;
    const unsigned char* p= image->getPixel(0,y);
    int delta= image->hasAlpha() ? 4 : 3;
    for( int x= 0; x < width; ++x )
      {
      *q++= (JSAMPLE) p[0];
      *q++= (JSAMPLE) p[1];
      *q++= (JSAMPLE) p[2];
      p+= delta;
      }

    jpeg_write_scanlines( &jpeg_info, scanline, 1 );
    }

  jpeg_finish_compress( &jpeg_info );

  // Free memory.
  jpeg_destroy_compress( &jpeg_info );
  delete jpeg_pixels;
  jpeg_pixels= 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////
