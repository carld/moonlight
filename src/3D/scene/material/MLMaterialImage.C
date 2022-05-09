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
  MLMaterialImage.C

  Stephane Rehel

  August 30 1997
*/

#include "MLMaterialImage.h"

#include "tools/MLFileEntry.h"

#include "image/MLImage.h"
#include "image/MLImageFormat.h"
#include "image/MLImageReader.h"

#include "scene/file/MLDatabasesList.h"
#include "scene/file/MLDatabase.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLMaterialImage::MLMaterialImage( MLScene* _scene )
{
  scene= _scene;
  name= "";
  image= 0;
  load_error= "";
  load_filename= "";
  load_time= -1;
}

/////////////////////////////////////////////////////////////////////////////

MLMaterialImage::~MLMaterialImage()
{
  delete image;
  image= 0;

  name= "";
  scene= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialImage::setName( const MLString& _name )
{
  name= _name;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLMaterialImage::getImageFilename() const
{
  MLDatabasesList dblist;
  MLDatabase* db= dblist.getDefaultDatabase();

  if( db == 0 )
    return MLString("/") + name;

  return db->getImagePath() + name;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLMaterialImage::load()
{
  load_error= "";

  MLString filename= getImageFilename();
  long mod_time= MLFileEntry(filename).lastModified();

  if( mod_time == 0. )
    {
    load_error= "No such file";
    delete image;
    image= 0;
    return false;
    }

  if( image != 0 &&
      filename == load_filename &&
      mod_time <= load_time )
    return true; // already loaded

  load_filename= filename;
  load_time= mod_time;

  delete image;
  image= 0;

  MLImageReader* reader= MLImageFormat::getInstance()->guessReader(filename.get());

  if( reader == 0 )
    {
    load_error= "Unknown format extension";
    return false;
    }

  if( ! reader->open(filename) )
    {
    load_error= reader->getError();
    delete reader;
    reader= 0;
    return false;
    }

  image= new MLImage;

  if( ! reader->read(image) )
    {
    delete image;
    image= 0;
    load_error= reader->getError();
    delete reader;
    reader= 0;
    return false;
    }

  delete reader;
  reader= 0;

  image->upsidedown();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
