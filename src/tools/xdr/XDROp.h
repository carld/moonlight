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
  XDROp.h

  Stephane Rehel
  August 4 1996
*/

#ifndef __XDROp_h
#define __XDROp_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class MLString;

/////////////////////////////////////////////////////////////////////////////

class XDROp
{
protected:
  void* xdr; // actually XDR* (avoid including headers...)
  boolean error;

public:
  XDROp();
  virtual ~XDROp();

  boolean getError() const
    {
    return error;
    }

public:

  boolean putByte( char b );
  boolean putChar( char ch );
  boolean putUChar( unsigned char ch );
  boolean putInteger( int i );
  boolean putUInteger( unsigned int i );
  boolean putShort( short i );
  boolean putUShort( unsigned short i );
  boolean putLong( long l );
  boolean putULong( unsigned long l );
  boolean putFloat( float f );
  boolean putDouble( double d );
  boolean putString( const MLString& s );

  XDROp& operator << ( char c )          { putChar(c); return *this; }
  XDROp& operator << ( unsigned char c ) { putUChar(c); return *this; }
  XDROp& operator << ( int i )           { putInteger(i); return *this; }
  XDROp& operator << ( unsigned int i )  { putUInteger(i); return *this; }
  XDROp& operator << ( short i )         { putShort(i); return *this; }
  XDROp& operator << ( unsigned short i ){ putUShort(i); return *this; }
  XDROp& operator << ( long l )          { putLong(l); return *this; }
  XDROp& operator << ( unsigned long l ) { putULong(l); return *this; }
  XDROp& operator << ( float f )         { putFloat(f); return *this; }
  XDROp& operator << ( double d )        { putDouble(d); return *this; }
  XDROp& operator << ( const MLString& s ){ putString(s); return *this; }

  // return false if error
  boolean getByte( char* b );
  boolean getChar( char* ch );
  boolean getUChar( unsigned char* ch );
  boolean getInteger( int* i );
  boolean getUInteger( unsigned int* i );
  boolean getShort( short* i );
  boolean getUShort( unsigned short* i );
  boolean getLong( long* l );
  boolean getULong( unsigned long* l );
  boolean getFloat( float* f );
  boolean getDouble( double* d );
  boolean getString( MLString* s );

  XDROp& operator >> ( char& c )          { getChar(&c); return *this; }
  XDROp& operator >> ( unsigned char& c ) { getUChar(&c); return *this; }
  XDROp& operator >> ( int& i )           { getInteger(&i); return *this; }
  XDROp& operator >> ( unsigned int& i )  { getUInteger(&i); return *this; }
  XDROp& operator >> ( short& i )         { getShort(&i); return *this; }
  XDROp& operator >> ( unsigned short& i ){ getUShort(&i); return *this; }
  XDROp& operator >> ( long& l )          { getLong(&l); return *this; }
  XDROp& operator >> ( unsigned long& l ) { getULong(&l); return *this; }
  XDROp& operator >> ( float& f )         { getFloat(&f); return *this; }
  XDROp& operator >> ( double& d )        { getDouble(&d); return *this; }
  XDROp& operator >> ( MLString& s )       { getString(&s); return *this; }

  static int getByteSize() { return 4; }
  static int getIntegerSize() { return 4; }
  static int getDoubleSize() { return 8; }
  static int getCharSize() { return 4; }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __XDROp_h
