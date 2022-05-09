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
  KBModuleKeyboardHandler.h

  Stephane Rehel

  October 25 1997
*/

#ifndef __KBModuleKeyboardHandler_h
#define __KBModuleKeyboardHandler_h

#ifndef __MLKeyboardHandler_h
#include "graphics/MLKeyboardHandler.h"
#endif

class KBModule;

/////////////////////////////////////////////////////////////////////////////

class KBModuleKeyboardHandler: public MLKeyboardHandler
{
protected:
  KBModule* kb;

public:
  KBModuleKeyboardHandler( KBModule* _kb );

  ~KBModuleKeyboardHandler();

  virtual boolean handleMLKeyboard( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __KBModuleKeyboardHandler_h
