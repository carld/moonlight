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
  MLImageFormat.C

  Stephane Rehel

  September 20 1997
*/

#include <cctype>

#include "tools/MLFilename.h"

#include "MLImageFormat.h"

#include "MLImageReader.h"
#include "MLImageWriter.h"

#include "pnm/MLImageFactoryPNM.h"
#include "gif/MLImageFactoryGIF.h"
#include "tga/MLImageFactoryTGA.h"
#include "rgb/MLImageFactoryRGB.h"
#include "bmp/MLImageFactoryBMP.h"
#include "jpg/MLImageFactoryJPG.h"
#include "tif/MLImageFactoryTIF.h"

static MLImageFormat theImageFormat;

MLImageFormat *MLImageFormat::instance=0;

MLImageFormat::MLImageFormat()
{
  if(instance==0) {
    instance=(MLImageFormat *)1; // force it to a non-zero value to avoid infinite recursion
    instance=new MLImageFormat;
  } else {
    // do real construction work here
    factories.clear();
    factories.push_back(new MLImageFactoryJPG);
    factories.push_back(new MLImageFactoryTIF);
    factories.push_back(new MLImageFactoryGIF);
    factories.push_back(new MLImageFactoryRGB);
    factories.push_back(new MLImageFactoryTGA);
    factories.push_back(new MLImageFactoryPNM);
    factories.push_back(new MLImageFactoryBMP);
  }
}

MLImageFormat::~MLImageFormat()
{
  if(instance!=0) {
    MLImageFormat *_instance=instance;
    instance=0;
    delete instance;
  } else {
    // do real destruction work here:
    for(int i=0;i<factories.size();i++) {
      delete factories[i];
    }
  }
}

void MLImageFormat::registerFactory(MLImageFactory *factory)
{
  factories.push_back(factory);
}

/////////////////////////////////////////////////////////////////////////////

int MLImageFormat::getNFormats()
{
  return factories.size()-1;
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= i <= nFormats
int MLImageFormat::getFromIndex( int i )
{
  if(i < 0 || i >= factories.size())
    return -1;

  return i;
}

/////////////////////////////////////////////////////////////////////////////

std::string MLImageFormat::getName( int fmt )
{
  return factories[fmt]->name.c_str();
}

/////////////////////////////////////////////////////////////////////////////

int MLImageFormat::getFromName( const std::string& name )
{
  if( name.length() == 0 )
    return -1;

  for( int i= 0; i <= getNFormats(); ++i ) {
    MLImageFormat::FORMAT fmt= getFromIndex(i);
    if( getName(fmt) == name ) {
      return fmt;
    }
  }

  return -1;
}

/////////////////////////////////////////////////////////////////////////////

std::string MLImageFormat::getExtension( MLImageFormat::FORMAT fmt )
{
  if(fmt < 0 || fmt >= factories.size())
    return "";

  return factories[fmt]->fileExtensions[0];
}

/////////////////////////////////////////////////////////////////////////////

int MLImageFormat::getFromFilename(
                                                   const std::string& filename )
{
  std::string ext= MLFilename(filename.c_str()).getExtension().get();

  // convert extension to lower case
  for(int i=0;i<ext.length();i++) {
    ext[i]=std::tolower(ext[i]);
  }

  for(int i=0;i<factories.size();i++) {
    for(int j=0;j<factories[i]->fileExtensions.size();j++) {
      if(ext == factories[i]->fileExtensions[i])
	return i;
    }
  }
  return -1;

}

/////////////////////////////////////////////////////////////////////////////

bool MLImageFormat::availableReader( int fmt )
{
  MLImageReader* r=getReader(fmt);

  if( r == 0 )
    return false;

  delete r;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

bool MLImageFormat::availableWriter( int fmt )
{
  MLImageWriter* w= getWriter(fmt);

  if( w == 0 )
    return false;

  delete w;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLImageReader* MLImageFormat::getReader( int fmt )
{
  return factories[fmt]->getReader();
}

/////////////////////////////////////////////////////////////////////////////

MLImageReader* MLImageFormat::guessReader( const std::string& filename )
{
  return getReader( getFromFilename(filename) );
}

/////////////////////////////////////////////////////////////////////////////

MLImageWriter* MLImageFormat::getWriter( int fmt )
{
  return factories[fmt]->getWriter();
}

/////////////////////////////////////////////////////////////////////////////
