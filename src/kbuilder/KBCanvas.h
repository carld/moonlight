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
  KBCanvas.h

  Stephane Rehel

  October 19 1997
*/

#ifndef __KBCanvas_h
#define __KBCanvas_h

#ifndef __MLCanvas_h
#include "interface/MLCanvas.h"
#endif

class KBECanvas;
class Canvas;
class KBModule;

/////////////////////////////////////////////////////////////////////////////

class KBCanvas: public MLCanvas
{
protected:
  KBModule* kbuilder;

  Canvas* builder_canvas;

  KBECanvas* canvasEngine; // creator but not owner

public:
  KBCanvas( KBModule* _kbuilder );

  virtual ~KBCanvas();

  virtual void create( int _where );

  void refreshScreen( unsigned what );

  Canvas* getBuilderCanvas() const;

protected:
  virtual void write( MLWFileBlock& block );
  virtual void read( MLRFileBlock& block );

  void activateDefaultEngine();
  virtual void resetAll();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __KBCanvas_h
