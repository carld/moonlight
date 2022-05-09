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
  MLModulesBar.h

  Stephane Rehel

  November 24 1996
*/

#ifndef __MLModulesBar_h
#define __MLModulesBar_h

#ifndef __Color_h
#include "tools/Color.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __IRectangle_h
#include "tools/IRectangle.h"
#endif

#ifndef __SoftWindow_h
#include "graphics/SoftWindow.h"
#endif

class Interface;
class MLModulesBar;
class MLFont;
class MLModule;

/////////////////////////////////////////////////////////////////////////////

class MLModulesBar: public SoftWindow
{
protected:
  Interface* interface;

  struct ModuleInfo
    {
    int id;
    MLString title;
    Color color;
    IRectangle rect;
    IPoint textPos;
    };

  MLFont* font;

  ModuleInfo* modules;
  int nModules;

public:
  MLModulesBar( Interface* _interface );
  virtual ~MLModulesBar();

  void create();

  void appendModule( int id );

  void activate( int id );

protected:
  void draw();
  virtual boolean handleEvent( const MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLModulesBar_h
