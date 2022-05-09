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
  MLLog.C

  Stephane Rehel

  May 26 1998
*/

#include <stdio.h>
#include <stdarg.h>

#include "MLString.h"

#include "MLLog.h"
#include "MLLogListener.h"

/////////////////////////////////////////////////////////////////////////////

// static
const int MLLog::bufsize= 1024;
const int MLLog::history_size= 1024;

/////////////////////////////////////////////////////////////////////////////

MLLog::MLLog( MLLogListener* ll )
{
  logListener= ll;
  buffer= new char [ bufsize + 1 ];

  current= "";
  touched_since_last_flush= false;
}

/////////////////////////////////////////////////////////////////////////////

MLLog::~MLLog()
{
  while( ! log.empty() )
    {
    MLString* s= log.getFirst();
    log.removeFirst();
    delete s;
    }

  delete buffer;
  buffer= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLLog::setListener( MLLogListener* ll )
{
  logListener= ll;
}

/////////////////////////////////////////////////////////////////////////////

MLLogListener* MLLog::getListener() const
{
  return logListener;
}

/////////////////////////////////////////////////////////////////////////////

// text has no '\n' in it
void MLLog::_print( const char* text, int length )
{
  if( text == 0 || length <= 0 )
    return;

  current += MLString(text,0,length);
}

/////////////////////////////////////////////////////////////////////////////

void MLLog::println()
{
  log.append( new MLString(current) );
  if( log.getNElements() > MLLog::history_size )
    {
    delete log.getFirst();
    log.removeFirst();
    }

  touched_since_last_flush= true;

  if( logListener != 0 )
    logListener->newLog(current);

  current= "";
}

/////////////////////////////////////////////////////////////////////////////

void MLLog::print( const char* msg )
{
  if( msg == 0 )
    return;

  const char* s= msg;
  int length= 0;
  while( s[length] != '\0' )
    {
    if( s[length] == '\n' )
      {
      _print( s, length );
      println();
      s+= length+1;
      length= 0;
      }
     else
      ++length;
    }

  if( length != 0 )
    _print( s, length );
}

/////////////////////////////////////////////////////////////////////////////

void MLLog::print( const MLString& msg )
{
  return print(msg.get());
}

/////////////////////////////////////////////////////////////////////////////

void MLLog::printf( const char* format, ... )
{
  if( format == 0 )
    return;

  va_list args;

  va_start( args, format );
  print( MLString::vprintf( format, args ) );
  va_end( args );
}

/////////////////////////////////////////////////////////////////////////////

MLLog& MLLog::operator << ( const char* msg )
{
  print(msg);
  return *this;
}

/////////////////////////////////////////////////////////////////////////////

MLLog& MLLog::operator << ( const MLString& msg )
{
  print(msg);
  return *this;
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLLog::getLastMessage() const
{
  if( log.empty() )
    return MLString::empty();

  return *(log.getLast());
}

/////////////////////////////////////////////////////////////////////////////

void MLLog::flush()
{
  if( ! touched_since_last_flush )
    return;

  touched_since_last_flush= false;
  if( logListener != 0 )
    logListener->flush();
}

/////////////////////////////////////////////////////////////////////////////
