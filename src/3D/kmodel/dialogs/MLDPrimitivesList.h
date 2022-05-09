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
  MLDPrimitivesList.h

  Stephane Rehel

  January 2 1998
*/

#ifndef __MLDPrimitivesList_h
#define __MLDPrimitivesList_h

#ifndef __MLScrollingDialog_h
#include "interface/MLScrollingDialog.h"
#endif

class ModelModule;
class MLObject;
//class IntegerArray;

/////////////////////////////////////////////////////////////////////////////

class MLDPrimitivesList: public MLScrollingDialog
{
protected:
  ModelModule* model;
//  IntegerArray* list;

public:
  MLDPrimitivesList( ModelModule* _model );

  virtual ~MLDPrimitivesList();

public:
  void update();

private:
  // recursive
  void _buildList( int folder_id, MLObject* o );
  void buildList();

  void updateList();
  void checkNameAvailability();

public:
  virtual boolean kwCallback( KWEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDPrimitivesList_h
