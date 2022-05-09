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
  MLDQuitInterface.C

  Stephane Rehel

  August 8 1997
*/

#ifndef __MLDQuitInterface_h
#define __MLDQuitInterface_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLGlobalDialog_h
#include "../MLGlobalDialog.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLDQuitInterface: public MLGlobalDialog
{
public:
  enum {
       REALLY_QUIT= 1,
       DONT_QUIT,
       ICONIFY
       };

  int state;

public:
  MLDQuitInterface( Interface* interface );

  virtual ~MLDQuitInterface();

  int run();

protected:
  virtual boolean kwCallback( KWEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDQuitInterface_h
