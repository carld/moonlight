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
  MLSaveGeometry.h

  Stephane Rehel
  April 22 1997
*/

#ifndef __MLSaveGeometry_h
#define __MLSaveGeometry_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLGlobalDialog_h
#include "interface/MLGlobalDialog.h"
#endif

class ModelModule;
class MLAbstractMesh;
class MLDatabasesList;
class MLDatabase;
class MLString;
class ListFiles;

/////////////////////////////////////////////////////////////////////////////

class MLSaveGeometry: public MLGlobalDialog
{
protected:
  ModelModule* model;
  MLDatabasesList* dblist;
  ListFiles* files;
  MLDatabase* currentDatabase;

public:
  MLSaveGeometry( ModelModule* _model );
  virtual ~MLSaveGeometry();

  // return the abstract object read
  // return 0 if cancelled or error
  boolean run( MLAbstractMesh* ao );

protected:
  boolean save( const MLString& filename, MLAbstractMesh* ao );
  void updateFilesList();

  virtual boolean kwCallback( KWEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLSaveGeometry_h
