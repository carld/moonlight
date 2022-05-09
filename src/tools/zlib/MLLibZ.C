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
  MLLibZ.C

  Stephane Rehel
  March 25 1997
*/

#include "MLLibZ.h"

#include "zlib.h"

/////////////////////////////////////////////////////////////////////////////

boolean MLLibZ::deflate( void* in_uncompr, int in_uncomprLen,
                       void* out_compr,  int out_comprLen,
                       int* compressedSize,
                       int level )
{
  if( compressedSize != 0 )
    *compressedSize= 0;

  if( in_uncompr == 0  || out_compr == 0 ||
      in_uncomprLen <= 0 || out_comprLen <= 0 )
    return false;

  z_stream c_stream; // compression stream
  int err;

  c_stream.zalloc= (alloc_func)0;
  c_stream.zfree = (free_func)0;
  c_stream.opaque= (voidpf)0;

//  err= deflateInit( &c_stream, Z_DEFAULT_COMPRESSION );
  err= deflateInit( &c_stream, level );
  if( err != Z_OK )
    return false;

  c_stream.next_in = (Byte*) in_uncompr;
  c_stream.next_out= (Byte*) out_compr;

  uLong _in_uncomprLen= (uLong) in_uncomprLen;
  uLong _out_comprLen= (uLong) out_comprLen;

  while( c_stream.total_in != _in_uncomprLen &&
         c_stream.total_out < _out_comprLen )
    {
    c_stream.avail_in= c_stream.avail_out = 1; // force small buffers
    err= ::deflate( &c_stream, Z_NO_FLUSH );
    if( err != Z_OK )
      return false;
    }

  // Finish the stream, still forcing small buffers:
  for(;;)
    {
    c_stream.avail_out= 1;
    err= ::deflate( &c_stream, Z_FINISH );
    if( err == Z_STREAM_END )
      break;
    if( err != Z_OK )
      return false;
    }

  err= deflateEnd( &c_stream );

  if( compressedSize != 0 )
    *compressedSize= (int) c_stream.total_out;

  return err == Z_OK;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLibZ::inflate( void* in_compr,    int in_comprLen,
                       void* out_uncompr, int out_uncomprLen )
{
  if( in_compr == 0    || out_uncompr == 0 ||
      in_comprLen <= 0 || out_uncomprLen <= 0 )
    return false;

/*
  z_stream d_stream; // decompression stream
  int err;

  d_stream.zalloc= (alloc_func)0;
  d_stream.zfree = (free_func)0;
  d_stream.opaque= (voidpf)0;

  err = inflateInit(&d_stream);
  if( err != Z_OK )
    return false;

  d_stream.next_in = (Byte*) in_compr;
  d_stream.next_out= (Byte*) out_uncompr;

  uLong _in_comprLen= (uLong) in_comprLen;
  uLong _out_uncomprLen= (uLong) out_uncomprLen;

  while( d_stream.total_out < _out_uncomprLen &&
         d_stream.total_in < _in_comprLen )
    {
    d_stream.avail_in= d_stream.avail_out= 1; // force small buffers
    err= ::inflate( &d_stream, Z_NO_FLUSH );
    if( err == Z_STREAM_END )
      break;
    if( err != Z_OK )
      return false;
    }

  err= inflateEnd(&d_stream);

  return err == Z_OK;
*/

  z_stream d_stream; // decompression stream

  d_stream.zalloc= (alloc_func)0;
  d_stream.zfree = (free_func)0;
  d_stream.opaque= (voidpf)0;

  int err= inflateInit(&d_stream);
  if( err != Z_OK )
    return false;

  d_stream.next_in = (Byte*) in_compr;
  d_stream.avail_in= (uInt) in_comprLen;

  for(;;)
    {
    d_stream.next_out= (Byte*) out_uncompr; // discard the output
    d_stream.avail_out= (uInt) out_uncomprLen;
    err= ::inflate( &d_stream, Z_NO_FLUSH );
    if( err == Z_STREAM_END )
      break;
    if( err != Z_OK )
      return false;
    }

  err= inflateEnd(&d_stream);
  if( err != Z_OK )
    return false;

/*
  if( int(d_stream.total_out) != int(2*out_uncomprLen + in_comprLen/2) )
    {
    // fprintf(stderr, "bad large inflate: %ld\n", d_stream.total_out);
    return false;
    }
*/

  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLZDeflated::MLZDeflated( void* _p )
{
  p= _p;

  if( p == 0 )
    {
    compressedLength= 0;
    uncompressedLength= 0;
    }
   else
    {
    unsigned char* pc= (unsigned char*) p;
    compressedLength= (int(pc[0*4+0]) <<  0) |
                      (int(pc[0*4+1]) <<  8) |
                      (int(pc[0*4+2]) << 16) |
                      (int(pc[0*4+3]) << 24) ;
    uncompressedLength= (int(pc[1*4+0]) <<  0) |
                        (int(pc[1*4+1]) <<  8) |
                        (int(pc[1*4+2]) << 16) |
                        (int(pc[1*4+3]) << 24) ;

    p= (void*) (pc + 2 * 4);
    if( compressedLength < 0 ) compressedLength= 0;
    if( uncompressedLength < 0 ) uncompressedLength= 0;
    }

}

/////////////////////////////////////////////////////////////////////////////

boolean MLZDeflated::inflate( void* dest )
{
  return MLLibZ::inflate( p, compressedLength, dest, uncompressedLength );
}

/////////////////////////////////////////////////////////////////////////////

void* MLZDeflated::inflate()
{
  if( uncompressedLength <= 0 )
    return 0;

  void* dest= (void*) new char [ uncompressedLength ];
  if( ! inflate(dest) )
    {
    delete dest;
    return 0;
    }

  return dest;
}

/////////////////////////////////////////////////////////////////////////////

