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
  KBECanvas.h

  Stephane Rehel

  October 24 1997
*/

#ifndef __KBECanvas_h
#define __KBECanvas_h

#ifndef __KBEngine_h
#include "kbuilder/KBEngine.h"
#endif

class KBCanvas;
class KBEvent;
class MLXORRectAction;
class KBCommand;

/////////////////////////////////////////////////////////////////////////////

class KBECanvas: public KBEngine
{
public:
  KBECanvas( KBCanvas* _canvas );

  virtual ~KBECanvas();

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

#endif // ifndef __KBECanvas_h
