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
  KBCanvas.C

  Stephane Rehel

  October 19 1997
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "KBCanvas.h"
#include "KBModule.h"

#include "engines/KBECanvas.h"

#include "kbuilder/appcore/Canvas.h"

/////////////////////////////////////////////////////////////////////////////

KBCanvas::KBCanvas( KBModule* _kbuilder ): MLCanvas(_kbuilder)
{
  kbuilder= _kbuilder;

  canvasEngine= 0;

  builder_canvas= 0;
}

/////////////////////////////////////////////////////////////////////////////

KBCanvas::~KBCanvas()
{
  delete builder_canvas;
  builder_canvas= 0;

  canvasEngine= 0;
}

/////////////////////////////////////////////////////////////////////////////

void KBCanvas::create(  int _where )
{
  MLCanvas::create(_where);

  assert( MLCanvas::getGfxWindow() != 0 );
  builder_canvas= new Canvas( MLCanvas::getGfxWindow(), this );

  canvasEngine= new KBECanvas(this);
  canvasEngine->updatePopupsFlags();

  MLCanvas::addEngine( canvasEngine );

  activateDefaultEngine();
}

/////////////////////////////////////////////////////////////////////////////

void KBCanvas::refreshScreen( unsigned what )
{
  if( (what & KBModule::REFRESH_BUILDER) != 0 )
    {
    postRefresh();
    return;
    }

  if( MLCanvas::engine == 0 )
    return;

  ((KBEngine*)MLCanvas::engine)->refreshScreen(what);
}

/////////////////////////////////////////////////////////////////////////////

Canvas* KBCanvas::getBuilderCanvas() const
{
  return builder_canvas;
}

/////////////////////////////////////////////////////////////////////////////

void KBCanvas::write( MLWFileBlock& block )
{
  MLCanvas::write(block);
  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void KBCanvas::read( MLRFileBlock& block )
{
  MLCanvas::read(block);
  if( block.getInteger() == 0 )
    return;
}

/////////////////////////////////////////////////////////////////////////////

void KBCanvas::activateDefaultEngine()
{
  MLCanvas::activateEngine( canvasEngine->getID() );

/*
  int index= ((KBModule*)module)->getCanvasIndex(this);

  switch( index )
    {
    case 1: MLCanvas::activateEngine( redEngine->getID() );
            break;
    case 2: MLCanvas::activateEngine( blueEngine->getID() );
            break;
    case 3: MLCanvas::activateEngine( coloredEngine->getID() );
            break;
    case 4: MLCanvas::activateEngine( greenEngine->getID() );
            break;
    default:
      break;
    }
*/
}

/////////////////////////////////////////////////////////////////////////////

void KBCanvas::resetAll()
{
  MLCanvas::resetAll();

  activateDefaultEngine();
}

/////////////////////////////////////////////////////////////////////////////
