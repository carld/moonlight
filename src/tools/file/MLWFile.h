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

#ifndef __MLWFile_h
#define __MLWFile_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLFile_h
#include "MLFile.h"
#endif

class MLWFileBlock;
class MLString;
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

class MLWFile: public MLFile
{
protected:
  MLWFileBlock* mainBlock;

public:
  MLWFile();
  virtual ~MLWFile();

  virtual void close();

  boolean create( const MLString& filename );

  boolean init( unsigned int signature );

  MLWFileBlock& getMainBlock() const
    {
    return *mainBlock;
    }

  void write( const void* data, int length );

  void putByte( char b );
  void putChar( char ch );
  void putUChar( unsigned char ch );
  void putInteger( int i );
  void putUInteger( unsigned int i );
  void putShort( short i );
  void putUShort( unsigned short i );
  void putLong( long l );
  void putULong( unsigned long l );
  void putFloat( float f );
  void putDouble( double d );
  void putString( const MLString& s );
  void putPoint( const Point& p );
  void putPoint2( const Point2& p );
  void putVector( const Vector& v );
  void putVector2( const Vector2& v );
  void putBox( const Box& b );
  void putMatrix4x4( const Matrix4x4& m );
  void putTransform( const Transform& t );
  void putColor( const Color& c );
  void putSpectrum( const Spectrum& s );

  MLWFile& operator << ( char c )          { putChar(c); return *this; }
  MLWFile& operator << ( unsigned char c ) { putUChar(c); return *this; }
  MLWFile& operator << ( int i )           { putInteger(i); return *this; }
  MLWFile& operator << ( unsigned int i )  { putUInteger(i); return *this; }
  MLWFile& operator << ( short i )         { putShort(i); return *this; }
  MLWFile& operator << ( unsigned short i ){ putUShort(i); return *this; }
  MLWFile& operator << ( long l )          { putLong(l); return *this; }
  MLWFile& operator << ( unsigned long l ) { putULong(l); return *this; }
  MLWFile& operator << ( float f )         { putFloat(f); return *this; }
  MLWFile& operator << ( double d )        { putDouble(d); return *this; }
  MLWFile& operator << ( const MLString& s ){ putString(s); return *this; }

  MLWFile& operator << ( const Point& p )  { putPoint(p); return *this; }
  MLWFile& operator << ( const Point2& p ) { putPoint2(p); return *this; }
  MLWFile& operator << ( const Vector& v ) { putVector(v); return *this; }
  MLWFile& operator << ( const Vector2& v ){ putVector2(v); return *this; }
  MLWFile& operator << ( const Box& b )    { putBox(b); return *this; }
  MLWFile& operator << ( const Matrix4x4& m){ putMatrix4x4(m); return *this; }
  MLWFile& operator << ( const Transform& t){ putTransform(t); return *this; }
  MLWFile& operator << ( const Color& c )  { putColor(c); return *this; }
  MLWFile& operator << ( const Spectrum& s){ putSpectrum(s); return *this; }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLWFile_h
