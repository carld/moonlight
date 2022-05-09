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
  ModelEngine.h

  Stephane Rehel

  December 8 1996
*/

#ifndef __ModelEngine_h
#define __ModelEngine_h

#ifndef __MLEngine_h
#include "interface/MLEngine.h"
#endif

class ModelCanvas;
class ModelModule;
class MLScene;
class MLCamera;

/////////////////////////////////////////////////////////////////////////////

class ModelEngine: public MLEngine
{
protected:
  ModelModule* model; // not owner
  MLScene* scene; // not owner

public:
  ModelEngine( ModelCanvas* _canvas );
  virtual ~ModelEngine();

  ModelModule* getModelModule() const
    {
    return model;
    }

  virtual void refreshScreen( unsigned what );
  virtual void refreshMainCamera( const MLCamera& );

  virtual boolean handleAction( MLAction* action );

  virtual void write( MLWFileBlock& block );
  virtual void read( MLRFileBlock& block );

  virtual void resetAll();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelEngine_h
