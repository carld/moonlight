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
  MLRFile.C

  Stephane Rehel
  April 6 1997
*/

#include "MLRFile.h"
#include "MLRFileBlock.h"

#include "tools/xdr/XDRFile.h"
#include "tools/MLString.h"
#include "tools/Vector.h"
#include "tools/Vector2.h"
#include "tools/Box.h"
#include "tools/Matrix4x4.h"
#include "tools/Transform.h"
#include "tools/Color.h"
#include "tools/Spectrum.h"

/////////////////////////////////////////////////////////////////////////////

MLRFile::MLRFile(): MLFile()
{
  mainBlock= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLRFile::~MLRFile()
{}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::close()
{
  delete mainBlock;
  mainBlock= 0;

  MLFile::close();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLRFile::open( const MLString& filename )
{
  close();

  file= new XDRFile;

  if( ! file->open_for_reading(filename) )
    {
    delete file;
    file= 0;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLRFile::init( unsigned int expected_signature )
{
  if( file == 0 )
    return false;

  file->setPosition(0);
  *file >> signature;

  if( signature != expected_signature )
    return false;

  delete mainBlock;
  mainBlock= new MLRFileBlock;

  if( ! mainBlock->open(this) )
    {
    delete mainBlock;
    mainBlock= 0;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

long MLRFile::getSize()
{
  if( MLFile::file != 0 )
    return MLFile::file->getSize();
   else
    return 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::read( void* data, int length )
{
  if( data == 0 || length < 0 || file == 0 )
    return;

  file->rawRead( data, length );
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getByte( char* b )
{
  if( MLFile::file != 0 )
    MLFile::file->getByte(b);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getChar( char* ch )
{
  if( MLFile::file != 0 )
    MLFile::file->getChar(ch);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getUChar( unsigned char* ch )
{
  if( MLFile::file != 0 )
    MLFile::file->getUChar(ch);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getInteger( int* i )
{
  if( MLFile::file != 0 )
    MLFile::file->getInteger(i);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getUInteger( unsigned int* i )
{
  if( MLFile::file != 0 )
    MLFile::file->getUInteger(i);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getShort( short* i )
{
  if( MLFile::file != 0 )
    MLFile::file->getShort(i);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getUShort( unsigned short* i )
{
  if( MLFile::file != 0 )
    MLFile::file->getUShort(i);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getLong( long* l )
{
  if( MLFile::file != 0 )
    MLFile::file->getLong(l);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getULong( unsigned long* l )
{
  if( MLFile::file != 0 )
    MLFile::file->getULong(l);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getFloat( float* f )
{
  if( MLFile::file != 0 )
    MLFile::file->getFloat(f);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getDouble( double* d )
{
  if( MLFile::file != 0 )
    MLFile::file->getDouble(d);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getString( MLString* s )
{
  if( MLFile::file != 0 )
    MLFile::file->getString(s);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getPoint( Point* p )
{
  if( MLFile::file != 0 )
    {
    double x, y, z;
    (*MLFile::file) >> x >> y >> z;
    (*p)[0]= x;
    (*p)[1]= y;
    (*p)[2]= z;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getPoint2( Point2* p )
{
  if( MLFile::file != 0 )
    {
    double x, y;
    (*MLFile::file) >> x >> y;
    (*p)[0]= x;
    (*p)[1]= y;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getVector( Vector* v )
{
  if( MLFile::file != 0 )
    {
    double x, y, z;
    (*MLFile::file) >> x >> y >> z;
    (*v)[0]= x;
    (*v)[1]= y;
    (*v)[2]= z;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getVector2( Vector2* v )
{
  if( MLFile::file != 0 )
    {
    double x, y;
    (*MLFile::file) >> x >> y;
    (*v)[0]= x;
    (*v)[1]= y;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getBox( Box* b )
{
  Point minP, maxP;
  *this >> minP >> maxP;
  b->setMin(minP);
  b->setMax(maxP);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getMatrix4x4( Matrix4x4* m )
{
  for( int i= 0; i < 4; ++i )
    for( int j= 0; j < 4; ++j )
      {
      double d;
      *this >> d;
      m->aij(i,j)= d;
      }
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getTransform( Transform* t )
{
  Matrix4x4 w2o, o2w;
  *this >> w2o;
  *this >> o2w;

  t->set(o2w,w2o);
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getColor( Color* c )
{
  double r, g, b;
  *this >> r >> g >> b;

  (*c)[0]= r;
  (*c)[1]= g;
  (*c)[2]= b;
}

/////////////////////////////////////////////////////////////////////////////

void MLRFile::getSpectrum( Spectrum* s )
{
  Spectrum::real r, g, b;
  *this >> r >> g >> b;

  (*s)[0]= r;
  (*s)[1]= g;
  (*s)[2]= b;
}

/////////////////////////////////////////////////////////////////////////////
