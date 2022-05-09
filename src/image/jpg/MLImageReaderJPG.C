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
  MLImageReaderJPG.C

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

#include "MLImageReaderJPG.h"

/////////////////////////////////////////////////////////////////////////////

MLImageReaderJPG::MLImageReaderJPG()
{}

/////////////////////////////////////////////////////////////////////////////

MLImageReaderJPG::~MLImageReaderJPG()
{}

/////////////////////////////////////////////////////////////////////////////

typedef unsigned char Quantum;

/////////////////////////////////////////////////////////////////////////////

static unsigned int GetCharacter( j_decompress_ptr jpeg_info )
{
  struct jpeg_source_mgr* data;

  data= jpeg_info->src;
  if (data->bytes_in_buffer == 0)
    (*data->fill_input_buffer) (jpeg_info);
  data->bytes_in_buffer--;
  return(GETJOCTET(*data->next_input_byte++));
}

/////////////////////////////////////////////////////////////////////////////

static jpg_boolean CommentHandler(j_decompress_ptr jpeg_info)
{
  long int length;

  // Determine length of comment.
  length= GetCharacter(jpeg_info) << 8;
  length+= GetCharacter(jpeg_info);
  length-= 2;
/*
  if( image->comments != (char *) NULL )
    {
    image->comments=(char *) realloc((char *) image->comments,
      (unsigned int) (Extent(image->comments)+length+1)*sizeof(char));
    }
   else
    {
    image->comments=(char *)
      malloc((unsigned int) (length+1)*sizeof(char));
    if (image->comments != (char *) NULL)
      *image->comments='\0';
    }
  if( image->comments == (char *) NULL )
    {
      Warning("Memory allocation error",(char *) NULL);
      return(False);
    }

  // Read comment.
  register char *p= image->comments+Extent(image->comments);
  while (--length >= 0)
    *p++=GetCharacter(jpeg_info);
  *p='\0';
  return(True);
*/
  while (--length >= 0)
    GetCharacter(jpeg_info);

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

static jmp_buf error_recovery;
static char message[JMSG_LENGTH_MAX];

static void EmitMessage( j_common_ptr jpeg_info, int level )
{
  struct jpeg_error_mgr* jpeg_error;

  jpeg_error= jpeg_info->err;
  (jpeg_error->format_message)( jpeg_info, message );

  if( level < 0 )
    {
//    if( (jpeg_error->num_warnings == 0) || (jpeg_error->trace_level >= 3) )
//      get_message= true;

    jpeg_error->num_warnings++;
    }
   else
    {
//    if( jpeg_error->trace_level >= level )
//      get_message= true;
    }
}

/////////////////////////////////////////////////////////////////////////////

static void ErrorExit( j_common_ptr jpeg_info )
{
  EmitMessage( jpeg_info, 0 );

  longjmp( error_recovery, 1 );
}


/////////////////////////////////////////////////////////////////////////////

boolean MLImageReaderJPG::read( MLImage* image )
{
  if( image == 0 || file == 0 )
    return false;

  JSAMPLE* jpeg_pixels;

  Quantum blue, green, red;

  register JSAMPLE *p;

  struct jpeg_decompress_struct
    jpeg_info;

  struct jpeg_error_mgr
    jpeg_error;


  unsigned short
    index;

  jpeg_info.err= jpeg_std_error(&jpeg_error);
  jpeg_info.err->emit_message= EmitMessage;
  jpeg_info.err->error_exit= ErrorExit;
  jpeg_pixels= (JSAMPLE *) NULL;

  ::message[0]= '\0';

  if( setjmp(error_recovery) )
    {
    error= ::message;

    // JPEG image is corrupt.
    if( jpeg_pixels != (JSAMPLE *) NULL )
      {
      delete jpeg_pixels;
      jpeg_pixels= 0;
      jpeg_destroy_decompress( &jpeg_info );
      }
    return false;
    }

  jpeg_create_decompress( &jpeg_info );
  jpeg_set_marker_processor( &jpeg_info, JPEG_COM, CommentHandler );
  jpeg_stdio_src( &jpeg_info, file );

  jpeg_read_header( &jpeg_info, TRUE );

/*
  if (jpeg_info.saw_JFIF_marker)
    {

    // Set image resolution.
    image->x_resolution= jpeg_info.X_density;
    image->y_resolution= jpeg_info.Y_density;
    if( jpeg_info.density_unit == 1 )
      image->units= PixelsPerInchResolution;
    if( jpeg_info.density_unit == 2 )
      image->units= PixelsPerCentimeterResolution;
    }
*/

/*
  if( image_info->size != (char *) NULL )
    {
    unsigned int height, width;
    unsigned long scale_factor;

    // Let the JPEG library subsample for us.
    jpeg_calc_output_dimensions( &jpeg_info );
    image->magick_columns=jpeg_info.output_width;
    image->magick_rows=jpeg_info.output_height;
    width=jpeg_info.output_width;
    height=jpeg_info.output_height;
    (void) ParseImageGeometry(image_info->size,&x,&y,&width,&height);
    if (width == 0)
      width=1;
    scale_factor=UpShift(jpeg_info.output_width)/width;
    if (height == 0)
      height=1;
    if (scale_factor > (UpShift(jpeg_info.output_height)/height))
      scale_factor=UpShift(jpeg_info.output_height)/height;
    jpeg_info.scale_denom=DownShift(scale_factor);
    jpeg_calc_output_dimensions(&jpeg_info);
    }
*/

//  enum { NonInterlace= 1, PlaneInterlace };

#if (JPEG_LIB_VERSION >= 61)
  jpeg_info.dct_method= JDCT_FLOAT;
//  image->interlace= jpeg_info.progressive_mode ? PlaneInterlace
//                                               : NoneInterlace;
#endif

  jpeg_start_decompress( &jpeg_info );
  unsigned int columns= jpeg_info.output_width;
  unsigned int rows= jpeg_info.output_height;

//  image->packets=0;
//  packets= Max((image->columns*image->rows+2) >> 2,1);
//  image->pixels= (RunlengthPacket *) malloc(packets*sizeof(RunlengthPacket));

  image->createRGB( columns, rows );
  jpeg_pixels= new JSAMPLE [ jpeg_info.output_components * columns ];

/*
  if( jpeg_info.out_color_space == JCS_GRAYSCALE )
    {
    // Initialize grayscale colormap.
    image->class=PseudoClass;
    image->colors=256;
    image->colormap=(ColorPacket *) malloc(image->colors*sizeof(ColorPacket));
    if (image->colormap == (ColorPacket *) NULL)
      PrematureExit("Unable to allocate memory",image);
    for (i=0; i < image->colors; i++)
      {
      image->colormap[i].red=UpScale(i);
      image->colormap[i].green=UpScale(i);
      image->colormap[i].blue=UpScale(i);
      }
    }
*/

  for( unsigned int y= 0; y < rows; y++ )
    {
    JSAMPROW scanline[1];
    scanline[0]= (JSAMPROW) jpeg_pixels;
    jpeg_read_scanlines( &jpeg_info, scanline, 1 );

    p= jpeg_pixels;
    unsigned char* q= image->getPixel(0,y);
    for( unsigned int x= 0; x < columns; x++ )
      {
      const int QuantumDepth= 8;
      if( jpeg_info.data_precision > QuantumDepth )
        {
        if( jpeg_info.out_color_space == JCS_GRAYSCALE )
          {
          index= GETJSAMPLE(*p++) >> 4;
          red= index;
          green= index;
          blue= index;
          }
         else
          {
          red= Quantum( GETJSAMPLE(*p++) >> 4 );
          green= Quantum( GETJSAMPLE(*p++) >> 4 );
          blue= Quantum( GETJSAMPLE(*p++) >> 4 );
          if( jpeg_info.out_color_space == JCS_CMYK )
            index= Quantum( GETJSAMPLE(*p++) >> 4 );
          }
        }
       else
        {
        if( jpeg_info.out_color_space == JCS_GRAYSCALE )
          {
          index= GETJSAMPLE(*p++);
          red= index;
          green= index;
          blue= index;
          }
         else
          {
// UPSCALE
          red= Quantum( GETJSAMPLE(*p++) );
          green= Quantum( GETJSAMPLE(*p++) );
          blue= Quantum( GETJSAMPLE(*p++) );
          if( jpeg_info.out_color_space == JCS_CMYK )
            index= Quantum( GETJSAMPLE(*p++) );
          }

      if( jpeg_info.out_color_space == JCS_CMYK )
        {
        index= MAXJSAMPLE - index;
        if( int(red-index) < 0 )
          red= 0;
         else
          red-= index;
        if( int(green-index) < 0 )
          green= 0;
         else
          green-= index;
        if( int(blue-index) < 0 )
          blue= 0;
         else
          blue-= index;
        index= 0;
        }

      q[0]= red;
      q[1]= green;
      q[2]= blue;
      q+= 3;
      }

//    ProgressMonitor(LoadImageText,y,image->rows);
    }
  }

  // Free memory.
  delete jpeg_pixels;
  jpeg_pixels= 0;

  jpeg_finish_decompress( &jpeg_info );
  jpeg_destroy_decompress( &jpeg_info );

  return true;
}

/////////////////////////////////////////////////////////////////////////////
