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
  ModelFunctions.C

  Stephane Rehel

  August 2 1997
*/

#include "graphics/KeyCodes.h"

#include "interface/MLFunctions.h"

#include "ModelFunctions.h"
#include "ModelModule.h"

#include "functions/MLFSelect.h"
#include "functions/MLFGetInfo.h"
#include "functions/MLFHideUnhide.h"
#include "functions/MLFDuplicate.h"
#include "functions/MLFInstantiate.h"
#include "functions/MLFDelete.h"
#include "functions/MLFTranslate.h"
#include "functions/MLFTranslateXYZ.h"
#include "functions/MLFScale.h"
#include "functions/MLFScaleXYZ.h"
#include "functions/MLFRotate.h"
#include "functions/MLFRotateXYZ.h"
#include "functions/MLFCameraOrbit.h"
#include "functions/MLFCameraZoom.h"
#include "functions/MLFCameraWalk.h"
#include "functions/MLFUndoRedo.h"
#include "functions/MLFRefreshScreen.h"
#include "functions/MLFReparent.h"
#include "functions/MLFOrientObject.h"
#include "functions/MLFTagVertex.h"
#include "functions/MLFTagPolygon.h"
#include "functions/MLFEditMaterial.h"

/////////////////////////////////////////////////////////////////////////////

// static
void ModelFunctions::init( ModelModule* model )
{
  if( model == 0 )
    return;
  MLFunctions* functions= model->getFunctions();

  if( functions == 0 )
    return;

#define add(id,function,key) functions->add( id, Key(key), new function(model) )

  add( SELECT,        MLFSelect,       ' ' );
  add( GET_INFO,      MLFGetInfo,      'i' );
  add( HIDE_UNHIDE,   MLFHideUnhide,   'h' );
  add( DUPLICATE,     MLFDuplicate,    'd' );
  add( INSTANTIATE,   MLFInstantiate,  KeyCodes::ALT_D );
  add( DELETE,        MLFDelete,       KeyCodes::BACKSPACE );
  add( TRANSLATE,     MLFTranslate,    'v' );
  add( TRANSLATE_XYZ, MLFTranslateXYZ, KeyCodes::ALT_V );
  add( SCALE,         MLFScale,        'x' );
  add( SCALE_XYZ,     MLFScaleXYZ,     KeyCodes::ALT_X );
  add( ROTATE,        MLFRotate,       'c' );
  add( ROTATE_XYZ,    MLFRotateXYZ,    KeyCodes::ALT_C );
  add( REPARENT,      MLFReparent,     'f' );

  add( CAMERA_ORBIT,  MLFCameraOrbit,  'o' );
  add( CAMERA_ZOOM,   MLFCameraZoom,   'z' );
  add( CAMERA_WALK,   MLFCameraWalk,   'p' );

  add( UNDO_REDO,     MLFUndoRedo,     'u' );
  add( REFRESH_SCREEN,MLFRefreshScreen,'r' );

  add( ORIENT_OBJECT, MLFOrientObject, 'C' );

  add( TAG_VERTEX,    MLFTagVertex ,   't' );
  add( TAG_POLYGON,   MLFTagPolygon ,  'g' );

  add( EDIT_MATERIAL, MLFEditMaterial, 'a' );
#undef add

//  add( 'm', MOVE,     motion,"Move" );
}

/////////////////////////////////////////////////////////////////////////////
