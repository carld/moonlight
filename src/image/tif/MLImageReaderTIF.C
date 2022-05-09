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
  MLImageReaderTIF.C

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
#include <setjmp.h>

#include "tools/Common.h"

#include "tiffio.h"

#include "image/MLImage.h"

#include "MLImageReaderTIF.h"

/////////////////////////////////////////////////////////////////////////////

MLImageReaderTIF::MLImageReaderTIF()
{
  _tiff= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLImageReaderTIF::~MLImageReaderTIF()
{}

/////////////////////////////////////////////////////////////////////////////

typedef unsigned char Quantum;

/////////////////////////////////////////////////////////////////////////////

void MLImageReaderTIF::close()
{
  if( _tiff != 0 )
    {
    TIFF* tiff= (TIFF*) _tiff;
    TIFFClose(tiff);
    _tiff= 0;
    }

  MLImageReader::close();
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

boolean MLImageReaderTIF::open( const MLString& filename )
{
//printf( "<%p>\n",_tiff);
  close();
  error= "";

  if( ! MLImageReader::open(filename) )
    return false;

  close();

  TIFFSetErrorHandler(TIFFWarningMessage);
  TIFFSetWarningHandler(TIFFWarningMessage);
  TIFF* tiff= TIFFOpen( filename.get(), "rb" );

  if( tiff == 0 )
    {
    error= "Unable to open file";
    return false;
    }

  _tiff= (void*) tiff;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLImageReaderTIF::read( MLImage* image )
{
  if( image == 0 || _tiff == 0 )
    return false;

  message[0]= '\0';

  char
    *text;

  int
    range;

  Quantum
    blue,
    green,
    red;

  register unsigned char
    *p;

  TIFF
    *tiff;

  uint16
    extra_samples,
    *sample_info;

  unsigned char
    *scanline;

  unsigned int
    height,
    method,
    status,
    width;

  unsigned short
    bits_per_sample,
    index,
    interlace,
    max_sample_value,
    min_sample_value,
    pages,
    photometric,
    samples_per_pixel,
    units,
    value;
  float x_resolution,
        y_resolution;

  TIFFSetErrorHandler(TIFFWarningMessage);
  TIFFSetWarningHandler(TIFFWarningMessage);

  tiff= (TIFF*) _tiff;

  do{
//    if (image_info->verbose)
//      TIFFPrintDirectory(tiff,stderr,False);

    TIFFGetField(tiff,TIFFTAG_IMAGEWIDTH,&width);
    TIFFGetField(tiff,TIFFTAG_IMAGELENGTH,&height);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_PLANARCONFIG,&interlace);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_BITSPERSAMPLE,&bits_per_sample);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_MINSAMPLEVALUE,&min_sample_value);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_MAXSAMPLEVALUE,&max_sample_value);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_PHOTOMETRIC,&photometric);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_SAMPLESPERPIXEL,&samples_per_pixel);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_RESOLUTIONUNIT,&units);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_XRESOLUTION,&x_resolution);
    TIFFGetFieldDefaulted(tiff,TIFFTAG_YRESOLUTION,&y_resolution);

    // Allocate memory for the image and pixel buffer.
    int columns= width;
    int rows= height;

/*
    if (units == RESUNIT_INCH)
      image->units=PixelsPerInchResolution;
    if (units == RESUNIT_CENTIMETER)
      image->units=PixelsPerCentimeterResolution;
*/
    int image_depth= bits_per_sample;
    if( bits_per_sample < 8 )
      image_depth=8;
/*
    image->packets=0;
    packets=Max((image->columns*image->rows+4) >> 3,1);
    if (bits_per_sample == 1)
      packets=Max((image->columns*image->rows+8) >> 4,1);
    image->pixels=(RunlengthPacket *) malloc(packets*sizeof(RunlengthPacket));
*/
    TIFFGetFieldDefaulted(tiff,TIFFTAG_PAGENUMBER,&value,&pages);
//    image->scene=value;
    text=(char *) NULL;
    TIFFGetField(tiff,TIFFTAG_PAGENAME,&text);
/*
    if( text != (char *) NULL )
      {
        image->label=(char *)
          malloc((unsigned int) (Extent(text)+1)*sizeof(char));
        if (image->label == (char *) NULL)
          {
            TIFFClose(tiff);
            _tiff= 0;
            PrematureExit("Unable to allocate memory",image);
          }
        (void) strcpy(image->label,text);
      }
*/
    text=(char *) NULL;
    TIFFGetField(tiff,TIFFTAG_IMAGEDESCRIPTION,&text);
/*
    if (text != (char *) NULL)
      {
        image->comments=(char *)
          malloc((unsigned int) (Extent(text)+1)*sizeof(char));
        if (image->comments == (char *) NULL)
          {
            TIFFClose(tiff);
            _tiff= 0;
            PrematureExit("Unable to allocate memory",image);
          }
        (void) strcpy(image->comments,text);
      }
*/
    range=max_sample_value-min_sample_value;
    if (range < 0)
      range=max_sample_value;

