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
  MLWFile.h

  Stephane Rehel
  April 6 1997
*/

#include "MLWFile.h"
#include "MLWFileBlock.h"

#include "tools/xdr/XDRFile.h"
#include "tools/Vector.h"
#include "tools/Vector2.h"
#include "tools/Box.h"
#include "tools/Matrix4x4.h"
#include "tools/Transform.h"
#include "tools/Color.h"
#include "tools/Spectrum.h"

/////////////////////////////////////////////////////////////////////////////

MLWFile::MLWFile()
{
  mainBlock= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLWFile::~MLWFile()
{}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::close()
{
  if( mainBlock != 0 )
    {
    mainBlock->close();
    delete mainBlock;
    mainBlock= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLWFile::create( const MLString& filename )
{
  close();

  file= new XDRFile;

  if( ! file->open_for_writing(filename) )
    {
    delete file;
    file= 0;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLWFile::init( unsigned int signature )
{
  if( file == 0 )
    return false;

  file->setPosition(0);
  *file << signature;

  delete mainBlock;
  mainBlock= new MLWFileBlock;

  if( ! mainBlock->open(this) )
    {
    delete mainBlock;
    mainBlock= 0;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::write( const void* data, int length )
{
  if( data == 0 || length < 0 || file == 0 )
    return;

  file->rawWrite(data,length);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putByte( char b )
{
  if( MLFile::file != 0 )
    MLFile::file->putByte(b);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putChar( char ch )
{
  if( MLFile::file != 0 )
    MLFile::file->putChar(ch);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putUChar( unsigned char ch )
{
  if( MLFile::file != 0 )
    MLFile::file->putUChar(ch);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putInteger( int i )
{
  if( MLFile::file != 0 )
    MLFile::file->putInteger(i);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putUInteger( unsigned int i )
{
  if( MLFile::file != 0 )
    MLFile::file->putUInteger(i);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putShort( short i )
{
  if( MLFile::file != 0 )
    MLFile::file->putShort(i);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putUShort( unsigned short i )
{
  if( MLFile::file != 0 )
    MLFile::file->putUShort(i);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putLong( long l )
{
  if( MLFile::file != 0 )
    MLFile::file->putLong(l);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putULong( unsigned long l )
{
  if( MLFile::file != 0 )
    MLFile::file->putULong(l);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putFloat( float f )
{
  if( MLFile::file != 0 )
    MLFile::file->putFloat(f);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putDouble( double d )
{
  if( MLFile::file != 0 )
    MLFile::file->putDouble(d);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putString( const MLString& s )
{
  if( MLFile::file != 0 )
    MLFile::file->putString(s);
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putPoint( const Point& p )
{
  *this << double(p.x()) << double(p.y()) << double(p.z());
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putPoint2( const Point2& p )
{
  *this << double(p.x()) << double(p.y());
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putVector( const Vector& v )
{
  *this << double(v.x()) << double(v.y()) << double(v.z());
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putVector2( const Vector2& v )
{
  *this << double(v.x()) << double(v.y());
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putBox( const Box& b )
{
  *this << b.getMin() << b.getMax();
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putMatrix4x4( const Matrix4x4& m )
{
  for( int i= 0; i < 4; ++i )
    for( int j= 0; j < 4; ++j )
      *this << double(m(i,j));
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putTransform( const Transform& t )
{
  *this << t.get_w2o();
  *this << t.get_o2w();
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putColor( const Color& c )
{
  *this << double(c.r())
        << double(c.g())
        << double(c.b());
}

/////////////////////////////////////////////////////////////////////////////

void MLWFile::putSpectrum( const Spectrum& s )
{
  *this << Spectrum::real(s.r())
        << Spectrum::real(s.g())
        << Spectrum::real(s.b());
}

/////////////////////////////////////////////////////////////////////////////

