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
  MLDPool.h

  Stephane Rehel

  Mqrch 6 1997
*/

#ifndef __MLDPool_h
#define __MLDPool_h

#ifndef __MLScrollingDialog_h
#include "interface/MLScrollingDialog.h"
#endif

class ModelModule;
class MLColorWidget;
class MLLightPool;

/////////////////////////////////////////////////////////////////////////////

class MLDPool: public MLScrollingDialog
{
protected:
  ModelModule* model;
  MLColorWidget* colorWidget;

public:
  MLDPool( ModelModule* _model );

  virtual ~MLDPool();

protected:
  MLLightPool* getSelectedPool();

public:
  void update();

protected:
  void updateList();

public:
  virtual boolean kwCallback( KWEvent& );

protected:
  virtual void justCreated();

  void apply_button();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDPool_h
