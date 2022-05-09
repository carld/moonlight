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
  OOMDraw.C

  Stephane Rehel

  December 2 1997
*/

#include "graphics/mlgl.h"
#include "graphics/SystemWindow.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"

#include "oom/OOMColors.h"
#include "oom/OOMModule.h"

#include "oom/model/OOModel.h"
#include "oom/model/OOPackage.h"
#include "oom/model/OOClass.h"
#include "oom/model/OORelation.h"
#include "oom/model/OOConnection.h"

#include "OOMDraw.h"
#include "OOMCamera.h"
#include "OOMECompose.h"

/////////////////////////////////////////////////////////////////////////////

OOMDraw::OOMDraw( OOMECompose* _compose )
{
  compose= _compose;
}

/////////////////////////////////////////////////////////////////////////////

OOMDraw::~OOMDraw()
{}

/////////////////////////////////////////////////////////////////////////////

void OOMDraw::draw()
{
  if( compose == 0 )
    return;

  SystemWindow* window= compose->getGfxWindow();
  if( window == 0 )
    return;

  window->current();
  mlBack();

  OOMCamera& camera= compose->getCamera();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  IVector windowSize= window->getSize();
  int x_res= windowSize.x();
  int y_res= windowSize.y();

  camera.compute( x_res, y_res );

  glMultMatrixd( camera.projection.dump() );

/*
  int screen_x1= 0 - x_res/2 + camera.x;
  int screen_x2= x_res - x_res/2 + camera.x;
  int screen_y1= 0 - y_res/2 + camera.y;
  int screen_y2= y_res - y_res/2 + camera.y;

  glOrtho( double(screen_x1) / double(camera.zoom),
           double(screen_x2) / double(camera.zoom),
           double(screen_y1) / double(camera.zoom),
           double(screen_y2) / double(camera.zoom),
           -1., 1. );
*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glShadeModel(GL_FLAT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glClearColor( OOMColors::wire_background.r(),
                OOMColors::wire_background.g(),
                OOMColors::wire_background.b(),
                1. );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  OOMModule* oom= compose->getOOMModule();
  if( oom == 0 )
    return;

  OOModel* model= oom->getModel();
  if( model == 0 )
    return;

  SIListIterator<OOPackage> lip(model->getPackages());
  while( ! lip.eol() )
    {
    OOPackage* package= lip++;

    drawPackage(package);
    }
}

/////////////////////////////////////////////////////////////////////////////

void OOMDraw::drawPackage( OOPackage* package )
{
  if( package == 0 )
    return;

  SIListIterator<OOClass> lic(package->getClasses());
  while( ! lic.eol() )
    {
    OOClass* c= lic++;

    drawClass(c);
    }

  SIListIterator<OORelation> lir(package->getRelations());
  while( ! lir.eol() )
    {
    OORelation* r= lir++;

    drawRelation(r);
    }
}


/////////////////////////////////////////////////////////////////////////////

void OOMDraw::drawClass( OOClass* c )
{
  if( c == 0 )
    return;

  if( c->selected() )
    mlColor(OOMColors::selected);
   else
    mlColor(OOMColors::unselected);

  mlFrame( c->draw.x1, c->draw.y1, c->draw.x2, c->draw.y2 );

  int center_x= (c->draw.x1 + c->draw.x2) / 2;
  int center_y= (c->draw.y1 + c->draw.y2) / 2;

}

/////////////////////////////////////////////////////////////////////////////

void OOMDraw::drawRelation( OORelation* relation )
{
  if( relation == 0 )
    return;

  if( relation->selected() )
    mlColor(OOMColors::selected);
   else
    mlColor(OOMColors::unselected);

  int nConnections= relation->getNConnections();
  switch( nConnections )
    {
    case 1: break;
    case 2:
      {
      break;
      }
    default:
      break;
    }

  // draw connections
  for( int i= 1; i <= nConnections; ++i )
    {
    OOConnection* c= relation->getConnection(i);
    if( c == 0 )
      continue;

    if( relation->selected() )
      mlColor(OOMColors::selected);
     else
      mlColor(OOMColors::unselected);

    mlRect( c->point.x() - 3, c->point.y() - 3,
            c->point.x() + 3, c->point.y() + 3 );
    }
}

/////////////////////////////////////////////////////////////////////////////
