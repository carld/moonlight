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
  MLImageReader.C

  Stephane Rehel

  August 12 1997
*/

#include <string.h>
#include <errno.h>

#include "MLImageReader.h"

/////////////////////////////////////////////////////////////////////////////

MLImageReader::MLImageReader()
{
  file= 0;
  error= "";
}

/////////////////////////////////////////////////////////////////////////////

MLImageReader::~MLImageReader()
{
  close();
}

/////////////////////////////////////////////////////////////////////////////

void MLImageReader::close()
{
  if( file != 0 )
    {
    fclose(file);
    file= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLImageReader::open( const MLString& filename )
{
  close();
  error= "";

  file= fopen( filename.get(), "rb" );
  if( file == 0 )
    {
    error= strerror(errno);
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLImageReader::read( MLImage* )
{
  error= "no reader available?!";

  return false;
}

/////////////////////////////////////////////////////////////////////////////
