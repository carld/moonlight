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
  XDRFile.C

  Stephane Rehel
  Creation: SR, March 1st, 1996
  Revisions:
   SR August 4 1996
*/

#include "xdrinc.h"

#include <stdio.h>

#include "XDRFile.h"
#include "tools/MLPath.h"

/////////////////////////////////////////////////////////////////////////////

XDRFile::XDRFile()
{
  filename= MLString("");
  file= 0;
  headerSize= 0;
}

/////////////////////////////////////////////////////////////////////////////

XDRFile::~XDRFile()
{
  close();
}

/////////////////////////////////////////////////////////////////////////////

// protected
void XDRFile::close()
{
  delete xdr;
  xdr= 0;

  if( file != 0 )
    fclose(file);
  file= 0;
  filename= MLString("");
  headerSize= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDRFile::open_for_reading( const MLString& _filename )
{
  close();
  filename= _filename;
  xdr= (void*) new XDR;

  file= fopen( MLPath::checkPathSeparators(filename.get()).get(), "rb" );

  if( file != 0 )
    {
    xdrstdio_create( (XDR*)xdr, file, XDR_DECODE );
    error= false;
    }
   else
    error= false;

  return file != 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDRFile::open_for_writing( const MLString& _filename )
{
  close();
  filename= _filename;
  xdr= (void*) new XDR;

  file= fopen( MLPath::checkPathSeparators(filename.get()).get(), "wb" );

  if( file != 0 )
    {
    xdrstdio_create( (XDR*)xdr, file, XDR_ENCODE );
    error= false;
    }
   else
    error= true;

  return file != 0;
}

/////////////////////////////////////////////////////////////////////////////

void XDRFile::setHeaderSize( long hs )
{
  headerSize= hs;
}

/////////////////////////////////////////////////////////////////////////////

long XDRFile::getSize()
{
  assert( file != 0 );

  long cur= ftell(file);

  fseek( file, 0, SEEK_END );
  long size= ftell(file);

  fseek( file, cur, SEEK_SET );

  return size - headerSize; // hope not negative...
}

/////////////////////////////////////////////////////////////////////////////

long XDRFile::getPosition() const
{
  assert( file != 0 );

  return ftell(file) - headerSize; // hope not negative...
}

/////////////////////////////////////////////////////////////////////////////

void XDRFile::setPosition( long newPosition )
{
  assert( file != 0 );

  assert( newPosition <= getSize() );

  newPosition+= headerSize;
  fseek( file, newPosition, SEEK_SET );
}

/////////////////////////////////////////////////////////////////////////////

void XDRFile::rawWrite( const void* data, int length )
{
  assert( file != 0 );
  fwrite( data, length, 1, file );
}

/////////////////////////////////////////////////////////////////////////////

void XDRFile::rawRead( void* data, int length )
{
  assert( file != 0 );
  fread( data, length, 1, file );
}

/////////////////////////////////////////////////////////////////////////////
