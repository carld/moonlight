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
  MLConfigWriter.C

  Stephane Rehel

  March 7 1998
*/

#include "MLConfigWriter.h"

#include "MLConfig.h"

/////////////////////////////////////////////////////////////////////////////

MLConfigWriter::MLConfigWriter()
{}

/////////////////////////////////////////////////////////////////////////////

MLConfigWriter::~MLConfigWriter()
{}

/////////////////////////////////////////////////////////////////////////////

void MLConfigWriter::write( FILE* file, MLConfig* config )
{
  if( file == 0 || config == 0 )
    return;

  for( int i= 0; i < config->getNGroups(); ++i )
    {
    MLConfig::Group* g= config->groups[i];
    if( g == 0 )
      continue;

    fprintf( file, "[%s]\n", g->name.get() );

    SIListIterator<MLConfig::Variable> li(g->vars);
    while( ! li.eol() )
      {
      const MLConfig::Variable* v= li++;
      if( v == 0 )
        continue;

      fprintf( file, "%s=%s\n", v->name.get(), v->initial_value.get() );
      }

    fprintf( file, "\n" );
    }
}

/////////////////////////////////////////////////////////////////////////////
