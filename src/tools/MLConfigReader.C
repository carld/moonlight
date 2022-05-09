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
  MLConfigReader.C

  Stephane Rehel

  March 7 1998
*/

#include <string.h>

#include "MLConfigReader.h"
#include "MLConfig.h"

/////////////////////////////////////////////////////////////////////////////

MLConfigReader::MLConfigReader()
{
  filename= "";
  file= 0;
  lineN= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLConfigReader::~MLConfigReader()
{
  close();
}

/////////////////////////////////////////////////////////////////////////////

void MLConfigReader::close()
{
  if( file != 0 )
    {
    fclose(file);
    file= 0;
    lineN= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLConfigReader::open( const MLString& _filename )
{
  close();

  filename= _filename;
  file= fopen( filename.get(), "r" );

  if( file == 0 )
    return false;

  lineN= 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLConfigReader::read( MLConfig* config )
{
  if( config == 0 )
    return true;

  if( file == 0 )
    return false;

  MLString current_group= "Default";
  for(;;)
    {
    char* _line= getline();
    if( _line == 0 )
      break;

    MLString line= _line;
    if( line[0] == '[' )
      {
      int i= line.indexOf(']');
      if( i == -1 )
        return false;

      MLString group= line.substring(1,i-1);
      if( group.length() == 0 )
        return false;

      current_group= group;
      continue;
      }

    int i= line.indexOf('=');
    if( i == -1 )
      return false;

    MLString name= line.substring(0,i-1).trim();
    MLString value= line.substring( i+1, line.length()-1 );
    if( name.length() == 0 )
      return false;

    config->setVariable( current_group, name, value );
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if eof
// skip comments ('#)
char* MLConfigReader::getline()
{
  if( file == 0 )
    return 0;

  for(;;)
    {
    if( fgets(one_line,1023,file) == 0 )
      return 0;

    ++lineN;

    char* line= one_line;
    while( *line == ' ' || *line == '\t' )
      ++line;

    if( *line == '#' )
      continue;

    int length= strlen(line) - 1;
    if( length >= 0 )
      {
      if( line[length] == '\n' )
        {
        line[length]= '\0';
        --length;
        }
      }

    while( length >= 0 )
      {
      if( line[length] != ' ' && line[length] != '\t' )
        break;

      line[length]= '\0';
      --length;
      }

    if( line[0] == '\0' )
      continue;

    if( line[length] == '\\' )
      {
      line[length]= '\0';
      --length;
      MLString save= line;
      char* next= getline();
      if( next == 0 )
        return line;
      save += MLString(line);
      strncpy(one_line,save.get(),1023);
      return one_line;
      }

    return line;
    }
}

/////////////////////////////////////////////////////////////////////////////
