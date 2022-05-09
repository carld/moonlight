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
  OOMEngine.h

  Stephane Rehel

  October 19 1997
*/

#ifndef __OOMEngine_h
#define __OOMEngine_h

#ifndef __MLEngine_h
#include "interface/MLEngine.h"
#endif

class OOMCanvas;
class OOMModule;

/////////////////////////////////////////////////////////////////////////////

class OOMEngine: public MLEngine
{
protected:
  OOMModule* oomModule; // not owner

public:
  OOMEngine( OOMCanvas* _canvas );
  virtual ~OOMEngine();

  OOMModule* getOOMModule() const
    {
    return oomModule;
    }

  virtual void refreshScreen( unsigned what );

  virtual boolean handleAction( MLAction* action );

  virtual void write( MLWFileBlock& block );
  virtual void read( MLRFileBlock& block );

  virtual void resetAll();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __OOMEngine_h
