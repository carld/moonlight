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
  ModelStatusLineEntry.h

  Stephane Rehel

  June 7 1998
*/

#ifndef __ModelStatusLineEntry_h
#define __ModelStatusLineEntry_h

#ifndef __MLStatusLineEntry_h
#include "interface/MLStatusLineEntry.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class ModelStatusLineEntry: public MLStatusLineEntry
{
protected:
public:
  ModelStatusLineEntry( MLModule* module,
                        MLStatusLine* _line, int n_chars,
                        MLStatusLineItem::Side _side,
                        boolean _expandable = false );

  virtual ~ModelStatusLineEntry();

  virtual boolean eventHandler( MLWindow*, MLEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelStatusLineEntry_h
