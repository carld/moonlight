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
  MLDExportAs.h

  Stephane Rehel

  August 11 1997
*/

#ifndef __MLDExportAs_h
#define __MLDExportAs_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __MLGlobalDialog_h
#include "interface/MLGlobalDialog.h"
#endif

class ModelModule;
class KWFileDialogManager;

/////////////////////////////////////////////////////////////////////////////

class MLDExportAs: public MLGlobalDialog
{
protected:
  ModelModule* model;
  KWFileDialogManager* manager;

public:
  MLDExportAs( ModelModule* _model );
  virtual ~MLDExportAs();

  boolean run();

protected:
  virtual boolean kwCallback( KWEvent& );

  void go_export( const MLString& filename, int format_index );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDExportAs_h
