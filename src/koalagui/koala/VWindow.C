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
   VWindow.C

   Creation: SR, July 24th, 1995
   Revisions:
     + SR, November 21st 1995 (add `void VWindow::closeWindow()')
     + SR, February 8th 1996: found the SGI OpenGL trick...
     + SR, July 5 1996
*/

#include <stdio.h>
#include <assert.h>

#include "GL/opengl.h"

#include "VWindow.h"

#include "tools/MLString.h"

#include "VOptions.h"
#include "VWindowEventHandler.h"

#include "graphics/SoftWindow.h"
#include "graphics/SystemWindow.h"
#include "graphics/MLEventManager.h"
#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"
#include "graphics/MLPixmap.h"
#include "graphics/MLGLPixmap.h"

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

VWindow::VWindow( MLWindow* _systemParent )
{
  assert( _systemParent != 0 );

  parent= 0;
  systemParent= _systemParent;
  init();
}

/////////////////////////////////////////////////////////////////////////////

VWindow::VWindow( VWindow* _parent )
{
  assert( _parent != 0 );

  parent= _parent;
  systemParent= _parent->systemWindow;
  init();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::init()
{
  assert( parent != 0 || systemParent != 0 );

  ownImage= false;
  if( parent == 0 )
    image= 0;
   else
    image= parent->image;
  windowImagePosition= IPoint(0,0);
  frontMode= true;

  if( vOptions == 0 )
    {
    vOptions= new VOptions;
    vOptions->init();
    }

  font= widgetFont;
  backgroundColor= Color8(0,0,0);

  veventHandler= 0;

  systemWindow= new SoftWindow;

  isMapped= false;
}

/////////////////////////////////////////////////////////////////////////////

VWindow::~VWindow()
{
  while( ! children.empty() )
    {
    SIListElement<VWindow>* ile= children.getLastElement();
    VWindow* child= ile->element;
    children.remove(ile);
    delete child;
    }

  font= 0;

  if( parent != 0 )
    {
    parent->removeChild(this);
    parent= 0;
    }

  MLEventManager* eventManager= MLEventManager::getEventManager();
  if( eventManager != 0 )
    eventManager->removeEvents(systemWindow);

  delete systemWindow;
  systemWindow= 0;

  if( image != 0 )
    {
    if( ownImage )
      delete image;
    image= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

// protected
void VWindow::addChild( VWindow* child )
{
  assert( child != 0 );

  assert( ! children.exist(child) );
  children.cons( child );
}

/////////////////////////////////////////////////////////////////////////////

// protected
void VWindow::removeChild( VWindow* child )
{
  assert( child != 0 );

  children.remove( child );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::create( int x, int y, int width, int height )
{
  width= max( 1, width );
  height= max( 1, height );

  boolean outcome= systemWindow->create( systemParent,
                                       x, y, width, height );

  if( ! outcome )
    {
    fprintf( stderr, "Fatal: Could create window pos=(%d,%d), size=(%d,%d)\n",
             x,y,width,height );
    assert(0);
    return;
    }

  if( parent != 0 )
    parent->addChild(this);

  systemWindow->setEventHandler(this);

  if( ownImage || parent == 0 )
    windowImagePosition= IPoint(0,0);
   else
    windowImagePosition= parent->windowImagePosition + getPosition();

//  systemWindow->map();
  isMapped= systemWindow->mapped();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::create( const IPoint& pos, const IVector& size )
{
  create( pos.x(), pos.y(), size.x(), size.y() );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::create( const IPoint& p1, const IPoint& p2 )
{
  const IPoint P1= min(p1,p2);
  const IPoint P2= max(p1,p2);
  const IVector diag= P2 - P1;

  create( P1.x(), P1.y(), diag.x()+1, diag.y()+1 );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::createWindowImage()
{
  if( image != 0 && ownImage )
    return; // already created

  const IVector& size= getSize();
  if( size.x() == 0 || size.y() == 0 )
    return; // window is not created!!

  image= MLGLPixmap::newMLGLPixmap();
  image->create( size.x(), size.y() );
  ownImage= true;

  windowImagePosition= IPoint(0,0);

  SIListIterator<VWindow> li(children);
  while( ! li.eol() )
    li.next()->setWindowImage();
}

/////////////////////////////////////////////////////////////////////////////

// protected
void VWindow::setWindowImage()
{
  if( parent == 0 )
    return; // mmhh..

  if( image != 0 && ownImage )
    delete image;

  ownImage= false;
  image= parent->image;
  windowImagePosition= parent->windowImagePosition + getPosition();

  SIListIterator<VWindow> li(children);
  while( ! li.eol() )
    li.next()->setWindowImage();
}

/////////////////////////////////////////////////////////////////////////////

// public
// prevent user to call 'delete' twice on the same window pointer:
// -> call 'delete' only if window is still in children list
// In addition: set child pointer to 0
void VWindow::deleteChild( VWindow* child )
{
  if( child == 0 )
    return;

  if( children.exist(child) )
    {
    delete child;
    assert( ! children.exist(child) );
    }
}

/////////////////////////////////////////////////////////////////////////////

const IPoint& VWindow::getPosition() const
{
  return systemWindow->getPosition();
}

/////////////////////////////////////////////////////////////////////////////

IPoint VWindow::getAbsolutePosition() const
{
  return systemWindow->getAbsolutePosition();
}

/////////////////////////////////////////////////////////////////////////////

const IVector& VWindow::getSize() const
{
  return systemWindow->getSize();
}

/////////////////////////////////////////////////////////////////////////////

IRectangle VWindow::getRectangle() const
{
  return IRectangle( getPosition(), getSize() );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::setPosition( const IPoint& newPosition )
{
  IVector d= getPosition() - newPosition;
  if( d == IVector(0,0) )
    return;

  systemWindow->setPosition(newPosition);

  if( image != 0 && ! ownImage )
    setWindowImage(); // update windowImagePosition
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::setSize( const IVector& newSize )
{
  systemWindow->setSize(newSize);
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::setEventHandler( VWindowEventHandler* _handler )
{
  veventHandler= _handler;
}

/////////////////////////////////////////////////////////////////////////////

boolean VWindow::eventHandler( MLWindow*, MLEvent& event )
{
  if( veventHandler == 0 )
    return handleEvent(event);
   else
    return veventHandler->VWindowEvent(this,event);
}

/////////////////////////////////////////////////////////////////////////////

// handle one event
// return TRUE if handled
// virtual
boolean VWindow::handleEvent( MLEvent& /*event*/ )
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::removeEvent( int eventType )
{
  MLEventManager* eventManager= MLEventManager::getEventManager();
  if( eventManager != 0 )
    eventManager->removeTyped( (MLWindow*) systemWindow,
                               MLEvent::Type(eventType) );
}

/////////////////////////////////////////////////////////////////////////////

boolean VWindow::findEvent( int eventType )
{
  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager == 0 )
    return 0;

  return eventManager->findTyped( (MLWindow*) systemWindow,
                                  MLEvent::Type(eventType) );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::postPrivateEvent( int eventType )
{
  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager == 0 )
    return;

  MLEvent* event= new MLEvent;

  event->window= VWindow::systemWindow;
  event->privateEvent= true;
  event->type= MLEvent::Type(eventType);

  eventManager->postEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::postRedraw()
{
  boolean ignore_unmapped= (image != 0);
  systemWindow->postRedraw(ignore_unmapped);
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::removeRedraw()
{
  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager != 0 )
    eventManager->removeTyped( systemWindow, MLEvent::REDRAW );
}

/////////////////////////////////////////////////////////////////////////////

/*
// client has received a ButtonPress event
// event must be a ButtonPress event
// wait a moment and test if another ButtonPress event arrives
// then remove all ButtonPress AND ButtonRelease from the X queue
boolean VWindow::isDoubleClick( XEvent* event )
{
  assert( event->type == ButtonPress );

  const Time t1= event->xbutton.time;
  getVDisplay()->delay( getVDisplay()->getOptions()->doubleClickDelay );

  getVDisplay()->getEvents();

  // is there another ButtonPress event ?
  XEvent* e= eventsQueue->peekTyped(ButtonPress);
  boolean yes;
  if( e == 0 )
    yes= FALSE;
   else
    {
    assert( e->type == ButtonPress );
    yes= int(e->xbutton.time - t1)
         <= int(getVDisplay()->getOptions()->doubleClickDelay);
    }

  // let's remove all ButtonPress and ButtonRelease events
  eventsQueue->removeTyped(ButtonPress);
  eventsQueue->removeTyped(ButtonRelease);

  return yes;
}
*/

/////////////////////////////////////////////////////////////////////////////

void VWindow::map( boolean yes /* = true */ )
{
  systemWindow->map(yes);
  isMapped= systemWindow->mapped();
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// GraphicsOp
//
/////////////////////////////////////////////////////////////////////////////

void VWindow::front( boolean yes /* = TRUE */ )
{
  if( image == 0 )
    glDrawBuffer( yes ? GL_FRONT : GL_BACK );
  _front(yes);
}

/////////////////////////////////////////////////////////////////////////////

// private
void VWindow::_front( boolean yes )
{
  frontMode= yes;
  SIListIterator<VWindow> li(children);
  while( ! li.eol() )
    li.next()->_front(yes);
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::back()
{
  front(false);
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::swapBuffers()
{
  if( frontMode || !isMapped )
    return;

  if( image != 0 )
    {
    flush(); // need by windoze opengl's
    const IVector& size= getSize();
    image->print( systemWindow,
                  windowImagePosition.x(), windowImagePosition.y(),
                  size.x(), size.y(),
                  windowImagePosition.x(), windowImagePosition.y() );
    }
   else
    systemWindow->getSystemParent()->swapBuffers();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::putImage()
{
  if( !isMapped || image == 0 )
    return;

  const IVector& size= getSize();
  image->print( systemWindow,
                windowImagePosition.x(), windowImagePosition.y(),
                size.x(), size.y(),
                windowImagePosition.x(), windowImagePosition.y() );
}

/////////////////////////////////////////////////////////////////////////////

boolean VWindow::beginDraw()
{
  if( ! isMapped && image == 0 )
    return false;

  if( image == 0 )
    {
    systemWindow->currentPixel();
    glDrawBuffer( frontMode ? GL_FRONT : GL_BACK );
    }
   else
    {

    const IVector& size= getSize();
    image->current();
    glViewport( windowImagePosition.x(),
                image->getSize().y()-1-(windowImagePosition.y()+size.y()-1),
                size.x(), size.y() );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0., GLfloat(size.x()), 0., GLfloat(size.y()), -1., 1. );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#ifdef __WINDOWS__
    glTranslatef( 0.5, 0.375, 0. );
#else
    glTranslatef( 0.375, 0.375, 0. );
#endif
    glShadeModel(GL_FLAT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::endDraw()
{
  if( ! isMapped && image == 0 )
    return;

  if( image != 0 && frontMode )
    {
    flush(); // need by windoze opengl's
    const IVector& size= getSize();

    image->print( systemWindow,
                  windowImagePosition.x(), windowImagePosition.y(),
                  size.x(), size.y(),
                  0, 0 );
    }

  glFlush();
//  systemWindow->flush();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::flush()
{
  if( isMapped )
    {
#ifdef __WINDOWS__
    GdiFlush();
#endif
    glFlush();
    }
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::scissor()
{
  if( isMapped && systemWindow != 0 )
    systemWindow->scissor();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::unscissor()
{
  if( isMapped && systemWindow != 0 )
    systemWindow->unscissor();
}

/////////////////////////////////////////////////////////////////////////////

Color8 VWindow::getGray0() // black
{
  return vOptions->colorGray0;
}

/////////////////////////////////////////////////////////////////////////////

Color8 VWindow::getGray1()
{
  return vOptions->colorGray1;
}

/////////////////////////////////////////////////////////////////////////////

Color8 VWindow::getGray2()
{
  return vOptions->colorGray2;
}

/////////////////////////////////////////////////////////////////////////////

Color8 VWindow::getGray3() // white
{
  return vOptions->colorGray3;
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::gray0() // black
{
  if( ! isMapped )
    return;

  glColor3ubv( vOptions->colorGray0.getConstCoord() );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::gray1()
{
  if( ! isMapped )
    return;

  glColor3ubv( vOptions->colorGray1.getConstCoord() );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::gray2()
{
  if( ! isMapped )
    return;

  glColor3ubv( vOptions->colorGray2.getConstCoord() );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::gray3() // white
{
  if( ! isMapped )
    return;

  glColor3ubv( vOptions->colorGray3.getConstCoord() );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::setForeground( Color8 color )
{
  if( ! isMapped )
    return;

  glColor3ubv(color.getConstCoord());
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::setBackground( Color8 color )
{
  backgroundColor= color;
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawLine( int x1, int y1, int x2, int y2 )
{
  if( ! isMapped )
    return;

  int s= systemWindow->getSize().y()-1;

  glBegin(GL_LINES);
  glVertex2i(x1,s-y1);
  glVertex2i(x2,s-y2);
  glEnd();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawLine( const IPoint& p1, const IPoint& p2 )
{
  if( ! isMapped )
    return;

  drawLine( p1.x(), p1.y(), p2.x(), p2.y() );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawHorizLine( int x1, int x2, int y )
{
  if( ! isMapped )
    return;

  y= systemWindow->getSize().y()-1 - y;

  glBegin(GL_LINES);
  glVertex2i(x1,y);
  glVertex2i(x2,y);
  glEnd();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawHorizLine( const IPoint& origin, int length )
{
  if( ! isMapped )
    return;

  drawLine( origin.x(), origin.y(), origin.x()+length-1, origin.y() );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawVertLine( int x, int y1, int y2 )
{
  if( ! isMapped )
    return;

  drawLine( x, y1, x, y2 );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawVertLine( const IPoint& origin, int length )
{
  if( ! isMapped )
    return;

  drawLine( origin.x(), origin.y(), origin.x(), origin.y()+length-1 );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawRectangle( const IPoint& p1, const IPoint& p2 )
{
  if( ! isMapped )
    return;

  int s= systemWindow->getSize().y()-1;

  glBegin(GL_LINE_LOOP);
  glVertex2i( p1.x(), s-p1.y() );
  glVertex2i( p2.x(), s-p1.y() );
  glVertex2i( p2.x(), s-p2.y() );
  glVertex2i( p1.x(), s-p2.y() );
  glEnd();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawRectangle( int x, int y, int width, int height )
{
  if( ! isMapped )
    return;

  drawRectangle( IPoint(x,y), IPoint(x+width-1,y+height-1) );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawDotRectangle( const IPoint& p1, const IPoint& p2 )
{
  if( ! isMapped )
    return;

  int s= systemWindow->getSize().y()-1;

  glEnable(GL_LINE_STIPPLE);
  glLineStipple( 1, 0x5555 );

  glBegin(GL_LINE_LOOP);
  glVertex2i( p1.x(), s-p1.y() );
  glVertex2i( p2.x(), s-p1.y() );
  glVertex2i( p2.x(), s-p2.y() );
  glVertex2i( p1.x(), s-p2.y() );
  glEnd();

  glDisable(GL_LINE_STIPPLE);
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::fillRectangle( const IPoint& p1, const IPoint& p2 )
{
  if( ! isMapped )
    return;

  int s= systemWindow->getSize().y()-1;

  register int x1= p1.x();
  register int y1= s - p1.y();
  register int x2= p2.x();
  register int y2= s - p2.y();

  glRecti( x1, y1, x2, y2 );
  glBegin(GL_LINE_LOOP);
  glVertex2i(x1,y1);
  glVertex2i(x2,y1);
  glVertex2i(x2,y2);
  glVertex2i(x1,y2);
  glEnd();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::fillRectangle( int x, int y, int width, int height )
{
  if( ! isMapped )
    return;

  fillRectangle( IPoint(x,y), IPoint(x+width-1,y+height-1) );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::fillGrayRectangle( const IPoint& p1, const IPoint& p2 )
{
  if( ! isMapped )
    return;

  static GLubyte p50[] = {
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55,
    0xaa, 0xaa, 0xaa, 0xaa,    0x55, 0x55, 0x55, 0x55  };

  glEnable( GL_POLYGON_STIPPLE );
  glPolygonStipple( p50 );

  int s= systemWindow->getSize().y()-1;
  register int x1= p1.x();
  register int y1= s - p1.y();
  register int x2= p2.x();
  register int y2= s - p2.y();
  glRecti( x1, y1, x2, y2 );

  glDisable( GL_POLYGON_STIPPLE );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::fillGrayRectangle2( const IPoint& p1, const IPoint& p2 )
{
  if( ! isMapped )
    return;

  static GLubyte p50[] = {
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55,    0xaa, 0xaa, 0xaa, 0xaa };

  glEnable( GL_POLYGON_STIPPLE );
  glPolygonStipple( p50 );

  int s= systemWindow->getSize().y()-1;
  register int x1= p1.x();
  register int y1= s - p1.y();
  register int x2= p2.x();
  register int y2= s - p2.y();
  glRecti( x1, y1, x2, y2 );

  glDisable( GL_POLYGON_STIPPLE );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::fillQuad( const IPoint& p1, const IPoint& p2,
                        const IPoint& p3, const IPoint& p4 )
{
  if( ! isMapped )
    return;

  int s= systemWindow->getSize().y()-1;

  glBegin(GL_QUADS);
  glVertex2i(p1.x(),s-p1.y());
  glVertex2i(p2.x(),s-p2.y());
  glVertex2i(p3.x(),s-p3.y());
  glVertex2i(p4.x(),s-p4.y());
  glEnd();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawTriangle( const IPoint& p1,
                            const IPoint& p2,
                            const IPoint& p3 )
{
  if( ! isMapped )
    return;

  int s= systemWindow->getSize().y()-1;

  glBegin(GL_LINES);
  glVertex2i(p1.x(),s-p1.y());
  glVertex2i(p2.x(),s-p2.y());
  glVertex2i(p3.x(),s-p3.y());
  glEnd();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::fillTriangle( const IPoint& p1,
                            const IPoint& p2,
                            const IPoint& p3 )
{
  if( ! isMapped )
    return;

  int s= systemWindow->getSize().y()-1;

  glBegin(GL_TRIANGLES);
  glVertex2i(p1.x(),s-p1.y());
  glVertex2i(p2.x(),s-p2.y());
  glVertex2i(p3.x(),s-p3.y());
  glEnd();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawPressedRectangle( int x, int y, int width, int height )
{
  if( ! isMapped )
    return;
/*
  gray1();
  drawHorizLine( x, width-2, y );
  drawVertLine( x, y, height-2 );

  gray0();
  drawHorizLine( x+1, width-2, y+1 );
  drawVertLine( x+1, y+1, height-3 );
  gray3();
  drawHorizLine( x, width-1, height-1 );
  drawVertLine( width-1, y, height-1 );
  drawHorizLine( x+1, width-3, y+1 );
  drawVertLine( x+1, y+1, height-3 );
  gray3();
  drawHorizLine( x, width-1, height-1 );
  drawVertLine( width-1, y, height-1 );
*/

  if( ! isMapped )
    return;

  gray1();
  drawHorizLine( x, width-2, y );
  drawVertLine( x, y, height-2 );
  gray0();
  drawHorizLine( x+1, width-3, y+1 );
  drawVertLine( x+1, y+1, height-3 );
  gray3();
  drawHorizLine( x, width-1, height-1 );
  drawVertLine( width-1, y, height-1 );

}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawUnpressedRectangle( int x, int y, int width, int height )
{
  if( ! isMapped )
    return;

  gray3();
  drawHorizLine( x, width-1, y );
  drawVertLine( x, y, height-1 );
  gray1();
  drawHorizLine( x+1, width-1, height-2 );
  drawVertLine( width-2, y+1, height-2 );
  gray0();
int dx= 0;
#ifdef __WINDOWS__
++dx;
#endif
  drawHorizLine( x, width  -1+dx  , height-1 );
  drawVertLine( width-1, y, height-1 );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawPressedRectangle( int width, int height )
{
  drawPressedRectangle( 0, 0, width, height );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawUnpressedRectangle( int width, int height )
{
  drawUnpressedRectangle( 0, 0, width, height );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::fillWindow( Color8 color )
{
  if( ! isMapped )
    return;

  setForeground( color );

  const IVector size= getSize();
  fillRectangle( 0, 0, size.x(), size.y() );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::setFont( MLFont* _font )
{
  font= _font;
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawString( int x, int y, const char* string, int length )
{
  if( ! isMapped )
    return;

  if( string == 0 || length <= 0 )
    return;

  y= systemWindow->getSize().y() - 1 - y;
  font->print( x, y, string, length );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawString( int x, int y, const char* string )
{
  if( ! isMapped )
    return;

  if( string == 0 )
    return;

  drawString( x, y, string, strlen(string) );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawString( const IPoint& pos, const char* string )
{
  if( ! isMapped )
    return;

  drawString( pos.x(), pos.y(), string );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawString( const IPoint& pos, const MLString& string )
{
  if( ! isMapped )
    return;

  drawString( pos.x(), pos.y(), string.get(), string.length() );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawImageString( int x, int y, const char* string, int length )
{
  if( ! isMapped )
    return;

  if( string == 0 || length <= 0 )
    return;

  y= systemWindow->getSize().y() - 1 - y;

  int height= font->getHeight();
  int width= font->getStringWidth(string,length);

  glPushAttrib(GL_CURRENT_BIT); // save current color
  glColor3ubv(backgroundColor.getConstCoord());
  int x1= x;
  int y1= y - font->getDescent();
  int x2= x1+width-1;
  int y2= y1+height-1;
  glRecti( x1, y1, x2, y2 );
  glBegin(GL_LINE_LOOP);
  glVertex2i(x1,y1);
  glVertex2i(x2,y1);
  glVertex2i(x2,y2);
  glVertex2i(x1,y2);
  glEnd();
  glPopAttrib();

  font->print( x, y, string, length );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawImageString( int x, int y, const char* string )
{
  if( ! isMapped )
    return;

  drawImageString( x, y, string, strlen(string) );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawImageString( const IPoint& pos, const MLString& string )
{
  if( ! isMapped )
    return;

  drawImageString( pos.x(), pos.y(), string.get(), string.length() );
}

/////////////////////////////////////////////////////////////////////////////

int VWindow::textWidth( const char* string )
{
  return font->getStringWidth(string);
}

/////////////////////////////////////////////////////////////////////////////

int VWindow::textHeight() const
{
  return font->getHeight();
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawPixmap( MLPixmap* pixmap, int x, int y )
{
  if( ! isMapped )
    return;
  if( pixmap == 0 )
    return;

  y= systemWindow->getSize().y() - 1 - y;
  pixmap->print( x, y );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawImage( MLGLPixmap* image, int dest_x, int dest_y )
{
  if( ! isMapped )
    return;
  if( image == 0 )
    return;

  drawImage( image,
             0, 0,
             image->getSize().x(), image->getSize().y(),
             dest_x, dest_y );
}

/////////////////////////////////////////////////////////////////////////////

void VWindow::drawImage( MLGLPixmap* _image,
                         int src_x, int src_y,
                         int width, int height,
                         int dest_x, int dest_y )
{
  if( ! isMapped )
    return;
  if( _image == 0 )
    return;

  if( image != 0 )
    _image->print( image,
                   src_x, src_y, width, height,
                   dest_x+windowImagePosition.x(),
                   dest_y+windowImagePosition.y() );
   else
    _image->print( systemWindow,
                   src_x, src_y, width, height, dest_x, dest_y );
}

/////////////////////////////////////////////////////////////////////////////

boolean VWindow::cursorInto( const MLEvent& event ) const
{
  if( event.position.x() < 0 || event.position.y() < 0 )
    return false;

  const IVector& size= getSize();

  return event.position.x() < size.x() &&
         event.position.y() < size.y();
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString VWindow::cutString( MLFont* font,
                             const MLString& _string, int max_width )
{
  if( _string.length() == 0 )
    return MLString("");

  int total_width= font->getStringWidth(_string);
  if( total_width <= max_width )
    return _string;

  int point_width= font->getCharWidth('.');
  if( max_width <= point_width )
    return MLString("");
  if( max_width <= 2*point_width )
    return MLString(".");
  if( max_width <= 3*point_width )
    return MLString("..");

  MLString string(_string);
  total_width+= 3 * point_width;
  for(;;)
    {
    if( string.length() == 0 )
      break;
    total_width -= font->getCharWidth( string[string.length()-1] );
    string.remove( string.length()-1 );
    if( total_width <= max_width )
      break;
    }

  return string + MLString("...");
}

/////////////////////////////////////////////////////////////////////////////

MLString VWindow::cutString( const MLString& _string, int max_width ) const
{
  return VWindow::cutString( font, _string, max_width );
}

/////////////////////////////////////////////////////////////////////////////

// return length
static int extract_word( const MLString& string, int& i, int& i1 )
{
  while( string.charAt(i) == ' ' )
    {
    ++i;
    if( i >= string.length() )
      break;
    }

  if( i >= string.length() )
    return -1; // the end

  i1= i;
  int length= 0;
  while( string.charAt(i) != '\n' &&
         string.charAt(i) != '\0' &&
         string.charAt(i) != ' ' )
    {
    ++i;
    ++length;
    }

  return length;
}

// static
SIList<MLString>* VWindow::splitString( MLFont* font,
                                        const MLString& string,
                                        int max_width )
{
  SIList<MLString>* list= new SIList<MLString>;
  if( font == 0 || string.length() == 0 || max_width <= 0 )
    return list;

  int space_width= font->getCharWidth(' ');

  int i= 0;
  int width= 0;
  MLString line("");
  for(;;) // for words
    {
    int i0;
    int length= extract_word(string,i,i0);
    if( length == -1 )
      break; // the end
    MLString word= string.substring( i0, i0+length-1 );

    int word_width= font->getStringWidth(word);
    if( word.length() == 0 )
      {
      if( string.charAt(i) == '\n' )
        {
        list->append( new MLString(line) );
        line= "";
        width= 0;
        ++i;
        }
      continue;
      }

    boolean addSpace= (width>0);
    if( width+word_width+(addSpace?space_width:0) <= max_width )
      {
      if( addSpace )
        line+= ' ';
      line+= word;
      width+= word_width + (addSpace ? space_width : 0);
      }
     else
      {
      list->append( new MLString(line) );
      line= word;
      width= word_width;
      }
    }

  if( line.length() > 0 )
    list->append( new MLString(line) );

  return list;
}

/////////////////////////////////////////////////////////////////////////////
