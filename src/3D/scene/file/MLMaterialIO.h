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
  MLMaterialIO.h

  Stephane Rehel

  September 14 1997
*/

#ifndef __MLMaterialIO_h
#define __MLMaterialIO_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __MLIO_h
#include "MLIO.h"
#endif

class MLWFileBlock;
class MLRFileBlock;

class MLMaterial;
class MLObject;
class MLScene;
class MLDatabase;
class MLName;
class MLIOProgress;

/////////////////////////////////////////////////////////////////////////////

class MLMaterialIO: public MLIO
{
protected:
  MLDatabase* database;
  MLScene* scene;

public:
  MLMaterialIO( MLDatabase* _database, MLScene* _scene,
                MLIOProgress* _progress = 0 );

  virtual ~MLMaterialIO();

  boolean write( MLMaterial* m, const MLString& filename );

  void write( MLMaterial* m, MLWFileBlock& block );

  // return an unregistered material!
  MLMaterial* read( MLRFileBlock& block );

  // return an unregistered material!
  static MLMaterial* read( MLDatabase* database,
                           MLScene* scene,
                           const MLString& filename,
                           MLIOProgress* progress = 0 );

  // return an unregistered material!
  static MLMaterial* read( MLDatabase* database,
                           MLScene* scene,
                           const MLName& name,
                           MLIOProgress* progress = 0 );

  static boolean write( MLDatabase* database, MLMaterial* m,
                      MLIOProgress* progress = 0 );

  // Write all referenced materials of this hierarchy
  static boolean writeReferenced( MLDatabase* database,
                                MLObject* o,
                                MLIOProgress* progress = 0 );

protected:
  void writeMaterial( MLMaterial* m, MLWFileBlock& block );

  // in MLMaterial_1_0.C
  MLMaterial* read_1_0( MLRFileBlock& );

public:
  // e.g. my_scene,1.2,material-1.mlm
  static MLString getFilename( MLMaterial* m );
  static MLString getFilename( const MLName& sceneName, const MLName& name );

  // e.g. /usr/ml/database/GEOM/my_scene,1.2,material-1.mlm
  static MLString getMaterialFilename( MLDatabase* database,
                                      MLMaterial* m );

  // e.g. /usr/ml/database/GEOM/my_scene,1.2,material-1.mlm
  static MLString getMaterialFilename( MLDatabase* database,
                                      const MLName& sceneName,
                                      const MLName& name );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMaterialIO_h
