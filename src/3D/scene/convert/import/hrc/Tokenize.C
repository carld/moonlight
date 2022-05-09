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
  Tokenize.C

  CTL
  Stephane Rehel, January 22nd, 1996
*/

#include <stdio.h>
#include <math.h>

#include "Tokenize.h"

/////////////////////////////////////////////////////////////////////////////

// static
int Tokenize::maxIdentifierLength= 64;
int Tokenize::maxErrors= 25;

/////////////////////////////////////////////////////////////////////////////

Tokenize::Tokenize()
{
  includePaths= 0;
  currentStream= 0;
  errorString= "";
  nErrors= 0;
}

/////////////////////////////////////////////////////////////////////////////

Tokenize::~Tokenize()
{
  includePaths= 0;
  currentStream= 0;

  while( ! streams.isEmpty() )
    {
    InputStream* is= streams.pop();
    is->close();
    delete is;
    }

  tokens.destroyElements();

  while( ! usedFilenames.empty() )
    {
    MLString* first= usedFilenames.getFirst();
    usedFilenames.removeFirst();
    delete first;
    }
}

/////////////////////////////////////////////////////////////////////////////

void Tokenize::openInputString( const char* input_string )
{
  assert( input_string != 0 );

  MLString* filename= new MLString("string_input");
  usedFilenames.append(filename);
  filenames.push(filename);

  InputStream* is= new InputStream(input_string);
  streams.push(is);

  currentStream= is;
}

/////////////////////////////////////////////////////////////////////////////

void Tokenize::openInputFile( FILE* file, const char* _filename /* = 0 */ )
{
  assert( file != 0 );

  if( _filename == 0 )
    _filename= "input_stream";

  MLString* filename= new MLString(_filename);

  usedFilenames.append(filename);
  filenames.push(filename);

  InputStream* is= new InputStream(file);
  streams.push(is);

  currentStream= is;
}

/////////////////////////////////////////////////////////////////////////////

