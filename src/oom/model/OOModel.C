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
  OOModel.C

  Stephane Rehel

  December 2 1997
*/

#include "OOModel.h"
#include "OOPackage.h"

/////////////////////////////////////////////////////////////////////////////

OOModel::OOModel()
{
  name= "noname";

  newPackage("");
}

/////////////////////////////////////////////////////////////////////////////

OOModel::~OOModel()
{
  while( ! packages.empty() )
    {
    OOPackage* p= packages.getLast();
    packages.removeLast();
    delete p;
    }
}

/////////////////////////////////////////////////////////////////////////////

OOPackage* OOModel::findPackage( const MLString& name )
{
  SIListIterator<OOPackage> li(packages);
  while( ! li.eol() )
    {
    OOPackage* p= li++;
    if( p->getName() == name )
      return p;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

OOPackage* OOModel::newPackage( const MLString& name )
{
  MLString the_name= name;

  if( the_name == "" )
    {
    int i= 1;
    for(;;)
      {
      the_name= MLString("package-") + MLString::valueOf(i);
      if( findPackage(the_name) == 0 )
        break;
      ++i;
      }
    }

  OOPackage* p= new OOPackage(this,the_name);

  packages.append(p);

  return p;
}

/////////////////////////////////////////////////////////////////////////////

OOPackage* OOModel::getDefaultPackage()
{
  if( packages.empty() )
    return 0;
  return packages.getFirst();
}

/////////////////////////////////////////////////////////////////////////////
