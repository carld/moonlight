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
  ModelModuleIO.h

  Stephane Rehel

  April 23 1997
*/

#ifndef __ModelModuleIO_h
#define __ModelModuleIO_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class ModelModule;
class MLRFileBlock;
class MLWFileBlock;
class MLDatabase;
class MLPicture;
class MLIOProgress;

/////////////////////////////////////////////////////////////////////////////

class ModelModuleIO
{
protected:
  ModelModule* model;

public:
  ModelModuleIO( ModelModule* _model );

  virtual ~ModelModuleIO();

protected:
  void writeFlags( MLWFileBlock& block );

public:
  void writeInterfaceBlock( MLWFileBlock& block );
  void writeMainBlock( MLWFileBlock& block );
  boolean write( MLDatabase* database );

protected:
  void readFlags_1( MLRFileBlock& block );
  void readFlags_2( MLRFileBlock& block );
  void readFlags_3( MLRFileBlock& block );

public:
  void read( MLRFileBlock& block );
  boolean readMainBlock( MLRFileBlock& block,
                       MLDatabase* database,
                       MLIOProgress* progress,
                       boolean read_scene = false );
  boolean read( const MLString& filename, MLDatabase* database );

  static MLPicture* readSnapshot( const MLString& filename );

  static MLString getProjectFilename( ModelModule* model,
                                     MLDatabase* database );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelModuleIO_h
