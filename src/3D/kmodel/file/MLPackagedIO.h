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
  MLPackagedIO.h

  Stephane Rehel

  December 17 1997
*/

#ifndef __MLPackagedIO_h
#define __MLPackagedIO_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MLWFileBlock;
class MLRFileBlock;
class MLIOProgress;

class ModelModule;

/////////////////////////////////////////////////////////////////////////////

class MLPackagedIO
{
protected:
  ModelModule* model;

public:
  static const unsigned int signature;

  enum {
       PKG_FIRST              = 0,
       PKG_DRIVER_VERSION     = 1,
       PKG_FILE_INFO          = 2,
       PKG_SCENE_INFO         = 3,
       PKG_GEOMETRIES         = 4,
       PKG_HIERARCHIES        = 5,
       PKG_SCENE              = 6,
       PKG_MATERIALS          = 7,
       PKG_PROJECT            = 8,
       PKG_LAST
       };

public:
  MLPackagedIO( ModelModule* _model );

  virtual ~MLPackagedIO();

  boolean write( const MLString& filename );

  boolean read( const MLString& filename );

private:
  // in MLPackaged_1_0.C
  boolean read_1_0( MLRFileBlock& mainBlock, MLIOProgress* progress );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPackagedIO_h
