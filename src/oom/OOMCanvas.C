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
  OOMCanvas.C

  Stephane Rehel

  October 19 1997
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "OOMCanvas.h"
#include "OOMModule.h"

#include "engines/OOMECompose.h"

/////////////////////////////////////////////////////////////////////////////

OOMCanvas::OOMCanvas( OOMModule* _oom ): MLCanvas(_oom)
{
  oom= _oom;

  composeEngine= 0;
}

/////////////////////////////////////////////////////////////////////////////

OOMCanvas::~OOMCanvas()
{
  composeEngine= 0;
}

/////////////////////////////////////////////////////////////////////////////

void OOMCanvas::create( int _where )
{
  MLCanvas::create(_where);

  composeEngine= new OOMECompose(this);
  composeEngine->updatePopupsFlags();

  MLCanvas::addEngine( composeEngine );

  activateDefaultEngine();
}

/////////////////////////////////////////////////////////////////////////////

void OOMCanvas::refreshScreen( unsigned what )
{
  if( MLCanvas::engine == 0 )
    return;

  ((OOMEngine*)MLCanvas::engine)->refreshScreen(what);
}

/////////////////////////////////////////////////////////////////////////////

void OOMCanvas::write( MLWFileBlock& block )
{
  MLCanvas::write(block);
  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void OOMCanvas::read( MLRFileBlock& block )
{
  MLCanvas::read(block);
  if( block.getInteger() == 0 )
    return;
}

/////////////////////////////////////////////////////////////////////////////

void OOMCanvas::activateDefaultEngine()
{
  int index= ((OOMModule*)module)->getCanvasIndex(this);

  switch( index )
    {
    case 1: MLCanvas::activateEngine( composeEngine->getID() );
            break;
    case 2: MLCanvas::activateEngine( composeEngine->getID() );
            break;
    case 3: MLCanvas::activateEngine( composeEngine->getID() );
            break;
    case 4: MLCanvas::activateEngine( composeEngine->getID() );
            break;

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void OOMCanvas::resetAll()
{
  MLCanvas::resetAll();

  activateDefaultEngine();
}

/////////////////////////////////////////////////////////////////////////////
