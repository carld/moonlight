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
  kw.h

  The Koala Widget API (kWAPI)

  Stephane Rehel
  August 4 1996
*/

#ifndef __kw_h
#define __kw_h

#ifndef __Key_h
#include "graphics/Key.h"
#endif

#ifndef __KWCallbackClass_h
#include "KWCallbackClass.h"
#endif

class KWDialog;

/////////////////////////////////////////////////////////////////////////////

class KWEvent
{
public:
  enum Type
    {
    FIRST= 1,

    DRAW,
    SELECT,
    DOUBLE_SELECT,
    KEY,
    VALUE,
    FOCUS,
    UNFOCUS,
    KILL_DIALOG, // just used for scrolling dialogs
    VALIDATE, // just used for scrolling dialogs

    KEY_PROCESSED,

    LAST
    };

  Type type;
  KWidget widget;
  Key key;
  KWDialog* dialog;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __kw_h
