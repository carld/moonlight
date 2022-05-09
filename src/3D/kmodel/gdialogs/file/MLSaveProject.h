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
  MLSaveProject.h

  Stephane Rehel
  April 24 1997
*/

#ifndef __MLSaveProject_h
#define __MLSaveProject_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLGlobalDialog_h
#include "interface/MLGlobalDialog.h"
#endif

#ifndef __MLName_h
#include "scene/MLName.h"
#endif

class ModelModule;
class MLAbstractMesh;
class MLDatabasesList;
class MLDatabase;
class MLString;
class ListFiles;

/////////////////////////////////////////////////////////////////////////////

class MLSaveProject: public MLGlobalDialog
{
protected:
  ModelModule* model;
  MLDatabasesList* dblist;
  ListFiles* files;
  MLDatabase* currentDatabase;
  MLName name;

public:
  MLSaveProject( ModelModule* _model );
  virtual ~MLSaveProject();

  // return the abstract object read
  // return 0 if cancelled or error
  boolean run();

protected:
  MLString getFilename();
  boolean save( const MLString& versionString,
              const MLString& nameString,
              boolean includeSnapshot );

public:
  static boolean save( MLDatabase* database, ModelModule* model );

protected:
  void updateFilesList();

  virtual boolean kwCallback( KWEvent& );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLSaveProject_h
