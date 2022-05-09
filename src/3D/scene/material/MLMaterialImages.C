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
  MLMaterialImages.C

  Stephane Rehel

  August 31 1997
*/

#include <assert.h>

#include "tools/MLString.h"

#include "MLMaterialImages.h"
#include "MLMaterialImage.h"

#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLMaterialImages::MLMaterialImages( MLScene* _scene )
{
  scene= _scene;

  images= 0;
  max_images= 0;
  nImages= 0;

  clear();
}

/////////////////////////////////////////////////////////////////////////////

MLMaterialImages::~MLMaterialImages()
{
  clear();
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialImages::clear()
{
  if( images != 0 )
    {
    for( int i= 1; i <= max_images; ++i )
      {
      delete images[i];
      images[i]= 0;
      }

    delete images;
    images= 0;
    }

  max_images= 10;
  nImages= 0;

  images= new MLMaterialImage* [ max_images + 1 ];

  for( int i= 0; i <= max_images; ++i )
    images[i]= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialImages::ref( MLMaterialImage* mi )
{
  if( mi == 0 )
    return;

// don't do this test, it's safer
//  if( find_mi(mi) == 0 )
//    return; // ??

  ::ref(mi);
}

/////////////////////////////////////////////////////////////////////////////

// eventually create it, but don't load it!
MLMaterialImage* MLMaterialImages::ref( const MLString& name )
{
  if( images == 0 )
    return 0;

  MLMaterialImage* mi= 0;
  boolean found= false;

  for( int i= 1; i <= max_images; ++i )
    {
    mi= images[i];
    if( mi == 0 )
      continue;
    if( mi->getName() == name )
      {
      found= true;
      break;
      }
    }

  if( found )
    {
    ref(mi);
    return mi;
    }

  mi= new MLMaterialImage(scene);
  mi->setName(name);

  add_mi(mi);

  ref(mi);

  return mi;
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialImages::unref( MLMaterialImage* mi )
{
  if( mi == 0 )
    return;

  int i= find_mi(mi);

  if( i == 0 )
    return; // ??

  ::unref(mi);
  if( mi == 0 )
    {
    images[i]= 0;
    --nImages;
    }
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if not found
int MLMaterialImages::find_mi( MLMaterialImage* mi )
{
  if( mi == 0 || images == 0 )
    return 0;

  for( int i= 1; i <= max_images; ++i )
    if( images[i] == mi )
      return i;

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialImages::add_mi( MLMaterialImage* mi )
{
  if( images == 0 || mi == 0 )
    return;

  assert( find_mi(mi) == 0 );

  if( nImages == max_images )
    {
    int new_max_images= max_images + max_images / 2;
    MLMaterialImage** new_images=
                             new MLMaterialImage* [ new_max_images + 1 ];
    int i;
    for( i= 0; i <= max_images; ++i )
      new_images[i]= images[i];

    for( i= max_images+1; i <= new_max_images; ++i )
      new_images[i]= 0;

    delete images;
    images= new_images;

    max_images= new_max_images;
    }

  for( int i= 1; i <= max_images; ++i )
    {
    if( images[i] == 0 )
      {
      // found a hole!
      images[i]= mi;
      ++nImages;
      break;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////
