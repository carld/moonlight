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
  MLPictures.C

  Stephane Rehel
  August 16 1996
*/

#include "tools/SIList.h"
#include "tools/MLString.h"

#include "MLPictures.h"
#include "MLPicture.h"

struct Pict
{
  MLPicture* picture;
  MLString name; // when possible
};

static SIList<Pict> list;

/////////////////////////////////////////////////////////////////////////////

static Pict* findPicture( MLPicture* p )
{
  SIListIterator<Pict> li(list);
  while( ! li.eol() )
    {
    Pict* pict= li.next();
    if( pict->picture == p )
      return pict;
    }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////

static Pict* findPicture( const MLString& name )
{
  SIListIterator<Pict> li(list);
  while( ! li.eol() )
    {
    Pict* pict= li.next();
    if( pict->name == name )
      return pict;
    }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////

MLPictures::MLPictures()
{}

/////////////////////////////////////////////////////////////////////////////

void MLPictures::ref( MLPicture* picture )
{
  if( picture == 0 )
    return;

  picture->refCount++;
}

/////////////////////////////////////////////////////////////////////////////

int MLPictures::unref( MLPicture* picture )
{
  if( picture == 0 )
    return 0;

  picture->refCount--;

  if( picture->refCount <= 0 )
    {
    Pict* p= findPicture(picture);
    list.remove(p);
    delete p;
    delete picture;
    return 0;
    }

  return picture->refCount;
}

/////////////////////////////////////////////////////////////////////////////

void MLPictures::add( MLPicture* picture, const MLString& name )
{
  if( picture == 0 )
    return;

  if( name.length() > 0 )
    if( find(name) != 0 )
      return;

  Pict* p= new Pict;
  p->picture= picture;
  p->name= name;

  list.append(p);
}

/////////////////////////////////////////////////////////////////////////////

void MLPictures::add( MLPicture* picture )
{
  add( picture, MLString("") );
}

/////////////////////////////////////////////////////////////////////////////

MLPicture* MLPictures::find( void* data )
{
  if( data == 0 )
    return 0;

  SIListIterator<Pict> li(list);
  while( ! li.eol() )
    {
    Pict* pict= li.next();
    if( pict->picture->data == data )
      return pict->picture;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

MLPicture* MLPictures::find( const MLString& name )
{
  Pict* p= findPicture(name);
  if( p == 0 )
    return 0;

  return p->picture;
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLPictures::getName( MLPicture* picture )
{
  Pict* p= findPicture(picture);
  if( p == 0 )
    return MLString::empty();

  return p->name;
}

/////////////////////////////////////////////////////////////////////////////

