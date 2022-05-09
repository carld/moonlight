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
  MLLog.h

  Stephane Rehel

  May 26 1998
*/

#ifndef __MLLog_h
#define __MLLog_h

#ifndef __IList_h
#include "IList.h"
#endif

#ifndef __MLString_h
#include "MLString.h"
#endif

class MLLogListener;

/////////////////////////////////////////////////////////////////////////////

class MLLog
{
private:
  static const int history_size;
  static const int bufsize;
  char* buffer;

protected:
  IList<MLString> log;
  MLLogListener* logListener;

  MLString current;
  boolean touched_since_last_flush;

public:
  MLLog( MLLogListener* ll );

  virtual ~MLLog();

  void setListener( MLLogListener* ll );

  MLLogListener* getListener() const;

  const MLString& getCurrentLine() const
    {
    return current;
    }

protected:
  void _print( const char* text, int length );

public:
  void println();
  void println( const MLString& msg );
  void print( const MLString& msg );

  void println( const char* format );
  void print( const char* format );

  void printf( const char* format, ... );

  MLLog& operator << ( const char* msg );

  MLLog& operator << ( const MLString& msg );

  // doesn't return the current line!
  const MLString& getLastMessage() const;

  void flush();

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLLog_h