boolean Tokenize::openInputFile( const char* _filename )
{
  assert( _filename != 0 );

  MLString base_filename= MLString(_filename);
  MLString* filename= 0;

  FILE* file= fopen( base_filename.get(), "rt" );

  if( file == 0 )
    {
    if( includePaths == 0 )
      return false;

    ConstIListIterator<MLString> li(includePaths);
    while( ! li.eol( ) )
      {
      const MLString& path= *li.next();
      MLString efilename;
      if( path.charAt(path.length()-1) != '/' )
        efilename= path + MLString("/") + base_filename;
       else
        efilename= path + base_filename;
      file= fopen( efilename.get(), "rt" );
      if( file != 0 )
        {
        filename= new MLString(efilename);
        break;
        }
      }
    }
   else
    filename= new MLString(base_filename);

  if( filename == 0 )
    {
    if( currentStream != 0 )
      {
      errorLine= currentStream->getLine();
      errorFilename= getCurrentFilename().get();
      }
     else
      {
      errorLine= 0;
      errorFilename= "";
      }
    errorString= MLString("unable to open file `") + base_filename
                +MLString("'");
    return false;
    }

  openInputFile( file, filename->get() );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void Tokenize::closeCurrentStream()
{
  assert( currentStream != 0 );
  assert( ! streams.isEmpty() );
  assert( currentStream == streams.peek() );

  currentStream->close();
  delete streams.pop();

  filenames.pop(); // dont delete char[], it's used by token, and
                   // kept into usedFilenames

  currentStream= streams.isEmpty() ? 0 : streams.peek();
}

/////////////////////////////////////////////////////////////////////////////

// return -1 if end of the whole stream
int Tokenize::getChar()
{
  assert( currentStream != 0 );

  static boolean space_after_eos= false;

again:
  int ch= currentStream->get();

  if( ch == int('\n') )
    ch= int(' ');
   else
    {
    if( ch == -1 )
      {
      if( ! space_after_eos )
        {
        space_after_eos= true;
        return int(' ');
        }
      space_after_eos= false;
      closeCurrentStream();
      if( currentStream == 0 )
        return -1; // end of the whole stream

      goto again; // current stream has changed
      }
    }

  return ch;
}

/////////////////////////////////////////////////////////////////////////////

// we just got a '"' char, so get the string
// return 0 if error, and set error string
MLString* Tokenize::getString()
{
  assert( currentStream != 0 );

  MLString* string= new MLString("");

  int startLine= currentStream->getLine();

  for(;;)
    {
    int ch= currentStream->get();
    if( ch == -1 )
      {
      delete string;
      errorLine= startLine;
      errorFilename= getCurrentFilename().get();
      errorString= MLString("unterminated string constant");
      return 0;
      }
    if( ch == '"' )
      break;
    *string+= char(ch);
    }

  return string;
}

/////////////////////////////////////////////////////////////////////////////

// we just got a '//' token: skip this line, it's a comment
void Tokenize::skipLineComment()
{
  assert( currentStream != 0 );

  for(;;)
    {
    int ch= currentStream->get();

    if( ch == -1 )
      break;
    if( ch == int('\n') )
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

// we just got a '/*' token: find out a '*/'
// return false if unterminated comment
// Careful: doesn't support nested comments
boolean Tokenize::skipComment()
{
  assert( currentStream != 0 );

  int startLine= currentStream->getLine();

  int state= 0; // == 0 if nothing
                // == 1 if just got a '*'
  for(;;)
    {
    int ch= currentStream->get();

    if( ch == -1 )
      {
      errorLine= startLine;
      errorFilename= getCurrentFilename().get();
      errorString= MLString("unterminated comment");
      return false;
      }

    if( ch == '*' )
      {
      state= 1;
      continue;
      }
    if( ch == '/' && state == 1 )
      {
      // woo, got it!
      break;
      }
    state= 0;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// get a scalar: 3, 3., 3.2, .3, 1e-3, 1E+3, +2.4, -1.4
//
// returns false if error and set error string
//
boolean Tokenize::getScalar( double& scalar, boolean& isInteger )
{
  assert( currentStream != 0 );

  double d= 0.;
  boolean afterOneChar= false;
  boolean afterPoint= false;
  boolean afterSign= false;
  boolean negative= false;
  int exponent= 0;
  double point= 0.;
  boolean afterExponent= false;
  boolean afterExponentSign= false;
  boolean exponentNegative= false;
  boolean afterOneExponentChar= false;

  isInteger= true;
  int startLine= currentStream->getLine();

  int ch;
  for(;;)
    {
    ch= currentStream->get();
    if( ch == int('.') )
      {
      if( afterPoint || afterExponent )
        goto error;
      point= 0.1;
      afterPoint= true;
      isInteger= false;
      continue;
      }
    if( ch == int('+') )
      {
      if( afterExponent )
        {
        if( afterExponentSign )
          goto error;
        afterExponentSign= true;
        }
       else
        {
        if( afterOneChar )
          goto ParsingEnd;
        if( afterSign )
          goto error;
        afterSign= true;
        }
      continue;
      }
    if( ch == int('-') )
      {
      if( afterExponent )
        {
        if( afterExponentSign )
          goto error;
        afterExponentSign= true;
        exponentNegative= true;
        }
       else
        {
        if( afterOneChar )
          goto ParsingEnd;
        if( afterSign )
          goto error;
        negative= true;
        afterSign= true;
        }
      continue;
      }
    if( (ch == int('E') || ch == int('e')) && !afterExponent )
      {
      if( !afterOneChar )
        goto error;
      afterExponent= true;
      isInteger= false;
      continue;
      }
    if( ch >= int('0') && ch <= int('9') )
      {
      if( afterExponent )
        {
        if( exponent <= 300 )
          exponent= exponent*10 + int( ch - int('0') );
        afterOneExponentChar= true;
        }
       else
        {
        if( afterPoint )
          {
          d+= point * double( ch - int('0') );
          point /= 10.;
          }
         else
          d= d*10. + double( ch - int('0') );
        afterOneChar= true;
        }
      continue;
      }
    if( !afterOneChar || (!afterOneExponentChar && afterExponent) )
      goto error;
ParsingEnd:
    currentStream->backward();
    break;
error:
    currentStream->backward();
    errorLine= startLine;
    errorFilename= getCurrentFilename().get();
    errorString= MLString("parse error in scalar");
    return false;
    }

  if( exponent != 0 )
    {
    if( exponent > 300 )
      exponent= 300;
    if( exponentNegative )
      d/= pow( 10., double(exponent) );
     else
      d*= pow( 10., double(exponent) );
    }

  if( negative )
    d= -d;

  scalar= d;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

// get a name = { '0'..'9', 'a'..'z', 'A'..'Z', '_' }
// Doesn't begin with a digit
//
// return false if error and set error string
//
boolean Tokenize::getName( char* word, int maxLength )
{
  assert( currentStream != 0 );
  assert( word != 0 );
  assert( maxLength > 0 );

  int startLine= currentStream->getLine();

  int i= 0;
  for(;;)
    {
    int ch= currentStream->get();

    if( !isIdentifierChar(ch) || (i==0 && isDigitChar(ch)) )
      {
      currentStream->backward();
      word[i]= 0;

      if( i == 0 )
        {
        errorLine= startLine;
        errorFilename= getCurrentFilename().get();
        errorString= MLString("identifier expected");
        return false;
        }

      return true;
      }

// put a warning: identifier is too long?
    if( i < maxLength )
      word[i++]= (char) ch;
    }
}

/////////////////////////////////////////////////////////////////////////////

// return false if found errors or too many errors
boolean Tokenize::tokenize( int maxTokens /* = -1 */ )
{
  assert( currentStream != 0 );

  static boolean gotSpace= false;
  Token* t= 0;
  Token::Char tc;
  int nTokens= 0;

  for(;;)
    {
    if( nErrors >= maxErrors )
      return false;

    int line= currentStream->getLine();
    int ch= getChar();

    if( ch == ' ' )
      {
      gotSpace= true;
      continue;
      }

    if( ch == -1 )
      {
      // ok, eos
      return nErrors == 0;
      }

    assert( currentStream != 0 );

    if( ch == int('.') )
      {
      // may be a scalar?
      int ch2= currentStream->get();
      if( isDigitChar(ch2) )
        {
        // case: .434356
        currentStream->backward(); // restore ch2
        currentStream->backward(); // restore '.'

        goto get_scalar;
        }

      currentStream->backward();
      }

    if( ch == int('"') )
      {
      MLString* string= getString();
      if( string == 0 )
        goto got_error;

      t= new Token( Token::STRING, *string, getCurrentFilename().get(), line );
      goto got_it;
      }

    tc= Token::isChar(ch);
    if( tc != Token::null_char )
      {
      if( ! gotSpace && tokens.getNElements() > 0 )
        {
        // try to merge it with previous token
        Token* previousToken= tokens.getLast();

        if( previousToken->getType() == Token::CHAR )
          {
          Token::Char tc2= Token::canMerge(previousToken->getChar(),tc);
          if( tc2 != Token::null_char )
            {
            // let's merge them
            tokens.removeLast();
             delete previousToken;
            tc= tc2;

            if( tc == Token::slash2 )
              {
              skipLineComment();
              gotSpace= true;
              continue;
              }

            if( tc == Token::slash_star )
              {
              if( ! skipComment() )
                goto got_error;
              gotSpace= true;
              continue; // comment skipped
              }
            }
          }
        }
      // store token char tc into a new Token
      t= new Token( tc, getCurrentFilename().get(), line );
      goto got_it;
      }

   // it's a scalar or an identifier

   if( isDigitChar(ch) )
     {
     // scalar
     currentStream->backward();

get_scalar:
     boolean isInteger; // not used
     double scalar;
     if( ! getScalar(scalar,isInteger) )
       goto got_error;

     if( ! gotSpace && ! tokens.empty() )
       {
       Token::Type lastType= tokens.getLast()->getType();
       assert( lastType != Token::SCALAR );
       if( lastType == Token::NAME )
         {
         // case: 5TOTO or 5.9e3TOTO
         errorLine= line;
         errorFilename= getCurrentFilename().get();
         errorString= MLString("parse error");
         goto got_error;
         }
       }

     t= new Token( scalar, getCurrentFilename().get(), line );
     goto got_it;
     }

   if( isIdentifierChar(ch) )
     {
     // it's a string
     currentStream->backward();
     static char* string= 0;
     if( string == 0 )
       string= new char [ maxIdentifierLength + 1 ];

     if( ! getName( string, maxIdentifierLength ) )
       goto got_error;

     if( ! gotSpace && ! tokens.empty() )
       {
       Token::Type lastType= tokens.getLast()->getType();
       assert( lastType != Token::NAME );
       if( lastType == Token::SCALAR )
         {
         // case: TOTO.8e5
         errorLine= line;
         errorFilename= getCurrentFilename().get();
         errorString= MLString("parse error");
         goto got_error;
         }
       }

     t= new Token( Token::NAME, string, getCurrentFilename().get(), line );
     goto got_it;
     }

   errorLine= line;
   errorFilename= getCurrentFilename().get();
     {
     char s[20];
     sprintf( s, "%o", ch );
     errorString= MLString("unexpected character ") + MLString(s);
     }

got_error:
   t= new Token( Token::ERROR, errorString, errorFilename, errorLine );
   ++nErrors;

got_it:
   assert( t != 0 );
   tokens.append(t);

   t= 0;
   gotSpace= false;

   ++nTokens;
   if( nTokens > maxTokens )
     return true;
   }
}

/////////////////////////////////////////////////////////////////////////////

