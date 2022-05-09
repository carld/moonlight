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
  MLCommandLine.cc

  Unix-like command line: one command + some arguments

  Creation: SR, January 30th, 1996
  Revisions:

*/

#include <stdio.h>

#include "MLCommandLine.h"


/////////////////////////////////////////////////////////////////////////////

void MLCommandLine::destroy()
{
  while( ! arguments.empty() )
    {
    delete arguments.getFirst();
    arguments.removeFirst();
    }
}

/////////////////////////////////////////////////////////////////////////////

// private
const char* MLCommandLine::skipSpaces( const char* s ) const
{
  if( s == 0 )
    return 0;

  while( *s == ' ' || *s == '\t' )
    ++s;

  return s;
}

/////////////////////////////////////////////////////////////////////////////

void MLCommandLine::addArgument( const MLString& arg )
{
  addArgument( arg.get() );
}

/////////////////////////////////////////////////////////////////////////////

void MLCommandLine::addArgument( const char* arg )
{
  arguments.append( new Argument(arg) );
}

/////////////////////////////////////////////////////////////////////////////

void MLCommandLine::init( const char* fullMLCommandLineLine )
{
  destroy();
  command= "";

  if( fullMLCommandLineLine == 0 )
    return;

  register const char* fcl= fullMLCommandLineLine;

  for(;;)
    {
    fcl= skipSpaces(fcl);
    if( *fcl == '\0' )
      break;
    register int length= 0;
    while( fcl[length] != '\0' && fcl[length] != ' ' && fcl[length] != '\t' )
      ++length;
    assert( length > 0 );
    addArgument( MLString( fcl, 0, length ) );
    fcl += length;
    }

  if( arguments.getNElements() == 0 )
    return;

  command= arguments.getFirst()->arg;
  delete arguments.getFirst();
  arguments.removeFirst();

  error= 0;
  li.reset();
}

/////////////////////////////////////////////////////////////////////////////

void MLCommandLine::init( int argc, const char* argv[] )
{
  destroy();
  command= "";
  if( argc < 1 || argv == 0 )
    return;

  for( int i= 0; i < argc; ++i )
    {
    const char* s= argv[i];
    if( s == 0 )
      continue;

    addArgument(s);
    }

  if( arguments.getNElements() == 0 )
    return;

  command= arguments.getFirst()->arg;
  delete arguments.getFirst();
  arguments.removeFirst();

  error= 0;
  li.reset();
}

/////////////////////////////////////////////////////////////////////////////

void MLCommandLine::reset()
{
  li.reset();
  while( ! li.eol() )
    li.next()->used= false;

  li.reset();
  error= 0;
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLCommandLine::argv( int i ) const
{
  assert( i >= 0 );
  assert( i < argc() );

  if( i == 0 )
    return command;

  return arguments.get(i)->arg;
}

/////////////////////////////////////////////////////////////////////////////

int MLCommandLine::getInteger()
{
  if( li.eol() )
    {
    error= 1;
    return 0;
    }

  Argument* a= li.next();
  if( a->used )
    {
    error= 1;
    return 0;
    }

  int i;
  if( sscanf( a->arg.get(), "%d", &i ) != 1 )
    {
    error= 1;
    return 0;
    }

  a->used= true;

  return i;
}

/////////////////////////////////////////////////////////////////////////////

double MLCommandLine::getDouble()
{
  if( li.eol() )
    {
    error= 1;
    return 0.;
    }

  Argument* a= li.next();
  if( a->used )
    {
    error= 1;
    return 0.;
    }

  double d;
  if( sscanf( a->arg.get(), "%lf", &d ) != 1 )
    {
    error= 1;
    return 0.;
    }

  a->used= true;

  return d;
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLCommandLine::getString()
{
  static MLString foo("");

  if( li.eol() )
    {
    error= 1;
    return foo;
    }

  Argument* a= li.next();
  if( a->used )
    {
    error= 1;
    return foo;
    }

  a->used= true;

  return a->arg;
}

/////////////////////////////////////////////////////////////////////////////

int MLCommandLine::getIntegerAfter( const char* option )
{
  if( ! findOption(option) )
    {
    error= 1;
    return 0;
    }
  return getInteger();
}

/////////////////////////////////////////////////////////////////////////////

double MLCommandLine::getDoubleAfter( const char* option )
{
  if( ! findOption(option) )
    {
    error= 1;
    return 0.;
    }
  return getDouble();
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLCommandLine::getStringAfter( const char* option )
{
  if( ! findOption(option) )
    {
    error= 1;
    static MLString foo("");
    return foo;
    }
  return getString();
}

/////////////////////////////////////////////////////////////////////////////

// private
MLCommandLine::Argument* MLCommandLine::getFirstArgument()
{
  li.reset();
  return getNextArgument();
}

/////////////////////////////////////////////////////////////////////////////

// private
MLCommandLine::Argument* MLCommandLine::getNextArgument()
{
  for(;;)
    {
    if( li.eol() )
      break;
    Argument* a= li.next();
    if( ! a->used )
      return a;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLCommandLine::getFirst()
{
  Argument* a= getFirstArgument();

  if( a == 0 )
    {
    static MLString foo("");
    return foo;
    }
  a->used= true;
  return a->arg;
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLCommandLine::getNext()
{
  Argument* a= getNextArgument();

  if( a == 0 )
    {
    static MLString foo("");
    return foo;
    }
  a->used= true;
  return a->arg;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCommandLine::findOption( const MLString& option )
{
  const MLString o= (option.charAt(0)=='-') ? option
                                            : (MLString("-") + option);

  Argument* a= getFirstArgument();
  while( a != 0 )
    {
    if( !a->used && a->option && a->arg.length() >= 2 )
      {
      if( a->arg == o )
        {
        a->used= true;
        return true;
        }
      }

    a= getNextArgument();
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCommandLine::findOption( const char* s )
{
  return findOption( MLString(s) );
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCommandLine::findOption( char ch )
{
  char s[2];
  s[0]= ch;
  s[1]= '\0';

  return findOption( MLString(s) );
}

/////////////////////////////////////////////////////////////////////////////

//
// Example: -I/foo/tools
// -> then getOptionString( 'I' ) returns "/foo/tools"
// return 0 if not found
//
const char* MLCommandLine::findOptionString( char ch )
{
  Argument* a= getFirstArgument();

  while( a != 0 )
    {
    if( !a->used && a->option && a->arg.length() >= 2 )
      {
      if( a->arg[1] == ch )
        {
        a->used= true;
        return a->arg.get() + 2;
        }
      }

    a= getNextArgument();
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

const char* MLCommandLine::getFirstUnusedOption()
{
  Argument* a= getFirstArgument();

  while( a != 0 )
    {
    if( !a->used && a->option && a->arg.length() >= 2 )
      return a->arg.get();

    a= getNextArgument();
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

