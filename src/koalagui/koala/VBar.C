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
   VBar.C

   Creation: SR, July 28th, 1995
   Revisions:
     + SR July 5 1996
*/

#include "graphics/MLEvent.h"

#include "VOptions.h"

#include "VBar.h"
#include "VBlankButton.h"

/////////////////////////////////////////////////////////////////////////////

VBar::VBar( VWindow* _parent, const IVector& _X ): VWidget(_parent)
{
  X= _X;
  Y= IVector( X.y(), X.x() );
  bar.button= 0;
  inTop= inBottom= false;

  scrollBar_width= vOptions->scrollBar.width;
}

/////////////////////////////////////////////////////////////////////////////

VBar::~VBar()
{
  delete bar.button;
  bar.button= 0;
}

/////////////////////////////////////////////////////////////////////////////

void VBar::create( int x, int y, int _height,
                   int totalLength, int visualLength )
{
  assert( visualLength >= 1 );

  width= scrollBar_width;
  height= _height;

  image.visualLength= visualLength;
  image.pos= 0;

  int bg_z= 0; //vOptions->scrollBar.bg_z;

  top= IPoint( bg_z, bg_z );
  bottom= IPoint(0,0) + X*(width-1-bg_z) + Y*(height-1-bg_z);

  scrollingHeight= height - 2*bg_z;
  bar.width= width - 2*bg_z;
  bar.y= 0;

  VWindow::create( IPoint(x,y),
                   IPoint(x,y) + X*(width-1) + Y*(height-1) );

  setWidgetEventHandler(this);
  image.totalLength= -1; // be sure button will be created

  changeTotalLength( totalLength );
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= vl
void VBar::changeVisualLength( int vl )
{
  assert( vl >= 1 );

  if( vl == image.visualLength )
    return;

  int tl= image.totalLength;
  image.totalLength= -1; // be sure button will be created
  image.visualLength= vl;
  changeTotalLength(tl);
}

/////////////////////////////////////////////////////////////////////////////

void VBar::changeTotalLength( int tl )
{
  tl= max( 0, tl );

  if( bar.button != 0 )
    {
    if( image.totalLength == tl )
      return; // done
    }

  image.totalLength= tl;

  boolean createBar= true;
  if( image.totalLength == 0 )
    {
    bar.height= scrollingHeight;
    createBar= false;
    }
   else
    {
    int d= image.totalLength-1+image.visualLength;
    if( d <= 0 )
      bar.height= scrollingHeight;
     else
      bar.height= scrollingHeight * image.visualLength / d;

    bar.height= min( bar.height, scrollingHeight );
    bar.height= max( bar.height, bar.width );
    if( bar.height == scrollingHeight )
      createBar= false;
    }

  bar.freeHeight= scrollingHeight - bar.height + 1-1;

  if( ! createBar )
    {
    delete bar.button;
    bar.button= 0;
    postRedraw();
    return;
    }

  const IPoint bar_p1= top;
  const IPoint bar_p2= top + X * (bar.width-1) + Y * (bar.height-1);

  if( bar.button == 0 )
    {
    // create button window
    bar.button= new VBlankButton(this);
    bar.button->create( bar_p1, bar_p2 );
    if( ! isAvailable() )
      bar.button->setAvailable(false);
    bar.button->setWidgetEventHandler(this);
    changePosition(image.pos+1);
    bar.button->postRedraw();
    if( ! mapped() )
      ((VWindow*)bar.button)->map(false);
    }
   else
    {
    changePosition(image.pos+1);
    const IPoint minP= min( bar_p1, bar_p2 );
    const IPoint maxP= max( bar_p1, bar_p2 );
    bar.button->setSize( maxP - minP + IVector(1,1) );
    }

  inTop= inBottom= false;
  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void VBar::changePosition( int i )
{
  i= max( 1, i );
  i= min( image.totalLength, i );

  image.pos= 0;
  --i;
  callPrivateWidgetEventHandler( VWidgetEvent::SetBarPos(this,i) );
}

/////////////////////////////////////////////////////////////////////////////

boolean VBar::handlePrivateWidgetEvent( const VWidgetEvent& event )
{
  static boolean scrollIsSent= false;

  if( event.type() == VWidgetEvent::SCROLLBARPOS )
    {
    scrollIsSent= true;
    return callPrivateWidgetEventHandler(
                             VWidgetEvent::SetBarPos(this,event.getPos()) );
    }

  if( event.type() == VWidgetEvent::SETBARPOS )
    {
    assert( event.getWidget() == this );
    int new_y;
    if( image.totalLength <= 1 )
      {
      image.pos= 0;
      new_y= coord(top);
      }
     else
      {
      image.pos= event.getPos();
      image.pos= max( 0, image.pos );
      image.pos= min( image.totalLength-1, image.pos );
      new_y= coord(top) + image.pos * bar.freeHeight / (image.totalLength-1);
      new_y= min( new_y, bar.freeHeight );
      }

    if( new_y != bar.y && bar.button != 0 )
      {
      const IVector pos= bar.button->getPosition();
      bar.button->setPosition( IPoint(0,0) + X*coordx(pos) + Y*new_y );
      bar.y= new_y;
//      drawBarBackground();
//      bar.button->drawWidget();
      postRedraw();
      bar.button->postRedraw();
      }

    return true;
    }

  if( event.type() == VWidgetEvent::MLEVENT )
    {
    static int y_delta= 0;
    static boolean pressed= false;
    static IPoint old_pos;

    assert( event.getWidget()->type() == VWidget::BLANKBUTTON );
    VBlankButton* bb= (VBlankButton*) event.getWidget();

    MLEvent* e= event.getMLEvent();
    switch( e->type )
      {
      case MLEvent::REDRAW:
        bb->drawButton();
        break;

      case MLEvent::MOUSE_MOTION:
        {
        if( image.totalLength != 0 )
          {
          int pointer_pos= coord(bb->getPosition())+coord(e->position);
          int new_y= y_delta + pointer_pos;
//                    +coord( IVector(xe->xmotion.x_root,xe->xmotion.y_root) );

          new_y= max( coord(top), new_y );
          new_y= min( coord(top)+bar.freeHeight, new_y );
          int new_pos;
          if( bar.freeHeight <= 0 )
            new_pos= coord(top);
           else
            new_pos= (new_y-coord(top)) * image.totalLength / bar.freeHeight;
          new_pos= min( image.totalLength-1, new_pos );
          if( new_pos != image.pos )
            {
            if( bar.button != 0 )
              bar.button->removeEvent( int(MLEvent::MOUSE_MOTION) );            scrollIsSent= false;
            callPrivateWidgetEventHandler(
                                  VWidgetEvent::ScrollBarPos(this,new_pos) );
            }
          }
        break;
        }

      case MLEvent::MOUSE_PRESS:
        {
        scrollIsSent= true;
        old_pos= bb->getPosition();
        int pointer_pos= coord(bb->getPosition())+coord(e->position);
        y_delta= coord(old_pos) - pointer_pos;

        pressed= true;
        return false;
        }

      case MLEvent::MOUSE_RELEASE:
        {
        pressed= false;
        drawBarBackground();
        // we finish scrolling, is new pos recorded by client?
        if( ! scrollIsSent )
          callPrivateWidgetEventHandler(
                                  VWidgetEvent::SetBarPos(this,image.pos) );
        return false;
        }

      default:
        return false;
      }

    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void VBar::drawBarBackground()
{
  if( ! beginDraw() )
    return;

  _drawBarBackground();
  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

void VBar::_drawBarBackground()
{
  if( vertical() )
    {
//    gray0();
//    drawRectangle( IPoint(0,0), IPoint(width-1,height-1) );
//    gray2();
//    drawRectangle( IPoint(1,1), IPoint(width-2,height-2) );

int tt= 0;
#ifdef __WINDOWS__
--tt;
#endif
    if( bar.button == 0 )
      {
      int y1= 0;
      int y2= height - 1;
      gray1();
      fillGrayRectangle( IPoint(0+tt,y1-1), IPoint(width,y2) );
      gray2();
      fillGrayRectangle2( IPoint(0+tt,y1-1), IPoint(width,y2) );
      return;
      }

    int y1= 0;
    int y2= bar.button->getPosition().y()-1;
    int y3= y2+1+bar.button->getSize().y();
    int y4= height - 1;

    if( y1 <= y2 )
      {
      gray1();
      fillGrayRectangle( IPoint(0+tt,y1-1), IPoint(width,y2) );
      gray2();
      fillGrayRectangle2( IPoint(0+tt,y1-1), IPoint(width,y2) );
      }

    if( y3 <= y4 )
      {
      gray1();
      fillGrayRectangle( IPoint(0+tt,y3-1), IPoint(width,y4) );
      gray2();
      fillGrayRectangle2( IPoint(0+tt,y3-1), IPoint(width,y4) );
      }

    }
   else
    {
    int width= getSize().x();
    int height= getSize().y();

//    gray0();
//    drawRectangle( IPoint(0,0), IPoint(width-1,height-1) );
//    gray2();
//    drawRectangle( IPoint(1,1), IPoint(width-2,height-2) );

    if( bar.button == 0 )
      {
      int x1= 1;
      int x2= width;
      gray1();
      fillGrayRectangle( IPoint(x1-1,-1), IPoint(x2+1,height-1) );
      gray2();
      fillGrayRectangle2( IPoint(x1-1,-1), IPoint(x2+1,height-1) );
      return;
      }

    int x1= 1;
    int x2= bar.button->getPosition().x()-1;
    int x3= x2+1+bar.button->getSize().x();
    int x4= width;

    if( x1 <= x2 )
      {
      gray1();
      fillGrayRectangle( IPoint(x1-1,-1), IPoint(x2+1,height-1) );
      gray2();
      fillGrayRectangle2( IPoint(x1-1,-1), IPoint(x2+1,height-1) );
      }

    if( x3 <= x4 )
      {
      gray1();
      fillGrayRectangle( IPoint(x3,-1), IPoint(x4,height-1) );
      gray2();
      fillGrayRectangle2( IPoint(x3,-1), IPoint(x4,height-1) );
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void VBar::drawWidget()
{
  removeRedraw();
  drawBarBackground();
  if( bar.button != 0 )
    {
    bar.button->removeRedraw();
    bar.button->drawButton();
    }
}

/////////////////////////////////////////////////////////////////////////////

VWidget** VBar::getWidgetsList( int* nWidgets )
{
  static VWidget* w[2];

  w[0]= this;
  w[1]= bar.button;
  *nWidgets= 2;

  return w;
}

/////////////////////////////////////////////////////////////////////////////

boolean VBar::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawBarBackground();
    return true;
    }

  if( ! isAvailable() )
    return VWindow::handleEvent(event);

  switch( event.type )
    {
    case MLEvent::MOUSE_PRESS:
      {
      if( bar.button == 0 )
        break;
      const IVector pos= bar.button->getPosition();
      int y= coord(event.position);
      if( y >= coord(top) && y < coord(pos) )
        page_up();
      if( y <= coord(bottom) && y >= coord(pos)+bar.height )
        page_down();
      drawBarBackground();
      }
      break;

/*
    case EnterNotify:
      {
      if( bar.button == 0 )
        break;
      const IVector pos= bar.button->getPosition();
      int y= coord(event.position);
      inTop= ( y >= 0 && y < coord(pos) );
      inBottom= ( y < height && y >= coord(pos)+bar.height );
      drawBarBackground();
      }
      break;

    case LeaveNotify:
      inTop= inBottom= false;
      drawBarBackground();
      break;
*/

    default:
      return VWindow::handleEvent(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void VBar::up( int dy /* = 1 */ )
{
  callPrivateWidgetEventHandler(
                          VWidgetEvent::SetBarPos( this, image.pos - dy ) );
}

/////////////////////////////////////////////////////////////////////////////

void VBar::down( int dy /* = 1 */ )
{
  up( -dy );
}

/////////////////////////////////////////////////////////////////////////////

void VBar::page_down()
{
  down( image.visualLength + 1 );
}

/////////////////////////////////////////////////////////////////////////////

void VBar::page_up()
{
  down( -(image.visualLength + 1) );
}

/////////////////////////////////////////////////////////////////////////////