//    q=image->pixels;

    method= 0;
    if( (samples_per_pixel > 1) || TIFFIsTiled(tiff) )
      {
      method= 2;
      if( (samples_per_pixel >= 3) && (photometric == PHOTOMETRIC_RGB) &&
          (interlace == PLANARCONFIG_CONTIG))
        method= 1;
      }

#define QuantumDepth 8
#define ReadQuantum(quantum,p)  \
         {  \
         if( image_depth == 8 ) \
           quantum= *p++; \
          else \
           { \
           value= (*p++) << 8;\
           value|= (*p++); \
           quantum= (value >> (image_depth-QuantumDepth)) >> 8; \
           } \
         }

    switch( method )
      {
      case 0: // case method == 0
        {
        Quantum *quantum_scanline;
        register Quantum *r;
        const unsigned int MaxRGB= 255;

        // Convert TIFF image to PseudoClass MIFF image.
        int nColors= range+1;

        if( bits_per_sample > QuantumDepth )
          nColors= MaxRGB+1;

        struct ColorPacket
          {
          unsigned char red,
                        green,
                        blue;
          };

        ColorPacket* colormap= new ColorPacket [ nColors ];

        quantum_scanline= new Quantum [ width ];
        scanline= new unsigned char [ TIFFScanlineSize(tiff) + 1 ];

        // Create colormap.
        switch( photometric )
          {
          case PHOTOMETRIC_MINISBLACK:
            {
            for( int i= 0; i < nColors; i++ )
              {
              colormap[i].red=(MaxRGB*i)/(nColors-1);
              colormap[i].green=(MaxRGB*i)/(nColors-1);
              colormap[i].blue=(MaxRGB*i)/(nColors-1);
              }
            break;
            }
          case PHOTOMETRIC_MINISWHITE:
            {
            unsigned int colors= nColors;

            for( int i= 0; i < nColors; i++ )
              {
              colormap[colors-i-1].red=(MaxRGB*i)/(nColors-1);
              colormap[colors-i-1].green=(MaxRGB*i)/(nColors-1);
              colormap[colors-i-1].blue=(MaxRGB*i)/(nColors-1);
              }
            break;
            }
          case PHOTOMETRIC_PALETTE:
            {
            long range;

            unsigned short
              *blue_colormap,
              *green_colormap,
              *red_colormap;

            TIFFGetField( tiff,
                          TIFFTAG_COLORMAP,
                          &red_colormap,
                          &green_colormap,
                          &blue_colormap );
            range= 256L; // might be old style 8-bit colormap
            for( int i= 0; i < nColors; i++ )
              {
              if( (red_colormap[i] >= 256) ||
                  (green_colormap[i] >= 256) ||
                  (blue_colormap[i] >= 256) )
                {
                range=65535L;
                break;
                }
              }

            for( int i= 0; i < nColors; i++ )
              {
              colormap[i].red= Quantum(long(MaxRGB*red_colormap[i])/range);
              colormap[i].green= Quantum(long(MaxRGB*green_colormap[i])/range);
              colormap[i].blue= Quantum(long(MaxRGB*blue_colormap[i])/range);
              }
            break;
            }
          default:
            break;
          }

        image->createRGB( columns, rows );

        // Convert image
        for( int y= 0; y < rows; y++ )
          {
          TIFFReadScanline( tiff,(char *) scanline, y, 0 );
          p= scanline;
          r= quantum_scanline;
          switch( bits_per_sample )
            {
            case 1:
              {
              for( int x= 0; x < ((int) width-7); x+=8 )
                {
                for( int bit=7; bit >= 0; bit--)
                  *r++=((*p) & (0x01 << bit) ? 0x01 : 0x00);
                p++;
                }
              if( (width % 8) != 0 )
                {
                for( int bit=7; bit >= int(8-(width % 8)); bit--)
                  *r++=((*p) & (0x01 << bit) ? 0x01 : 0x00);
                p++;
                }
              break;
              }
            case 2:
              {
              for( int x= 0; x < ((int) width-3); x+=4 )
                {
                *r++=(*p >> 6) & 0x3;
                *r++=(*p >> 4) & 0x3;
                *r++=(*p >> 2) & 0x3;
                *r++=(*p) & 0x3;
                p++;
                }
              if( (width % 4) != 0 )
                {
                for( int i= 3; i >= int(4-(width % 4)); i-- )
                  *r++=(*p >> (i*2)) & 0x03;
                p++;
                }
              break;
              }
            case 4:
              {
              for( int x= 0; x < ((int) width-1); x+=2 )
                {
                *r++=(*p >> 4) & 0xf;
                *r++=(*p) & 0xf;
                p++;
                }
              if( (width % 2) != 0 )
                *r++=(*p++ >> 4) & 0xf;
              break;
              }
            case 8:
              {
              for( int x= 0; x < int(width); x++ )
                *r++=(*p++);
              break;
              }
            case 16:
              {
              for( int x= 0; x < columns; x++ )
                {
                ReadQuantum(*r,p);
                r++;
                }
              break;
              }
            default:
              break;
            } // end switch(bits_per_sample)

          // Transfer image scanline.
          r= quantum_scanline;
          unsigned char* line= image->getPixel(0,y);
          for( int x= 0; x < columns; x++ )
            {
            index= (*r++);

            assert( index < nColors );
            line[0]= colormap[index].red;
            line[1]= colormap[index].green;
            line[2]= colormap[index].blue;
            line+= 3;
            }
          }

        delete colormap; colormap= 0;
        delete scanline; scanline= 0;
        delete quantum_scanline; quantum_scanline= 0;
        break;
        }

      case 1: // case method == 1
        {
        // Convert TIFF image
        scanline= new unsigned char [ (TIFFScanlineSize(tiff) << 1)+1 ];
        TIFFGetFieldDefaulted( tiff,
                               TIFFTAG_EXTRASAMPLES,
                               &extra_samples,
                               &sample_info );
        boolean hasAlpha= (extra_samples == 1) &&
                        ((*sample_info == EXTRASAMPLE_ASSOCALPHA) ||
                         (*sample_info == EXTRASAMPLE_UNASSALPHA));

        if( hasAlpha )
          image->createRGBA(columns,rows);
         else
          image->createRGB(columns,rows);

        for( int y= 0; y < rows; y++ )
          {
          TIFFReadScanline( tiff, (char*) scanline, y, 0 );
          if( bits_per_sample == 4 )
            {
            register unsigned char *r;

            width= TIFFScanlineSize(tiff);
            p= scanline+width-1;
            r= scanline+(width << 1)-1;
            for( int x= 0; x < (int) width; x++ )
              {
              *r--=((*p) & 0xf) << 4;
              *r--=((*p >> 4) & 0xf) << 4;
              p--;
              }
            }
          p= scanline;
          unsigned char* line= image->getPixel(0,y);
          int delta= hasAlpha ? 4 : 3;
          for( int x= 0; x < columns; x++ )
            {
            ReadQuantum(red,p);
            ReadQuantum(green,p);
            ReadQuantum(blue,p);
            index=0;
            if (samples_per_pixel == 4)
              ReadQuantum(index,p);

            line[0]= red;
            line[1]= green;
            line[2]= blue;
            if( hasAlpha )
              line[3]= index;
            line+= delta;
            }
          }
        delete scanline; scanline= 0;
        break;
        }

      case 2: // case method == 2
      default:
        {
        register uint32
          *p,
          *pixels;

        // Convert TIFF image to DirectClass MIFF image.
        TIFFGetFieldDefaulted( tiff,
                               TIFFTAG_EXTRASAMPLES,
                               &extra_samples,
                               &sample_info);
        boolean hasAlpha= ((extra_samples == 1) &&
                        (sample_info[0] == EXTRASAMPLE_ASSOCALPHA));
        pixels= new uint32 [ columns*rows+columns ];
        status= TIFFReadRGBAImage(tiff,columns,rows,pixels,0);
        if( ! status )
          {
          delete pixels;
          pixels= 0;
          TIFFClose(tiff);
          _tiff= 0;
          error= "Unable to read image";
          return false;
          }

        image->create( columns, rows, 0, hasAlpha );
        int delta= hasAlpha ? 4 : 3;

        // Convert image
        for( int y= rows-1; y >= 0; y-- )
          {
          p= pixels+y*columns;
          unsigned char* line= image->getPixel(0,rows-1-y);

          for( int x= 0; x < columns; x++ )
            {
            red= TIFFGetR(*p);
            green= TIFFGetG(*p);
            blue= TIFFGetB(*p);
            index= hasAlpha ? TIFFGetA(*p) : 0;

            line[0]= red;
            line[1]= green;
            line[2]= blue;
            if( hasAlpha )
              line[3]= index;
            line += delta;

            p++;
            }
          }
        delete pixels;
        pixels= 0;
        break;
        }
      } // end switch


break;
/*
    // Proceed to next image.
    if (image_info->subrange != 0)
      if (image->scene >= (image_info->subimage+image_info->subrange-1))
        break;
    status=TIFFReadDirectory(tiff);
    if( status == True )
      {
      // Allocate next image structure.
      AllocateNextImage(image_info,image);
      if (image->next == (Image *) NULL)
        {
          DestroyImages(image);
          return((Image *) NULL);
        }
      image=image->next;
      ProgressMonitor(LoadImageText,image->scene-1,image->scene);
      }
*/
    }
  while( status );

  TIFFClose(tiff);
  _tiff= 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////
