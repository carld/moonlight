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
  MLObjectIO.h

  Stephane Rehel
  April 13 1997
*/

#ifndef __MLObjectIO_h
#define __MLObjectIO_h

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

class MLObject;
class MLScene;
class MLDatabase;
class MLName;
class MLIOProgress;
class MLLight;
class MLMesh;

/////////////////////////////////////////////////////////////////////////////

class MLObjectIO: public MLIO
{
protected:
  MLDatabase* database;
  MLScene* scene;

public:
  MLObjectIO( MLDatabase* _database, MLScene* _scene,
              MLIOProgress* _progress = 0 );

  virtual ~MLObjectIO();

  // Also write referenced materials
  boolean write( MLObject* o, const MLString& filename );

  void write( MLObject* o, MLWFileBlock& block );

  MLObject* read( MLRFileBlock& block );

  static MLObject* read( MLDatabase* database,
                         MLScene* scene,
                         const MLString& filename,
                         MLIOProgress* progress = 0 );
  static MLObject* read( MLDatabase* database,
                         MLScene* scene,
                         const MLName& name,
                         MLIOProgress* progress = 0 );
  static boolean write( MLDatabase* database, MLObject* o,
                      MLIOProgress* progress = 0 );

protected:
  void writeObject( MLObject* o, MLWFileBlock& block );

  // in MLObject_1_0.C
  MLObject* read_1_0( MLObject* father, MLRFileBlock& );

  // in MLObject_1_1.C
  MLObject* read_1_1( MLObject* father, MLRFileBlock& );

  // in MLObject_1_2.C
  MLObject* read_1_2( MLObject* father, MLRFileBlock& );

protected:
  // in MLLightIO.C
  void writeLight( MLLight* light, MLWFileBlock& block );
  void readLight_1( MLLight* light, MLRFileBlock& block );
  void readLight_2( MLLight* light, MLRFileBlock& block );
  void readLight( MLLight* light, MLRFileBlock& block );

  // in MLMeshIO.C
  void writeMesh( MLMesh* mesh, MLWFileBlock& block );
  void readMesh_1( MLMesh* Mesh,
                   const MLName& sceneName,
                   MLRFileBlock& block );
  void readMesh( MLMesh* mesh,
                 const MLName& sceneName,
                 MLRFileBlock& block );

public:
  // e.g. my_scene,1.2,hier-1.mlh
  static MLString getFilename( MLObject* o );
  static MLString getFilename( const MLName& sceneName, const MLName& name );

  // e.g. /usr/ml/database/GEOM/my_scene,1.2,hier-1.mlh
  static MLString getObjectFilename( MLDatabase* database,
                                    MLObject* o );

  // e.g. /usr/ml/database/GEOM/my_scene,1.2,hier-1.mlh
  static MLString getObjectFilename( MLDatabase* database,
                                    const MLName& sceneName,
                                    const MLName& name );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLObjectIO_h
