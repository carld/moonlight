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
  MLCSetXForms.C

  Stephane Rehel

  September 21 1997
*/

#include "scene/MLObject.h"
#include "scene/MLScene.h"
#include "scene/lighting/MLGlobalLighting.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommands.h"

#include "MLCSetXForms.h"

/////////////////////////////////////////////////////////////////////////////

MLCSetXForms::MLCSetXForms()
{
  type= ModelCommands::SET_TRANSFORMATIONS;

  xf= 0;
  nObjects= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLCSetXForms::~MLCSetXForms()
{
  delete xf;
  xf= 0;

  nObjects= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLCSetXForms::alloc( int _nObjects )
{
  delete xf;
  xf = new XF [ _nObjects + 1 ];
  nObjects= _nObjects;

  for( int i= 0; i <= nObjects; ++i )
    {
    xf[i].object_index= -1;
    xf[i].scale= Vector(1,1,1);
    xf[i].rotate= Vector(0,0,0);
    xf[i].translate= Vector(0,0,0);
    xf[i].set_preXForm= false;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLCSetXForms::make( MLScene* scene, IntegerArray& indices )
{
  alloc( indices.getSize() );

  for( int i= 1; i <= indices.getSize(); ++i )
    {
    int index= indices[i-1];
    MLObject* o= scene->getObject(index);
    if( o == 0 )
      {
      xf[i].object_index= -1;
      continue;
      }

    xf[i].object_index= index;
    xf[i].scale= o->getScale();
    xf[i].rotate= o->getRotate();
    xf[i].translate= o->getTranslate();
    xf[i].set_preXForm= false;
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCSetXForms::doit( ModelModule* model )
{
  MLScene* scene= model->getScene();

  model->stopLightingProcess();

  for( int i= 1; i <= nObjects; ++i )
    {
    MLObject* o= scene->getObject(xf[i].object_index);
    if( o == 0 )
      return true;

    o->scale(xf[i].scale);
    o->rotate(xf[i].rotate);
    o->translate(xf[i].translate);

    if( xf[i].set_preXForm )
      o->local_xform.preXForm= xf[i].preXForm;

    scene->getGlobalLighting()->object_is_transformed(o);
    }

  scene->updateTransformations();

  model->refreshScreen( ModelModule::REFRESH_TRANSFORM |
                        ModelModule::REFRESH_GEOMETRY |
                        ModelModule::REFRESH_DIALOGS );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

