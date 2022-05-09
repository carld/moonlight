// This file is part of Moonlight Creator
//   Copyright (C) 1998  Stephane Rehel
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
  offlex.l

  Stephane Rehel

  August 11 1997
*/

%{
#include <stdlib.h>
#include <string.h>
class OFFFormat;
#include "offparse.h"
extern "C" int offlex();
extern "C" int offwrap();

extern "C" void offerror( char* );

%}

%option yylineno
%option yywrap

DIGIT  [0-9]
ALPHA  [a-zA-Z_]
IDENT  [ALPHA]({DIGIT}|{ALPHA})*
BLANK  [ \t\n]
BLANKS {BLANK}+
REAL   [-+]?([0-9]+|(([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*))([eE][-+]?[0-9]+)?)
COMMENT "#".\n

%%

{COMMENT} { }

{REAL} {
  double d;
  if( sscanf( offtext, "%lf", &d ) != 1 )
    {
    offerror("invalid value");
    yyterminate();
    }
   else
    {
    offlval.s.d= d;
    return REAL;
    }
  }

{IDENT} {
  offlval.s.string= strdup(offtext);
  return IDENT;
  }

