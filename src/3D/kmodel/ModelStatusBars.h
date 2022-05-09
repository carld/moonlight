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
  ModelStatusBars.h

  Stephane Rehel

  August 3 1997
*/

#ifndef __ModelStatusBars_h
#define __ModelStatusBars_h

#ifndef __MLActionHandler_h
#include "interface/MLActionHandler.h"
#endif

class ModelModule;
class MLStatusBars;
class MLStatusLineText;
class ModelStatusLineEntry;
class MLStatusLineButton;
class MLStatusLightingSwitch;

/////////////////////////////////////////////////////////////////////////////

class ModelStatusBars: public MLActionHandler
{
  friend class ModelModule;
protected:
  ModelModule* model;
  MLStatusBars* statusBars;

  ModelStatusLineEntry* logs;

  MLStatusLineText* modeInfo;
  MLStatusLineText* message;
  MLStatusLightingSwitch* dlt; // direct lighting
  MLStatusLightingSwitch* ilt; // indirect lighting

  MLStatusLineButton* single_multiple;

  struct
    {
    MLStatusLineButton* obj;
    MLStatusLineButton* vtx;
    MLStatusLineButton* pol;
    }
    component;

public:
  ModelStatusBars( ModelModule* _model, MLStatusBars* _statusBars );

  virtual ~ModelStatusBars();

  void init();

  void update();

protected:
  virtual boolean handleAction( MLAction* action );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelStatusBars_h
