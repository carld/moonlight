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
  MLRFile.h

  Stephane Rehel
  April 6 1997
*/

#ifndef __MLRFile_h
#define __MLRFile_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLFile_h
#include "MLFile.h"
#endif

class MLString;
class MLRFileBlock;
class Point;
class Point2;
class Vector;
class Vector2;
class Box;
class Matrix4x4;
class Transform;
class Color;
class Spectrum;

/////////////////////////////////////////////////////////////////////////////

class MLRFile: public MLFile
{
protected:
  unsigned int signature;
  MLRFileBlock* mainBlock;

public:
  MLRFile();
  virtual ~MLRFile();

  virtual void close();

  boolean open( const MLString& filename );

  boolean init( unsigned int expected_signature );

  unsigned int getSignature() const
    {
    return signature;
    }

  MLRFileBlock& getMainBlock() const
    {
    return *mainBlock;
    }

  long getSize();

  void read( void* data, int length );

  void getByte( char* b );
  void getChar( char* ch );
  void getUChar( unsigned char* ch );
  void getInteger( int* i );
  void getUInteger( unsigned int* i );
  void getShort( short* i );
  void getUShort( unsigned short* i );
  void getLong( long* l );
  void getULong( unsigned long* l );
  void getFloat( float* f );
  void getDouble( double* d );
  void getString( MLString* s );
  void getPoint( Point* p );
  void getPoint2( Point2* p );
  void getVector( Vector* v );
  void getVector2( Vector2* v );
  void getBox( Box* b );
  void getMatrix4x4( Matrix4x4* m );
  void getTransform( Transform* t );
  void getColor( Color* c );
  void getSpectrum( Spectrum* s );

  MLRFile& operator >> ( char& c )          { getChar(&c); return *this; }
  MLRFile& operator >> ( unsigned char& c ) { getUChar(&c); return *this; }
  MLRFile& operator >> ( int& i )           { getInteger(&i); return *this; }
  MLRFile& operator >> ( unsigned int& i )  { getUInteger(&i); return *this; }
  MLRFile& operator >> ( short& i )         { getShort(&i); return *this; }
  MLRFile& operator >> ( unsigned short& i ){ getUShort(&i); return *this; }
  MLRFile& operator >> ( long& l )          { getLong(&l); return *this; }
  MLRFile& operator >> ( unsigned long& l ) { getULong(&l); return *this; }
  MLRFile& operator >> ( float& f )         { getFloat(&f); return *this; }
  MLRFile& operator >> ( double& d )        { getDouble(&d); return *this; }
  MLRFile& operator >> ( MLString& s )       { getString(&s); return *this; }

  MLRFile& operator >> ( Point& p )         { getPoint(&p); return *this; }
  MLRFile& operator >> ( Point2& p )        { getPoint2(&p); return *this; }
  MLRFile& operator >> ( Vector& v )        { getVector(&v); return *this; }
  MLRFile& operator >> ( Vector2& v )       { getVector2(&v); return *this; }
  MLRFile& operator >> ( Box& b )           { getBox(&b); return *this; }
  MLRFile& operator >> ( Matrix4x4& m )     { getMatrix4x4(&m); return *this; }
  MLRFile& operator >> ( Transform& t )     { getTransform(&t); return *this; }
  MLRFile& operator >> ( Color& c )         { getColor(&c); return *this; }
  MLRFile& operator >> ( Spectrum& s )      { getSpectrum(&s); return *this; }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLRFile_h
