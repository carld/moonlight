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
  MLMode.h

  Stephane Rehel

  December 10 1996
*/

#ifndef __MLMode_h
#define __MLMode_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MLModule;

/////////////////////////////////////////////////////////////////////////////

class MLMode
{
  friend class MLModes; // set id

private:
  int mode_id;

protected:
  MLModule* module;

  MLString name;

public:
  enum ACTION
    {
    MOTION_ACTION= 1,
    CLICKED_MOTION_ACTION,
    RECT_ACTION
    };

  ACTION action;

public:
  MLMode( MLModule* _module );

  virtual ~MLMode();

  int getID() const
    {
    return mode_id;
    }

  const MLString& getName() const
    {
    return name;
    }

  virtual void enter();
  virtual void leave();
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MLMode_h
