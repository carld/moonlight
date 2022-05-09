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
  MLSceneIO.h

  Stephane Rehel
  April 15 1997
*/

#ifndef __MLSceneIO_h
#define __MLSceneIO_h

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

/////////////////////////////////////////////////////////////////////////////

class MLSceneIO: public MLIO
{
protected:
  MLDatabase* database;

public:
  MLSceneIO( MLDatabase* _database, MLIOProgress* _progress = 0 );

  virtual ~MLSceneIO();

  boolean write( MLScene* scene, const MLString& filename );

  void write( MLScene* scene, MLWFileBlock& block );

  boolean read( MLRFileBlock& block, MLScene* scene );

  static boolean read( MLDatabase* database,
                     MLScene* scene,
                     const MLString& filename,
                     MLIOProgress* _progress = 0 );
  static boolean read( MLDatabase* database,
                     MLScene* scene,
                     const MLName& sceneName,
                     MLIOProgress* _progress = 0 );
  static boolean write( MLDatabase* database,
                      MLScene* scene,
                      const MLName& sceneName,
                      MLIOProgress* _progress = 0 );

protected:
  boolean read_1_0( MLScene* scene, MLRFileBlock& );
  boolean read_1_1( MLScene* scene, MLRFileBlock& );
  boolean read_1_2( MLScene* scene, MLRFileBlock& );
  boolean read_1_3( MLScene* scene, MLRFileBlock& );

protected:
  void writeRendererOptions( MLScene* scene, MLWFileBlock& block );
  void readRendererOptions( MLScene* scene, MLRFileBlock& block );

public:
  // e.g. my_scene,1.2.mls
  static MLString getFilename( const MLName& sceneName );

  // e.g. /usr/ml/database/SCENES/my_scene,1.2.mls
  static MLString getSceneFilename( MLDatabase* database,
                                   const MLName& sceneName );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLSceneIO_h
