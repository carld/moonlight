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
  ModelRender.h

  Stephane Rehel

  September 21 1997
*/

#ifndef __ModelRender_h
#define __ModelRender_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class ModelModule;
class MLScene;
class MLRendererOptions;
class MLRayRenderer;
class ModelRenderWindow;
class MLPicture;

/////////////////////////////////////////////////////////////////////////////

class ModelRender
{
protected:
  ModelModule* model;
  ModelRenderWindow* window;

  MLScene* scene;
  MLRendererOptions* options;
  MLRayRenderer* renderer;
  MLPicture* picture;

  boolean _isRendering;

public:
  ModelRender( ModelModule* _model );

  virtual ~ModelRender();

  ModelModule* getModel() const
    {
    return model;
    }

  MLPicture* getPicture() const
    {
    return picture;
    }

  void init();

  // return false if unable to start
  boolean run();

  void wakeup();

  boolean isRendering() const
    {
    return _isRendering;
    }

protected:
  MLString seconds_to_string( double s ) const;
  MLString getStatsLine( int j, double dt ) const;

public:
  boolean save();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelRender_h
