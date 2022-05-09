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
  MLString.h

  Stephane Rehel
  March 18 1998
*/

#ifndef __MLString_h
#define __MLString_h

#include <assert.h>
#include <string.h>
#include <stdarg.h>

#include "boolean.h"

/////////////////////////////////////////////////////////////////////////////

class MLString
{
protected:
  static char* empty_string;

  char* string;
  int _length;

public:
  static const MLString& empty();

  static MLString printf( const char* format, ... );
  static MLString vprintf( const char* format, va_list ap );

  MLString()
    {
    string= empty_string;
    _length= 0;
    }

  MLString( const char* _string )
    {
    string= 0;
    set(_string);
    }

  MLString( const char* _string, int offset, int count )
    {
    string= 0;
    set( (_string==0) ? 0 : (_string+offset), count );
    }

  MLString( const MLString& _string )
    {
    string= 0;
    set(_string);
    }

  // allocate a string with specified length.
  // the string is initialized with spaces
  MLString( int __length );

  virtual ~MLString()
    {
    empty_it();
    }

  static MLString singleChar( char ch )
    {
    return MLString( &ch, 0, 1 );
    }

protected:
  void empty_it()
    {
    if( string != empty_string )
      delete string;
    string= empty_string;
    _length= 0;
    }

public:
  void set( const char* _string );
  void set( const MLString& _string );
  void set( const char* _string, int __length );

  int length() const
    {
    return _length;
    }

  char charAt( int index ) const
    {
    if( index < 0 || index >= _length )
      return '\0';
    return string[index];
    }

  char& operator [] ( int index )
    {
    assert( index >= 0 );
    assert( index < _length );
    return string[index];
    }

  void setCharAt( int index, char ch );

  MLString& operator = ( const MLString& s )
    {
    set(s);
    return *this;
    }

  MLString& operator = ( const char* s )
    {
    set(s);
    return *this;
    }

  const char* get() const
    {
    return string;
    }

  char* getBytes()
    {
    return string;
    }

  const char* getConstBytes() const
    {
    return string;
    }

  virtual int hashCode() const;

  virtual boolean equals( const MLString& s ) const
    {
    if( s._length != _length )
      return false;
    return strcmp(string,s.string) == 0;
    }

  virtual boolean equalsIgnoreCase( const MLString& s ) const
    {
    if( s._length != _length )
      return false;
    return strcasecmp(string,s.string) == 0;
    }

  int compareTo( const MLString& s ) const
    {
    return strcmp( string, s.string );
    }

  boolean operator < ( const MLString& s ) const
    {
    return compareTo(s) < 0;
    }
  boolean operator <= ( const MLString& s ) const
    {
    return compareTo(s) <= 0;
    }

  boolean operator == ( const MLString& s ) const
    {
    return compareTo(s) == 0;
    }
  boolean operator != ( const MLString& s ) const
    {
    return ! (operator == ( s ));
    }

  boolean startsWith( const MLString& prefix, int offset = 0 );
  boolean endsWith( const MLString& suffix );
  int indexOf( char ch, int fromIndex = 0 ) const;
  int lastIndexOf( char ch, int fromIndex ) const;
  int lastIndexOf( char ch ) const;
  int indexOf( const MLString& str, int fromIndex = 0 ) const;
  int lastIndexOf( const MLString& str, int fromIndex ) const;
  int lastIndexOf( const MLString& str ) const;
  MLString substring( int beginIndex, int endIndex ) const;
  MLString concat( const MLString& str );
  MLString replace( char oldChar, char newChar ) const;
  MLString toLowerCase() const;
  MLString toUpperCase() const;
  MLString trim() const;

  static MLString valueOf( boolean b );
  static MLString valueOf( int i );
  static MLString valueOf( double d );

  MLString& operator += ( char ch )
    {
    *this= concat(singleChar(ch));
    return *this;
    }

  MLString& operator += ( const MLString& s )
    {
    *this= concat(s);
    return *this;
    }

  MLString operator + ( char ch ) const
    {
    MLString s1(*this);
    return s1.concat( singleChar(ch) );
    }

  MLString operator + ( const MLString& s ) const
    {
    MLString s1(*this);
    return s1.concat(s);
    }

  virtual MLString* clone() const
    {
    return new MLString(*this);
    }

  void remove( int i );

  boolean glob( const MLString& pattern ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLString_h
