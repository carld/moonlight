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
  MLMDrag.C

  Stephane Rehel

  August 2 1997
*/

#include "interface/MLMode.h"
#include "interface/MLModes.h"

#include "kmodel/ModelModule.h"

#include "MLMDrag.h"

/////////////////////////////////////////////////////////////////////////////

MLMDrag::MLMDrag( ModelModule* _model ):
  MLMode(_model)
{
  button= 0;
  v= Vector(0,0,0);
  screenVector= Vector2(0,0);
  position1_x= 0;
  position1_y= 0;
  position2_x= 0;
  position2_y= 0;
  growing= true;
  view_vector= Vector(0,0,-1);
  ortho_camera= false;

  name= "Drag";
  action= MLMode::MOTION_ACTION;
}

/////////////////////////////////////////////////////////////////////////////

MLMDrag::~MLMDrag()
{
  button= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLMDrag::enter()
{
  v= Vector(0,0,0);
  screenVector= Vector2(0,0);
  position1_x= 0;
  position1_y= 0;
  position2_x= 0;
  position2_y= 0;
  growing= true;
  view_vector= Vector(0,0,-1);
  ortho_camera= false;
}

/////////////////////////////////////////////////////////////////////////////

/*
  // shortcuts
  MLMode::ACTION motion= MLMode::MOTION_ACTION;
  MLMode::ACTION rect= MLMode::RECT_ACTION;

#define add(mode,a,msg) m->set(int(mode),msg,a)
  add( SELECT,   rect,  "Select: L(object)  M(object+children)" );

  add( SCALE_XYZ,     motion,"Scale: L(x)  M(y)  R(z)" );
  add( ROTATE_XYZ,    motion,"Rotate: L(x)  M(y)  R(z)" );
  add( TRANSLATE_XYZ, motion,"Translate: L(x)  M(y)  R(z)" );
  add( SCALE,             motion,"XYZ scale" );
  add( TRANSLATE,         motion,"Translate (drag)" );

  add( CAMERA_ORBIT,motion,"Camera Orbit: L(both)  M(left/right)  R(down/up)" );
  add( CAMERA_WALK, motion,"Camera Walk" );
  add( CAMERA_ZOOM, motion,"Camera Zoom: L(Pan)  M(zoom)  R(zoom)" );

  add( REPARENT, rect,"Select Child to re-parent" );

  add( MOVE,     motion,"Move" );
  add( HIDE_UNHIDE, rect,"Hide/Show" );
  add( GET_INFO, rect,"Get object information" );

  add( DUPLICATE, rect,"Duplicate" );
  add( DELETE, rect,"Delete" );

  add( UNDO_REDO, MLMode::CLICKED_MOTION_ACTION,
            "Undo/Redo:  L(undo)  R(Redo)" );

  add( REFRESH_SCREEN, MLMode::CLICKED_MOTION_ACTION, "" );
*/
