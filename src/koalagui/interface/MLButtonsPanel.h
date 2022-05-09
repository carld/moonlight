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
  MLButtonsPanel.h

  Stephane Rehel
  November 21 1996
*/

#ifndef __MLButtonsPanel_h
#define __MLButtonsPanel_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __MLEventHandler_h
#include "graphics/MLEventHandler.h"
#endif

class SoftWindow;
class MLPopup;
class MLString;
class MLModule;

/////////////////////////////////////////////////////////////////////////////

class MLButtonsPanel: public MLEventHandler
{
protected:
  MLModule* module;
  SoftWindow** windows;
  MLPopup** buttons;
  MLString* titles;

  int activated;

public:
  MLButtonsPanel( MLModule* _module );
  virtual ~MLButtonsPanel();

  void create( const IPoint& position, const IVector& size );
  void map( boolean yes = true );
  void postRedraw();
  void setButton( int i, const MLString& title, MLPopup* popup );

  void draw();
  void unpress();

protected:
  void drawButton( int i, boolean pressed, boolean highlighted = false );
  virtual boolean eventHandler( MLWindow*, MLEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLButtonsPanel_h
