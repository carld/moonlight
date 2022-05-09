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
  MLCommandLine.h

  Unix-like command line: one command + some arguments

  Creation: SR, January 30th, 1996
  Revisions:

*/

#ifndef __MLCommandLine_h
#define __MLCommandLine_h

#ifndef __IList_h
#include "IList.h"
#endif

#ifndef __MLString_h
#include "MLString.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLCommandLine
{
protected:
  struct Argument
    {
    Argument( const MLString& _arg )
      {
      arg= _arg;
      used= false;
      option= (arg.length()>0) ? (arg[0]=='-') : false;
      }
    Argument( const char* _arg )
      {
      arg= (_arg==0) ? "" : _arg;
      used= false;
      option= (arg.length()>0) ? (arg[0]=='-') : false;
      }
    MLString arg;
    boolean used, option;
    };

  MLString command;
  IList<Argument> arguments;
  IListIterator<Argument> li;
  int error;

public:
  MLCommandLine( const char* fcl ): li(arguments)
    {
    init(fcl);
    }
  MLCommandLine( const MLString& fcl ): li(arguments)
    {
    init(fcl.get());
    }
  MLCommandLine( int argc, const char* argv[] ): li(arguments)
    {
    init( argc, argv );
    }

  virtual ~MLCommandLine()
    {
    destroy();
    }

  void destroy();

  // fullMLCommandLineLine may be 0
  void init( const char* fullMLCommandLineLine );
  void init( int argc, const char* argv[] );

  void addArgument( const MLString& arg );
  void addArgument( const char* arg );

  void reset();

// parsing
  const MLString& get() const
    {
    return command;
    }

  int getNArguments() const
    {
    return 1 + arguments.getNElements();
    }
  int argc() const
    {
    return getNArguments();
    }
  const MLString& argv( int i ) const;

  boolean getError() const
    {
    return error != 0;
    }

  int getInteger();
  double getDouble();
  const MLString& getString();
  int getIntegerAfter( const char* option );
  double getDoubleAfter( const char* option );
  const MLString& getStringAfter( const char* option );

private:
  Argument* getFirstArgument();
  Argument* getNextArgument();

public:
  const MLString& getFirst();
  const MLString& getNext();

  boolean findOption( const MLString& option );
  boolean findOption( const char* s );
  boolean findOption( char ch );

  //
  // Example: -I/foo/tools
  // -> then getOptionString( 'I' ) returns "/foo/tools"
  // return 0 if not found
  //
  const char* findOptionString( char ch );

  const char* getFirstUnusedOption();

private:
  const char* skipSpaces( const char* ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCommandLine_h

