// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  GraphicsSystem.C

  Stephane Rehel
  June 28, 1996
*/

#include <stdio.h>

#include "GL/opengl.h"

#include "tools/Color.h"
#include "tools/Color8.h"
#include "tools/MLCommandLine.h"
#include "tools/MLGammaCorrection.h"
#include "tools/MLConfig.h"

#include "GraphicsSystem.h"

#include "Key.h"
#include "SystemWindow.h"

#include "MLFonts.h"
#include "MLPixmaps.h"
#include "MLCursors.h"

/////////////////////////////////////////////////////////////////////////////

// static
GraphicsSystem* GraphicsSystem::graphicsSystem= 0; // THE graphics system

// usd in mlgl.h
unsigned char gray_level_0= 0,
              gray_level_1= 85,
              middle_gray = 128,
              gray_level_2= 170,
              gray_level_3= 255;

static boolean tag= false;

/////////////////////////////////////////////////////////////////////////////

GraphicsSystem::GraphicsSystem()
{
  rc_group= "Moonlight";

  assert( GraphicsSystem::graphicsSystem == 0 );
  GraphicsSystem::graphicsSystem= this;

  reallyFullScreen= true;
  gammaCorrection= 0;
  _doRedrawEvents= false;//true;
  _supportSaveUnder= false;
  _isIconic= false;

  Key::init_KeyCodes();

  rootWindow= 0;
}

/////////////////////////////////////////////////////////////////////////////

GraphicsSystem::~GraphicsSystem()
{
  tag= false;

  delete gammaCorrection;
  gammaCorrection= 0;

  delete rootWindow;
  rootWindow= 0;
}

/////////////////////////////////////////////////////////////////////////////

SystemWindow* GraphicsSystem::newFullScreenWindow()
{
  assert( rootWindow != 0 );

  IVector size= getScreenSize();

  return rootWindow->newSystemChild( 0, 0, size.x(), size.y() );
}

/////////////////////////////////////////////////////////////////////////////

boolean GraphicsSystem::init( MLConfig* config, MLCommandLine& options )
{
  double gamma= 1.;
  double rc_gamma;
  if( config->getRealVariable(rc_group.get(),"Gamma",rc_gamma) )
    gamma= rc_gamma;

  if( options.findOption("-gamma") )
    gamma= options.getDouble();

  gammaCorrection= new MLGammaCorrection(gamma);

  gray_level_0= gammaCorrection->do_ub(gray_level_0);
  gray_level_1= gammaCorrection->do_ub(gray_level_1);
  middle_gray = gammaCorrection->do_ub(middle_gray );
  gray_level_2= gammaCorrection->do_ub(gray_level_2);
  gray_level_3= gammaCorrection->do_ub(gray_level_3);

  assert( rootWindow == 0 );

  if( ! _supportSaveUnder )
    _doRedrawEvents= true;

  IVector size= getScreenSize();

  if( size.x() < 1000 )
    {
    fprintf( stderr, "Please use resolution 1024x768 or higher.\n" );
    fprintf( stderr, "Press Enter to continue, Ctrl-c to stop.\n" );
    getchar();
    }

  MLCursors::init();

  rootWindow= new SystemWindow;
  if( ! rootWindow->create( 0,
                            0, 0, size.x(), size.y() ) )
    {
    fprintf( stderr, "unable to create main window\n" );
    delete rootWindow;
    rootWindow= 0;
    return false;
    }

  rootWindow->map();
  rootWindow->createContext();
  rootWindow->currentPixel();

  initDefaultLists();

  glDrawBuffer(GL_FRONT);
  glFlush();

#ifdef __X11__
  if( ! _doRedrawEvents )
    {
    // now create and remove a dummy window in order to generate
    // a dummy repaint event
    SystemWindow* dummyW= new SystemWindow;
    dummyW->create( rootWindow, 0, 0, 10, 10 );
    dummyW->map();
    glFlush();

    delete dummyW;
    dummyW= 0;
    }
#endif

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean GraphicsSystem::hasGammaCorrection() const
{
  if( gammaCorrection == 0 )
    return false;
  return gammaCorrection->hasCorrection();
}

/////////////////////////////////////////////////////////////////////////////

boolean GraphicsSystem::initDefaultLists()
{
  boolean ok1= MLFonts::init();
  boolean ok2= MLPixmaps::init();

  return ok1 && ok2;
}

/////////////////////////////////////////////////////////////////////////////

void GraphicsSystem::iconify( boolean yes )
{
  if( (yes?1:0) == (_isIconic?1:0) )
    return;

  _iconify(yes);

  _isIconic= yes;

  autoRepeat(yes);
}

/////////////////////////////////////////////////////////////////////////////

void GraphicsSystem::setApplicationName( const char* )
{}

/////////////////////////////////////////////////////////////////////////////

void GraphicsSystem::refreshScreen()
{
  IPoint p1(0,0);
  IPoint p2= p1 + getScreenSize() + IVector(-1,-1);

  _invalidateRect( p1, p2, getRootWindow() );
}

/////////////////////////////////////////////////////////////////////////////

void GraphicsSystem::invalidateRect( SystemWindow* sw )
{
  if( sw == 0 || !_doRedrawEvents )
    return;

  IPoint p1= sw->getAbsolutePosition();
  IPoint p2= p1 + sw->getSize() + IVector(-1,-1);
  IVector screenSize= getScreenSize();

  p1[0]= max( 0, min( screenSize.x(), p1[0] ) );
  p1[1]= max( 0, min( screenSize.y(), p1[1] ) );
  p2[0]= max( 0, min( screenSize.x(), p2[0] ) );
  p2[1]= max( 0, min( screenSize.y(), p2[1] ) );

  if( p1.x() > p2.x() || p1.y() > p2.y() )
    return;

  _invalidateRect( p1, p2, getRootWindow() );
  
}

/////////////////////////////////////////////////////////////////////////////

// private
void GraphicsSystem::_invalidateRect( const IPoint& p1, const IPoint& p2,
                                      SystemWindow* sw )
{
  if( sw == 0 )
    return;

  if( ! sw->mapped() )
    return;

  IPoint wp1= sw->getAbsolutePosition();
  IPoint wp2= wp1 + sw->getSize() + IVector(-1,-1);

  if( p2.x() < wp1.x() || p1.x() > wp2.x() ||
      p2.y() < wp1.y() || p1.y() > wp2.y() )
    return;

  sw->postRedraw();

  IListIterator<SystemWindow> li(sw->systemWindows);
  while( ! li.eol() )
    {
    SystemWindow* child= li++;
    if( child == 0 )
      continue;
    if( ! child->mapped() )
      continue;
    _invalidateRect(p1,p2,child);
    }
    glFlush();
}

/////////////////////////////////////////////////////////////////////////////

void GraphicsSystem::pushWaitCursor()
{
  if( rootWindow != 0 )
    rootWindow->pushWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////

void GraphicsSystem::popWaitCursor()
{
  if( rootWindow != 0 )
    rootWindow->popWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////

MLWindow* GraphicsSystem::findCursorWindow( const IPoint& p )
{
  if( rootWindow == 0 )
    return 0;

  return rootWindow->findCursorWindow(p);
}

/////////////////////////////////////////////////////////////////////////////
