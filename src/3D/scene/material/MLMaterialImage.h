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
  MLMaterialImage.h

  Stephane Rehel

  August 30 1997
*/

#ifndef __MLMaterialImage_h
#define __MLMaterialImage_h

#ifndef __MLReference_h
#include "tools/MLReference.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MLScene;
class MLImage;

/////////////////////////////////////////////////////////////////////////////

class MLMaterialImage: public MLReference
{
protected:
  MLScene* scene;
  MLString name;

  MLImage* image;
  MLString load_filename;
  long load_time;

  MLString load_error;

public:
  MLMaterialImage( MLScene* _scene );

  virtual ~MLMaterialImage();

  void setName( const MLString& _name );
  const MLString& getName() const
    {
    return name;
    }

  MLString getImageFilename() const;

  boolean load();

  const MLString& getLoadError() const
    {
    return load_error;
    }

  MLImage* getImage() const
    {
    return image;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMaterialImage_h
