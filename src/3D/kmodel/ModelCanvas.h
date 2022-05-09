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
  ModelCanvas.h

  Stephane Rehel
  November 30 1996
*/

#ifndef __ModelCanvas_h
#define __ModelCanvas_h

#ifndef __MLCanvas_h
#include "interface/MLCanvas.h"
#endif

class MLEPerspective;
class MLEOrthoAxis;
class MLERay;
class MLCamera;
class ModelModule;

/////////////////////////////////////////////////////////////////////////////

class ModelCanvas: public MLCanvas
{
protected:
  ModelModule* model;

  MLEPerspective* perspective_Engine; // not owner
  MLEOrthoAxis* orthoX_Engine;
  MLEOrthoAxis* orthoY_Engine;
  MLEOrthoAxis* orthoZ_Engine;
  MLERay*       ray_Engine;

public:
  ModelCanvas( ModelModule* _model );

  virtual ~ModelCanvas();

  virtual void create( int _where );

  const MLCamera& getPerspectiveCamera() const;

  void refreshScreen( unsigned what );
  void refreshMainCamera( const MLCamera& camera );

protected:
  virtual void write( MLWFileBlock& block );
  virtual void read( MLRFileBlock& block );

  void activateDefaultEngine();
  virtual void resetAll();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelCanvas_h
