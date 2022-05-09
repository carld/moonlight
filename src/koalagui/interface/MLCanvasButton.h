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
  MLCanvasButton.h

  Created by an engine, associated with a canvas title, drawn and
  clickable on a canvas title

  Stephane Rehel

  November 30 1996
*/

#ifndef __MLCanvasButton_h
#define __MLCanvasButton_h

#ifndef __MLCanvasAction_h
#include "MLCanvasAction.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

class MLPixmap;
class MLWindow;

/////////////////////////////////////////////////////////////////////////////

class MLCanvasButton: public MLCanvasAction
{
protected:
  MLString title;
  MLPixmap* pixmap;             // not owner
  MLPixmap* highlighted_pixmap; // not owner
  int text_y;

public:
  MLCanvasButton( MLCanvas* _canvas );
  virtual ~MLCanvasButton();

  void create( const MLString& _title );
  void create( MLPixmap* _pixmap, MLPixmap* _highlighted_pixmap = 0 );

protected:
  void create( const MLString& _title,
               MLPixmap* _pixmap, MLPixmap* _highlighted_pixmap );

public:
  void set( MLPixmap* _pixmap, MLPixmap* _highlighted_pixmap = 0 );

  void setTitle( const MLString& _title );

  void createWindow();

  virtual boolean eventHandler( MLWindow*, MLEvent& event );

public:
  void draw();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCanvasButton_h
