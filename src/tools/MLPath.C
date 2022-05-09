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
  MLPath.C

  Stephane Rehel
  April 15 1997
*/

#include <stdlib.h>
#include <unistd.h>

#include "MLPath.h"
#include "MLFilename.h"
#include "MLFileEntry.h"

/////////////////////////////////////////////////////////////////////////////

#define unixSeparator '/'
#define  dosSeparator '\\'

// static
const char MLPath::separator= unixSeparator;

static const char* currentPath= "./";

/////////////////////////////////////////////////////////////////////////////

MLPath::MLPath()
{
  path= currentPath;
}

/////////////////////////////////////////////////////////////////////////////

MLPath::MLPath( const MLPath& p )
{
  path= p.path;
}

/////////////////////////////////////////////////////////////////////////////

MLPath::MLPath( const MLString& fullname )
{
  operator = ( fullname );
}

/////////////////////////////////////////////////////////////////////////////

MLPath& MLPath::operator = ( const MLString& _fullname )
{
  MLString fullname= _fullname;

  path= fullname.trim();

  path= MLPath::toUnixPathSeparators(path);

  if( path.length() == 0 )
    path= currentPath;

  if( path == currentPath )
    return *this;

  fullname= MLFilename::expand(fullname.get());

  if( path.length() > 1 )
    {
    if( path.charAt( path.length() - 1 ) == '.' )
      path.remove(path.length() - 1);
    }

  // remove multiple '/'
  int i;
  for( i= 0; i < int(path.length()-1); ++i )
    {
    if( path.charAt(i) == separator && path.charAt(i+1) == separator )
#ifdef __WINDOWS__
      if( i > 0 ) // allow "//D/ml/database" for cygwin
#endif
        path.remove(i);
    }

  if( path.charAt( path.length() - 1 ) != separator )
    path += separator;

  // replace "/./" by "/"
  for( i= 0; i < int(path.length()-2); ++i )
    {
    if( path.charAt(i) == separator &&
        path.charAt(i+1) == '.' &&
        path.charAt(i+2) == separator )
      {
      path.remove(i);
      path.remove(i);
      }
    }

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPath::isAbsolute() const
{
  if( path.charAt(0) == separator )
    return true;

  if( path.length() < 2 )
    return false;

#ifdef __WINDOWS__
  if( path[1] == ':' )
    return true;
#endif

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPath::isCurrent() const
{
  return path == currentPath;
}

/////////////////////////////////////////////////////////////////////////////

// if error, return FALSE
boolean MLPath::operator == ( const MLPath& path2 ) const
{
  if( &path2 == this )
    return true;
  if( get() == path2.get() )
    return true;

  return MLFileEntry::sameFile( get(), path2.get() );
}

/////////////////////////////////////////////////////////////////////////////

MLPath& MLPath::operator += ( const MLString& p )
{
  if( p == "" || p == "." || p == currentPath )
    return *this;

  MLString pp= p;
  if( pp.charAt(0) == '.' )
    pp.remove(0);

  if( pp.charAt(0) == MLPath::separator )
    pp.remove(0);

  return operator = ( path + p );
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLPath::checkPathSeparators( const MLString& filename )
{
  MLString f= filename;

  char replace=(separator==unixSeparator) ? dosSeparator : unixSeparator;

  for( int i= 0; i < f.length(); ++i )
    if( f.charAt(i) == replace )
      f.getBytes()[i]= separator;

  return f;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLPath::toUnixPathSeparators( const MLString& filename )
{
  MLString f= filename;

  for( int i= 0; i < f.length(); ++i )
    if( f.charAt(i) == dosSeparator )
      f.getBytes()[i]= unixSeparator;

  return f;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLPath MLPath::getHomeDirectory()
{
  const char* home= getenv("HOME");
  if( home == 0 )
    {
problem:
    // return "/"
    char s[2];
    s[0]= MLPath::separator;
    s[1]= '\0';
    return MLPath(s);
    }

  if( home[0] == '\0' )
    goto problem;

  return MLPath(home);
}

/////////////////////////////////////////////////////////////////////////////

// static
MLPath MLPath::getCurrentDirectory()
{
  char d[512+1];
  getcwd( d, 512 );

  if( d[0] == '\0' )
    {
    // Problem: return "/"
    char s[2];
    s[0]= MLPath::separator;
    s[1]= '\0';
    return MLPath(s);
    }

  return MLPath(d);
}

/////////////////////////////////////////////////////////////////////////////
