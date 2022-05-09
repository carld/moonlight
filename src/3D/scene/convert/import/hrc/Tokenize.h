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
  Tokenize.h

  CTL
  Stephane Rehel, January 22nd, 1996
*/

#ifndef __Tokenize_h
#define __Tokenize_h

#ifndef __IStack_h
#include "tools/IStack.h"
#endif

#ifndef __Token_h
#include "Token.h"
#endif

#ifndef __InputStream_h
#include "InputStream.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class Tokenize
{
public:
  static int maxIdentifierLength;
  static int maxErrors;

protected:
  IList<MLString> usedFilenames;

  const IList<MLString>* includePaths; // not owner

  IStack<InputStream> streams;
  IStack<MLString> filenames;

  InputStream* currentStream; // speedup

  DList<Token,TOKENS> tokens;

  const char* errorFilename;
  int errorLine;
  MLString errorString;

  int nErrors;

public:

  Tokenize();

  virtual ~Tokenize();

  DList<Token,TOKENS>& getTokens()
    {
    return tokens;
    }

  const MLString& getCurrentFilename() const
    {
    assert( ! filenames.isEmpty() );
    return *filenames.peek();
    }

  boolean isDigitChar( int ch ) const
    {
    return ch >= int('0') && ch <= int('9');
    }

  boolean isIdentifierChar( int ch ) const
    {
    return isDigitChar(ch) ||
            ch=='_' ||
           (ch>=int('A') && ch<=int('Z')) ||
           (ch>=int('a') && ch<=int('z'));
    }

  int getNErrors() const
    {
    return nErrors;
    }

  void openInputString( const char* input_string );
  void openInputFile( FILE* file, const char* _filename = 0 );
  boolean openInputFile( const char* _filename );
  void closeCurrentStream();
  int getChar();
  MLString* getString();
  void skipLineComment();
  boolean skipComment();
  boolean getScalar( double& scalar, boolean& isInteger );
  boolean getName( char* word, int maxLength );
  boolean tokenize( int maxTokens = -1 );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Tokenize_h

