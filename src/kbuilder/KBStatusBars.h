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
  KBStatusBars.h

  Stephane Rehel

  October 19 1997
*/

#ifndef __KBStatusBars_h
#define __KBStatusBars_h

#ifndef __MLActionHandler_h
#include "interface/MLActionHandler.h"
#endif

class KBModule;
class MLStatusBars;
class MLStatusLineText;
class MLStatusLineButton;

/////////////////////////////////////////////////////////////////////////////

class KBStatusBars: public MLActionHandler
{
  friend class KBModule;
protected:
  KBModule* kb;
  MLStatusBars* statusBars;

  MLStatusLineText* message;
  MLStatusLineText* modeInfo;
  MLStatusLineText* widgetMessage;

//  MLStatusLineButton* single_multiple;

public:
  KBStatusBars( KBModule* _kb, MLStatusBars* _statusBars );

  virtual ~KBStatusBars();

  void init();

  void update();

protected:
  virtual boolean handleAction( MLAction* action );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __KBStatusBars_h
