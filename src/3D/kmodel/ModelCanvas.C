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
  ModelCanvas.C

  Stephane Rehel
  November 30 1996
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "ModelCanvas.h"
#include "ModelModule.h"

#include "engines/MLEPerspective.h"
#include "engines/MLEOrthoAxis.h"
#include "engines/MLERay.h"

/////////////////////////////////////////////////////////////////////////////

ModelCanvas::ModelCanvas( ModelModule* _model ): MLCanvas(_model)
{
  model= _model;

  perspective_Engine= 0;
  orthoX_Engine     = 0;
  orthoY_Engine     = 0;
  orthoZ_Engine     = 0;
  ray_Engine        = 0;
}

/////////////////////////////////////////////////////////////////////////////

ModelCanvas::~ModelCanvas()
{}

/////////////////////////////////////////////////////////////////////////////

void ModelCanvas::create( int _where )
{
  MLCanvas::create(_where);

  perspective_Engine= new MLEPerspective(this);
  perspective_Engine->updatePopupsFlags();

  orthoX_Engine     = new MLEOrthoAxis(this,MLEOrthoAxis::X_AXIS);
  orthoX_Engine->updatePopupsFlags();

  orthoY_Engine     = new MLEOrthoAxis(this,MLEOrthoAxis::Y_AXIS);
  orthoY_Engine->updatePopupsFlags();

  orthoZ_Engine     = new MLEOrthoAxis(this,MLEOrthoAxis::Z_AXIS);
  orthoZ_Engine->updatePopupsFlags();

  ray_Engine        = new MLERay(this);
  ray_Engine->updatePopupsFlags();

  MLCanvas::addEngine( perspective_Engine );
  MLCanvas::addEngine( orthoX_Engine );
  MLCanvas::addEngine( orthoY_Engine );
  MLCanvas::addEngine( orthoZ_Engine );
  MLCanvas::addEngine( ray_Engine );

  activateDefaultEngine();
}

/////////////////////////////////////////////////////////////////////////////

const MLCamera& ModelCanvas::getPerspectiveCamera() const
{
  return perspective_Engine->getCamera();
}

/////////////////////////////////////////////////////////////////////////////

void ModelCanvas::refreshScreen( unsigned what )
{
  if( MLCanvas::engine == 0 )
    return;

  ((ModelEngine*)MLCanvas::engine)->refreshScreen(what);
}

/////////////////////////////////////////////////////////////////////////////

void ModelCanvas::refreshMainCamera( const MLCamera& camera )
{
  for( int i= 0; i < nEngines; ++i )
    {
    ModelEngine* engine= (ModelEngine*) engines[i];
    engine->refreshMainCamera(camera);
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelCanvas::write( MLWFileBlock& block )
{
  MLCanvas::write(block);
  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void ModelCanvas::read( MLRFileBlock& block )
{
  MLCanvas::read(block);
  if( block.getInteger() == 0 )
    return;
}

/////////////////////////////////////////////////////////////////////////////

void ModelCanvas::activateDefaultEngine()
{
  int index= ((ModelModule*)module)->getCanvasIndex(this);
  switch( index )
    {
    case 1: MLCanvas::activateEngine( orthoY_Engine->getID() );
            break;
    case 2: MLCanvas::activateEngine( perspective_Engine->getID() );
            perspective_Engine->setMainCameraEngine();
            break;
    case 3: MLCanvas::activateEngine( orthoZ_Engine->getID() );
            break;
    case 4: MLCanvas::activateEngine( orthoX_Engine->getID() );
            break;
    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelCanvas::resetAll()
{
  MLCanvas::resetAll();

  activateDefaultEngine();
}

/////////////////////////////////////////////////////////////////////////////
