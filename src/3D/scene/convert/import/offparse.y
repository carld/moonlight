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
  offparse.y

  Stephane Rehel

  August 11 1997
*/

%{
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "tools/Vector.h"
#include "OFFFormat.h"
extern "C" void offerror( char* );
extern "C" int offlex();
extern "C" void yyterminate();

%}

%start off_description

%union {
       struct {
              char* string;
              double d;
              double x, y, z;
              int n;
              int* indices;
              OFFFormat* off;
              } s;
       }

/*
%token SIGNATURE SIZES VERTICES_LIST POLYGONS_LIST
*/

%token <s.string> IDENT
%token <s.d> REAL

%type <s.n> INTEGER

%type <s> sizes VERTEX signature vertices_list polygons_list


%left '='
%right '(' '{' '[' ','
%left  ')' '}' ']'

%%

off_description : signature sizes vertices_list polygons_list
  {
  }

signature : IDENT
  {
  if( strcmp($1,"OFF") != 0 )
    {
    offerror("need the 'OFF' signature header");
    yyterminate();
    // ERROR!
    }
  free( (void*) $1 );
  $1= 0;
  }

sizes : INTEGER INTEGER INTEGER
  {
  OFFFormat*& off= $$.off;
  off= new OFFFormat;
  off->create( $1, $2, $3 );
  }

vertices_list: VERTEX
  {
  $$.n= 1;
  $$.off->vertices[1]= Point( $1.x, $1.y, $1.z );
  }
  | vertices_list VERTEX
  {
  if( $$.n >= $$.off->nVertices )
    {
    offerror("unexpected vertex");
    yyterminate();
    }
  ($$.n)++;
  $$.off->vertices[ $$.n ]= Point( $2.x, $2.y, $2.z );
  }

VERTEX : REAL REAL REAL
  {
  $$.x= $1;
  $$.y= $2;
  $$.z= $3;
  }

INTEGER : REAL
  {
  int i= int($1);
//  double f= $1 - double(i);
//  if( fabs(f) > 1e-5 )
//    error!!!
  $$= i;
  }

polygons_list : REAL
  {
  }
