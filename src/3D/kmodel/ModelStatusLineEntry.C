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
  ModelStatusLineEntry.C

  Stephane Rehel

  June 7 1998
*/

#include "graphics/MLEvent.h"

#include "ModelStatusLineEntry.h"

#include "gdialogs/MLDMessagesLogs.h"

/////////////////////////////////////////////////////////////////////////////

ModelStatusLineEntry::ModelStatusLineEntry( MLModule* module,
                                        MLStatusLine* _line, int n_chars,
                                        MLStatusLineItem::Side _side,
                                        boolean _expandable /*= false*/ ):
  MLStatusLineEntry(module,_line,n_chars,_side,_expandable)
{}

/////////////////////////////////////////////////////////////////////////////

ModelStatusLineEntry::~ModelStatusLineEntry()
{}

/////////////////////////////////////////////////////////////////////////////

boolean ModelStatusLineEntry::eventHandler( MLWindow* w, MLEvent& event )
{
  if( event.type == MLEvent::MOUSE_PRESS &&
      event.button == 2 || event.button == 3 )
    {
    // open the dialog
    MLDMessagesLogs logs( (ModelModule*) module );
    logs.run();
    return true;
    }

  return MLStatusLineEntry::eventHandler( w, event );
}

/////////////////////////////////////////////////////////////////////////////
