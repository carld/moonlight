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
  MLString.C

  Stephane Rehel
  March 18 1998
*/

#include <string.h>
#include <stdio.h>

#include "MLString.h"

/////////////////////////////////////////////////////////////////////////////

char* MLString::empty_string= "";

/////////////////////////////////////////////////////////////////////////////

// static
const MLString& MLString::empty()
{
  static MLString* emptyString= 0;
  if( emptyString == 0 )
    emptyString= new MLString("");

  return *emptyString;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLString::printf( const char* format, ... )
{
  va_list args;

  static const int bufsize= 1024;
  char buffer [ bufsize+1 ];

  va_start( args, format );
#ifdef __USE_BSD
  vsnprintf( buffer, bufsize, format, args );
#else
  vsprintf( buffer, format, args );
#endif
  va_end( args );

  return MLString(buffer);
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLString::vprintf( const char* format, va_list ap )
{
  static const int bufsize= 1024;
  char buffer [ bufsize+1 ];

#ifdef __USE_BSD
  vsnprintf( buffer, bufsize, format, ap );
#else
  vsprintf( buffer, format, ap );
#endif

  return MLString(buffer);
}

/////////////////////////////////////////////////////////////////////////////

MLString::MLString( int __length )
{
  if( __length <= 0 )
    {
    string= empty_string;
    _length= 0;
    return;
    }

  string= new char [ __length + 1 ];
  memset( (void*) string, ' ', __length );
  string[__length]= '\0';
  _length= __length;
}

/////////////////////////////////////////////////////////////////////////////

void MLString::set( const char* _string )
{
  if( _string == 0 || _string == empty_string )
    {
    empty_it();
    return;
    }

  set( _string, strlen(_string) );
}

/////////////////////////////////////////////////////////////////////////////

void MLString::set( const MLString& _string )
{
  set( _string.string, _string._length );
}

/////////////////////////////////////////////////////////////////////////////

void MLString::set( const char* _string, int __length )
{
  if( _string == 0 || _string == empty_string || __length <= 0 )
    {
    empty_it();
    return;
    }

  if( *_string == '\0' )
    {
    empty_it();
    return;
    }

  char* new_string= new char [ __length + 1 ];
  memcpy( (void*) new_string, (const void*)_string, __length );
  new_string[__length]= '\0';

  if( string != empty_string )
    delete string;
  string= new_string;
  _length= __length;
}

/////////////////////////////////////////////////////////////////////////////

void MLString::setCharAt( int index, char ch )
{
  if( index < 0 || index >= _length )
    return;

  string[index]= ch;
}

/////////////////////////////////////////////////////////////////////////////

int MLString::hashCode() const
{
  // from gcc-2.8.1/cccp.c

  int r= 0;
  const unsigned char* s= (const unsigned char*)string;
  for(;;)
    {
    int c= int(*(s++));
    if( c == 0 )
      break;
    r= (r<<2) + c;
    }

  return (r & 0x7fffffff); // % hashsize;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLString::startsWith( const MLString& prefix,
                              int offset /* = 0 */ )
{
  if( prefix._length == 0 )
    return true;

  if( offset < 0 )
    return false;

  if( offset >= _length )
    return false;

  return strncmp( string,
                  prefix.string,
                  _length <? prefix._length ) == 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLString::endsWith( const MLString& suffix )
{
  if( suffix._length == 0 )
    return true;

  return startsWith( suffix, _length - 1 - suffix._length );
}

/////////////////////////////////////////////////////////////////////////////

int MLString::indexOf( char ch, int fromIndex /* = 0 */ ) const
{
  if( fromIndex < 0 || fromIndex > _length )
    return -1;

  int i= fromIndex;
  while( string[i] != '\0' && string[i] != ch )
    ++i;

  return (string[i]==ch) ? i : -1;
}

/////////////////////////////////////////////////////////////////////////////

int MLString::lastIndexOf( char ch, int fromIndex ) const
{
  if( fromIndex < 0 || fromIndex > _length )
    return -1;

  int i= fromIndex;
  while( i >= 0 && string[i] != ch )
    --i;

  return i;
}

/////////////////////////////////////////////////////////////////////////////

int MLString::lastIndexOf( char ch ) const
{
  return lastIndexOf(ch,_length-1);
}

/////////////////////////////////////////////////////////////////////////////

int MLString::indexOf( const MLString& str, int fromIndex /* = 0 */ ) const
{
  if( fromIndex < 0 || fromIndex > _length )
    return -1;

  const char* s= strstr( string, str.string );
  if( s == 0 )
    return -1;

  return int(s-string);
}

/////////////////////////////////////////////////////////////////////////////

int MLString::lastIndexOf( const MLString& str, int fromIndex ) const
{
  if( fromIndex < 0 || fromIndex > _length )
    return -1;

  int i= fromIndex;
  while( i >= 0 )
    {
    if( strcmp(string+i,str.string) == 0 )
      return i;
    --i;
    }

  return -1;
}

/////////////////////////////////////////////////////////////////////////////

int MLString::lastIndexOf( const MLString& str ) const
{
  if( str._length == 0 )
    return _length;

  return lastIndexOf( str, _length-1 );
}

/////////////////////////////////////////////////////////////////////////////

MLString MLString::substring( int beginIndex, int endIndex ) const
{
  if( _length == 0 )
    return *this;

  if( beginIndex < 0 )
    beginIndex= 0;
  if( beginIndex >= _length )
    return MLString();

  if( endIndex < 0 )
    return MLString();
  if( endIndex >= _length )
    endIndex= _length-1;

  if( beginIndex > endIndex )
    return MLString();

  return MLString( string, beginIndex, endIndex - beginIndex + 1 );
}

/////////////////////////////////////////////////////////////////////////////

MLString MLString::concat( const MLString& str )
{
  if( str._length == 0 )
    return *this;
  if( _length == 0 )
    return str;

  char* new_string= new char [ _length + str._length + 1 ];
  memcpy( (void*) new_string, (const void*) string, _length );
  memcpy( (void*) (new_string+_length), (const void*) str.string,
          str._length );

  MLString s;
  s._length= _length + str._length;
  s.string= new_string;

  return s;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLString::replace( char oldChar, char newChar ) const
{
  if( oldChar == newChar )
    return *this;

  MLString s(*this);

  int i= s._length - 1;
  while( i >= 0 )
    {
    if( s.string[i] == oldChar )
      s.string[i]= newChar;
    --i;
    }

  return s;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLString::toLowerCase() const
{
  MLString s(*this);

  int i= _length - 1;
  while( i >= 0 )
    {
    char ch= s.string[i];
    if( ch >= 'A' && ch <= 'Z' )
      s.string[i]= ch + ' ';
    --i;
    }

  return s;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLString::toUpperCase() const
{
  MLString s(*this);

  int i= _length - 1;
  while( i >= 0 )
    {
    char ch= s.string[i];
    if( ch >= 'a' && ch <= 'z' )
      s.string[i]= ch - ' ';
    --i;
    }

  return s;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLString::trim() const
{
  int i1= 0;
  while( i1 < _length && string[i1] <= ' ' )
    ++i1;

  int i2= _length - 1;
  while( i2 >= i1 && string[i2] <= ' ' )
    --i2;

  if( i2 < i1 )
    return MLString("");

  return MLString( string, i1, i2-i1+1 );
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLString::valueOf( boolean b )
{
  if( b )
    return MLString("true");

  return MLString("false");
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLString::valueOf( int i )
{
  char tmp[32];
  sprintf( tmp, "%d", i );
  return MLString(tmp);
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLString::valueOf( double d )
{
/*
  if( d == GDouble::POSITIVE_INFINITY.doubleValue() )
    return MLString("Infinity");
  if( d == GDouble::NEGATIVE_INFINITY.doubleValue() )
    return MLString("Infinity");
  if( d == GDouble::GNAN.doubleValue() )
    return MLString("NaN");
*/

  char tmp[32];
  sprintf( tmp, "%g", d );
  return MLString(tmp);
}

/////////////////////////////////////////////////////////////////////////////

void MLString::remove( int i )
{
  if( i < 0 || i >= _length )
    return;

  while( i < _length )
    {
    string[i]= string[i+1];
    ++i;
    }

  --_length;
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// From ImageMagick 3.6.5: magick/utility.c: GlobExpression(...)
//
static boolean glob( const char* expression, const char* pattern )
{
  if( pattern == 0 )
    return true;

  if( strlen(pattern) == 0 )
    return true;
  if( strcmp( pattern, "*" ) == 0 )
    return true;

  boolean done= false;
  while( (*pattern != '\0') && !done )
    {
    if( *expression == '\0' )
      if( (*pattern != '{') && (*pattern != '*') )
        break;
    switch( *pattern )
      {
      case '\\':
        {
        pattern++;
        if( *pattern != '\0' )
          pattern++;
        break;
        }

      case '*':
        {
        pattern++;
        boolean status= false;
        while( (*expression != '\0') && !status )
          status= glob( expression++, pattern );
        if( status )
          {
          while (*expression != '\0')
            expression++;
          while (*pattern != '\0')
            pattern++;
          }
        break;
        }

      case '[':
        {
        pattern++;
        for(;;)
          {
          if( (*pattern == '\0') || (*pattern == ']') )
            {
            done= true;
            break;
            }
          if( *pattern == '\\' )
            {
            pattern++;
            if( *pattern == '\0' )
              {
              done= true;
              break;
              }
            }
          if( *(pattern+1) == '-' )
            {
            char c= *pattern;
            pattern+= 2;
            if( *pattern == ']' )
              {
              done= true;
              break;
              }
            if( *pattern == '\\' )
              {
              pattern++;
              if( *pattern == '\0' )
                {
                done= true;
                break;
                }
              }
            if( (*expression < c) || (*expression > *pattern) )
              {
              pattern++;
              continue;
              }
            }
           else
            if( *pattern != *expression )
              {
              pattern++;
              continue;
              }
          pattern++;
          while( (*pattern != ']') && (*pattern != '\0') )
            {
            if( (*pattern == '\\') && (*(pattern+1) != '\0') )
              pattern++;
            pattern++;
            }
          if( *pattern != '\0' )
            {
            pattern++;
            expression++;
            }
          break;
          } // end for(;;)
        break;
        } // end case '['

      case '?':
        {
        pattern++;
        expression++;
        break;
        }

      case '{':
        {
        pattern++;
        while( (*pattern != '}') && (*pattern != '\0') )
          {
          const char* p= expression;
          boolean match= true;
          while( (*p != '\0') && (*pattern != '\0') &&
                 (*pattern != ',') && (*pattern != '}') && match )
            {
            if( *pattern == '\\' )
              pattern++;
            match= (*pattern == *p);
            p++;
            pattern++;
            }
          if( *pattern == '\0' )
            {
            match= false;
            done= true;
            break;
            }
           else
            if( match )
              {
              expression=p;
              while ((*pattern != '}') && (*pattern != '\0'))
                {
                pattern++;
                if( *pattern == '\\' )
                  {
                  pattern++;
                  if( *pattern == '}' )
                    pattern++;
                  }
                }
              }
             else
              {
              while( (*pattern != '}') && (*pattern != ',') &&
                     (*pattern != '\0') )
                {
                pattern++;
                if( *pattern == '\\' )
                  {
                  pattern++;
                  if( (*pattern == '}') || (*pattern == ',') )
                    pattern++;
                  }
                }
              }
          if( *pattern != '\0' )
            pattern++;
          } // end while
        break;
        } // end case '{'

      default:
        {
        if( *expression != *pattern )
          done= true;
         else
          {
          expression++;
          pattern++;
          }
        }
      } // end switch
    } // end white

  while( *pattern == '*' )
    pattern++;

  return (*expression == '\0') && (*pattern == '\0');
}

/////////////////////////////////////////////////////////////////////////////

boolean MLString::glob( const MLString& pattern ) const
{
  return ::glob( string, pattern.string );
}

/////////////////////////////////////////////////////////////////////////////
