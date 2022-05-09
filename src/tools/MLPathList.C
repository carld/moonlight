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
  MLPathList.C

  Stephane Rehel

  March 7 1998

  For instance: ".:/home/foo:/usr/:" or ".;/home/foo;/usr/;"
*/

#include "MLPathList.h"
#include "MLPath.h"
#include "MLFilename.h"

#include "MLFileEntry.h"

/////////////////////////////////////////////////////////////////////////////

MLPathList::MLPathList()
{}

/////////////////////////////////////////////////////////////////////////////

MLPathList::MLPathList( const MLString& string )
{
  set(string);
}

/////////////////////////////////////////////////////////////////////////////

MLPathList::~MLPathList()
{
  SIList_destroyElements(MLPath,list);
}

/////////////////////////////////////////////////////////////////////////////

void MLPathList::set( const MLString& string )
{
  int i1= 0;

  for(;;)
    {
    int i2= i1;
    while( string.charAt(i2) != '\0' &&
#ifndef __WINDOWS__
           string.charAt(i2) != ':' &&
#endif
           string.charAt(i2) != ';' 
         )
      ++i2;

    if( i2 == i1 )
      {
      if( string.charAt(i2) == '\0' )
        break;
      ++i1;
      continue;
      }

    MLString p= string.substring(i1,i2-1);
    for( int i= 0; i < p.length(); ++i )
      if( p[i] == '\t' )
        p[i]= ' ';

    p= p.trim();

    if( p.length() != 0 )
      {
      MLPath path( MLFilename::expand(p.get(),false) );
      append(path);
      }

    i1= i2 + 1;
    }
}

/////////////////////////////////////////////////////////////////////////////

MLPath* MLPathList::find( const MLPath& p )
{
  SIListIterator<MLPath> li(list);
  while( ! li.eol() )
    {
    MLPath* p0= li++;
    if( *p0 == p )
      return p0;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLPathList::append( const MLPath& path )
{
  if( find(path) == 0 )
    list.append( new MLPath(path) );
}

/////////////////////////////////////////////////////////////////////////////

// subsitute home directory by ~
MLString MLPathList::get() const
{
  MLString home= MLPath::getHomeDirectory().get();
  if( home.length() < 2 )
    home= "";
   else
    {
    // remove trailing '/'
    if( home.charAt(home.length()-1) == MLPath::separator )
      home= home.substring( 0, home.length() - 2 );
    }

  MLString string= "";

  boolean first= true;

  ConstSIListIterator<MLPath> li(list);
  while( ! li.eol() )
    {
    const MLPath* p= li++;
    MLString s= p->get();

    if( home.length() != 0 )
      {
      if( s.substring(0,home.length()-1) == home )
        {
        MLString ss("~");
        ss+= MLPath::separator;
        if( s[home.length()] == MLPath::separator )
          s= ss + s.substring(home.length()+1,s.length()-1);
         else
          s= ss + s.substring(home.length(),s.length()-1);
        }
      }

    if( s.length() > 1 )
      {
      if( s[ s.length() - 1 ] == MLPath::separator )
        s= s.substring( 0, s.length() - 2 ); // remove the last '/'
      }

    if( ! first )
      string += ':';
    string += s;
    first= false;
    }

  return string;
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if unable to find file in path list
MLPath* MLPathList::findFile( const MLString& filename )
{
  SIListIterator<MLPath> li(list);
  while( ! li.eol() )
    {
    MLPath* p= li++;

    MLString fn= p->get() + filename;
    if( MLFileEntry(fn).exists() )
      return p;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

int MLPathList::getNPaths() const
{
  return list.getNElements();
}

/////////////////////////////////////////////////////////////////////////////

// i >= 1
MLPath* MLPathList::getPath( int i )
{
  if( i < 1 || i > getNPaths() )
    return 0;

  return list.get(i);
}

/////////////////////////////////////////////////////////////////////////////
