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
  MLDSavePackaged.h

  Stephane Rehel

  December 29 1997
*/

#ifndef __MLDSavePackaged_h
#define __MLDSavePackaged_h

#ifndef __MLGlobalDialog_h
#include "interface/MLGlobalDialog.h"
#endif

class ModelModule;
class MLPicture;
class KWFileDialogManager;

/////////////////////////////////////////////////////////////////////////////

class MLDSavePackaged: public MLGlobalDialog
{
protected:
  ModelModule* model;
  KWFileDialogManager* manager;
  MLPicture* snapshot;

public:
  MLDSavePackaged( ModelModule* _model );

  virtual ~MLDSavePackaged();

  // return the abstract object read
  // return 0 if cancelled or error
  boolean run();

protected:
  boolean save( const MLString& filename, boolean include_snapshot );

  virtual boolean kwCallback( KWEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDSavePackaged_h
