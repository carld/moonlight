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
  MLDModesList.C

  Stephane Rehel

  December 31 1997
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "interface/MLFunctionsManager.h"
#include "interface/MLFunction.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelFunctions.h"

#include "MLDModesList.h"
#include "ModesListDlg.h"

#include "kw/KWDialog.h"

/////////////////////////////////////////////////////////////////////////////

struct FUNCTION
  {
  char* key;
  char* name;
  int key_id;
  int id;
  KWidget kwidget;
  KWidget key_kwidget;
  };
static const int max_functions= 128;
static FUNCTION functions[max_functions+1];
static int nFunctions= 0;

/////////////////////////////////////////////////////////////////////////////

static void add( char* key, char* name,
                 int id, KWidget kwidget, KWidget key_kwidget )
{
  if( nFunctions >= max_functions )
    {
    fprintf( stderr, "too many functions in MLDFunctionsList.C !\n" );
    exit(1);
    }
  ++nFunctions;
  functions[nFunctions].key= key;
  functions[nFunctions].name= name;
  functions[nFunctions].id= id;
  functions[nFunctions].kwidget= kwidget;
  functions[nFunctions].key_kwidget= key_kwidget;
}

/////////////////////////////////////////////////////////////////////////////

static FUNCTION* find_function( MLFunction* f )
{
  if( nFunctions == 0 )
    return 0;

  if( f == 0 )
    return functions+1;

  for( int i= 1; i <= nFunctions; ++i )
    if( functions[i].id == f->getID() )
      return functions+i;

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

MLDModesList::MLDModesList( ModelModule* _model ):
  MLScrollingDialog(_model)
{
  model= _model;
  data= ModesListDlg::data;
  name= "ModesList";

  nFunctions= 0;
  add(       "Esc", "None",            0,                             ModesListDlg::none,          ModesListDlg::l_none );
  add(         "u", "Undo/Redo",       ModelFunctions::UNDO_REDO,     ModesListDlg::undo,          ModesListDlg::l_undo );
  add(         "o", "Orbit",           ModelFunctions::CAMERA_ORBIT,  ModesListDlg::orbit,         ModesListDlg::l_orbit );
  add(         "p", "Walk",            ModelFunctions::CAMERA_WALK,   ModesListDlg::walk,          ModesListDlg::l_walk );
  add(         "z", "Zoom/Pan",        ModelFunctions::CAMERA_ZOOM,   ModesListDlg::zoom,          ModesListDlg::l_zoom );
  add(     "Space", "Select",          ModelFunctions::SELECT,        ModesListDlg::select,        ModesListDlg::l_select );
  add( "Backspace", "Delete",          ModelFunctions::DELETE,        ModesListDlg::_delete,       ModesListDlg::l_delete );
  add(         "h", "Hide/Unhide",     ModelFunctions::HIDE_UNHIDE,   ModesListDlg::hide,          ModesListDlg::l_hide );
  add(         "i", "Get information", ModelFunctions::GET_INFO,      ModesListDlg::get_info,      ModesListDlg::l_get_info );
  add(         "d", "Duplicate",       ModelFunctions::DUPLICATE,     ModesListDlg::duplicate,     ModesListDlg::l_duplicate );
  add(     "Alt-d", "Instantiate",     ModelFunctions::INSTANTIATE,   ModesListDlg::instantiate,   ModesListDlg::l_instantiate );
  add(         "f", "Reparent",        ModelFunctions::REPARENT,      ModesListDlg::reparent,      ModesListDlg::l_reparent );
  add(         "v", "Translate",       ModelFunctions::TRANSLATE,     ModesListDlg::translate,     ModesListDlg::l_translate );
  add(     "Alt-v", "Translate XYZ",   ModelFunctions::TRANSLATE_XYZ, ModesListDlg::translate_xyz, ModesListDlg::l_translate_xyz );
  add(         "x", "Scale",           ModelFunctions::SCALE,         ModesListDlg::scale,         ModesListDlg::l_scale );
  add(     "Alt-x", "Scale XYZ",       ModelFunctions::SCALE_XYZ,     ModesListDlg::scale_xyz,     ModesListDlg::l_scale_xyz );
  add(         "c", "Rotate",          ModelFunctions::ROTATE,        ModesListDlg::rotate,        ModesListDlg::l_rotate );
  add(     "Alt-c", "Rotate XYZ",      ModelFunctions::ROTATE_XYZ,    ModesListDlg::rotate_xyz,    ModesListDlg::l_rotate_xyz );
  add(         "C", "Orient",          ModelFunctions::ORIENT_OBJECT, ModesListDlg::orient,        ModesListDlg::l_orient );
  add(         "t", "Tag vertices",    ModelFunctions::TAG_VERTEX,    ModesListDlg::tag_vertices,  ModesListDlg::l_tag_vertices );
  add(         "g", "Tag polygons",    ModelFunctions::TAG_POLYGON,   ModesListDlg::tag_polygons,  ModesListDlg::l_tag_polygons );
  add(         "a", "Edit material",   ModelFunctions::EDIT_MATERIAL, ModesListDlg::edit_material, ModesListDlg::l_edit_material );

  MLScrollingDialog::updateFlags= ModelModule::REFRESH_DIALOGS;
}

/////////////////////////////////////////////////////////////////////////////

MLDModesList::~MLDModesList()
{}

/////////////////////////////////////////////////////////////////////////////

void MLDModesList::update()
{
  MLFunctionsManager* functions_manager= model->getFunctionsManager();
  if( functions_manager == 0 )
    return;

  MLFunction* f= functions_manager->getCurrentFunction();

  update(f);
}

/////////////////////////////////////////////////////////////////////////////

void MLDModesList::update( MLFunction* f )
{
  FUNCTION* fct= find_function(f);

  if( fct == 0 )
    return;

  kwDialog->setChecked(fct->kwidget);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDModesList::kwCallback( KWEvent& event )
{
  switch( event.type )
    {
    case KWEvent::SELECT:
      {
      FUNCTION* fct= 0;
      for( int i= 1; i <= nFunctions; ++i )
        if( functions[i].kwidget == event.widget )
          {
          fct= functions+i;
          break;
          }

      if( fct == 0 )
        return true;

      model->enterFunction(fct->id);
      return true;
      }

    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLDModesList::justCreated()
{
  if( kwDialog == 0 || nFunctions == 0 )
    return;

  for( int i= 1; i <= nFunctions; ++i )
    {
    FUNCTION& f= ::functions[i];

    kwDialog->setText( f.key_kwidget, f.key );
    kwDialog->setText( f.kwidget,     f.name );
    }

  kwDialog->setChecked(functions[0].kwidget);
}

/////////////////////////////////////////////////////////////////////////////
