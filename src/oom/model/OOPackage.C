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
  OOPackage.C

  Stephane Rehel

  December 2 1997
*/

#include "OOPackage.h"
#include "OOClass.h"
#include "OORelation.h"

/////////////////////////////////////////////////////////////////////////////

OOPackage::OOPackage( OOModel* _model, const MLString& _name )
{
  model= _model;
  name= _name;
}

/////////////////////////////////////////////////////////////////////////////

OOPackage::~OOPackage()
{
  while( ! relations.empty() )
    {
    OORelation* r= relations.getLast();
    relations.removeLast();
    delete r;
    }

  while( ! classes.empty() )
    {
    OOClass* c= classes.getLast();
    classes.removeLast();
    delete c;
    }

  model= 0;
}

/////////////////////////////////////////////////////////////////////////////

OOClass* OOPackage::findClass( const MLString& name )
{
  SIListIterator<OOClass> li(classes);
  while( ! li.eol() )
    {
    OOClass* c= li++;
    if( c->getName() == name )
      return c;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

OOClass* OOPackage::newClass( const MLString& name )
{
  MLString the_name= name;

  if( the_name == "" )
    {
    int i= 1;
    for(;;)
      {
      the_name= MLString("class-") + MLString::valueOf(i);
      if( findClass(the_name) == 0 )
        break;
      ++i;
      }
    }

  OOClass* c= new OOClass(this,the_name);

  classes.append(c);

  return c;
}

/////////////////////////////////////////////////////////////////////////////

OORelation* OOPackage::newRelation( int n )
{
  OORelation* r= new OORelation(this,n);

  relations.append(r);

  return r;
}

/////////////////////////////////////////////////////////////////////////////
