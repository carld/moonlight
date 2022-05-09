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
  MLLoadGeometry.h

  Stephane Rehel
  April 20 1997
*/

#ifndef __MLLoadGeometry_h
#define __MLLoadGeometry_h

#ifndef __MLGlobalDialog_h
#include "interface/MLGlobalDialog.h"
#endif

#ifndef __MLEventhandler_h
#include "graphics/MLEventHandler.h"
#endif

class ModelModule;
class MLAbstractMesh;
class MLDatabasesList;
class MLDatabase;
class MLQuickReadGeometry;
class MLCamera;
class SystemWindow;
class ListFiles;

/////////////////////////////////////////////////////////////////////////////

class MLLoadGeometry: public MLGlobalDialog,
                      public MLEventHandler
{
protected:
  ModelModule* model;
  MLDatabasesList* dblist;
  ListFiles* files;
  MLDatabase* currentDatabase;
  MLQuickReadGeometry* qrg;
  SystemWindow* pWindow;
  MLCamera* camera;

public:
  MLLoadGeometry( ModelModule* _model );
  virtual ~MLLoadGeometry();

  // return the abstract object read
  // return 0 if cancelled or error
  MLAbstractMesh* run();

protected:
  MLAbstractMesh* load( const MLString& filename );

  void updateFilesList();

  virtual boolean kwCallback( KWEvent& );
  void updatePreview();
  void drawPreview();

  virtual boolean eventHandler( MLWindow*, MLEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLLoadGeometry_h
