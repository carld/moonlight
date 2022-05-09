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
  MLMaterialImages.h

  Stephane Rehel

  August 31 1997
*/

#ifndef __MLMaterialImages_h
#define __MLMaterialImages_h

class MLScene;
class MLMaterialImage;
class MLString;

/////////////////////////////////////////////////////////////////////////////

class MLMaterialImages
{
protected:
  MLScene* scene;

  int max_images;
  int nImages;
  MLMaterialImage** images;

public:
  MLMaterialImages( MLScene* _scene );

  virtual ~MLMaterialImages();

  void clear();

  void ref( MLMaterialImage* mi );

  // eventually create it, but don't load it!
  MLMaterialImage* ref( const MLString& name );

  void unref( MLMaterialImage* mi );

protected:
  // return 0 if not found
  int find_mi( MLMaterialImage* mi );

  void add_mi( MLMaterialImage* mi );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMaterialImages_h
