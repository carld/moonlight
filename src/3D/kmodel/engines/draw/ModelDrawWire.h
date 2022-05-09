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
  ModelDrawWire.h

  Stephane Rehel

  September 7 1997
*/

#ifndef __ModelDrawWire_h
#define __ModelDrawWire_h

#ifndef __ModelDrawDriver_h
#include "ModelDrawDriver.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class ModelDrawWire: public ModelDrawDriver
{
public:
  ModelDrawWire( ModelDraw* _modelDraw );

  virtual ~ModelDrawWire();

  virtual void clear();
  virtual void drawGrid();
  void drawCamera( const MLCamera& c );

  virtual void drawNullObject( MLNullObject* no );
  virtual void drawMesh( MLMesh* mesh );
  virtual void drawCurve( MLCurve* curve );
  virtual void drawLight( MLLight* l );

  virtual void drawTagVertices( MLObject* object );

  virtual void drawTagPolygons( MLObject* object );

  virtual void drawNURBSControlNet( MLMesh* mesh );
  virtual void drawNURBSControlNet( MLCurve* curve );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelDrawWire_h
