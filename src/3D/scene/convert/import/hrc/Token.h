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
  Token.h

  CTL
  Stephane Rehel, January 22nd, 1996
*/

#ifndef __Token_h
#define __Token_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __DLIST_H
#include "tools/DList.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class TOKENS;

class Token: public DListElement<Token,TOKENS>
{
public:
  typedef int Keyword;

  enum Type
       {
       SCALAR, // ex: 1, 1.6, 1.4e8
       STRING, // ex: "toto"
       CHAR, // ex: ,.;:[]{}()<<*+...
       NAME, // ex: function, toto
       KEYWORD, // ex: double, for, ...
       ERROR // ex: 'parse error before...'
       };

  enum Char
       {
       null_char= 0     ,
       dot='.'          ,
       comma=','        ,
       semicolon=';'    ,
       colon=':'        ,
       pound='#'        ,
       dollar='$'       ,
       percent='%'      ,
       ampersand='&'    ,
       quote='\''       ,
       o_paren='('      ,
       c_paren=')'      ,
       o_brace='{'      ,
       c_brace='}'      ,
       o_bracket='['    ,
       c_bracket=']'    ,
       star='*'         ,
       plus='+'         ,
       minus='-'        ,
       slash='/'        ,
       o_angle='<'      ,
       c_angle='>'      ,
       equal='='        ,
       query='?'        ,
       at='@'           ,
       bang='!'         ,
       double_quote='"' ,
       backslash='\\'   ,
       hat='^'          ,
       underline='_'    ,
       bar='|'          ,
       tilde='~'        ,
       backquote='`'    ,
       arrow=1000       , // '->'
       ampersand2       , // '&&'
       bar2             , // '||'
       equal2           , // '=='
       star2            , // '**'
       o_angle2         , // '<<'
       c_angle2         , // '>>'
       backet2          , // '[]'
       plus2            , // '++'
       minus2           , // '--'
       plus_equal       , // '+='
       minus_equal      , // '-='
       star_equal       , // '*='
       slash_equal      , // '/='
       ampersand_equal  , // '&='
       bar_equal        , // '|='
       slash_star       , // '/*'
       star_slash       , // '*/'
       slash2             // '//'
       };

protected:
  Type theType;

  double theScalar;
  MLString theString;
  Char theChar;
  Keyword theKeyword;

  const char* filename;
  int line;

public:
  Token( double d, const char* _filename, int _line ):
    theType(SCALAR), theScalar(d),
    filename(_filename),
    line(_line)
    {}

  Token( Keyword kw, const char* _filename, int _line ):
    theType(KEYWORD), theKeyword(kw),
    filename(_filename),
    line(_line)
    {}

  Token( Char ch, const char* _filename, int _line ):
    theType(CHAR), theChar(ch),
    filename(_filename),
    line(_line)
    {}

  Token( Type t, const char* s, const char* _filename, int _line ):
    theType(t), theString(s),
    filename(_filename),
    line(_line)
    {}

  Token( Type t, const MLString& s, const char* _filename, int _line ):
    theType(t), theString(s),
    filename(_filename),
    line(_line)
    {}

  virtual ~Token()
    {}

  Type getType() const
    {
    return theType;
    }

  double getScalar() const
    {
    assert( theType == SCALAR );
    return theScalar;
    }

  const MLString& getString() const
    {
    assert( theType == STRING );
    return theString;
    }

  Char getChar() const
    {
    assert( theType == CHAR );
    return theChar;
    }

  Keyword getKeyword() const
    {
    assert( theType == KEYWORD );
    return theKeyword;
    }

  const MLString& getName() const
    {
    assert( theType == NAME );
    return theString;
    }

  const MLString& getError() const
    {
    assert( theType == ERROR );
    return theString;
    }

  const char* getFilename() const
    {
    assert( filename != 0 );
    return filename;
    }

  int getLine() const
    {
    return line;
    }

private:
  static boolean* charsArray;
  static void initCharsArray();

public:
  // return null_char if ch is not a token char
  static Char isChar( int ch )
    {
    if( charsArray == 0 )
      Token::initCharsArray();
    if( ch < 0 || ch >= 128 )
      return null_char;
    return charsArray[ch] ? Char(ch) : null_char;
    }

  // can merge 'tc1 + tc2' into a new token char ??
  // return null_char if not
  static Char canMerge( Char tc1, Char tc );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Token_h
