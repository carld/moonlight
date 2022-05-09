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
  MLAbstractMeshIO.h

  Stephane Rehel
  April 6 1997
*/

#ifndef __MLAbstractMeshIO_h
#define __MLAbstractMeshIO_h

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

class MLAbstractMesh;
class MLScene;
class MeshRep;
class MLDatabase;
class MLName;
class MLIOProgress;

/////////////////////////////////////////////////////////////////////////////

class MLAbstractMeshIO: public MLIO
{
protected:
  MLDatabase* database;
  MLScene* scene;

public:
  MLAbstractMeshIO( MLDatabase* _database, MLScene* _scene,
                      MLIOProgress* _progress = 0 );

  virtual ~MLAbstractMeshIO();

  boolean write( MLAbstractMesh* ao, const MLString& filename );

  void write( MLAbstractMesh* ao, MLWFileBlock& wb );

  MLAbstractMesh* read( MLRFileBlock& );

  static MLAbstractMesh* read( MLDatabase* database,
                                 MLScene* scene,
                                 MLRFileBlock& block,
                                 MLIOProgress* progress = 0 );
  static MLAbstractMesh* read( MLDatabase* database,
                                 MLScene* scene,
                                 const MLString& filename,
                                 MLIOProgress* progress = 0 );
  static MLAbstractMesh* read( MLDatabase* database,
                                 MLScene* scene,
                                 const MLName& name,
                                 MLIOProgress* progress = 0 );
  static boolean write( MLDatabase* database, MLAbstractMesh* ao,
                      MLIOProgress* progress = 0 );

protected:
  void writeMesh( MeshRep* mesh, MLWFileBlock& );

protected:
  MLAbstractMesh* read_1_0( MLRFileBlock& );
  MLAbstractMesh* read_1_1( MLRFileBlock& );

public:
  // e.g. my_scene,1.2,geom-1.mlg
  static MLString getFilename( MLAbstractMesh* ao );
  static MLString getFilename( const MLName& sceneName, const MLName& name );

  // e.g. /usr/ml/database/GEOM/my_scene,1.2,geom-1.mlg
  static MLString getAbstractObjectFilename( MLDatabase* database,
                                            MLAbstractMesh* ao );

  // e.g. /usr/ml/database/GEOM/my_scene,1.2,geom-1.mlg
  static MLString getAbstractObjectFilename( MLDatabase* database,
                                            const MLName& sceneName,
                                            const MLName& name );

  static MLAbstractMesh* getByFilename( MLScene* scene,
                                          const MLString& filename );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLAbstractMeshWriter_h
