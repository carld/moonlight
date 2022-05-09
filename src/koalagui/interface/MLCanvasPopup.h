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
  MLCanvasPopup.h

  Created by an engine, associated with a canvas title, drawn and
  clickable on a canvas title

  Stephane Rehel

  November 27 1996
*/

#ifndef __MLCanvasPopup_h
#define __MLCanvasPopup_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __MLCanvasAction_h
#include "MLCanvasAction.h"
#endif

#ifndef __MLPopupHandler_h
#include "MLPopupHandler.h"
#endif

class MLPopup;

/////////////////////////////////////////////////////////////////////////////

class MLCanvasPopup: public MLCanvasAction, public MLPopupHandler
{
  friend class MLCanvas;
protected:
  MLPopup* popup; // owner!
  MLString title;
  int text_y;
  int selected_label;

public:
  MLCanvasPopup( MLCanvas* _canvas );

  virtual ~MLCanvasPopup();

  void create( const MLString& _title );
  void setTitle( const MLString& _title );

  void createWindow();

  MLPopup* getPopup() const
    {
    return popup;
    }

  void draw();

  virtual boolean eventHandler( MLWindow*, MLEvent& );

  int getLabel() const
    {
    return selected_label;
    }

  void setChecked( int label, boolean yes );
  boolean getChecked( int label ) const;
  void setAvailable( int label, boolean yes );
  boolean getAvailable( int label ) const;

protected:
  virtual void popupCommand( MLPopup* _popup, int label );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCanvasPopup_h
