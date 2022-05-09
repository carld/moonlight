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
  MLDMeshTesselation.h

  Stephane Rehel

  December 13 1997
*/

#ifndef __MLDMeshTesselation_h
#define __MLDMeshTesselation_h

#ifndef __MLScrollingDialog_h
#include "interface/MLScrollingDialog.h"
#endif

class ModelModule;
class MLMesh;

/////////////////////////////////////////////////////////////////////////////

class MLDMeshTesselation: public MLScrollingDialog
{
protected:
  ModelModule* model;

public:
  MLDMeshTesselation( ModelModule* _model );

  virtual ~MLDMeshTesselation();

protected:
  MLMesh* getValidFormal();

public:
  void update();

  void updateTotalPolygons();

  virtual boolean kwCallback( KWEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDMeshTesselation_h
