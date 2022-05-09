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
  MLFile.C

  Stephane Rehel
  April 6 1997
*/

#include "MLFile.h"

#include "tools/MLString.h"
#include "tools/xdr/XDRFile.h"

/////////////////////////////////////////////////////////////////////////////

MLFile::MLFile()
{
  file= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLFile::~MLFile()
{
  close();
}

/////////////////////////////////////////////////////////////////////////////

void MLFile::close()
{
  if( file == 0 )
    return;

  file->close();
  delete file;
  file= 0;
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLFile::getFilename() const
{
  if( file == 0 )
    return MLString::empty();

  return file->getFilename();
}

/////////////////////////////////////////////////////////////////////////////

long MLFile::getPosition() const
{
  if( file != 0 )
    return file->getPosition();
   else
    return 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLFile::setPosition( long newPosition )
{
  if( file != 0 )
    file->setPosition(newPosition);
}

/////////////////////////////////////////////////////////////////////////////
