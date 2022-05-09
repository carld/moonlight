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
  MLModulesBar.C

  Stephane Rehel

  November 24 1996
*/

#include "MLModulesBar.h"

#include "graphics/mlgl.h"
#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"

#include "Interface.h"
#include "MLModule.h"

#include "MLAllocator.h"

/////////////////////////////////////////////////////////////////////////////

static const int max_modules= 20;
static const int bottom_margin= 3;

/////////////////////////////////////////////////////////////////////////////

MLModulesBar::MLModulesBar( Interface* _interface )
{
  interface= _interface;

  modules= new ModuleInfo [ ::max_modules + 1 ];
  for( int i= 0; i <= ::max_modules; ++i )
    {
    ModuleInfo& m= modules[i];
    m.id= 0;
    m.title= MLString("");
    m.color= Color(0.05,0.05,0.25);
    m.rect= IRectangle( IPoint(0,0), IPoint(0,0) );
    m.textPos= IPoint(0,0);
    }
  nModules= 0;

  font= widgetFont;
}

/////////////////////////////////////////////////////////////////////////////

MLModulesBar::~MLModulesBar()
{
  if( modules != 0 )
    {
    delete [] modules;
    modules= 0;
    }
  nModules= 0;

  interface= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLModulesBar::create()
{
  SoftWindow::create( (MLWindow*) interface->window,
                      Interface::buttonsPanelWidth,
                      0,
                      interface->screenSize.x() - 1
                        - Interface::buttonsPanelWidth
                        - Interface::scrollingDialogsWidth,
                      Interface::modulesBarHeight );
  SoftWindow::map(true);
}

/////////////////////////////////////////////////////////////////////////////

void MLModulesBar::appendModule( int id )
{
  MLModule* module= MLAllocator::getModule(id);
  if( module == 0 )
    return;

  if( nModules >= ::max_modules )
    return;

  MLString title= module->getName();
  Color color= module->getColor();

  ++nModules;
  modules[nModules].id= id;
  modules[nModules].title= title;
  modules[nModules].color= color;

  int margin= font->getCharWidth(' ');
  int left= SoftWindow::getSize().x() - 1 - margin;

  int max_height= SoftWindow::getSize().y() - ::bottom_margin;
  int titleHeight= max_height;

  int y1= (max_height - titleHeight) / 2 + ::bottom_margin;
  int y2= y1 + titleHeight - 1;

  for( int i= nModules; i >= 1; --i )
    {
    ModuleInfo& m= modules[i];
    if( m.title.length() <= 0 )
      {
      m.rect= IRectangle( IPoint(0,0), IPoint(0,0) );
      m.textPos= IPoint(0,0);
      continue;
      }
    int x2= left;
    int x1= left - margin - font->getStringWidth(m.title) - margin;

    if( x1 < 0 )
      {
      x2+= (-x1);
      x1= 0;
      }

    m.rect= IRectangle( IPoint(x1,y1), IPoint(x2,y2) );

    m.textPos= IPoint( x1+margin,
                       y1+(titleHeight-font->getHeight())/2
                         +font->getDescent()+1 );

    left= x1 - 1;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLModulesBar::draw()
{
  currentPixel();
  mlFront();

  Color color= Color(0,0,0);
  int i;

  for( i= nModules; i >= 1; --i )
    {
    ModuleInfo& m= modules[i];
    if( interface->currentModule == m.id )
      {
      color= m.color;
      break;
      }
    }

  mlColor(color);
  mlRect( 0,
          ::bottom_margin,
          SoftWindow::getSize().x()-1,
          SoftWindow::getSize().y()-1 );

  if( ::bottom_margin > 0 )
    {
    mlBlack();
    mlRect( 0, 0, SoftWindow::getSize().x()-1, ::bottom_margin - 1 );
    }

  if( nModules > 0 )
    {
    mlBlack();
    font->print( font->getCharWidth(' ')*2,
                 modules[1].textPos.y()-1,
                 interface->programName );
    }

  for( i= nModules; i >= 1; --i )
    {
    ModuleInfo& m= modules[i];
    if( interface->currentModule == m.id )
      mlWhite();
     else
      mlDarkGray();

    font->print( m.textPos.x(), m.textPos.y()-1, m.title );
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLModulesBar::handleEvent( const MLEvent& event )
{
  switch( event.type )
    {
    case MLEvent::REDRAW:
      draw();
      break;

    case MLEvent::MOUSE_PRESS:
      {
      IPoint position= IPoint( event.position.x(),
                               SoftWindow::getSize().y()-1-event.position.y());
      int i;
      for( i= 1; i <= nModules; ++i )
        {
        if( modules[i].rect.into(position) )
          break;
        }

      if( i <= nModules )
        interface->activateModule(modules[i].id);

      break;
      }

    default:
      return SoftWindow::handleEvent(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
