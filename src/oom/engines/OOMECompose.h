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
  OOMECompose.h

  Stephane Rehel

  December 2 1997
*/

#ifndef __OOMECompose_h
#define __OOMECompose_h

#ifndef __OOMEngine_h
#include "oom/OOMEngine.h"
#endif

class MLCanvasPopup;
class MLCanvasButton;
class OOMCanvas;
class MLEvent;
class MLXORRectAction;
class OOMCommand;
class OOMCamera;
class OOMDraw;

/////////////////////////////////////////////////////////////////////////////

class OOMECompose: public OOMEngine
{
protected:
  MLCanvasButton* displayTypeButton;

  MLCanvasPopup* optionsPopup;

  int options_popup_wire_front;

  OOMCamera* camera;
  OOMDraw* oomDraw;

public:
  OOMECompose( OOMCanvas* _canvas );

  virtual ~OOMECompose();

  OOMCamera& getCamera() const
    {
    return *camera;
    }

protected:
  // return false if window is unmapped, etc.
  boolean _back_draw();

public:
  virtual void draw();

  virtual void refreshScreen( unsigned what );

  virtual void leaveMode( MLMode* mm );

protected:
  virtual boolean handleAction( MLAction* action );

  virtual void write( MLWFileBlock& block );
  virtual void read( MLRFileBlock& block );

  virtual void resetAll();

public:
  virtual void updatePopupsFlags();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __OOMECompose_h
