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
  MLImageReaderGIF.C

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

#include "MLImageReaderGIF.h"

/////////////////////////////////////////////////////////////////////////////

MLImageReaderGIF::MLImageReaderGIF()
{
  colormap= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLImageReaderGIF::~MLImageReaderGIF()
{
  delete colormap;
  colormap= 0;
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

static int ReadDataBlock( char *data, FILE *file )
{
  unsigned char count;

  assert( data != (char *) NULL );
  assert( file != (FILE *) NULL );

  boolean status= ReadData( (char*) &count, 1, 1, file );
  if( ! status )
    return -1;
  if( count == 0 )
    return 0;

  status= ReadData( data, 1, (unsigned int) count, file );
  if( ! status )
    return -1;

  return count;
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

boolean MLImageReaderGIF::read( MLImage* image )
{
  if( image == 0 || file == 0 )
    return false;

#define BitSet(byte,bit)  (((byte) & (bit)) == (bit))
#define LSBFirstOrder(x,y)  (((y) << 8) | (x))

  const int MaxTextExtent= 2048;
//  char geometry[MaxTextExtent];
  int status;
  int page_info_x;
  int page_info_y;
  int page_info_width;
  int page_info_height;

//  register RunlengthPacket *q;

  short int transparency_index;
  unsigned char background,
                c,
                flag,
                *global_colormap,
                header[MaxTextExtent],
                magick[12];

  unsigned int delay,
               dispose,
               global_colors,
               image_count,
               iterations;

  //Determine if this is a GIF file.
  status= ReadData( (char*) magick,1,6,file );
  if( !status ||
      ( (strncmp((char*)magick,"GIF87",5) != 0 ) &&
        (strncmp((char*)magick,"GIF89",5) != 0)) )
    {
    error= "Not a GIF image file";
    return false;
    }

  global_colors=0;
  global_colormap=(unsigned char *) NULL;
  page_info_width=LSBFirstReadShort(file);
  page_info_height=LSBFirstReadShort(file);
  ReadData((char *) &flag,1,1,file);
  ReadData((char *) &background,1,1,file);
  ReadData((char *) &c,1,1,file);  // reserved
  if( BitSet(flag,0x80) )
    {
    // Read global colormap.
    global_colors= 1 << ((flag & 0x07)+1);
    global_colormap= new unsigned char [ 3*global_colors ];
    ReadData((char *) global_colormap,3,global_colors,file);
    }

  delay=0;
  dispose=0;
  iterations=1;
  transparency_index=-1;
  image_count=0;

  for(;;)
    {
    status= ReadData((char*) &c,1,1,file);
    if( ! status )
      break;
    if( c == ';' )
      break; // terminator
    if( c == '!' )
      {
      // GIF Extension block.
      status= ReadData((char *) &c,1,1,file);
      if( ! status )
        {
        error= "Unable to read extention block";
        delete global_colormap;
        return false;
        }
      switch( c )
        {
        case 0xf9:
          {
          // Read Graphics Control extension.
          while( ReadDataBlock((char *) header,file) > 0 );
          dispose=header[0] >> 2;
          delay=(header[2] << 8) | header[1];
          if ((header[0] & 0x01) == 1)
            transparency_index= header[3];
          break;
          }

        case 0xfe:
          {
          int length;

          // Read Comment extension.
          for(;;)
            {
            length= ReadDataBlock((char *) header,file);
            if( length <= 0 )
              break;
            }
          break;
          }

        case 0xff:
          {
          // Read Netscape Loop extension.
          while( ReadDataBlock((char *) header,file ) > 0 );
          iterations=(header[2] << 8) | header[1];
          break;
          }

        default:
          {
          while( ReadDataBlock((char *) header,file) > 0 );
          break;
          }
        }
      }

    if( c != ',' )
      continue;

/*
    if( image_count != 0 )
      {
        //Allocate next image structure.
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

    image_count++;

    // Read image attributes.
//    image->class=PseudoClass;
    page_info_x= LSBFirstReadShort(file);
    page_info_y= LSBFirstReadShort(file);
    int columns= 0,
        rows= 0;
    columns= LSBFirstReadShort(file);
    rows= LSBFirstReadShort(file);
    if( columns*rows <= 0 )
      {
      error= "image size is 0";
      delete global_colormap;
      return false;
      }

    // Allocate image.
/*
    if (image->pixels != (RunlengthPacket *) NULL)
      free((char *) image->pixels);
    image->pixels=(RunlengthPacket *)
      malloc(image->packets*sizeof(RunlengthPacket));
    if (image->pixels == (RunlengthPacket *) NULL)
      PrematureExit("Unable to allocate memory",image);
    (void) sprintf(geometry,"%ux%u%+d%+d",page_info.width,page_info.height,
      page_info.x,page_info.y);
    image->page=PostscriptGeometry(geometry);
    image->delay=delay;
    image->dispose=dispose;
    image->iterations=iterations;
*/
    image->createRGB(columns,rows);

    delay= 0;
    dispose= 0;
    iterations= 1;

    // Inititialize colormap.
    ReadData( (char *) &flag,1,1,file );

    enum { PlaneInterlace= 1, LineInterlace, NoneInterlace };

    int interlace= BitSet(flag,0x40) ? PlaneInterlace : NoneInterlace;
    int nColors= (!BitSet(flag,0x80)) ? global_colors
                                      : (1 << ((flag & 0x07)+1));
    colormap= new unsigned char [ nColors * 3 ];

#define UpScale(p) p

    if( !BitSet(flag,0x80) )
      {
      // Use global colormap.
      unsigned char* p= global_colormap;
      for( int i= 0; i < nColors; i++ )
        {
        colormap[i*3+0]= UpScale(*p++);
        colormap[i*3+1]= UpScale(*p++);
        colormap[i*3+2]= UpScale(*p++);
        }
      background_color= colormap[background];
      }
     else
      {
      // Read local colormap.
      ReadData( (char *) colormap,3,nColors,file );
      unsigned char* p= colormap;
      for( int i= 0; i < nColors; i++ )
        {
        colormap[i*3+0]= UpScale(*p++);
        colormap[i*3+1]= UpScale(*p++);
        colormap[i*3+2]= UpScale(*p++);
        }
      }

    // Decode image.
    status= GIFDecodeImage(image);
    if( interlace != NoneInterlace )
      {
      static int interlace_rate[4] = { 8, 8, 4, 2 },
                 interlace_start[4]= { 0, 4, 2, 1 };

      // Interlace image.

//      image_info->interlace=LineInterlace;
//     image->orphan=True;
      int nPackets= image->getWidth() * image->getHeight();
      unsigned char* interlaced_image= new unsigned char [ nPackets * 3 ];
      memcpy( (void*) interlaced_image, (const void*) image->getData(),
              nPackets * 3 );

      unsigned char* p= interlaced_image;
//      unsigned char* q= image->getData();
      for( int pass= 0; pass < 4; pass++ )
        {
        int y= interlace_start[pass];
        while( y < rows )
          {
          unsigned char* q= image->getPixel(0,y); //image->pixels + (y*image->columns);
          for( int x= 0; x < columns; x++ )
            {
            q[0]= p[0];
            q[1]= p[1];
            q[2]= p[2];
            q+= 3;
            p+= 3;
            }
          y+= interlace_rate[pass];
          }
        }
      delete interlaced_image;
      }

/*
    if( transparency_index >= 0 )
      {
      // Create matte channel.
      q=image->pixels;
      for (i=0; i < image->packets; i++)
      {
        if (q->index != (unsigned short) transparency_index)
          q->index=Opaque;
        else
          q->index=Transparent;
        q++;
      }
      transparency_index=(-1);
      image->class=DirectClass;
      image->matte=True;
      }
*/

    if( ! status )
      {
      error= "Corrupt GIF image";
      return false;
      }

/*
    CompressImage(image);
    if (image_info->subrange != 0)
      if ((image->scene+1) < image_info->subimage)
        {
          Image subimage;

          // Destroy image.
          subimage=(*image);
          image->file=(FILE *) NULL;
          DestroyImage(image);
          image=AllocateImage(image_info);
          if (image == (Image *) NULL)
            return((Image *) NULL);
          image->file=subimage.file;
          image->scene=subimage.scene+1;
          image_count=0;
        }
      else
        if ((image->scene+1) > (image_info->subimage+image_info->subrange-1))
          break;
*/
    }

  delete global_colormap;
  global_colormap= 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

unsigned int MLImageReaderGIF::GIFDecodeImage( MLImage* image )
{
#define NullCode  (-1)

  int
    available,
    clear,
    code_mask,
    code_size,
    end_of_information,
    in_code,
    old_code,
    status;

  register int
    bits,
    code,
    count,
    i;

  register unsigned char
    *c;
  register unsigned int
    datum;
  short
    *prefix;
  unsigned char
    data_size,
    first,
    *packet,
    *pixel_stack,
    *suffix,
    *top_stack;

  // Allocate decoder tables.

  const int MaxStackSize= 4096;

  packet= new unsigned char [ 256 ];
  prefix= new short [ MaxStackSize ];
  suffix= new unsigned char [ MaxStackSize ];
  pixel_stack= new unsigned char [ MaxStackSize+1 ];

  // Initialize GIF data stream decoder.
  data_size= fgetc(file);
  clear= 1 << data_size;
  end_of_information= clear+1;
  available= clear+2;
  old_code= NullCode;
  code_size= data_size+1;
  code_mask= (1 << code_size)-1;
  for( code= 0; code < clear; code++ )
    {
    prefix[code]= 0;
    suffix[code]= code;
    }

  // Decode GIF pixel stream.
  datum= 0;
  bits= 0;
  c= 0;
  count= 0;
  first= 0;
  top_stack= pixel_stack;

  unsigned char* pixels= image->getPixel(0,0);
  int nPackets= image->getWidth() * image->getHeight();
  for( i= 0; i < nPackets; )
    {
    if( top_stack == pixel_stack )
      {
      if( bits < code_size )
        {
        // Load bytes until there is enough bits for a code.
        if( count == 0 )
          {
          // Read a new data block.
          count= ReadDataBlock( (char *) packet,file );
          if( count <= 0 )
            break;
          c= packet;
          }
        datum+= (*c) << bits;
        bits+= 8;
        c++;
        count--;
        continue;
        }

      // Get the next code.
      code= datum & code_mask;
      datum>>= code_size;
      bits-= code_size;

      // Interpret the code
      if( (code > available) || (code == end_of_information) )
        break;
      if( code == clear )
        {
        // Reset decoder.
        code_size= data_size+1;
        code_mask= (1 << code_size)-1;
        available= clear+2;
        old_code= NullCode;
        continue;
        }

      if( old_code == NullCode )
        {
        *top_stack++= suffix[code];
        old_code= code;
        first= code;
        continue;
        }
      in_code= code;
      if( code == available )
        {
        *top_stack++= first;
        code= old_code;
        }
      while( code > clear )
        {
        *top_stack++= suffix[code];
        code= prefix[code];
        }
      first= suffix[code];

      // Add a new string to the string table,
      if( available >= MaxStackSize )
        break;
      *top_stack++= first;
      prefix[available]= old_code;
      suffix[available]= first;
      available++;
      if( ((available & code_mask) == 0) && (available < MaxStackSize) )
        {
        code_size++;
        code_mask+= available;
        }
      old_code= in_code;
      }

    // Pop a pixel off the pixel stack.
    top_stack--;

    int pixel_index= (unsigned short) *top_stack;
    pixels[0]= colormap [ pixel_index * 3 + 0 ];
    pixels[1]= colormap [ pixel_index * 3 + 1 ];
    pixels[2]= colormap [ pixel_index * 3 + 2 ];
    pixels+= 3;

    i++;
//    if( QuantumTick(i,image) && (image->previous == (Image *) NULL) )
//      ProgressMonitor(LoadImageText,i,image->packets);
    }

  // Initialize any remaining color packets to a known color.
  status= (i == nPackets);
  for( ; i < nPackets; i++ )
    {
    pixels[0]= 0;
    pixels[1]= 0;
    pixels[2]= 0;
    pixels += 3;
    }

  // Free decoder memory.
  delete pixel_stack;
  delete suffix;
  delete prefix;
  delete packet;

  return status;
}

/////////////////////////////////////////////////////////////////////////////
