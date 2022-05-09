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
   MLName.C

   Stephane Rehel
   April 14 1997
*/

#include "MLName.h"

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

// static
const int MLName::default_name_index= 0;

/////////////////////////////////////////////////////////////////////////////

MLName::MLName()
{
  filename= "";
  name= "";
  name_index= default_name_index;
  major_version= 1;
  minor_version= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLName& MLName::operator = ( const MLName& n )
{
  filename= n.filename;
  name= n.name;
  name_index= n.name_index;
  major_version= n.major_version;
  minor_version= n.minor_version;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLName::operator == ( const MLName& n ) const
{
  return name == n.name &&
         name_index == n.name_index &&
         major_version == n.major_version &&
         minor_version == n.minor_version;
}

/////////////////////////////////////////////////////////////////////////////

void MLName::write( MLWFileBlock& block ) const
{
  block << name << int(name_index)
        << int(major_version) << int(minor_version);
}

/////////////////////////////////////////////////////////////////////////////

void MLName::read( MLRFileBlock& block )
{
  int major, minor;

  block >> name >> name_index
        >> major >> minor;

  major_version= major;
  minor_version= minor;
}

/////////////////////////////////////////////////////////////////////////////

