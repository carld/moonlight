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
  MLDialog.h

  Stephane Rehel
  July 1 1996
*/

#ifndef __MLDialog_h
#define __MLDialog_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __MLEventHandler_h
#include "MLEventHandler.h"
#endif

class SystemWindow;

/////////////////////////////////////////////////////////////////////////////

class MLDialog: public MLEventHandler
{
protected:
  SystemWindow* systemWindow;
  IPoint position;
  IVector size;
  MLString title;

public:
  MLDialog();
  virtual ~MLDialog();

  // _parent != 0
  boolean create( SystemWindow* _parent,
                int x, int y, int width, int height,
                const MLString& _title );

  void redraw();

  boolean eventHandler( MLWindow* dialog, MLEvent& event );

private:
  void drawFrame();

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDialog_h
