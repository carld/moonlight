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
  MLEngine3D.h

  Stephane Rehel

  December 1 1996
*/

#ifndef __MLEngine3D_h
#define __MLEngine3D_h

#include "tools/Vector2.h"

#ifndef __ModelEngine_h
#include "kmodel/ModelEngine.h"
#endif

#ifndef __MLCamera_h
#include "scene/MLCamera.h"
#endif

class MLCanvasPopup;
class MLCanvasButton;
class ModelCanvas;
class ModelDraw;
class MLEvent;
class MLXORRectAction;
class ModelCommand;
class IntegerArray;
class MLTagVertexArray;
class MLMotionAction;

/////////////////////////////////////////////////////////////////////////////

class MLEngine3D: public ModelEngine
{
protected:
  MLCamera camera;
  ModelDraw* modelDraw;

  MLCanvasButton* displayTypeButton;

  MLCanvasPopup* optionsPopup;
  int options_popup_wire_front;

public:
  MLEngine3D( ModelCanvas* _canvas );
  virtual ~MLEngine3D();

protected:
  // return false if window is unmapped, etc.
  boolean _back_draw();

public:
  virtual void draw();

  const MLCamera& getCamera() const
    {
    return camera;
    }

  virtual void refreshScreen( unsigned what );
  virtual void refreshMainCamera( const MLCamera& c );

  virtual void leaveMode( MLMode* mm );

protected:
  void pickObjects( MLXORRectAction* action, IntegerArray& objects );
  void pickVertices( MLXORRectAction* a, MLTagVertexArray& vertices );

  virtual boolean handleAction( MLAction* action );

protected:
  Vector getWorldVector( MLMotionAction* a ) const;
  Vector2 getScreenVector( MLMotionAction* a ) const;

public:
  virtual void write( MLWFileBlock& block );
  virtual void read( MLRFileBlock& block );

  virtual void resetAll();

public:
  virtual void updatePopupsFlags();

protected:
  virtual void drawViewAxes();

  virtual boolean followMainCamera() const
    {
    return false;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLEngine3D_h
